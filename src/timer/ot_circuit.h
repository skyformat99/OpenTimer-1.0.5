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

#ifndef OT_CIRCOT_H_
#define OT_CIRCOT_H_

#include "ot_headerdef.h"
#include "ot_utility.h"
#include "ot_classdef.h"
#include "ot_macrodef.h"
#include "ot_typedef.h"
#include "ot_pin.h"
#include "ot_net.h"
#include "ot_gate.h"
#include "ot_io_port.h"
#include "ot_clock.h"
#include "ot_dictionary.h"
#include "ot_lefreader.h"
#include "ot_defreader.h"

namespace OpenTimer{

// Class: Circuit
class Circuit {

  public:

    Circuit();                                                                  // Constructor.
    ~Circuit();                                                                 // Destructor.

    void_t clear();                                                             // Clear the object.

    inline string_crt name() const;                                             // Query the circuit name.

    inline rectangle_crt die() const;                                           // Query the die rectangle.

    inline void_t set_die(rectangle_crt);                                       // Set the die rectangle.
    inline void_t set_die(point_crt, point_crt);                                // Set the die rectangle.

    inline pin_pt pin_ptr(string_crt) const;                                    // Query the pin pointer.
    inline pin_dict_pt pin_dict_ptr() const;                                    // Query the pin dictionary pointer.

    inline net_pt net_ptr(string_crt) const;                                    // Query the net pointer.
    inline net_dict_pt net_dict_ptr() const;                                    // Query the net dictionary pointer.

    inline gate_pt gate_ptr(string_crt) const;                                  // Query the gate pointer.
    inline gate_dict_pt gate_dict_ptr() const;                                  // Query the gate dictionary pointer.

    inline clock_tree_pt clock_tree_ptr() const;                                // Query the clock source pointer.

    inline primary_input_pt primary_input_ptr(string_crt) const;                // Query the pi pointer.
    inline primary_input_dict_pt primary_input_dict_ptr() const;                // Query the pi dictionary pointer.
    inline primary_input_dict_rt primary_input_dict() const;                    // Query the pi dictionary pointer.
    inline primary_output_pt primary_output_ptr(string_crt) const;              // Query the po pointer.
    inline primary_output_dict_pt primary_output_dict_ptr() const;              // Query the po dictionary pointer.
    inline primary_output_dict_rt primary_output_dict() const;                  // Query the po dictionary pointer.

    inline testset_pt testset_ptr() const;                                      // Query the testset pointer.
    inline testset_rt testset() const;                                          // Query the testset reference.

    inline size_t num_pins() const;                                             // Query the pin count.
    inline size_t num_nets() const;                                             // Query the net count.

    inline size_t num_gates() const;                                            // Query the gate count.
    inline size_t num_primary_inputs() const;                                   // Query the PI count.
    inline size_t num_primary_outputs() const;                                  // Query the PO count.
    inline size_t num_tests() const;                                            // Query the test count.
  
    void_t remove_pin(string_crt) const;                                        // Remove a pin.
    void_t remove_net(string_crt) const;                                        // Remove a net.
    void_t remove_gate(string_crt) const;                                       // Remove an gate.
    void_t remove_primary_input(string_crt) const;                              // Remove a primary input.
    void_t remove_primary_output(string_crt) const;                             // Remove a primary output.
    void_t remove_test(test_pt) const;                                          // Remove a test.

    pin_pt insert_pin(string_crt) const;                                        // Insert a pin.
    pin_pt clock_tree_root_pin_ptr() const;                                     // Query the pin of the clock tree root.

    net_pt insert_net(string_crt) const;                                        // Insert a net.

    gate_pt insert_gate(string_crt) const;                                      // Insert an gate.

    primary_input_pt insert_primary_input(string_crt) const;                    // Insert a primary input.
    primary_output_pt insert_primary_output(string_crt) const;                  // Insert a primary output.

    test_pt insert_test(string_crt, string_crt, timing_arc_pt[]) const;         // Insert a test.
    test_pt insert_test(pin_pt, pin_pt, timing_arc_pt[]) const;                 // Insert a test.
    test_pt insert_test(pin_pt) const;                                          // Insert a test.

  private:
    
    string_t _name;                                                             // Name of the circuit.
    rectangle_t _die;                                                           // Die box.
    
    pin_dict_upt _pin_dict_uptr;                                                // Pin dictionary.
    net_dict_upt _net_dict_uptr;                                                // Net dictionary.
    gate_dict_upt _gate_dict_uptr;                                              // Gate dictionary.
    primary_input_dict_upt _primary_input_dict_uptr;                            // Primary input dictionary.
    primary_output_dict_upt _primary_output_dict_uptr;                          // Primary output dictionary.
    clock_tree_upt _clock_tree_uptr;                                            // Clock source.
    testset_upt _testset_uptr;                                                  // Constraint set.
};

// Function: name
// Return the pointer to the circuit name.
inline string_crt Circuit::name() const {
  return _name;
}

// Function: die
// Query the die rectangle.
inline rectangle_crt Circuit::die() const {
  return _die;
}

// Procedure: set_die
// Set the die rectangle.
inline void_t Circuit::set_die(rectangle_crt rec) {
  _die = rec;
}

// Procedure: set_die
// Set the die rectangle.
inline void_t Circuit::set_die(point_crt llp, point_crt urp) {
  _die.set_lower_left_point(llp);
  _die.set_upper_right_point(urp);
}

// Function: pin_ptr
// Query the pin pointer.
inline pin_pt Circuit::pin_ptr(string_crt name) const {
  return (*pin_dict_ptr())[name];
}

// Function: net_ptr
// Query the net pointer.
inline net_pt Circuit::net_ptr(string_crt name) const {
  return (*net_dict_ptr())[name];
}

// Function: gate_ptr
inline gate_pt Circuit::gate_ptr(string_crt name) const {
  return (*gate_dict_ptr())[name];
}

// Function: primary_input_ptr
inline primary_input_pt Circuit::primary_input_ptr(string_crt name) const {
  return (*primary_input_dict_ptr())[name];
}

// Function: primary_output_ptr
inline primary_output_pt Circuit::primary_output_ptr(string_crt name) const {
  return (*primary_output_dict_ptr())[name];
}

// Function: primary_input_dict_ptr
inline primary_input_dict_pt Circuit::primary_input_dict_ptr() const {
  return _primary_input_dict_uptr.get();
}

// Function: primary_input_dict
inline primary_input_dict_rt Circuit::primary_input_dict() const {
  return *primary_input_dict_ptr();
}

// Function: primary_output_dict_ptr
inline primary_output_dict_pt Circuit::primary_output_dict_ptr() const {
  return _primary_output_dict_uptr.get();
}

// Function: primary_output_dict
inline primary_output_dict_rt Circuit::primary_output_dict() const {
  return *primary_output_dict_ptr();
}

// Function: pin_dict_ptr
// Query the pin dictionary pointer.
inline pin_dict_pt Circuit::pin_dict_ptr() const {
  return _pin_dict_uptr.get();
}

// Function: net_dict_ptr
// Query the net dictionary pointer.
inline net_dict_pt Circuit::net_dict_ptr() const {
  return _net_dict_uptr.get();
}

// Function: isntance_dict_ptr
// Query the gate dictionary pointer.
inline gate_dict_pt Circuit::gate_dict_ptr() const {
  return _gate_dict_uptr.get();
}

// Function: clock_tree_ptr
inline clock_tree_pt Circuit::clock_tree_ptr() const {
  return _clock_tree_uptr.get();
}

// Function: num_pins
// Query the number of pins.
inline size_t Circuit::num_pins() const {
  return pin_dict_ptr()->size();
}

// Function: num_nets
// Query the number of nets.
inline size_t Circuit::num_nets() const {
  return net_dict_ptr()->size();
}

// Function: num_gates
// Query the number of gates.
inline size_t Circuit::num_gates() const {
  return gate_dict_ptr()->size();
}

// Function: num_primary_inputs
// Query the number of primary inputs.
inline size_t Circuit::num_primary_inputs() const {
  return primary_input_dict_ptr()->size();
}

// Function: num_primary_outputs
// Query the number of primary outputs
inline size_t Circuit::num_primary_outputs() const {
  return primary_output_dict_ptr()->size();
}

// Function: num_tests
// Query the number of tests.
inline size_t Circuit::num_tests() const {
  return testset_ptr()->size();
}

// Function: testset_ptr
// Return the pointer to the test set.
inline testset_pt Circuit::testset_ptr() const {
  return _testset_uptr.get();
}

// Function: testset
// Return the reference to the test set.
inline testset_rt Circuit::testset() const {
  return *testset_ptr();
}

};  // end of uit namespace.

#endif


