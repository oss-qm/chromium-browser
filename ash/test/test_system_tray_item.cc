// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/test/test_system_tray_item.h"

#include "ash/test/ash_test_base.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/view.h"

namespace ash {
namespace test {

TestSystemTrayItem::TestSystemTrayItem() : TestSystemTrayItem(UMA_TEST) {}

TestSystemTrayItem::TestSystemTrayItem(SystemTrayItem::UmaType uma_type)
    : SystemTrayItem(AshTestBase::GetPrimarySystemTray(), uma_type),
      has_views_(true),
      views_are_visible_(true),
      tray_view_(nullptr),
      default_view_(nullptr),
      detailed_view_(nullptr),
      notification_view_(nullptr) {}

TestSystemTrayItem::~TestSystemTrayItem() {}

views::View* TestSystemTrayItem::CreateTrayView(LoginStatus status) {
  if (!has_views_) {
    tray_view_ = nullptr;
    return tray_view_;
  }
  tray_view_ = new views::View;
  // Add a label so it has non-zero width.
  tray_view_->SetLayoutManager(new views::FillLayout);
  tray_view_->AddChildView(new views::Label(base::UTF8ToUTF16("Tray")));
  tray_view_->SetVisible(views_are_visible_);
  return tray_view_;
}

views::View* TestSystemTrayItem::CreateDefaultView(LoginStatus status) {
  if (!has_views_) {
    default_view_ = nullptr;
    return default_view_;
  }
  default_view_ = new views::View;
  default_view_->SetLayoutManager(new views::FillLayout);
  default_view_->AddChildView(new views::Label(base::UTF8ToUTF16("Default")));
  default_view_->SetVisible(views_are_visible_);
  return default_view_;
}

views::View* TestSystemTrayItem::CreateDetailedView(LoginStatus status) {
  if (!has_views_) {
    detailed_view_ = nullptr;
    return detailed_view_;
  }
  detailed_view_ = new views::View;
  detailed_view_->SetLayoutManager(new views::FillLayout);
  detailed_view_->AddChildView(new views::Label(base::UTF8ToUTF16("Detailed")));
  detailed_view_->SetVisible(views_are_visible_);
  return detailed_view_;
}

views::View* TestSystemTrayItem::CreateNotificationView(LoginStatus status) {
  if (!has_views_) {
    notification_view_ = nullptr;
    return notification_view_;
  }
  notification_view_ = new views::View;
  notification_view_->SetVisible(views_are_visible_);
  return notification_view_;
}

void TestSystemTrayItem::DestroyTrayView() {
  tray_view_ = nullptr;
}

void TestSystemTrayItem::DestroyDefaultView() {
  default_view_ = nullptr;
}

void TestSystemTrayItem::DestroyDetailedView() {
  detailed_view_ = nullptr;
}

void TestSystemTrayItem::DestroyNotificationView() {
  notification_view_ = nullptr;
}

void TestSystemTrayItem::UpdateAfterLoginStatusChange(LoginStatus status) {}

}  // namespace test
}  // namespace ash
