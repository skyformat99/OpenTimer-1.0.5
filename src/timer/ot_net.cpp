/******************************************************************************
 *                                                                            *
 * Copyright (c) 2015, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#include "ot_net.h"

namespace OpenTimer {

// Constructor
Net::Net(string_crt key):
  _name(key),
  _total_leaf_cap{{OT_DEFAULT_NET_LCAP, OT_DEFAULT_NET_LCAP}, 
                  {OT_DEFAULT_NET_LCAP, OT_DEFAULT_NET_LCAP}},
  _root_pin_ptr(nullptr),
  _rctree_ptr(nullptr),
  _pinlist_ptr(new pinlist_t())
{
}

// Destructor
Net::~Net() {
  destroy_rctree();
  delete _pinlist_ptr;
}

// Procedure: destroy_rctree
void_t Net::destroy_rctree() {
  if(_rctree_ptr != nullptr) {
    delete _rctree_ptr;
    _rctree_ptr = nullptr;
  }
}

// Procedure: insert_pin_ptr
// Insert a pin pointer into the net.
void_t Net::insert_pin_ptr(pin_pt pin_ptr) {

  CHECK(pin_ptr != nullptr);

  // Insert the pin into the pinlist.
  pinlist_iter_t satellite = pinlist_ptr()->push_back(pin_ptr);
  pin_ptr->set_pinlist_satellite(satellite);
  pin_ptr->set_net_ptr(this);

  // Set the root pin.
  if(pin_ptr->is_rctree_root()) set_root_pin_ptr(pin_ptr);
  else {
    EL_RF_ITER(el, rf) {
      dupdate_total_leaf_cap(el, rf, pin_ptr->cap(el, rf));
    }
  }

  CHECK(pin_ptr->rctree_node_ptr() == nullptr);

  // Enable RC timing update.
  enable_rc_timing_update();
}

// Procedure: remove_pin_ptr
// Remove a pin pointer from the net.
void_t Net::remove_pin_ptr(pin_pt pin_ptr) {

  CHECK(pin_ptr != nullptr);

  // Reset the root pin pointer.
  if(pin_ptr == root_pin_ptr()) set_root_pin_ptr(nullptr);

  // Remove the pin from the pinlist.
  pinlist_ptr()->remove(pin_ptr->pinlist_satellite());
  pin_ptr->set_pinlist_satellite(nullptr);     
  pin_ptr->set_net_ptr(nullptr);                   

  // Remove the pin capacitance from the net load.
  if(!(pin_ptr->is_rctree_root())) {
    EL_RF_ITER(el, rf) {
      dupdate_total_leaf_cap(el, rf, -pin_ptr->cap(el, rf));
    }
  }

  // Deattach the pin from the rctree node.
  if(pin_ptr->rctree_node_ptr()) {
    pin_ptr->rctree_node_ptr()->set_pin_ptr(nullptr);
    pin_ptr->set_rctree_node_ptr(nullptr);
  }
  
  // Enable RC timing update.
  enable_rc_timing_update();
}

// Procedure: create_new_rctree
void_t Net::create_new_rctree() {
  destroy_rctree();
  _rctree_ptr = new rctree_t();
  enable_rc_timing_update();
}

// Procedure: enable_rc_timing_update
void_t Net::enable_rc_timing_update() {
  if(rctree_ptr() == nullptr) return;
  rctree_ptr()->set_is_rc_timing_updated(false);
}

// Procedure: disable_rc_timing_update
void_t Net::disable_rc_timing_update() {
  if(rctree_ptr() == nullptr) return;
  rctree_ptr()->set_is_rc_timing_updated(true);
}

// Function: has_primary_input
bool_t Net::has_primary_input() const {
  for(const auto& pin_ptr : pinlist()) {
    if(pin_ptr->is_primary_input()) return true;
  }
  return false;
}

// Function: has_primary_output
bool_t Net::has_primary_output() const {
  for(const auto& pin_ptr : pinlist()) {
    if(pin_ptr->is_primary_output()) return true;
  }
  return false;
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------


