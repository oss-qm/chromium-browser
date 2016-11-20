// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/metrics/leak_detector/leak_detector.h"

#include <stdint.h>

#include <algorithm>

#include "base/allocator/allocator_extension.h"
#include "base/bind.h"
#include "base/lazy_instance.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/numerics/safe_conversions.h"
#include "base/threading/thread_local.h"
#include "components/metrics/leak_detector/custom_allocator.h"
#include "components/metrics/leak_detector/leak_detector_impl.h"
#include "components/metrics/proto/memory_leak_report.pb.h"

#if defined(OS_CHROMEOS)
#include <link.h>  // for dl_iterate_phdr
#else
#error "Getting binary mapping info is not supported on this platform."
#endif  // defined(OS_CHROMEOS)

namespace metrics {

using InternalLeakReport = leak_detector::LeakDetectorImpl::LeakReport;
template <typename T>
using InternalVector = leak_detector::LeakDetectorImpl::InternalVector<T>;

namespace {

// Add the thread-local alloc size count to the shared alloc size count
// (LeakDetector::total_alloc_size_) whenever the local counter reaches
// |LeakDetector::analysis_interval_bytes_| divided by this value. Choose a
// high enough value that there is plenty of granularity, but low enough that a
// thread is not frequently updating the shared counter.
const int kTotalAllocSizeUpdateIntervalDivisor = 1024;

#if defined(OS_CHROMEOS)
// For storing the address range of the Chrome binary in memory.
struct MappingInfo {
  uintptr_t addr;
  size_t size;
};
#endif  // defined(OS_CHROMEOS)

// Local data to be used in the alloc/free hook functions to keep track of
// things across hook function calls.
struct HookData {
  // The total number of bytes nominally allocated from the allocator on the
  // current thread.
  size_t alloc_size;

  // Flag indicating that one of the alloc hooks have already been entered. Used
  // to handle recursive hook calls. Anything allocated when this flag is set
  // should also be freed when this flag is set.
  bool entered_hook;
};

#if defined(OS_CHROMEOS)
// Callback for dl_iterate_phdr() to find the Chrome binary mapping.
int IterateLoadedObjects(struct dl_phdr_info* shared_object,
                         size_t /* size */,
                         void* data) {
  for (int i = 0; i < shared_object->dlpi_phnum; i++) {
    // Find the ELF segment header that contains the actual code of the Chrome
    // binary.
    const ElfW(Phdr)& segment_header = shared_object->dlpi_phdr[i];
    if (segment_header.p_type == SHT_PROGBITS && segment_header.p_offset == 0 &&
        data) {
      MappingInfo* mapping = reinterpret_cast<MappingInfo*>(data);

      // Make sure the fields in the ELF header and MappingInfo have the
      // same size.
      static_assert(sizeof(mapping->addr) == sizeof(shared_object->dlpi_addr),
                    "Integer size mismatch between MappingInfo::addr and "
                    "dl_phdr_info::dlpi_addr.");
      static_assert(sizeof(mapping->size) == sizeof(segment_header.p_offset),
                    "Integer size mismatch between MappingInfo::size and "
                    "ElfW(Phdr)::p_memsz.");

      mapping->addr = shared_object->dlpi_addr + segment_header.p_offset;
      mapping->size = segment_header.p_memsz;
      return 1;
    }
  }
  return 0;
}
#endif  // defined(OS_CHROMEOS)

// Convert a pointer to a hash value. Returns only the upper eight bits.
inline uint64_t PointerToHash(const void* ptr) {
  // The input data is the pointer address, not the location in memory pointed
  // to by the pointer.
  // The multiplier is taken from Farmhash code:
  //   https://github.com/google/farmhash/blob/master/src/farmhash.cc
  const uint64_t kMultiplier = 0x9ddfea08eb382d69ULL;
  return reinterpret_cast<uint64_t>(ptr) * kMultiplier;
}

// Converts an memory leak report generated by LeakDetectorImpl
// (InternalLeakReport) to protobuf format (MemoryLeakReportProto).
MemoryLeakReportProto ConvertLeakReportToProto(
    const InternalLeakReport& report) {
  MemoryLeakReportProto proto;

  proto.set_size_bytes(report.alloc_size_bytes());
  for (auto call_stack_entry : report.call_stack()) {
    proto.add_call_stack(call_stack_entry);
  }

  for (const auto& entry : report.alloc_breakdown_history()) {
    auto* proto_entry = proto.add_alloc_breakdown_history();
    for (const uint32_t count : entry.counts_by_size) {
      proto_entry->add_counts_by_size(count);
    }
    proto_entry->set_count_for_call_stack(entry.count_for_call_stack);
  }

  return proto;
}

// The only instance of LeakDetector that should be used.
base::LazyInstance<LeakDetector>::Leaky g_instance = LAZY_INSTANCE_INITIALIZER;

// Thread-specific data to be used by hook functions.
base::LazyInstance<base::ThreadLocalPointer<void>>::Leaky g_hook_data_tls =
    LAZY_INSTANCE_INITIALIZER;

// Returns the contents of |g_hook_data_tls| as a HookData structure.
inline HookData LoadHookDataFromTLS() {
  uintptr_t ptr_value =
      reinterpret_cast<uintptr_t>(g_hook_data_tls.Get().Get());

  // The lower bit of |ptr_value| indicates whether a hook has already been
  // entered. The remaining bits store the alloc size.
  HookData result;
  result.entered_hook = ptr_value & 0x01;
  result.alloc_size = ptr_value >> 1;
  return result;
}

// Stores a HookData structure in |g_hook_data_tls|. HookData is a trivial
// struct so it is faster to pass by value.
inline void StoreHookDataToTLS(HookData hook_data) {
  // NOTE: |alloc_size| loses its upper bit when it gets stored in the TLS here.
  // The effective max value of |alloc_size| is thus half its nominal max value.
  uintptr_t ptr_value =
      (hook_data.entered_hook ? 1 : 0) | (hook_data.alloc_size << 1);
  g_hook_data_tls.Get().Set(reinterpret_cast<void*>(ptr_value));
}

}  // namespace

// static
LeakDetector* LeakDetector::GetInstance() {
  return g_instance.Pointer();
}

// static
void LeakDetector::InitTLSSlot() {
  ignore_result(g_hook_data_tls.Get());
}

void LeakDetector::Init(const MemoryLeakReportProto::Params& params,
                        scoped_refptr<base::TaskRunner> task_runner) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_GT(params.sampling_rate(), 0);

  sampling_factor_ =
      base::saturated_cast<uint64_t>(params.sampling_rate() * UINT64_MAX);

  analysis_interval_bytes_ = params.analysis_interval_bytes();
  max_call_stack_unwind_depth_ = params.max_stack_depth();

  MappingInfo mapping = {0};
#if defined(OS_CHROMEOS)
  // Locate the Chrome binary mapping info.
  dl_iterate_phdr(IterateLoadedObjects, &mapping);
#endif  // defined(OS_CHROMEOS)

  task_runner_ = task_runner;

  // CustomAllocator can use the default allocator, as long as the hook
  // functions can handle recursive calls.
  leak_detector::CustomAllocator::Initialize();

  // The initialization should be done only once. Check for this by examining
  // whether |impl_| has already been initialized.
  CHECK(!impl_.get()) << "Cannot initialize LeakDetector more than once!";
  impl_.reset(new leak_detector::LeakDetectorImpl(
      mapping.addr, mapping.size, params.size_suspicion_threshold(),
      params.call_stack_suspicion_threshold()));

  // Register allocator hook functions. This must be done last since the
  // preceding code will need to call the allocator.
  base::allocator::SetHooks(&AllocHook, &FreeHook);
}

void LeakDetector::AddObserver(Observer* observer) {
  base::AutoLock lock(observers_lock_);
  observers_.AddObserver(observer);
}

void LeakDetector::RemoveObserver(Observer* observer) {
  base::AutoLock lock(observers_lock_);
  observers_.RemoveObserver(observer);
}

LeakDetector::LeakDetector()
    : total_alloc_size_(0),
      last_analysis_alloc_size_(0),
      analysis_interval_bytes_(0),
      max_call_stack_unwind_depth_(0),
      sampling_factor_(0) {}

LeakDetector::~LeakDetector() {}

// static
void LeakDetector::AllocHook(const void* ptr, size_t size) {
  HookData hook_data = LoadHookDataFromTLS();
  if (hook_data.entered_hook)
    return;

  hook_data.alloc_size += size;

  LeakDetector* detector = GetInstance();
  if (!detector->ShouldSample(ptr)) {
    StoreHookDataToTLS(hook_data);
    return;
  }

  hook_data.entered_hook = true;
  StoreHookDataToTLS(hook_data);

  // Get stack trace if necessary.
  std::vector<void*> stack;
  int depth = 0;
  if (detector->impl_->ShouldGetStackTraceForSize(size)) {
    stack.resize(detector->max_call_stack_unwind_depth_);
    depth = base::allocator::GetCallStack(stack.data(), stack.size());
  }

  {
    base::AutoLock lock(detector->recording_lock_);
    detector->impl_->RecordAlloc(ptr, size, depth, stack.data());

    const auto& analysis_interval_bytes = detector->analysis_interval_bytes_;
    auto& total_alloc_size = detector->total_alloc_size_;
    // Update the shared counter, |detector->total_alloc_size_|, once the local
    // counter reaches a threshold that is a fraction of the analysis interval.
    // The fraction should be small enough (and hence the value of
    // kTotalAllocSizeUpdateIntervalDivisor should be large enough) that the
    // shared counter is updated with sufficient granularity. This way, even if
    // a few threads were slow to reach the threshold, the leak analysis would
    // not be delayed by too much.
    if (hook_data.alloc_size >=
        analysis_interval_bytes / kTotalAllocSizeUpdateIntervalDivisor) {
      total_alloc_size += hook_data.alloc_size;
      hook_data.alloc_size = 0;
    }

    // Check for leaks after |analysis_interval_bytes_| bytes have been
    // allocated since the last time that was done.
    if (total_alloc_size >
        detector->last_analysis_alloc_size_ + analysis_interval_bytes) {
      // Try to maintain regular intervals of size |analysis_interval_bytes_|.
      detector->last_analysis_alloc_size_ =
          total_alloc_size - total_alloc_size % analysis_interval_bytes;

      InternalVector<InternalLeakReport> leak_reports;
      detector->impl_->TestForLeaks(&leak_reports);

      // Pass leak reports to observers.
      std::vector<MemoryLeakReportProto> leak_report_protos;
      leak_report_protos.reserve(leak_reports.size());
      std::transform(leak_reports.begin(), leak_reports.end(),
                     std::back_inserter(leak_report_protos),
                     &ConvertLeakReportToProto);
      detector->NotifyObservers(leak_report_protos);
    }
  }

  {
    // The internal memory of |stack| should be freed before setting
    // |entered_hook| to false at the end of this function. Free it here by
    // moving the internal memory to a temporary variable that will go out of
    // scope.
    std::vector<void*> dummy_stack;
    dummy_stack.swap(stack);
  }

  hook_data.entered_hook = false;
  StoreHookDataToTLS(hook_data);
}

// static
void LeakDetector::FreeHook(const void* ptr) {
  LeakDetector* detector = GetInstance();
  if (!detector->ShouldSample(ptr))
    return;

  HookData hook_data = LoadHookDataFromTLS();
  if (hook_data.entered_hook)
    return;

  hook_data.entered_hook = true;
  StoreHookDataToTLS(hook_data);

  {
    base::AutoLock lock(detector->recording_lock_);
    detector->impl_->RecordFree(ptr);
  }

  hook_data.entered_hook = false;
  StoreHookDataToTLS(hook_data);
}

inline bool LeakDetector::ShouldSample(const void* ptr) const {
  return PointerToHash(ptr) < sampling_factor_;
}

void LeakDetector::NotifyObservers(
    const std::vector<MemoryLeakReportProto>& reports) {
  if (reports.empty())
    return;

  if (!task_runner_->RunsTasksOnCurrentThread()) {
    task_runner_->PostTask(FROM_HERE,
                           base::Bind(&LeakDetector::NotifyObservers,
                                      base::Unretained(this), reports));
    return;
  }

  {
    base::AutoLock lock(observers_lock_);
    FOR_EACH_OBSERVER(Observer, observers_, OnLeaksFound(reports));
  }
}

}  // namespace metrics
