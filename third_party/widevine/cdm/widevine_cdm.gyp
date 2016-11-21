# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    # Allow widevinecdmadapter to be built in Chromium.
    'widevine_cdm_version_h_file%': 'widevine_cdm_version.h',
    'widevine_cdm_binary_files%': [],
    'widevine_cdm_manifest_file%': [],
    'conditions': [
      [ 'branding == "Chrome"', {
        'conditions': [
          [ 'chromeos == 1', {
            'widevine_cdm_version_h_file%':
                'chromeos/<(target_arch)/widevine_cdm_version.h',
            'widevine_cdm_binary_files%': [
              'chromeos/<(target_arch)/libwidevinecdm.so',
            ],
          }],
          [ 'OS == "linux" and chromeos == 0', {
            'widevine_cdm_version_h_file%':
                'linux/<(target_arch)/widevine_cdm_version.h',
            'widevine_cdm_binary_files%': [
              'linux/<(target_arch)/libwidevinecdm.so',
            ],
          }],
          [ 'OS == "mac"', {
            'widevine_cdm_version_h_file%':
                'mac/<(target_arch)/widevine_cdm_version.h',
            'widevine_cdm_binary_files%': [
              'mac/<(target_arch)/libwidevinecdm.dylib',
            ],
            'widevine_cdm_manifest_file%': [
              'mac/<(target_arch)/manifest.json',
            ],
          }],
          [ 'OS == "win"', {
            'widevine_cdm_version_h_file%':
                'win/<(target_arch)/widevine_cdm_version.h',
            'widevine_cdm_binary_files%': [
              'win/<(target_arch)/widevinecdm.dll',
              'win/<(target_arch)/widevinecdm.dll.lib',
            ],
            'widevine_cdm_manifest_file%': [
              'win/<(target_arch)/manifest.json',
            ],
          }],
        ],
      }],
      [ 'OS == "android"', {
        'widevine_cdm_version_h_file%':
            'android/widevine_cdm_version.h',
      }],
    ],
  },
  'includes': [
    '../../../build/util/version.gypi',
    '../../../media/cdm_paths.gypi',
  ],
  # Always provide a target, so we can put the logic about whether there's
  # anything to be done in this file (instead of a higher-level .gyp file).
  'targets': [
    {
      # GN version: //third_party/widevine/cdm:widevinecdmadapter_resources
      'target_name': 'widevinecdmadapter_resources',
      'type': 'none',
      'variables': {
        'output_dir': '.',
        'branding_path': '../../../chrome/app/theme/<(branding_path_component)/BRANDING',
        'template_input_path': '../../../chrome/app/chrome_version.rc.version',
        'extra_variable_files_arguments': [ '-f', 'BRANDING' ],
        'extra_variable_files': [ 'BRANDING' ], # NOTE: matches that above
      },
      'sources': [
        'widevinecdmadapter.ver',
      ],
      'includes': [
        '../../../chrome/version_resource_rules.gypi',
      ],
    },
    {
      'target_name': 'widevinecdmadapter_binary',
      'product_name': 'widevinecdmadapter',
      'type': 'none',
    },
    {
      # GN version: //third_party/widevine/cdm:widevinecdmadapter
      # On Mac this copies the widevinecdmadapter binary to
      # <(widevine_cdm_path). On all other platforms the binary is already
      # in <(widevine_cdm_path). See "product_dir" above.
      'target_name': 'widevinecdmadapter',
      'type': 'none',
      'dependencies': [
        'widevinecdmadapter_binary',
      ],
    },
    {
      # GN version: //third_party/widevine/cdm:version_h
      'target_name': 'widevine_cdm_version_h',
      'type': 'none',
      'copies': [{
        'destination': '<(SHARED_INTERMEDIATE_DIR)',
        'files': [ '<(widevine_cdm_version_h_file)' ],
      }],
    },
    {
      # GN version: //third_party/widevine/cdm:widevine_cdm_manifest
      'target_name': 'widevine_cdm_manifest',
      'type': 'none',
      'conditions': [
        [ 'widevine_cdm_manifest_file != []', {
          'copies': [{
            'destination': '<(PRODUCT_DIR)/WidevineCdm',
            'files': [ '<(widevine_cdm_manifest_file)' ],
          }],
        }],
      ],
    },
    {
      # GN version: //third_party/widevine/cdm:widevinecdm
      'target_name': 'widevinecdm',
      'type': 'none',
      'conditions': [
        [ 'branding == "Chrome"', {
          'conditions': [
            [ 'OS=="mac"', {
              'xcode_settings': {
                'COPY_PHASE_STRIP': 'NO',
              }
            }],
          ],
          'copies': [{
            'destination': '<(PRODUCT_DIR)/<(widevine_cdm_path)',
            'files': [ '<@(widevine_cdm_binary_files)' ],
          }],
        }],
      ],
    },
    {
      # GN version: //third_party/widevine/cdm:widevine_test_license_server
      'target_name': 'widevine_test_license_server',
      'type': 'none',
      'conditions': [
        [ 'branding == "Chrome" and OS == "linux"', {
          'dependencies': [
            '<(DEPTH)/third_party/widevine/test/license_server/license_server.gyp:test_license_server',
          ],
        }],
      ],
    },
  ],
}
