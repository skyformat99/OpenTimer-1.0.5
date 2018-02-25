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

#ifndef OT_GATE_H_
#define OT_GATE_H_

#include "ot_utility.h"
#include "ot_typedef.h"

namespace OpenTimer {

// Gate class.
class Gate {

  public:
    
    Gate(string_crt);                                                       // Constructor.
    ~Gate();                                                                // Destructor.
    
    inline string_crt name() const;                                         // Return the pin name.
    
    inline cell_pt cell_ptr() const;                                        // Query the default cell pointer.
    inline cell_pt cell_ptr(int) const;                                     // Query the cell pointer.

    inline point_crt location() const;                                      // Query the location.
    
    inline mobility_e mobility() const;                                     // Query the mobility.
    inline orientation_e orientation() const;                               // Query the orientation.
    inline gate_source_e source() const;                                    // Query the gate source.

    inline float_t weight() const;                                          // Query the weight.

    inline void_t set_weight(float_ct);                                     // Set the weight.
    inline void_t set_cell_ptr(int, cell_pt);                               // Set the cell pointer.
    inline void_t set_source(gate_source_ce);                               // Set the gate source.
    inline void_t set_mobility(mobility_ce);                                // Set the mobility.
    inline void_t set_orientation(orientation_ce);                          // Set the orientation.
    inline void_t set_location(point_crt);                                  // Set the location.
    inline void_t set_location(point_t::value_t, point_t::value_t);         // Set the location.

    Gate & operator = (const Gate &) = delete;                              // Disable copy constructor.
    
  private:
    
    string_t _name;                                                         // Gate name.
    point_t _location;                                                      // Location.

    float_t _weight;                                                        // Weight.

    cell_pt _cell_ptr[2];                                                   // Cell pointer.

    mobility_e _mobility;                                                   // Mobility.
    orientation_e _orientation;                                             // Orientation.
    gate_source_e _source;                                                  // Gate source.

};

// Function: name
// Return the name of this gate.
inline string_crt Gate::name() const {
  return _name;
}

// Procedure: set_source
// Set the source of the gate.
inline void_t Gate::set_source(gate_source_ce source) {
  _source = source;
}

// Function: orientation
// Return the orientation.
inline orientation_e Gate::orientation() const {
  return _orientation;
}

// Function: source
// Return the gate source.
inline gate_source_e Gate::source() const {
  return _source;
}

// Function: cell_ptr
// Return the cell pointer.
inline cell_pt Gate::cell_ptr(int el) const {
  return _cell_ptr[el];
}

// Function: cell_ptr
// Return the default cell pointer.
inline cell_pt Gate::cell_ptr() const {
  return cell_ptr(EARLY);
}

// Procedure: set_cell_ptr
// Set the cell pointer.
inline void_t Gate::set_cell_ptr(int el, cell_pt cell_ptr) {
  _cell_ptr[el] = cell_ptr;
}

// Procedure: set_mobility
// Set the mobility.
inline void_t Gate::set_mobility(mobility_ce mobility) {
  _mobility = mobility;
}

// Procedure: set_orientation
// Set the orientation.
inline void_t Gate::set_orientation(orientation_ce orientation) {
  _orientation = orientation;
}

// Function: location
// Query the location.
inline point_crt Gate::location() const {
  return _location;
}

// Procedure: set_location
// Set the location.
inline void_t Gate::set_location(point_t::value_t x, point_t::value_t y) {
  _location.set_x(x);
  _location.set_y(y);
}

// Procedure: set_location
// Set the location.
inline void_t Gate::set_location(point_crt p) {
  _location = p;
}

// Procedure: set_weight
// Set the weight.
inline void_t Gate::set_weight(float_ct weight) {
  _weight = weight;
}

// Function: weight
// Query the weight.
inline float_t Gate::weight() const {
  return _weight;
}

// Function: mobility
// Return the mobility.
inline mobility_e Gate::mobility() const {
  return _mobility;
}

};                                                          // End of uit namespace.

#endif

