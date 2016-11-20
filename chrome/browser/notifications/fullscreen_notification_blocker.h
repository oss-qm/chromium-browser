// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_FULLSCREEN_NOTIFICATION_BLOCKER_H_
#define CHROME_BROWSER_NOTIFICATIONS_FULLSCREEN_NOTIFICATION_BLOCKER_H_

#include "base/macros.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/message_center/notification_blocker.h"

// A notification blocker which checks the fullscreen state.
class FullscreenNotificationBlocker
    : public message_center::NotificationBlocker,
      public content::NotificationObserver {
 public:
  explicit FullscreenNotificationBlocker(
      message_center::MessageCenter* message_center);
  ~FullscreenNotificationBlocker() override;

  bool is_fullscreen_mode() const { return is_fullscreen_mode_; }

  // message_center::NotificationBlocker overrides:
  void CheckState() override;
  bool ShouldShowNotificationAsPopup(
      const message_center::NotifierId& notifier_id) const override;

 private:
  // content::NotificationObserver override.
  void Observe(int type,
               const content::NotificationSource& source,
               const content::NotificationDetails& details) override;

  bool is_fullscreen_mode_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenNotificationBlocker);
};

#endif  // CHROME_BROWSER_NOTIFICATIONS_FULLSCREEN_NOTIFICATION_BLOCKER_H_
