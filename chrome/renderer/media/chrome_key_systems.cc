// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/renderer/media/chrome_key_systems.h"

#include <stddef.h>

#include <string>
#include <vector>

#include "base/logging.h"
#include "base/strings/string16.h"
#include "base/strings/string_split.h"
#include "base/strings/utf_string_conversions.h"
#include "build/build_config.h"
#include "chrome/common/render_messages.h"
#include "components/cdm/renderer/widevine_key_system_properties.h"
#include "content/public/renderer/render_thread.h"
#include "media/base/eme_constants.h"
#include "media/base/key_system_properties.h"

#if defined(OS_ANDROID)
#include "components/cdm/renderer/android_key_systems.h"
#endif

#include "widevine_cdm_version.h" // In SHARED_INTERMEDIATE_DIR.

// The following must be after widevine_cdm_version.h.

using media::KeySystemProperties;
using media::SupportedCodecs;

#if defined(ENABLE_PEPPER_CDMS)
static const char kExternalClearKeyPepperType[] =
    "application/x-ppapi-clearkey-cdm";

static bool IsPepperCdmAvailable(
    const std::string& pepper_type,
    std::vector<base::string16>* additional_param_names,
    std::vector<base::string16>* additional_param_values) {
  bool is_available = false;
  content::RenderThread::Get()->Send(
      new ChromeViewHostMsg_IsInternalPluginAvailableForMimeType(
          pepper_type,
          &is_available,
          additional_param_names,
          additional_param_values));

  return is_available;
}

// KeySystemProperties implementation for external Clear Key systems.
class ExternalClearKeyProperties : public KeySystemProperties {
 public:
  explicit ExternalClearKeyProperties(const std::string& key_system_name)
      : key_system_name_(key_system_name) {}

  std::string GetKeySystemName() const override { return key_system_name_; }
  bool IsSupportedInitDataType(
      media::EmeInitDataType init_data_type) const override {
    switch (init_data_type) {
      case media::EmeInitDataType::WEBM:
      case media::EmeInitDataType::KEYIDS:
        return true;

      case media::EmeInitDataType::CENC:
#if defined(USE_PROPRIETARY_CODECS)
        return true;
#else
        return false;
#endif  // defined(USE_PROPRIETARY_CODECS)

      case media::EmeInitDataType::UNKNOWN:
        return false;
    }
    NOTREACHED();
    return false;
  }

  SupportedCodecs GetSupportedCodecs() const override {
#if defined(USE_PROPRIETARY_CODECS)
    return media::EME_CODEC_MP4_ALL | media::EME_CODEC_WEBM_ALL;
#else
    return media::EME_CODEC_WEBM_ALL;
#endif
  }

  media::EmeConfigRule GetRobustnessConfigRule(
      media::EmeMediaType media_type,
      const std::string& requested_robustness) const override {
    return requested_robustness.empty() ? media::EmeConfigRule::SUPPORTED
                                        : media::EmeConfigRule::NOT_SUPPORTED;
  }

  // Persistent license sessions are faked.
  media::EmeSessionTypeSupport GetPersistentLicenseSessionSupport()
      const override {
    return media::EmeSessionTypeSupport::SUPPORTED;
  }

  media::EmeSessionTypeSupport GetPersistentReleaseMessageSessionSupport()
      const override {
    return media::EmeSessionTypeSupport::NOT_SUPPORTED;
  }

  media::EmeFeatureSupport GetPersistentStateSupport() const override {
    return media::EmeFeatureSupport::REQUESTABLE;
  }

  media::EmeFeatureSupport GetDistinctiveIdentifierSupport() const override {
    return media::EmeFeatureSupport::NOT_SUPPORTED;
  }

  std::string GetPepperType() const override {
    return kExternalClearKeyPepperType;
  }

 private:
  const std::string key_system_name_;
};

// External Clear Key (used for testing).
static void AddExternalClearKey(
    std::vector<std::unique_ptr<KeySystemProperties>>* concrete_key_systems) {
  static const char kExternalClearKeyKeySystem[] =
      "org.chromium.externalclearkey";
  static const char kExternalClearKeyDecryptOnlyKeySystem[] =
      "org.chromium.externalclearkey.decryptonly";
  static const char kExternalClearKeyFileIOTestKeySystem[] =
      "org.chromium.externalclearkey.fileiotest";
  static const char kExternalClearKeyInitializeFailKeySystem[] =
      "org.chromium.externalclearkey.initializefail";
  static const char kExternalClearKeyCrashKeySystem[] =
      "org.chromium.externalclearkey.crash";

  std::vector<base::string16> additional_param_names;
  std::vector<base::string16> additional_param_values;
  if (!IsPepperCdmAvailable(kExternalClearKeyPepperType,
                            &additional_param_names,
                            &additional_param_values)) {
    return;
  }

  concrete_key_systems->emplace_back(
      new ExternalClearKeyProperties(kExternalClearKeyKeySystem));

  // Add support of decrypt-only mode in ClearKeyCdm.
  concrete_key_systems->emplace_back(
      new ExternalClearKeyProperties(kExternalClearKeyDecryptOnlyKeySystem));

  // A key system that triggers FileIO test in ClearKeyCdm.
  concrete_key_systems->emplace_back(
      new ExternalClearKeyProperties(kExternalClearKeyFileIOTestKeySystem));

  // A key system that Chrome thinks is supported by ClearKeyCdm, but actually
  // will be refused by ClearKeyCdm. This is to test the CDM initialization
  // failure case.
  concrete_key_systems->emplace_back(
      new ExternalClearKeyProperties(kExternalClearKeyInitializeFailKeySystem));

  // A key system that triggers a crash in ClearKeyCdm.
  concrete_key_systems->emplace_back(
      new ExternalClearKeyProperties(kExternalClearKeyCrashKeySystem));
}

#endif  // defined(ENABLE_PEPPER_CDMS)

void AddChromeKeySystems(
    std::vector<std::unique_ptr<KeySystemProperties>>* key_systems_properties) {
#if defined(ENABLE_PEPPER_CDMS)
  AddExternalClearKey(key_systems_properties);

#endif  // defined(ENABLE_PEPPER_CDMS)

#if defined(OS_ANDROID)
  cdm::AddAndroidWidevine(key_systems_properties);
#endif  // defined(OS_ANDROID)
}
