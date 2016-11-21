// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>
#include <vector>

#include "base/test/simple_test_tick_clock.h"
#include "content/public/renderer/content_renderer_client.h"
#include "content/renderer/media/render_media_client.h"
#include "content/test/test_content_client.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

namespace {

class TestKeySystemProperties : public media::KeySystemProperties {
 public:
  TestKeySystemProperties(const std::string& key_system_name)
      : key_system_name_(key_system_name) {}

  std::string GetKeySystemName() const override { return key_system_name_; }
  bool IsSupportedInitDataType(
      media::EmeInitDataType init_data_type) const override {
    return false;
  }
  media::SupportedCodecs GetSupportedCodecs() const override {
    return media::EME_CODEC_NONE;
  }
  media::EmeConfigRule GetRobustnessConfigRule(
      media::EmeMediaType media_type,
      const std::string& requested_robustness) const override {
    return requested_robustness.empty() ? media::EmeConfigRule::SUPPORTED
                                        : media::EmeConfigRule::NOT_SUPPORTED;
  }
  media::EmeSessionTypeSupport GetPersistentLicenseSessionSupport()
      const override {
    return media::EmeSessionTypeSupport::NOT_SUPPORTED;
  }
  media::EmeSessionTypeSupport GetPersistentReleaseMessageSessionSupport()
      const override {
    return media::EmeSessionTypeSupport::NOT_SUPPORTED;
  }
  media::EmeFeatureSupport GetPersistentStateSupport() const override {
    return media::EmeFeatureSupport::NOT_SUPPORTED;
  }
  media::EmeFeatureSupport GetDistinctiveIdentifierSupport() const override {
    return media::EmeFeatureSupport::NOT_SUPPORTED;
  }

 private:
  const std::string key_system_name_;
};

class TestContentRendererClient : public ContentRendererClient {
 public:
  TestContentRendererClient() : is_extra_key_system_enabled_(false) {}

  // ContentRendererClient implementation.
  void AddSupportedKeySystems(
      std::vector<std::unique_ptr<media::KeySystemProperties>>*
          key_systems_properties) override {
    key_systems_properties->emplace_back(
        new TestKeySystemProperties("test.keysystem"));
  }

  void EnableExtraKeySystem() { is_extra_key_system_enabled_ = true; }

 private:
  // Whether a platform-specific extra key system is "supported" by |this|.
  bool is_extra_key_system_enabled_;
};

}  // namespace

class RenderMediaClientTest : public testing::Test {
 protected:
  RenderMediaClientTest()
      : render_media_client_(GetRenderMediaClientInstanceForTesting()) {
    SetContentClient(&test_content_client_);
    SetRendererClientForTesting(&test_content_renderer_client_);
  }

  void EnableExtraKeySystem() {
    test_content_renderer_client_.EnableExtraKeySystem();
  }

  RenderMediaClient* render_media_client_;

 private:
  typedef base::hash_map<std::string, std::string> KeySystemNameForUMAMap;

  TestContentClient test_content_client_;
  TestContentRendererClient test_content_renderer_client_;
  KeySystemNameForUMAMap key_system_name_for_uma_map_;
};

TEST_F(RenderMediaClientTest, KeySystemNameForUMA) {
  std::vector<media::KeySystemInfoForUMA> key_systems_info_for_uma;
  render_media_client_->AddKeySystemsInfoForUMA(&key_systems_info_for_uma);

  std::string clearkey_uma_name;
  for (const media::KeySystemInfoForUMA& info : key_systems_info_for_uma) {
    if (info.key_system == "org.w3.clearkey")
      clearkey_uma_name = info.key_system_name_for_uma;
  }

  EXPECT_TRUE(clearkey_uma_name.empty()) << "Clear Key is added by media/ and "
                                            "should not be added by the "
                                            "MediaClient.";
}

TEST_F(RenderMediaClientTest, IsKeySystemsUpdateNeeded) {
  base::SimpleTestTickClock* tick_clock = new base::SimpleTestTickClock();
  render_media_client_->SetTickClockForTesting(
      std::unique_ptr<base::TickClock>(tick_clock));

  // IsKeySystemsUpdateNeeded() always returns true after construction.
  EXPECT_TRUE(render_media_client_->IsKeySystemsUpdateNeeded());

  std::vector<std::unique_ptr<media::KeySystemProperties>>
      key_systems_properties;
  render_media_client_->AddSupportedKeySystems(&key_systems_properties);

  // No update needed immediately after AddSupportedKeySystems() call.
  EXPECT_FALSE(render_media_client_->IsKeySystemsUpdateNeeded());

}

}  // namespace content
