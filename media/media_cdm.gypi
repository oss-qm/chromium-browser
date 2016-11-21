# Copyright 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'includes': [
    'cdm_paths.gypi',
    'media_variables.gypi',
  ],
  'variables': {
    # Set |use_fake_video_decoder| to 1 to ignore input frames in |clearkeycdm|,
    # and produce video frames filled with a solid color instead.
    'use_fake_video_decoder%': 0,
    # Set |use_libvpx_in_clear_key_cdm| to 1 to use libvpx for VP8 decoding in
    # |clearkeycdm|.
    'use_libvpx_in_clear_key_cdm%': 0,
  },
}
