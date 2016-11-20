// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_WINDOW_FINDER_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_WINDOW_FINDER_H_

#include <set>

#include "ui/gfx/native_widget_types.h"

namespace aura {
class Window;
}

namespace gfx {
class Point;
}

class WindowFinder {
 public:
  WindowFinder();
  virtual ~WindowFinder();

  virtual gfx::NativeWindow GetLocalProcessWindowAtPoint(
      const gfx::Point& screen_point,
      const std::set<gfx::NativeWindow>& ignore);

 private:
  DISALLOW_COPY_AND_ASSIGN(WindowFinder);
};

#endif  // CHROME_BROWSER_UI_VIEWS_TABS_WINDOW_FINDER_H_
