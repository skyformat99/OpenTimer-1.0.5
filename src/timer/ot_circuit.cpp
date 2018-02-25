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

#include "ot_circuit.h"

namespace OpenTimer {

// Constructor
Circuit::Circuit():
  _pin_dict_uptr(new pin_dict_t(OT_DEFAULT_PIN_DICTIONARY_SIZE)),
  _net_dict_uptr(new net_dict_t(OT_DEFAULT_NET_DICTIONARY_SIZE)),
  _gate_dict_uptr(new gate_dict_t(OT_DEFAULT_INSTANCE_DICTIONARY_SIZE)),
  _primary_input_dict_uptr (new primary_input_dict_t()),
  _primary_output_dict_uptr(new primary_output_dict_t()),
  _clock_tree_uptr(new clock_tree_t()),
  _testset_uptr(new testset_t())
{
}

// Destructor
Circuit::~Circuit() {
}

// Function: insert_gate
// Insert an gate into the circuit.
gate_pt Circuit::insert_gate(string_crt name) const {
  gate_pt gate_ptr = gate_dict_ptr()->insert(name, name);
  return gate_ptr;
}

// Function: insert_net
// Insert a net into the circuit.
net_pt Circuit::insert_net(string_crt name) const {
  net_pt net_ptr = net_dict_ptr()->insert(name, name);
  return net_ptr;
}

// Function: insert_pin
// Insert a pin into the circuit.
pin_pt Circuit::insert_pin(string_crt name) const {
  pin_pt pin_ptr = pin_dict_ptr()->insert(name, name);
  return pin_ptr;
}

// Function: insert_primary_input
// Insert an primary input into the circuit.
primary_input_pt Circuit::insert_primary_input(string_crt name) const {
  primary_input_pt pi_ptr = primary_input_dict_ptr()->insert(name, name);
  return pi_ptr;
}

// Function: insert_primary_output
// Insert an primary output into the circuit.
primary_output_pt Circuit::insert_primary_output(string_crt name) const {
  primary_output_pt po_ptr = primary_output_dict_ptr()->insert(name, name);
  return po_ptr;
}

// Function: clock_tree_root_pin_ptr
// Query the pin of the clock tree root.
pin_pt Circuit::clock_tree_root_pin_ptr() const {
  return clock_tree_ptr()->root_pin_ptr();
}

// Function: insert_test
// Insert a test into the circuit.
test_pt Circuit::insert_test(string_crt from_pin_name, string_crt to_pin_name, timing_arc_pt timing_arc_ptr[]) const {
  return insert_test(pin_ptr(from_pin_name), pin_ptr(to_pin_name), timing_arc_ptr); 
}

// Function: insert_test
// Insert a test into the circuit.
test_pt Circuit::insert_test(pin_pt from_pin_ptr, pin_pt to_pin_ptr, timing_arc_pt timing_arc_ptr[]) const {

  CHECK(from_pin_ptr && to_pin_ptr && timing_arc_ptr[EARLY] && timing_arc_ptr[LATE]);

  int_t idx = testset_ptr()->insert();
  test_pt test_ptr = testset()[idx];
  test_ptr->set_idx(idx);
  test_ptr->set_related_pin_ptr(from_pin_ptr);
  test_ptr->set_constrained_pin_ptr(to_pin_ptr);
  EL_ITER(el) {
    test_ptr->set_timing_arc_ptr(el, timing_arc_ptr[el]);
  }
  to_pin_ptr->set_test_ptr(test_ptr);

  return test_ptr;
}

// Fcunction: insert_test
// Insert a test into the circuit. The test is a RAT type which comes from a primary output.
test_pt Circuit::insert_test(pin_pt po_pin_ptr) const {

  CHECK(po_pin_ptr != nullptr);

  int_t idx = testset_ptr()->insert();
  test_pt test_ptr = testset()[idx];
  test_ptr->set_idx(idx);
  test_ptr->set_constrained_pin_ptr(po_pin_ptr);
  po_pin_ptr->set_test_ptr(test_ptr);

  return test_ptr;
}

// Procedure: remove_pin
void_t Circuit::remove_pin(string_crt name) const {
  pin_dict_ptr()->remove(name);
}

// Procedure: remove_net
void_t Circuit::remove_net(string_crt name) const {
  net_dict_ptr()->remove(name);
}

// Procedure: remove_gate
void_t Circuit::remove_gate(string_crt name) const {
  gate_dict_ptr()->remove(name);
}

// Procedure: remove_primary_input
void_t Circuit::remove_primary_input(string_crt name) const {
  primary_input_dict_ptr()->remove(name);
}

// Procedure: remove_output_input
void_t Circuit::remove_primary_output(string_crt name) const {
  primary_output_dict_ptr()->remove(name);
}

// Procedure: remove_test
void_t Circuit::remove_test(test_pt test_ptr) const {
  testset_ptr()->remove(test_ptr->idx());
}


};





