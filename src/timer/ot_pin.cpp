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

#include "ot_pin.h"

namespace OpenTimer {

// Constructor
Pin::Pin(string_crt key) :
  _name(key),
  _direction(UNDEFINED_PIN_DIRECTION),
  _use(UNDEFINED_PIN_USE),
  _mobility(UNDEFINED_MOBILITY),
  _orientation(UNDEFINED_ORIENTATION),
  _net_ptr(nullptr),
  _node_ptr(nullptr),
  _cellpin_ptr{nullptr, nullptr},
  _gate_ptr(nullptr),
  _rctree_node_ptr(nullptr),
  _primary_input_ptr(nullptr),
  _primary_output_ptr(nullptr),
  _pinlist_satellite(nullptr),
  _test_ptr(nullptr)
{
}

// Destructor
Pin::~Pin() {
}

// Function: is_constrained
// Query if the ot_pin.has a test object.
bool_t Pin::is_constrained() const {
  return test_ptr() != nullptr;
}

// Function: is_rctree_root
// Query if the pin is a rc root to a net.
bool_t Pin::is_rctree_root() const {
  return (direction() == PRIMARY_INPUT_PIN_DIRECTION || direction() == OUTPUT_CELLPIN_DIRECTION);
}

// Function: is_clock_sink
// Query if the pin is a clock sink pin.
bool_t Pin::is_clock_sink() const {
  if(!cellpin_ptr(EARLY)) return false;
  return cellpin_ptr(EARLY)->is_clock();
}

// Function: is_primary_input
bool_t Pin::is_primary_input() const {
  return primary_input_ptr() != nullptr;
}

// Function: is_primary_output
bool_t Pin::is_primary_output() const {
  return primary_output_ptr() != nullptr;
}

// Function: cap
// Query the pin capacitance. A pin belongs to three categories: 1) cellpin, 2) primary input,
// and 3) primary output. A cellot_pin.has only one type of capacitance. A primary input has no
// capacitance. The primary output has load capacitance.
float_t Pin::cap(int el, int rf) const {
  if(cellpin_ptr(el)) {
    return cellpin_ptr(el)->capacitance();
  }
  else if(primary_output_ptr()) {
    return primary_output_ptr()->load(el, rf);
  }
  return OT_FLT_ZERO;
}

// Function: load
// Query the load capacitance at the pin, which denotes the equivalent downstream capacitance
// seen from the pin of the cell. Several sophisticated models have been proposed for computing
// the load capacitance. For simplicity, the application of such models is considered to be
// out of the scope of the present contest, and a simple model is adopted. The load capacitance
// is assumed to be the sum of all the capacitances in the parasitic RC tree, including the cell
// pin capacitances at the taps of the interconnect network.
float_t Pin::load(int el, int rf) const {
  
  // Case 1: The pin is ever attached to a rctree node.
  if(rctree_node_ptr() != nullptr) return rctree_node_ptr()->load(el, rf);

  // Case 2: The pin is a rctree root.
  if(is_rctree_root()) return net_ptr() ? net_ptr()->total_leaf_cap(el, rf) : OT_FLT_ZERO;
  
  // Case 3: The pin is neither attached to a rctree nor a net.
  return cap(el, rf);
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------



