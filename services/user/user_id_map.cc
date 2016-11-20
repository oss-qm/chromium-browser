// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "services/user/user_id_map.h"

#include <map>

#include "base/lazy_instance.h"

namespace user_service {

namespace {
base::LazyInstance<std::map<std::string, base::FilePath>>
    g_user_id_to_data_dir = LAZY_INSTANCE_INITIALIZER;
}  // namespace

void AssociateShellUserIdWithUserDir(const std::string& user_id,
                                     const base::FilePath& user_dir) {
  g_user_id_to_data_dir.Get()[user_id] = user_dir;
}

void ForgetShellUserIdUserDirAssociation(const std::string& user_id) {
  auto it = g_user_id_to_data_dir.Get().find(user_id);
  if (it != g_user_id_to_data_dir.Get().end())
    g_user_id_to_data_dir.Get().erase(it);
}

base::FilePath GetUserDirForUserId(const std::string& user_id) {
  auto it = g_user_id_to_data_dir.Get().find(user_id);
  DCHECK(it != g_user_id_to_data_dir.Get().end());
  return it->second;
}

}  // namespace user_service
