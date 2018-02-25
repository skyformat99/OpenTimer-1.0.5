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

#include "ot_node.h"

namespace OpenTimer {

// Constructor
Node::Node() {

  EL_RF_ITER(el, rf) {
    if(el == EARLY) {
      _slew[el][rf] = OT_FLT_MAX;
      _at  [el][rf] = OT_FLT_MAX;
      _rat [el][rf] = OT_FLT_MIN;
    }
    else {
      _slew[el][rf] = OT_FLT_MIN;
      _at  [el][rf] = OT_FLT_MIN;
      _rat [el][rf] = OT_FLT_MAX;
    }
    _is_at_clocked[el][rf] = false;
    _at_parent_rf[el][rf] = UNDEFINED_TRANS;
    _at_parent_ptr[el][rf] = nullptr;
  }

  _idx = OT_UNDEFINED_IDX;
  _clock_tree_node_idx = OT_UNDEFINED_IDX;
  _level = OT_DEFAULT_NODE_LEVEL;

  _pin_ptr = nullptr;
  _pipeline_satellite = nullptr;
  _nodeset_ptr = nullptr;

  _fanin_ptr = new edgelist_t();
  _fanout_ptr = new edgelist_t();
  _jumpin_ptr = new jumplist_t();
  _jumpout_ptr = new jumplist_t();
}

// Destructor.
Node::~Node() {
  delete _fanin_ptr;
  delete _fanout_ptr;
  delete _jumpin_ptr;
  delete _jumpout_ptr;
}

// Procedure: remove_timing
// Remove the timing on nodes.
void_t Node::remove_timing() {
  EL_RF_ITER(el, rf) {
    remove_slew(el, rf);
    remove_at(el, rf);
    remove_rat(el, rf);
  }
}

// Procedure: remove_slew
// Remove the slew on nodes.
void_t Node::remove_slew() {
  EL_RF_ITER(el, rf) {
    remove_slew(el, rf);
  }
}

// Procedure: remove_at
// Remove the at on nodes.
void_t Node::remove_at() {
  EL_RF_ITER(el, rf) {
    remove_at(el, rf);
  }
}

// Procedure: remove_rat
// Remove the rat on nodes.
void_t Node::remove_rat() {
  EL_RF_ITER(el, rf) {
    remove_rat(el, rf);
  }
}

// Procedure: remove_fanin
// Remove an edge from the fanin of the node.
void_t Node::remove_fanin(edge_pt edge_ptr) {

  if(edge_ptr == nullptr) return;

  fanin_ptr()->remove(edge_ptr->fanin_satellite());
  edge_ptr->set_fanin_satellite(nullptr);
}

// Procedure: remove_fanout
// Remove an edge from the fanout of the node.
void_t Node::remove_fanout(edge_pt edge_ptr) {

  if(edge_ptr == nullptr) return;

  fanout_ptr()->remove(edge_ptr->fanout_satellite());
  edge_ptr->set_fanout_satellite(nullptr);
}

// Procedure: remove_jumpin
// Remove a jump from the fanin jumps of the node.
void_t Node::remove_jumpin(jump_pt jump_ptr) {
  
  if(jump_ptr == nullptr) return;
  
  jumpin_ptr()->remove(jump_ptr->jumpin_satellite());
  jump_ptr->set_jumpin_satellite(nullptr);
}

// Procedure: remove_jumpout
// Remove a jump from the fanout jumps of the node.
void_t Node::remove_jumpout(jump_pt jump_ptr) {
  
  if(jump_ptr == nullptr) return;

  jumpout_ptr()->remove(jump_ptr->jumpout_satellite());
  jump_ptr->set_jumpout_satellite(nullptr);
}

// Function: is_constrained
// Return true if the node is constrained by a test.
bool_t Node::is_constrained() const {
  return pin_ptr()->is_constrained();
}
    
// Function: is_clock_tree_leaf
bool_t Node::is_clock_tree_leaf() const {
  return is_in_clock_tree() && (num_fanouts() == 0 || is_clock_sink());
}

// Function: is_in_clock_tree
// Return true if the node is attached to a clock tree.
bool_t Node::is_in_clock_tree() const {
  return clock_tree_node_idx() != OT_UNDEFINED_IDX;
}

// Function: is_clock_sink
// Return true if the node is a clock sink node and false otherwise.
bool_t Node::is_clock_sink() const {
  return pin_ptr()->is_clock_sink();
}

// Function: is_primary_input
// Return true if the node is attached to a pi pin.
bool_t Node::is_primary_input() const {
  return pin_ptr()->is_primary_input();
}

// Function: is_primary_output
// Return true if the node is attached to a po pin.
bool_t Node::is_primary_output() const {
  return pin_ptr()->is_primary_output();
}

// Function: is_rctree_root
// Return true if the node is attached to a root pin.
bool_t Node::is_rctree_root() const {
  return pin_ptr()->is_rctree_root();
}

// Function: is_data_path_source
// Return true if the node is attached to a data source pin.
bool_t Node::is_data_path_source() const {
  return num_fanins() == 0 || pin_ptr()->is_clock_sink() || pin_ptr()->is_primary_input();
}

// Function: is_jump_head
// Return true if the node is a head of the jump.
bool_t Node::is_jump_head() const {
  if(num_fanouts() == 0 || num_fanouts() > 1 || is_clock_tree_leaf() /*|| is_in_clock_tree()*/) return true;
  return fanout_ptr()->head()->item()->timing_sense() == NON_UNATE;
}

// Function: is_jump_tail
// Return true if the node is a tail of the jump.
bool_t Node::is_jump_tail() const {
  if(num_fanins() == 0) return true;
  for(EdgelistIter i(fanin_ptr()); i(); ++i) {
    if(i.edge_ptr()->edge_type() == CONSTRAINT_EDGE_TYPE) continue;
    if(i.from_node_ptr()->is_jump_head()) {
      return true;
    }
  }
  return false;
}

// Procedure: insert_fanin
// Insert an edge into the fanin list of the node.
void_t Node::insert_fanin(edge_pt edge_ptr) {
  auto satellite = _fanin_ptr->push_back(edge_ptr); 
  edge_ptr->set_fanin_satellite(satellite);
}

// Procedure: insert_fanout
// Insert an edge into the fanout list of the node.
void_t Node::insert_fanout(edge_pt edge_ptr) {
  auto satellite = _fanout_ptr->push_back(edge_ptr);
  edge_ptr->set_fanout_satellite(satellite);
}

// Procedure: insert_jumpin
// Insert a jump into the fanin jump of the node.
void_t Node::insert_jumpin(jump_pt jump_ptr) {
  auto satellite = jumpin_ptr()->push_back(jump_ptr);
  jump_ptr->set_jumpin_satellite(satellite);
}

// Procedure: insert_jumpout
// Insert a jump into the fanout jump of the node.
void_t Node::insert_jumpout(jump_pt jump_ptr) {
  auto satellite = jumpout_ptr()->push_back(jump_ptr);
  jump_ptr->set_jumpout_satellite(satellite);
}

// Procedure: relax_at
// Update the arrival time of the node from a given fanin node.
void_t Node::relax_at(int el, int irf, int orf, edge_pt e) {
  
  auto new_at = e->from_node_ptr()->at(el, irf) + e->delay(el, irf, orf);

  switch (el) {
    case EARLY:
      if(new_at < at(el, orf)) {
        set_at(el, orf, new_at);
        set_is_at_clocked(el, orf, e->from_node_ptr()->is_at_clocked(el, irf));
        set_at_parent_ptr(el, orf, e->from_node_ptr());
        set_at_parent_rf(el, orf, irf);
      }
    break;
    case LATE:
      if(new_at > at(el, orf)) {
        set_at(el, orf, new_at);
        set_is_at_clocked(el, orf, e->from_node_ptr()->is_at_clocked(el, irf));
        set_at_parent_ptr(el, orf, e->from_node_ptr());
        set_at_parent_rf(el, orf, irf);
      }
    break;
    default:
      CHECK(false);
    break;
  }
}

// Procedure: relax_rat
// Update the arrival time of the node
void_t Node::relax_rat(int el, int rf, float_ct numeric) {
  switch(el) {
    case EARLY:
      if(numeric > rat(el, rf)) {
        set_rat(el, rf, numeric);
      }
    break;
    case LATE:
      if(numeric < rat(el, rf)) {
        set_rat(el, rf, numeric);
      }
    break;
    default:
      CHECK(false);
    break;
  };
}

// Procedure: relax_slew
// Update the slew of the node
void_t Node::relax_slew(int el, int rf, float_ct numeric) {
  switch(el) {
    case EARLY:
      if(numeric < slew(el, rf)) {
        set_slew(el, rf, numeric);
      }
    break;
    case LATE:
      if(numeric > slew(el, rf)) {
        set_slew(el, rf, numeric);
      }
    break;
    default:
      CHECK(false);
    break;
  };
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------




