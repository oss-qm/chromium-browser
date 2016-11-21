// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/media/cdm/render_cdm_factory.h"

#include <memory>

#include "base/bind.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/single_thread_task_runner.h"
#include "base/threading/thread_task_runner_handle.h"
#include "media/base/cdm_config.h"
#include "media/base/cdm_promise.h"
#include "media/base/key_systems.h"
#include "media/base/media_keys.h"
#include "media/cdm/aes_decryptor.h"
#include "url/gurl.h"

namespace content {

RenderCdmFactory::~RenderCdmFactory() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void RenderCdmFactory::Create(
    const std::string& key_system,
    const GURL& security_origin,
    const media::CdmConfig& cdm_config,
    const media::SessionMessageCB& session_message_cb,
    const media::SessionClosedCB& session_closed_cb,
    const media::LegacySessionErrorCB& legacy_session_error_cb,
    const media::SessionKeysChangeCB& session_keys_change_cb,
    const media::SessionExpirationUpdateCB& session_expiration_update_cb,
    const media::CdmCreatedCB& cdm_created_cb) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!security_origin.is_valid()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::Bind(cdm_created_cb, nullptr, "Invalid origin."));
    return;
  }

  if (media::CanUseAesDecryptor(key_system)) {
    // TODO(sandersd): Address this now that prefixed EME has been removed.
    // http://crbug.com/249976. The prefixed API always allowed distinctive
    // identifiers and persistent state. Once that changes we can sanity check
    // here that neither is allowed for AesDecryptor, since it does not support
    // them and should never be configured that way. http://crbug.com/455271
    scoped_refptr<media::MediaKeys> cdm(
        new media::AesDecryptor(security_origin, session_message_cb,
                                session_closed_cb, session_keys_change_cb));
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::Bind(cdm_created_cb, cdm, ""));
    return;
  }
}

}  // namespace content
