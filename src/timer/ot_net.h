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

#ifndef OT_NET_H_
#define OT_NET_H_

#include "ot_typedef.h"
#include "ot_pin.h"
#include "ot_rctree.h"

namespace OpenTimer {

// Net class.
class Net {

  public:
    
    Net(string_crt);                                            // Constructor.
    ~Net();                                                     // Destructor.

    void_t create_new_rctree();                                 // Create a rctree.
    void_t destroy_rctree();                                    // Destroy the rctree.
    void_t insert_pin_ptr(pin_pt);                              // Insert a pin pointer into the list.
    void_t remove_pin_ptr(pin_pt);                              // Remove a pin pointer from the list.
    void_t enable_rc_timing_update();                           // Enable the rc timing update. 
    void_t disable_rc_timing_update();                          // Disable the rc timing update.

    float_t HPWL() const;                                       // Query the half-parameter wirelength.
    float_t STWL() const;                                       // Query the Steiner wirelength.

    inline float_t total_leaf_cap(int, int) const;              // Query the load capacitance.

    inline string_crt name() const;                             // Query the net name.

    inline pinlist_pt pinlist_ptr() const;                      // Query the pinlist ptr.
    inline pinlist_rt pinlist() const;                          // Query the pinlist ref.

    inline rctree_pt rctree_ptr() const;                        // Query the rctree pointer.
    inline pin_pt root_pin_ptr() const;                         // Query the root pin pointer.

    inline size_t num_pins() const;                             // Query the number of pins.
    
    inline void_t dupdate_total_leaf_cap(int, int, float_ct);   // Add the load capacitance.
    inline void_t set_total_leaf_cap(int, int, float_ct);       // Set the load capacitance.
    inline void_t set_root_pin_ptr(pin_pt);                     // Set the root pin pointer.
    inline void_t set_rctree_ptr(rctree_pt);                    // Set the rctree pointer.

    bool_t has_primary_output() const;                          // Query the pinlist status.
    bool_t has_primary_input() const;                           // Query the pinlist status.
    
    Net & operator = (const Net &) = delete;                    // Disable copy assignment.

  private:
    
    string_t _name;                                             // Net name.
    
    float_t _total_leaf_cap[2][2];                              // Load capacitance of the net.

    pin_pt _root_pin_ptr;                                       // Root pin pointer of the rctree.
    rctree_pt _rctree_ptr;                                      // RC-tree pointer.
    pinlist_pt _pinlist_ptr;                                    // Pin ptr list type.
};

// Function: name
// Return the name of this net.
inline string_crt Net::name() const {
  return _name;
}

// Function: num_pins
inline size_t Net::num_pins() const {
  return _pinlist_ptr->size();
}

// Function: lcap
// Query the load capacitance.
inline float_t Net::total_leaf_cap(int el, int rf) const {
  return _total_leaf_cap[el][rf];
}

// Procedure: set_total_leaf_cap
// Set the load capacitance of this net.
inline void_t Net::set_total_leaf_cap(int el, int rf, float_ct cap) {
  _total_leaf_cap[el][rf] = cap;
}

// Procedure: dupdate_total_leaf_cap
// Update capacitance to the load capacitance of this net.
inline void_t Net::dupdate_total_leaf_cap(int el, int rf, float_ct delta) {
  if(delta != OT_FLT_ZERO) {
    _total_leaf_cap[el][rf] += delta;
    enable_rc_timing_update();
  }
}

// Procedure: set_root_pin_ptr
// Set the root pin pointer.
inline void_t Net::set_root_pin_ptr(pin_pt root_pin_ptr) {
  _root_pin_ptr = root_pin_ptr;
}

// Procedure: set_rctree_ptr
// Set the rctree pointer.
inline void_t Net::set_rctree_ptr(rctree_pt rctree_ptr) {
  _rctree_ptr = rctree_ptr;
}

// Function: root_pin_ptr
// Query the root pint pointer.
inline pin_pt Net::root_pin_ptr() const {
  return _root_pin_ptr;
}

// Function: pinlist_ptr
// Return the pointer to the pinlist.
inline pinlist_pt Net::pinlist_ptr() const {
  return _pinlist_ptr;
}

// Function: pinlist
// Return the ref to the pinlist.
inline pinlist_rt Net::pinlist() const {
  return *_pinlist_ptr;
}

// Function: rctree_ptr
// Return the rctree pointer.
inline rctree_pt Net::rctree_ptr() const {
  return _rctree_ptr;
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif

