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

#include "ot_test.h"

namespace OpenTimer {

// Constructor.
Test::Test() {

  EL_ITER(el) {
    RF_ITER(rf) {
      _endpoint[el][rf].set_el(el);
      _endpoint[el][rf].set_rf(rf);
      _endpoint[el][rf].set_test_ptr(this);
      _endpoint[el][rf].set_slack(OT_DEFAULT_TEST_SLACK);
      _rat[el][rf] = (el == EARLY) ? OT_FLT_MIN : OT_FLT_MAX;
    }
    _timing_arc_ptr[el] = nullptr; 
  }

  _idx = OT_UNDEFINED_IDX;
  _related_pin_ptr = nullptr;
  _constrained_pin_ptr = nullptr;
}

// Destructor.
Test::~Test() {

}

// Function: is_in_pipeline
// Query if the test is currently residing in the pipeline.
bool_t Test::is_in_pipeline() const {
  return constrained_node_ptr()->is_in_pipeline();
}

// Function: constrained_node_ptr
// Query the node pointer to the constrained pin.
node_pt Test::constrained_node_ptr() const {
  return constrained_pin_ptr() ? constrained_pin_ptr()->node_ptr() : nullptr;
}

// Function: related_node_ptr
// Query the node pointer to the constrained pin.
node_pt Test::related_node_ptr() const {
  return related_pin_ptr() ? related_pin_ptr()->node_ptr() : nullptr;
}

// Procedure: update_rat
// Update the required arrival time on this test. This procedure takes place only for constrained
// RAT (i.e., setup or hold time constraint).
void_t Test::update_rat(clock_tree_pt clock_tree_ptr) {
  EL_RF_ITER(el, rf) {
    update_rat(el, rf, clock_tree_ptr); 
  }
}

// Procedure: update_rat
// Update the required arrival time on this test. This procedure takes place only for constrained
// RAT (i.e., setup or hold time constraint). Notice that the rat is the pre-cppr rat.
void_t Test::update_rat(int el, int rf, clock_tree_pt clock_tree_ptr) {

  CHECK(constrained_pin_ptr() != nullptr);
  
  // Case 1: primary output test.
  if(related_pin_ptr() == nullptr) return;

  // Case 2: sequential test.
  float_t related_slew;                               // Clock slew.
  float_t related_at;                                 // Clock at.
  float_t constrained_slew;                           // Data pin slew.
  float_t constraint;                                 // Constraint value.
  float_t period;                                     // Clock period.

  int_t related_el;
  int_t related_rf;

  node_pt D = constrained_pin_ptr()->node_ptr();
  node_pt CK = related_pin_ptr()->node_ptr();

  period = clock_tree_ptr == nullptr ? OT_FLT_ZERO : clock_tree_ptr->period();

  CHECK(CK->is_clock_sink() == true);
  
  // Obtain the constrained RAT from the setup/hold constraint and then adjust it by the cppr 
  // credit coming from the worst post-cppr slack path.
  CHECK(timing_arc_ptr(el) != nullptr && timing_arc_ptr(el)->is_constraint());

  constrained_slew = D->slew(el, rf);
  related_el = !el;
  related_rf = timing_arc_ptr(el)->is_rising_edge_triggered() ? RISE : FALL;
        
  // Obtain the arrival time and slew at the clock pin that triggers the setup/hold
  // test and infer the corresponding constraint value. Here the RAT value does not 
  // consider the CPPR.
  related_at = CK->at(related_el, related_rf);
  related_slew = CK->slew(related_el, related_rf);
  constraint = timing_arc_ptr(el)->constraint(related_rf, rf, related_slew, constrained_slew);

  if(el == EARLY) set_rat(el, rf, constraint + related_at);
  else set_rat(el, rf, related_at + period - constraint);
}

//-------------------------------------------------------------------------------------------------

// Constructor: Endpoint
Endpoint::Endpoint():
  _min_heap_satellite(OT_UNDEFINED_IDX)
{

}

// Destructor: ~Endpoint
Endpoint::~Endpoint() {

}

// Function: constrained_pin_ptr
// Query the constrained pin pointer.
pin_pt Endpoint::constrained_pin_ptr() const {
  return test_ptr()->constrained_pin_ptr();
}

// Function: related_pin_ptr
// Query the constrained pin pointer.
pin_pt Endpoint::related_pin_ptr() const {
  return test_ptr()->related_pin_ptr();
}

// Function: timing_arc_ptr
// Query the timing arc pointer.
timing_arc_pt Endpoint::timing_arc_ptr() const {
  return test_ptr()->timing_arc_ptr(el());
}

// Function: is_in_pipeline
// Query the pipeline status.
bool_t Endpoint::is_in_pipeline() const {
  CHECK(test_ptr() != nullptr);
  return test_ptr()->is_in_pipeline();
}

// Function: is_sequential_constraint
bool_t Endpoint::is_sequential_constraint() const {
  CHECK(test_ptr() != nullptr);
  return test_ptr()->is_sequential_constraint();
}

// Procedure: update_slack
// Update the slack with respect to the RAT of the test.
void_t Endpoint::update_slack() {

  switch(el()) {
    case EARLY:
      set_slack(constrained_pin_ptr()->node_ptr()->at(el(), rf()) - test_ptr()->rat(el(), rf()));
    break;
    
    case LATE:
      set_slack(test_ptr()->rat(el(), rf()) - constrained_pin_ptr()->node_ptr()->at(el(), rf()));
    break;

    default:
      CHECK(false);
    break;
  }
}

//-------------------------------------------------------------------------------------------------

// Constructor
EndpointMinHeap::EndpointMinHeap() :
  _N(0),
  _heap(1, EndpointMinHeapKV(OT_FLT_MAX, nullptr))
{
}

// Destructor
EndpointMinHeap::~EndpointMinHeap() {

}

// Procedure: _swap
void_t EndpointMinHeap::_swap(int_t i, int_t j) {
  _heap[i].value()->set_min_heap_satellite(j);
  _heap[j].value()->set_min_heap_satellite(i);
  swap(_heap[i], _heap[j]);
}

// Procedure: insert
void_t EndpointMinHeap::insert(endpoint_pt ptr) {
 
  CHECK(ptr->min_heap_satellite() == OT_UNDEFINED_IDX);
  
  // Insert a new item and set the satellite.
  ++_N;
  _heap.push_back(EndpointMinHeapKV(ptr->slack(), ptr));
  ptr->set_min_heap_satellite(size());

  // Heapify up.
  _bubble_up(ptr->min_heap_satellite());
}

// Procedure: bubble_up
void_t EndpointMinHeap::_bubble_up(int_t i) {
  int_t p(i>>1);
  while(p && _heap[i].key() < _heap[p].key()) { 
    _swap(i, p); 
    i = p; 
    p >>= 1;
  }
}

// Procedure: bubble_down
void_t EndpointMinHeap::_bubble_down(int_t i) {
  
  if(i > (int_t)(size()>>1)) return;

  // Perform the heapify.
  int_t l(0), r(0), s(0);

  while(1) {
    l = i << 1;
    r = l |  1;
    s = (l <= (int_t)size() && _heap[l].key() < _heap[i].key()) ? l : i;
    s = (r <= (int_t)size() && _heap[r].key() < _heap[s].key()) ? r : s;
    if(s == i) break;
    _swap(i, s);
    i = s;
  }
}

// Procedure: top
// Extract the top k items to a given vector array. This won't affect the original
// heap structure.
void_t EndpointMinHeap::top(size_t K, endpoint_ptr_vrt endpoint_ptrs) {
  endpoint_ptrs.clear();
  endpoint_ptrs.reserve(K);
  while(size() && endpoint_ptrs.size() < K) {
    endpoint_ptrs.push_back(top());
    pop();
  }
  for(unsigned_t i=0; i<endpoint_ptrs.size(); i++) {
    insert(endpoint_ptrs[i]);
  }
}

// Procedure: _remove
// Remove an item given an index from the heap.
void_t EndpointMinHeap::_remove(int_t idx) {
  _swap(idx, size());
  _pop_back();
  _bubble_down(idx);
}

// Procedure: pop
// Pop the top item (the one with the smallest slack values in the heap) from the heap.
void_t EndpointMinHeap::pop() {
  
  if(size() == 0) return;
            
  // Swap the first and last items, then pop the last item and perform the heapify
  // to adjust the heap.
  _remove(1);
}

// Procedure: remove
// Remove an item from the heap.
void_t EndpointMinHeap::remove(endpoint_pt ptr) {
  
  CHECK(ptr != nullptr);

  // The endpoint is not in the heap.
  if(ptr->min_heap_satellite() == OT_UNDEFINED_IDX) return;

  CHECK(size() != 0);

  // Swap the first and last items, then pop the last item and perform the heapify
  // to adjust the heap.
  _remove(ptr->min_heap_satellite());
}

// Procedure: pop_back
void_t EndpointMinHeap::_pop_back() {
  _heap.back().value()->set_min_heap_satellite(OT_UNDEFINED_IDX);
  _heap.pop_back();
  --_N;
}

// Procedure: update
// Update an item.
void_t EndpointMinHeap::update(endpoint_pt ptr) {
  
  CHECK(ptr != nullptr);

  // Case 1: the given pointer is not in the heap.
  if(ptr->min_heap_satellite() == OT_UNDEFINED_IDX) {
    insert(ptr);
  }
  // Case 2: heapify the item.
  else {
    _heap[ptr->min_heap_satellite()].set_key(ptr->slack());
    _bubble_up(ptr->min_heap_satellite());
    _bubble_down(ptr->min_heap_satellite());
  }
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------




