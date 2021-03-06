# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Presubmit script for changes affecting content/test/gpu/page_sets/.

See http://dev.chromium.org/developers/how-tos/depottools/presubmit-scripts
for more details about the presubmit API built into depot_tools.
"""

import os
import sys


def _GetChromiumSrcDir(input_api):
  return input_api.os_path.abspath(input_api.os_path.join(
      input_api.PresubmitLocalPath(), '..', '..', '..', '..'))


def _CheckWprShaFiles(input_api, output_api):
  """Check whether the wpr sha files have matching URLs."""
  old_sys_path = sys.path
  try:
    catapult_base_path = input_api.os_path.join(
        _GetChromiumSrcDir(input_api), 'third_party', 'catapult',
        'catapult_base')
    sys.path.insert(1, catapult_base_path)
    from catapult_base import cloud_storage  # pylint: disable=import-error
  finally:
    sys.path = old_sys_path

  results = []
  for affected_file in input_api.AffectedFiles(include_deletes=False):
    filename = affected_file.AbsoluteLocalPath()
    if not filename.endswith('wpr.sha1'):
      continue
    expected_hash = cloud_storage.ReadHash(filename)
    is_wpr_file_uploaded = any(
        cloud_storage.Exists(bucket, expected_hash)
        for bucket in cloud_storage.BUCKET_ALIASES.itervalues())
    if not is_wpr_file_uploaded:
      wpr_filename = filename[:-5]
      results.append(output_api.PresubmitError(
          'The file matching %s is not in Cloud Storage yet.\n'
          'You can upload your new WPR archive file with the command:\n'
          'depot_tools/upload_to_google_storage.py --bucket '
          '<Your pageset\'s bucket> %s.\nFor more info: see '
          'http://www.chromium.org/developers/telemetry/'
          'record_a_page_set#TOC-Upload-the-recording-to-Cloud-Storage' %
          (filename, wpr_filename)))
  return results


def _CheckJson(input_api, output_api):
  """Checks whether JSON files in this change can be parsed."""
  for affected_file in input_api.AffectedFiles(include_deletes=False):
    filename = affected_file.AbsoluteLocalPath()
    if os.path.splitext(filename)[1] != '.json':
      continue
    try:
      input_api.json.load(open(filename))
    except ValueError:
      return [output_api.PresubmitError('Error parsing JSON in %s!' % filename)]
  return []


def _CommonChecks(input_api, output_api):
  """Performs common checks, which includes running pylint."""
  results = []
  results.extend(_CheckWprShaFiles(input_api, output_api))
  results.extend(_CheckJson(input_api, output_api))
  return results


def CheckChangeOnUpload(input_api, output_api):
  results = []
  results.extend(_CommonChecks(input_api, output_api))
  return results


def CheckChangeOnCommit(input_api, output_api):
  results = []
  results.extend(_CommonChecks(input_api, output_api))
  return results
