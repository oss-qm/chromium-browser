// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/component_updater/widevine_cdm_component_installer.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

#include "base/base_paths.h"
#include "base/bind.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/strings/string16.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "build/build_config.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/widevine_cdm_constants.h"
#include "components/component_updater/component_updater_service.h"
#include "components/component_updater/default_component_installer.h"
#include "components/version_info/version_info.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/cdm_service.h"
#include "content/public/browser/plugin_service.h"
#include "content/public/common/cdm_info.h"
#include "content/public/common/pepper_plugin_info.h"
#include "media/cdm/supported_cdm_versions.h"
#include "third_party/widevine/cdm/widevine_cdm_common.h"

#include "widevine_cdm_version.h"  // In SHARED_INTERMEDIATE_DIR. NOLINT

using content::BrowserThread;
using content::CdmService;
using content::PluginService;

namespace component_updater {

void RegisterWidevineCdmComponent(ComponentUpdateService* cus) {
}

}  // namespace component_updater
