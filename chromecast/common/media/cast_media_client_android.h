// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMECAST_COMMON_MEDIA_CAST_MEDIA_CLIENT_ANDROID_H_
#define CHROMECAST_COMMON_MEDIA_CAST_MEDIA_CLIENT_ANDROID_H_

#include <map>

#include "base/macros.h"
#include "media/base/android/media_client_android.h"

namespace chromecast {
namespace media {

class CastMediaClientAndroid : public ::media::MediaClientAndroid {
 public:
  CastMediaClientAndroid();
  ~CastMediaClientAndroid() override;

 private:
  // ::media::MediaClientAndroid implementation:
  void AddKeySystemUUIDMappings(KeySystemUuidMap* map) override;
  ::media::MediaDrmBridgeDelegate* GetMediaDrmBridgeDelegate(
      const ::media::UUID& scheme_uuid) override;

  DISALLOW_COPY_AND_ASSIGN(CastMediaClientAndroid);
};

}  // namespace media
}  // namespace chromecast

#endif  // CHROMECAST_COMMON_MEDIA_CAST_MEDIA_CLIENT_ANDROID_H_
