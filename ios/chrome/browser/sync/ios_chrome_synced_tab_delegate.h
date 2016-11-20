// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SYNC_TAB_CONTENTS_SYNCED_TAB_DELEGATE_H_
#define CHROME_BROWSER_UI_SYNC_TAB_CONTENTS_SYNCED_TAB_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "components/sessions/core/session_id.h"
#include "components/sync_sessions/synced_tab_delegate.h"
#include "ios/web/public/web_state/web_state_user_data.h"

class IOSChromeSyncedTabDelegate
    : public browser_sync::SyncedTabDelegate,
      public web::WebStateUserData<IOSChromeSyncedTabDelegate> {
 public:
  ~IOSChromeSyncedTabDelegate() override;

  // SyncedTabDelegate:
  SessionID::id_type GetWindowId() const override;
  SessionID::id_type GetSessionId() const override;
  bool IsBeingDestroyed() const override;
  std::string GetExtensionAppId() const override;
  bool IsInitialBlankNavigation() const override;
  int GetCurrentEntryIndex() const override;
  int GetEntryCount() const override;
  GURL GetVirtualURLAtIndex(int i) const override;
  GURL GetFaviconURLAtIndex(int i) const override;
  ui::PageTransition GetTransitionAtIndex(int i) const override;
  void GetSerializedNavigationAtIndex(
      int i,
      sessions::SerializedNavigationEntry* serialized_entry) const override;
  bool ProfileIsSupervised() const override;
  const std::vector<const sessions::SerializedNavigationEntry*>*
  GetBlockedNavigations() const override;
  bool IsPlaceholderTab() const override;
  int GetSyncId() const override;
  void SetSyncId(int sync_id) override;
  bool ShouldSync(sync_sessions::SyncSessionsClient* sessions_client) override;

 private:
  explicit IOSChromeSyncedTabDelegate(web::WebState* web_state);
  friend class web::WebStateUserData<IOSChromeSyncedTabDelegate>;

  web::WebState* web_state_;
  int sync_session_id_;

  DISALLOW_COPY_AND_ASSIGN(IOSChromeSyncedTabDelegate);
};

#endif  // CHROME_BROWSER_UI_SYNC_TAB_CONTENTS_SYNCED_TAB_DELEGATE_H_
