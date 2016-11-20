// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/input/scroll_state_data.h"

namespace cc {

ScrollStateData::ScrollStateData()
    : delta_x(0),
      delta_y(0),
      position_x(0),
      position_y(0),
      velocity_x(0),
      velocity_y(0),
      is_beginning(false),
      is_in_inertial_phase(false),
      is_ending(false),
      should_propagate(false),
      from_user_input(false),
      delta_consumed_for_scroll_sequence(false),
      is_direct_manipulation(false),
      delta_granularity(0),
      caused_scroll_x(false),
      caused_scroll_y(false),
      current_native_scrolling_node_(nullptr) {}

ScrollStateData::ScrollStateData(const ScrollStateData& other) = default;

ScrollNode* ScrollStateData::current_native_scrolling_node() const {
  return current_native_scrolling_node_;
}

void ScrollStateData::set_current_native_scrolling_node(
    ScrollNode* current_native_scrolling_node) {
  current_native_scrolling_node_ = current_native_scrolling_node;
  current_native_scrolling_element_ = ElementId();
}

ElementId ScrollStateData::current_native_scrolling_element() const {
  if (current_native_scrolling_node_)
    return current_native_scrolling_node_->data.element_id;
  return current_native_scrolling_element_;
}

void ScrollStateData::set_current_native_scrolling_element(
    ElementId element_id) {
  current_native_scrolling_element_ = element_id;
  current_native_scrolling_node_ = nullptr;
}

}  // namespace cc
