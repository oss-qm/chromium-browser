// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/common/wm/mru_window_tracker.h"

#include "ash/aura/wm_window_aura.h"
#include "ash/common/shell_window_ids.h"
#include "ash/common/wm/window_state.h"
#include "ash/common/wm_shell.h"
#include "ash/shell.h"
#include "ash/test/ash_test_base.h"
#include "ash/wm/window_state_aura.h"
#include "ash/wm/window_util.h"
#include "ui/aura/window.h"
#include "ui/base/hit_test.h"
#include "ui/views/widget/widget.h"

namespace ash {

class MruWindowTrackerTest : public test::AshTestBase {
 public:
  MruWindowTrackerTest() {}
  ~MruWindowTrackerTest() override {}

  aura::Window* CreateWindow() {
    return CreateTestWindowInShellWithBounds(gfx::Rect(0, 0, 400, 400));
  }

  ash::MruWindowTracker* mru_window_tracker() {
    return WmShell::Get()->mru_window_tracker();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MruWindowTrackerTest);
};

// Basic test that the activation order is tracked.
TEST_F(MruWindowTrackerTest, Basic) {
  std::unique_ptr<aura::Window> w1(CreateWindow());
  std::unique_ptr<aura::Window> w2(CreateWindow());
  std::unique_ptr<aura::Window> w3(CreateWindow());
  wm::ActivateWindow(w3.get());
  wm::ActivateWindow(w2.get());
  wm::ActivateWindow(w1.get());

  aura::Window::Windows window_list =
      WmWindowAura::ToAuraWindows(mru_window_tracker()->BuildMruWindowList());
  EXPECT_EQ(w1.get(), window_list[0]);
  EXPECT_EQ(w2.get(), window_list[1]);
  EXPECT_EQ(w3.get(), window_list[2]);
}

// Test that minimized windows are considered least recently used (but kept in
// correct relative order).
TEST_F(MruWindowTrackerTest, MinimizedWindowsAreLru) {
  std::unique_ptr<aura::Window> w1(CreateWindow());
  std::unique_ptr<aura::Window> w2(CreateWindow());
  std::unique_ptr<aura::Window> w3(CreateWindow());
  std::unique_ptr<aura::Window> w4(CreateWindow());
  std::unique_ptr<aura::Window> w5(CreateWindow());
  std::unique_ptr<aura::Window> w6(CreateWindow());
  wm::ActivateWindow(w6.get());
  wm::ActivateWindow(w5.get());
  wm::ActivateWindow(w4.get());
  wm::ActivateWindow(w3.get());
  wm::ActivateWindow(w2.get());
  wm::ActivateWindow(w1.get());

  wm::GetWindowState(w1.get())->Minimize();
  wm::GetWindowState(w4.get())->Minimize();
  wm::GetWindowState(w5.get())->Minimize();

  // Expect the relative order of minimized windows to remain the same, but all
  // minimized windows to be at the end of the list.
  aura::Window::Windows window_list =
      WmWindowAura::ToAuraWindows(mru_window_tracker()->BuildMruWindowList());
  EXPECT_EQ(w2.get(), window_list[0]);
  EXPECT_EQ(w3.get(), window_list[1]);
  EXPECT_EQ(w6.get(), window_list[2]);
  EXPECT_EQ(w1.get(), window_list[3]);
  EXPECT_EQ(w4.get(), window_list[4]);
  EXPECT_EQ(w5.get(), window_list[5]);
}

// Tests that windows being dragged are only in the WindowList once.
TEST_F(MruWindowTrackerTest, DraggedWindowsInListOnlyOnce) {
  std::unique_ptr<aura::Window> w1(CreateWindow());
  wm::ActivateWindow(w1.get());

  // Start dragging the window.
  wm::GetWindowState(w1.get())->CreateDragDetails(
      gfx::Point(), HTRIGHT, aura::client::WINDOW_MOVE_SOURCE_TOUCH);

  // During a drag the window is reparented by the Docked container.
  aura::Window* drag_container = Shell::GetContainer(
      Shell::GetTargetRootWindow(), kShellWindowId_DockedContainer);
  drag_container->AddChild(w1.get());
  EXPECT_TRUE(wm::GetWindowState(w1.get())->is_dragged());

  // The dragged window should only be in the list once.
  aura::Window::Windows window_list = WmWindowAura::ToAuraWindows(
      mru_window_tracker()->BuildWindowListIgnoreModal());
  EXPECT_EQ(1, std::count(window_list.begin(), window_list.end(), w1.get()));
}

// Tests that windows with propery of |kExcludeFromMruKey|==true are not in the
// window list.
TEST_F(MruWindowTrackerTest, ExcludedFormMru) {
  std::unique_ptr<aura::Window> w1(CreateWindow());
  std::unique_ptr<aura::Window> w2(CreateWindow());
  std::unique_ptr<aura::Window> w3(CreateWindow());

  wm::GetWindowState(w1.get())->SetExcludedFromMru(true);
  wm::GetWindowState(w3.get())->SetExcludedFromMru(true);

  wm::ActivateWindow(w3.get());
  wm::ActivateWindow(w2.get());
  wm::ActivateWindow(w1.get());

  // Expect the windows with |kExcludeFromMruKey| property being true are not
  // in the list.
  aura::Window::Windows window_list =
      WmWindowAura::ToAuraWindows(mru_window_tracker()->BuildMruWindowList());

  EXPECT_EQ(window_list.end(),
            std::find(window_list.begin(), window_list.end(), w1.get()));
  EXPECT_EQ(window_list.end(),
            std::find(window_list.begin(), window_list.end(), w3.get()));
  EXPECT_EQ(1u, window_list.size());
}

}  // namespace ash
