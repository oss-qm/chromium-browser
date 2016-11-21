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
#include "content/public/renderer/render_thread.h"
#include "media/base/eme_constants.h"
#include "media/base/key_system_properties.h"

#if defined(OS_ANDROID)
#include "components/cdm/renderer/android_key_systems.h"
#endif

using media::KeySystemProperties;
using media::SupportedCodecs;

void AddChromeKeySystems(
    std::vector<std::unique_ptr<KeySystemProperties>>* key_systems_properties) {
}
