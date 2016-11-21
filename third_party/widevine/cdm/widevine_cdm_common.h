// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIDEVINE_CDM_WIDEVINE_CDM_COMMON_H_
#define WIDEVINE_CDM_WIDEVINE_CDM_COMMON_H_

// This file defines constants common to all Widevine CDM versions.

// Widevine CDM version contains 4 components, e.g. 1.4.0.195.
const int kWidevineCdmVersionNumComponents = 4;

// "alpha" is a temporary name until a convention is defined.
const char kWidevineKeySystem[] = "com.widevine.alpha";

// This type is used to register the Widevine CDM.
const char kWidevineCdmType[] = "Widevine";

// Widevine CDM files are in a directory with this name.
const char kWidevineCdmBaseDirectory[] = "WidevineCdm";

// This name is used by UMA. Do not change it!
const char kWidevineKeySystemNameForUMA[] = "Widevine";

const char kWidevineCdmDisplayName[] = "Widevine Content Decryption Module";

// Will be parsed as HTML.
const char kWidevineCdmDescription[] =
    "Enables Widevine licenses for playback of HTML audio/video content.";

#endif  // WIDEVINE_CDM_WIDEVINE_CDM_COMMON_H_
