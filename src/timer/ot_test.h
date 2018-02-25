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

#ifndef OT_TEST_H_
#define OT_TEST_H_

#include "ot_utility.h"
#include "ot_typedef.h"
#include "ot_classdef.h"
#include "ot_pin.h"
#include "ot_node.h"
#include "ot_path.h"

namespace OpenTimer {

// Class: Endpoint
class Endpoint {

  public:

    Endpoint();                                                            // Constructor.
    ~Endpoint();                                                           // Destructor.

    inline int el() const;                                                 // Query the timing split.
    inline int rf() const;                                                 // Query the transition status. 
    
    inline test_pt test_ptr() const;                                       // Query the endpoint.
    
    inline void_t set_el(int);                                             // Set the el.
    inline void_t set_rf(int);                                             // Set the rf.
    inline void_t set_slack(float_ct);                                     // Set the slack.
    inline void_t set_test_ptr(test_pt);                                   // Set the test pointer.
    inline void_t set_min_heap_satellite(int_ct);                          // Set the heap satellite.      

    inline float_t slack() const;                                          // Query the slack value.

    void_t update_rat(clock_tree_pt);                                      // Update the rat.
    void_t update_slack();                                                 // Update the slack.

    bool_t is_in_pipeline() const;                                         // Query the pipeline status.
    bool_t is_sequential_constraint() const;                               // Query the test status.

    pin_pt constrained_pin_ptr() const;                                    // Query constrained pin ptr.
    pin_pt related_pin_ptr() const;                                        // Query related pin ptr.

    timing_arc_pt timing_arc_ptr() const;                                  // Query the timing arc.

    int_t min_heap_satellite() const;                                      // Query the heap satellite.

  private:

    int _el;                                                               // Early/Late.
    int _rf;                                                               // Rise/Fall.

    float_t _slack;                                                        // Slack.

    test_pt _test_ptr;                                                     // Test pointer.

    int_t _min_heap_satellite;                                             // Satellite of min heap.
};

// Function: el
// Query the timing split.
inline int Endpoint::el() const {
  return _el;
}

// Function: rf
// Query the timing transition.
inline int Endpoint::rf() const {
  return _rf;
}

// Function: test_ptr
// Query the endpointer.
inline test_pt Endpoint::test_ptr() const {
  return _test_ptr;
}

// Procedure: set_el
// Set the timing split.
inline void_t Endpoint::set_el(int el) {
  _el = el;
}

// Procedure: set_rf
// Set the transition.
inline void_t Endpoint::set_rf(int rf) {
  _rf = rf;
}

// Procedure: set_test_ptr
// Set the test pointer.
inline void_t Endpoint::set_test_ptr(test_pt test_ptr) {
  _test_ptr = test_ptr;
}

// Procedure: set_slack
// Set the slack of the endpoint.
inline void_t Endpoint::set_slack(float_ct slack) {
  _slack = slack;
}

// Procedure: set_min_heap_satellite
inline void_t Endpoint::set_min_heap_satellite(int_ct satellite) {
  _min_heap_satellite = satellite;
}

// Function: slack
// Query the slack of the endpoint.
inline float_t Endpoint::slack() const {
  return _slack;
}

// Function: min_heap_satellite
inline int_t Endpoint::min_heap_satellite() const {
  return _min_heap_satellite;
}

//-------------------------------------------------------------------------------------------------

// Class: Test
// This class specifies the timing test (i.e., regular PO rat and sequential setup/hold rat).
// Hold test is the early test (i.e., latest rat and earliest at) and setup test
// is the late test (i.e., earliest rat and latest at).
class Test {
  
  public:
    
    Test();                                                             // Constructor.
    ~Test();                                                            // Destructor.
    
    bool_t is_in_pipeline() const;                                      // Query the pipeline status.

    inline bool_t is_sequential_constraint() const;                     // Query the constraint status.

    inline void_t set_idx(int_t);                                       // Set the idx.
    inline void_t set_rat(int, int, float_ct);                          // Set the rat value.
    inline void_t set_timing_arc_ptr(int, timing_arc_pt);               // Set the timing arc.
    inline void_t set_related_pin_ptr(pin_pt);                          // Set the pin pointer.
    inline void_t set_constrained_pin_ptr(pin_pt);                      // Set the pin pointer.
    inline void_t set_slack(int, int, float_ct);                        // Set the slack.

    inline pin_pt related_pin_ptr() const;                              // Query the related pin pointer.
    inline pin_pt constrained_pin_ptr() const;                          // Query the constrained pin pointer.

    inline int_t idx() const;                                           // Query the idx.

    inline float_t rat(int, int) const;                                 // Query the rat value.
    inline float_t slack(int, int) const;                               // Query the slack.

    inline endpoint_pt endpoint_ptr(int, int);                          // Query the endpoint pointer.

    inline timing_arc_pt timing_arc_ptr(int) const;                     // Query the timing arc pointer. 

    void_t update_rat(clock_tree_pt);                                   // Update the rat.
    void_t update_rat(int, int, clock_tree_pt);                         // Update the rat.

    node_pt constrained_node_ptr() const;                               // Node for D pin.
    node_pt related_node_ptr() const;                                   // Node for CK pin.

  private:

    int_t _idx;                                                         // Test index.

    float_t _rat[2][2];

    endpoint_t _endpoint[2][2];                                         // Endpoints.

    pin_pt _constrained_pin_ptr;                                        // Constrained pin pointer.
    pin_pt _related_pin_ptr;                                            // Related pin pointer.

    timing_arc_pt _timing_arc_ptr[2];                                   // Constraint timing arc.
};

// Procedure: set_constrained_pin_ptr
// Set the constrained pin pointer.
inline void_t Test::set_constrained_pin_ptr(pin_pt pin_ptr) {
  _constrained_pin_ptr = pin_ptr;
}

// Procedure: set_related_pin_ptr
// Set the related pin pointer.
inline void_t Test::set_related_pin_ptr(pin_pt pin_ptr) {
  _related_pin_ptr = pin_ptr;
}

// Procedure: set_timing_arc_ptr
// Set the test timing arc.
inline void_t Test::set_timing_arc_ptr(int el, timing_arc_pt timing_arc_ptr) {
  _timing_arc_ptr[el] = timing_arc_ptr;
}

// Procedure: set_idx
// Set the test satellite pointer.
inline void_t Test::set_idx(int_t idx) {
  _idx = idx;
}

// Procedure: set_slack
// Set the slack.
inline void_t Test::set_slack(int el, int rf, float_ct slack) {
  _endpoint[el][rf].set_slack(slack);
}

// Function: is_sequential_constraint
// Query the constraint status.
inline bool_t Test::is_sequential_constraint() const {
  return related_pin_ptr() != NULL;
}

// Function: timing_arc_ptr
// Query the timing arc pointer.
inline timing_arc_pt Test::timing_arc_ptr(int el) const {
  return _timing_arc_ptr[el];
}

// Function: related_pin_ptr
// Query the related pin pointer.
inline pin_pt Test::related_pin_ptr() const {
  return _related_pin_ptr;
}

// Function: constrained_pin_ptr
// Query the constrained pin pointer.
inline pin_pt Test::constrained_pin_ptr() const {
  return _constrained_pin_ptr;
}

// Function: idx
// Query the idx
inline int_t Test::idx() const {
  return _idx;
}

// Function: rat
// Query the rat value.
inline float_t Test::rat(int el, int rf) const {
  return _rat[el][rf];
}

// Procedure: set_rat
inline void_t Test::set_rat(int el, int rf, float_ct val) {
  _rat[el][rf] = val;
}

// Function: slack
// Query the slack value.
inline float_t Test::slack(int el, int rf) const {
  return _endpoint[el][rf].slack();
}

// Function: endpoint_ptr
// Query the endpoint pointer.
inline endpoint_pt Test::endpoint_ptr(int el, int rf) {
  return &_endpoint[el][rf];
}

//-------------------------------------------------------------------------------------------------

// Class: EndpointMinHeapKV
class EndpointMinHeapKV {
  
  public: 
  
    inline EndpointMinHeapKV();                             // Constructor.
    inline EndpointMinHeapKV(float_ct, endpoint_pt);        // Constructor.
    inline ~EndpointMinHeapKV();                            // Destructor.

    inline float_t key() const;                             // Return the key.
    inline endpoint_pt value() const;                       // Return the value. 
      
    inline void_t set_key(float_ct);                        // Set the key.
    inline void_t set_value(endpoint_pt);                   // Set the value.

  private:

    float_t     _key;                                       // Key.
    endpoint_pt _value;                                     // Value.
};

// Constructor.
inline EndpointMinHeapKV::EndpointMinHeapKV() {
}

// Constructor.
inline EndpointMinHeapKV::EndpointMinHeapKV(float_ct key, endpoint_pt value):
  _key(key),
  _value(value)
{
}

// Destructor.
inline EndpointMinHeapKV::~EndpointMinHeapKV() {
}

// Function: key
inline float_t EndpointMinHeapKV::key() const {
  return _key;
}

// Function: value
inline endpoint_pt EndpointMinHeapKV::value() const {
  return _value;
}

// Procedure: set_key
inline void_t EndpointMinHeapKV::set_key(float_ct key) {
  _key = key;
}

// Procedure: set_value
inline void_t EndpointMinHeapKV::set_value(endpoint_pt value) {
  _value = value;
}

//-------------------------------------------------------------------------------------------------

// Class: EndpointMinHeap
class EndpointMinHeap {

  public:

    EndpointMinHeap();                                          // Constructor.
    ~EndpointMinHeap();                                         // Destructor.
    
    inline size_t size() const;                                 // Return the size of PQ.
    inline endpoint_pt top() const;                             // Return the maximum element.

    void_t insert(endpoint_pt);                                 // Insert an item into the PQ.
    void_t pop();                                               // Extract the maximum element.
    void_t update(endpoint_pt);                                 // Update an item.
    void_t top(size_t, endpoint_ptr_vrt);                       // Find the top k elements.
    void_t remove(endpoint_pt);                                 // Remove a given endpoint.

  private:

    size_t _N;                                                  // Heap size.

    vector <EndpointMinHeapKV> _heap;                           // Heap array.

    void_t _bubble_up(int_t);                                   // Heapify up.
    void_t _bubble_down(int_t);                                 // Heapify down.
    void_t _swap(int_t, int_t);                                 // Swap two items.
    void_t _pop_back();                                         // Pop the last item.
    void_t _remove(int_t);                                      // Remove the ith item.
};

// Function: size
// Return the size of min-max heap.
inline size_t EndpointMinHeap::size() const {
  return _N;
}

// Function: top_max
inline endpoint_pt EndpointMinHeap::top() const {
  return (size() >= 1) ? _heap[1].value() : NULL;
}

};


#endif




