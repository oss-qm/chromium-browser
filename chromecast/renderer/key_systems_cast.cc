// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromecast/renderer/key_systems_cast.h"

#include <string>

#include "base/command_line.h"
#include "base/logging.h"
#include "build/build_config.h"
#include "chromecast/media/base/key_systems_common.h"
#include "media/base/eme_constants.h"
#include "media/base/key_system_properties.h"
#include "media/media_features.h"

using ::media::EmeConfigRule;
using ::media::EmeFeatureSupport;
using ::media::EmeInitDataType;
using ::media::EmeMediaType;
using ::media::EmeSessionTypeSupport;
using ::media::SupportedCodecs;

namespace chromecast {
namespace shell {
namespace {

}  // namespace

void AddChromecastKeySystems(
    std::vector<std::unique_ptr<::media::KeySystemProperties>>*
        key_systems_properties,
    bool enable_persistent_license_support) {
}

}  // namespace shell
}  // namespace chromecast
