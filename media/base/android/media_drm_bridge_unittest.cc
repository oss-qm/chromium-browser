// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>

#include "base/android/build_info.h"
#include "base/bind.h"
#include "base/logging.h"
#include "base/memory/ptr_util.h"
#include "base/message_loop/message_loop.h"
#include "media/base/android/media_drm_bridge.h"
#include "media/base/android/provision_fetcher.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace media {

const char kAudioMp4[] = "audio/mp4";
const char kVideoMp4[] = "video/mp4";
const char kAudioWebM[] = "audio/webm";
const char kVideoWebM[] = "video/webm";
const char kInvalidKeySystem[] = "invalid.keysystem";
const MediaDrmBridge::SecurityLevel kDefault =
    MediaDrmBridge::SECURITY_LEVEL_DEFAULT;
const MediaDrmBridge::SecurityLevel kL1 = MediaDrmBridge::SECURITY_LEVEL_1;
const MediaDrmBridge::SecurityLevel kL3 = MediaDrmBridge::SECURITY_LEVEL_3;

// Helper functions to avoid typing "MediaDrmBridge::" in tests.

static bool IsKeySystemSupportedWithType(
    const std::string& key_system,
    const std::string& container_mime_type) {
  return MediaDrmBridge::IsKeySystemSupportedWithType(key_system,
                                                      container_mime_type);
}

namespace {
// Mock ProvisionFetcher.
class MockProvisionFetcher : public ProvisionFetcher {
 public:
  static std::unique_ptr<ProvisionFetcher> Create() {
    return base::WrapUnique(new MockProvisionFetcher);
  }

  // ProvisionFetcher implementation.
  void Retrieve(const std::string& default_url,
                const std::string& request_data,
                const ResponseCB& response_cb) override {}
};

}  // namespace (anonymous)

// Invalid key system is NOT supported regardless whether MediaDrm is available.
TEST(MediaDrmBridgeTest, IsKeySystemSupported_InvalidKeySystem) {
  EXPECT_FALSE(MediaDrmBridge::IsKeySystemSupported(kInvalidKeySystem));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, kAudioMp4));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, kVideoMp4));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, kAudioWebM));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, kVideoWebM));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, "unknown"));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, "video/avi"));
  EXPECT_FALSE(IsKeySystemSupportedWithType(kInvalidKeySystem, "audio/mp3"));
}

// Invalid key system is NOT supported regardless whether MediaDrm is available.
TEST(MediaDrmBridgeTest, CreateWithoutSessionSupport_InvalidKeySystem) {
  base::MessageLoop message_loop_;
  EXPECT_FALSE(MediaDrmBridge::CreateWithoutSessionSupport(
      kInvalidKeySystem, kDefault, base::Bind(&MockProvisionFetcher::Create)));
}

}  // namespace media
