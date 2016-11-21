// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromecast/common/media/cast_media_client_android.h"

#include "chromecast/media/base/key_systems_common.h"

namespace chromecast {
namespace media {

CastMediaClientAndroid::CastMediaClientAndroid() {}

CastMediaClientAndroid::~CastMediaClientAndroid() {}

void CastMediaClientAndroid::AddKeySystemUUIDMappings(KeySystemUuidMap* map) {
}

::media::MediaDrmBridgeDelegate*
CastMediaClientAndroid::GetMediaDrmBridgeDelegate(
    const ::media::UUID& scheme_uuid) {

  return nullptr;
}

}  // namespace media
}  // namespace chromecast
