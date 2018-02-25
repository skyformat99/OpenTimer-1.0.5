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

#ifndef OT_PIN_H_
#define OT_PIN_H_

#include "ot_utility.h"
#include "ot_typedef.h"
#include "ot_celllib.h"
#include "ot_test.h"
#include "ot_geometry.h"

namespace OpenTimer {

// Pin class.
class Pin {

  public:
    
    Pin(string_crt);                                                        // Constructor.
    ~Pin();                                                                 // Destructor.

    inline string_crt name() const;                                         // Return the pin name.
    inline pin_direction_e direction() const;                               // Query the pin direction.
    inline pin_use_e use() const;                                           // Query the pin use type.
    inline mobility_e mobility() const;                                     // Query the mobility.
    inline orientation_e orientation() const;                               // Query the orientation.
    inline net_pt net_ptr() const;                                          // Query the net pointer.
    inline node_pt node_ptr() const;                                        // Query the node pointer.
    inline cellpin_pt cellpin_ptr(int) const;                               // Query the cellpin pointer.
    inline gate_pt gate_ptr() const;                                        // Query the gate pointer.
    inline rctree_node_pt rctree_node_ptr() const;                          // Query the rctree node pointer.
    inline primary_input_pt primary_input_ptr() const;                      // Query the primary input pointer.
    inline primary_output_pt primary_output_ptr() const;                    // Query the primary output pointer.
    inline pinlist_iter_t pinlist_satellite() const;                        // Query the satellite on the plist.
    inline test_pt test_ptr() const;                                        // Query the endpointer.
    inline point_crt location() const;                                      // Query the location.

    inline void_t set_location(point_t::value_t, point_t::value_t);         // Set the pin location.
    inline void_t set_location(point_crt);                                  // Set the pin location.
    inline void_t set_direction(pin_direction_ce);                          // Set the pin direction.
    inline void_t set_use(pin_use_ce);                                      // Set the pin use.
    inline void_t set_mobility(mobility_ce);                                // Set the pin mobility.
    inline void_t set_orientation(orientation_ce);                          // Set the pin orientation.
    inline void_t set_cellpin_ptr(int, cellpin_pt);                         // Set the cell pin pointer.
    inline void_t set_gate_ptr(gate_pt);                                    // Set the gate pointer.
    inline void_t set_net_ptr(net_pt);                                      // Set the net pointer.
    inline void_t set_node_ptr(node_pt);                                    // Set the node pointer.
    inline void_t set_rctree_node_ptr(rctree_node_pt);                      // Set the rctree node pointer.
    inline void_t set_primary_input_ptr(primary_input_pt);                  // Set the primary input pointer.
    inline void_t set_primary_output_ptr(primary_output_pt);                // Set the primary output pointer.
    inline void_t set_pinlist_satellite(pinlist_iter_t);                    // Set the satellite.
    inline void_t set_test_ptr(test_pt);                                    // Set the endpointer.

    float_t cap(int, int) const;                                            // Query the pin capacitance.
    float_t load(int, int) const;                                           // Query the load capacitance.

    bool_t is_rctree_root() const;                                          // Query if the pin is a rc root.
    bool_t is_constrained() const;                                          // Query if the pin is a constrained pin.
    bool_t is_clock_sink() const;                                           // Query if the pin is a clock sink pin.
    bool_t is_primary_input() const;                                        // Query if the pin is a primary input pin.
    bool_t is_primary_output() const;                                       // Query if the pin is a primary output pin.
    
    Pin & operator = (const Pin &) = delete;                                // Disable copy assignment.

  private:
    
    string_t _name;                                                         // Pin name.
    point_t _location;                                                      // Pin location.
    
    pin_direction_e _direction;                                             // Pin direction.
    pin_use_e _use;                                                         // Pin use type.
    mobility_e _mobility;                                                   // Pin mobility.
    orientation_e _orientation;                                             // Pin orientation.
    
    net_pt _net_ptr;                                                        // Net pointer.
    node_pt _node_ptr;                                                      // Node pointer.
    cellpin_pt _cellpin_ptr[2];                                             // Cellpin pointer.
    gate_pt _gate_ptr;                                                      // Gate pointer.
    rctree_node_pt _rctree_node_ptr;                                        // RC tree node pointer.
    primary_input_pt _primary_input_ptr;                                    // Primary input port.
    primary_output_pt _primary_output_ptr;                                  // Primary output port.
    pinlist_iter_t _pinlist_satellite;                                      // Satellite pointer.
    test_pt _test_ptr;                                                      // Constraint pointer.
};

// Function: name
// Return the pin name.
inline string_crt Pin::name() const {
  return _name;
}

// Procedure: set_cellpin_ptr
// Set the cellpin pointer.
inline void_t Pin::set_cellpin_ptr(int el, cellpin_pt cellpin_ptr) {
  _cellpin_ptr[el] = cellpin_ptr;
}

// Procedure: set_gate_ptr
// Set the gate pointer.
inline void_t Pin::set_gate_ptr(gate_pt gate_ptr) {
  _gate_ptr = gate_ptr;
}

// Procedure: set_direction
// Set the pin direction.
inline void_t Pin::set_direction(pin_direction_ce direction) {
  _direction = direction;
}

// Procedure: set_location
// Set the pin location.
inline void_t Pin::set_location(point_crt location) {
  _location = location; 
}

// Procedure: set_location
// Set the pin location.
inline void_t Pin::set_location(point_t::value_t x, point_t::value_t y) {
  _location.set_x(x);
  _location.set_y(y);
}

// Procedure: set_mobility
// Set the pin mobility.
inline void_t Pin::set_mobility(mobility_ce mobility) {
  _mobility = mobility;
}

// Procedure: set_rctree_node_ptr
// Set the rctree node pointer.
inline void_t Pin::set_rctree_node_ptr(rctree_node_pt rctree_node_ptr) {
  _rctree_node_ptr = rctree_node_ptr;
}

// Procedure: set_primary_input_ptr
// Set the primary input pointer.
inline void_t Pin::set_primary_input_ptr(primary_input_pt primary_input_ptr) {
  _primary_input_ptr = primary_input_ptr;
}

// Procedure: set_primary_output_ptr
// Set the primary output pointer.
inline void_t Pin::set_primary_output_ptr(primary_output_pt primary_output_ptr) {
  _primary_output_ptr = primary_output_ptr;
}

// Procedure: set_test_ptr
// Set the endpointer.
inline void_t Pin::set_test_ptr(test_pt test_ptr) {
  _test_ptr = test_ptr;
}

// Procedure: set_net_ptr
// Set the net pointer.
inline void_t Pin::set_net_ptr(net_pt net_ptr) {
  _net_ptr = net_ptr;
}

// Procedure: set_node_ptr
// Set the node pointer.
inline void_t Pin::set_node_ptr(node_pt node_ptr) {
  _node_ptr = node_ptr;
}

// Procedure: set_orientation
inline void_t Pin::set_orientation(orientation_ce orientation) {
  _orientation = orientation;
}

// Procedure: set_pinlist_satellite
// Set the satellite.
inline void_t Pin::set_pinlist_satellite(pinlist_iter_t pinlist_satellite) {
  _pinlist_satellite = pinlist_satellite;
}

// Procedure: set_use
// Set the pin use.
inline void_t Pin::set_use(pin_use_ce use) {
  _use = use; 
}

// Function: direction
// Return the pin direction
inline pin_direction_e Pin::direction() const {
  return _direction;
}

// Function: mobility
// Return the mobility.
inline mobility_e Pin::mobility() const {
  return _mobility;
}

// Function: use
// Return the pin use.
inline pin_use_e Pin::use() const {
  return _use;
}

// Function: orientation
// Return the orientation.
inline orientation_e Pin::orientation() const {
  return _orientation;
}

// Function: cellpin_ptr
// Return the cellpin pointer.
inline cellpin_pt Pin::cellpin_ptr(int el) const {
  return _cellpin_ptr[el];
}

// Function: rctree_node_ptr
// Return the pointer to a rctree node.
inline rctree_node_pt Pin::rctree_node_ptr() const {
  return _rctree_node_ptr;
}

// Function: node_ptr
// Return the node pointer.
inline node_pt Pin::node_ptr() const {
  return _node_ptr;
}

// Function: gate_ptr
// Return the gate pointer.
inline gate_pt Pin::gate_ptr() const {
  return _gate_ptr;
}

// Function: pinlist_satellite
// Return the pinlist satellite
inline pinlist_iter_t Pin::pinlist_satellite() const {
  return _pinlist_satellite;
}

// Function: primary_input_ptr
// Return the primary input pointer.
inline primary_input_pt Pin::primary_input_ptr() const {
  return _primary_input_ptr;
}

// Function: primary_output_ptr
// Return the primary output pointer.
inline primary_output_pt Pin::primary_output_ptr() const {
  return _primary_output_ptr;
}

// Function: location
// Return the location.
inline point_crt Pin::location() const {
  return _location;
}

// Function: net_ptr
// Return the net pointer
inline net_pt Pin::net_ptr() const {
  return _net_ptr;
}

// Function: test_ptr
// Return the endpointer.
inline test_pt Pin::test_ptr() const {
  return _test_ptr;
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif


