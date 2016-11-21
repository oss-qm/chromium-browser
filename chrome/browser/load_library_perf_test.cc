// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stddef.h>
#include <stdint.h>

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/scoped_native_library.h"
#include "base/strings/utf_string_conversions.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/perf/perf_test.h"

namespace {

// Measures the size (bytes) and time to load (sec) of a native library.
// |library_relative_dir| is the relative path based on DIR_MODULE.
void MeasureSizeAndTimeToLoadNativeLibrary(
    const base::FilePath& library_relative_dir,
    const base::FilePath& library_name) {
  base::FilePath output_dir;
  ASSERT_TRUE(PathService::Get(base::DIR_MODULE, &output_dir));
  output_dir = output_dir.Append(library_relative_dir);
  base::FilePath library_path = output_dir.Append(library_name);
  ASSERT_TRUE(base::PathExists(library_path)) << library_path.value();

  int64_t size = 0;
  ASSERT_TRUE(base::GetFileSize(library_path, &size));
  perf_test::PrintResult("library_size",
                         "",
                         library_name.AsUTF8Unsafe(),
                         static_cast<size_t>(size),
                         "bytes",
                         true);

  base::NativeLibraryLoadError error;
  base::TimeTicks start = base::TimeTicks::Now();
  base::NativeLibrary native_library =
      base::LoadNativeLibrary(library_path, &error);
  double delta = (base::TimeTicks::Now() - start).InMillisecondsF();
  ASSERT_TRUE(native_library) << "Error loading library: " << error.ToString();
  base::UnloadNativeLibrary(native_library);
  perf_test::PrintResult("time_to_load_library",
                         "",
                         library_name.AsUTF8Unsafe(),
                         delta,
                         "ms",
                         true);
}

}  // namespace
