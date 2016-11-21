// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMECAST_MEDIA_CDM_INIT_DATA_H_
#define CHROMECAST_MEDIA_CDM_INIT_DATA_H_

#include <stdint.h>

#include <vector>

namespace chromecast {
namespace media {

enum class InitDataMessageType {
  UNKNOWN = 0x0,
  CUSTOM_DATA = 0x1,
  ENABLE_SECURE_STOP = 0x2,
  END
};

// Structured data for EME initialization as parsed from an initData blob.
struct ChromecastInitData {
  ChromecastInitData();
  ~ChromecastInitData();

  InitDataMessageType type;
  std::vector<uint8_t> data;
};

}  // namespace media
}  // namespace chromecast

#endif  // CHROMECAST_MEDIA_CDM_INIT_DATA_H_
