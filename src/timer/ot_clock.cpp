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

#include "ot_clock.h"

namespace OpenTimer {

// Constructor
ClockTree::ClockTree() {
  _size = 0;
  _level = -1;
  _primary_input_ptr = nullptr;
  _period = OT_DEFAULT_CLOCK_SOURCE_PERIOD;
  _is_clock_tree_updated = false;
}

// Destructor
ClockTree::~ClockTree() {
}

// Function: level
// Query the level of a clock tree pin.
int_t ClockTree::level(pin_pt pin_ptr) const {

  if(pin_ptr == nullptr) {
    return OT_UNDEFINED_LEVEL;
  }

  return level(pin_ptr->node_ptr());
}

// Function: level
// Query the level of a clock tree node.
int_t ClockTree::level(node_pt node_ptr) const {
  
  if(node_ptr == nullptr || node_ptr->is_in_clock_tree() == false) {
    return OT_UNDEFINED_LEVEL;
  }
  
  return _L[node_ptr->clock_tree_node_idx()];
}

// Function: num_negations
// Query the number of negations from the root to a given clock tree node.
int_t ClockTree::num_negations(node_pt node_ptr) const {
  
  if(node_ptr == nullptr || node_ptr->is_in_clock_tree() == false) {
    return 0;
  }

  return _N[node_ptr->clock_tree_node_idx()];
}

// Procedure: update_clock_tree
// This procedure builds the parameters for the clock tree. The parameters include the LUT
// of the sparse table for LCA retrieval.
void_t ClockTree::update_clock_tree() {

  // No update needed if the clock tree has been updated.
  if(is_clock_tree_updated()) return;

  node_pt _root(root_node_ptr());

  if(!_root) return;
  
  // Clear all the data storages.
  for(const auto& u : *(_root->nodeset_ptr())) {
    u->set_clock_tree_node_idx(OT_UNDEFINED_IDX);
  }
  _level = -1;
  _size = 0;
  __clear_vector(_E);
  __clear_vector(_L);
  __clear_vector(_N);
  __clear_matrix(_ST);

  _traverse(nullptr, _root, 0, 0);
  _update_sparse_table();     

  set_is_clock_tree_updated(true);
}

// Procedure: _traverse
// This procedure Builds the index of Euler path for clock tree traversal.
void_t ClockTree::_traverse(node_pt p, node_pt u, int_ct d, int_ct n)
{
  _size = _size + 1;             // Increment the clock tree size.

  int_t is_negative_unate(0);

  if(u->is_clock_sink()) {       // Reach the sink.
    _insert_ELHN(u, d, n);
    return;
  }

  for(auto& e : u->fanout()) {

    // Keep track of the negation count.
    switch(e->timing_sense()) {
      
      // Positive unate.
      case POSITIVE_UNATE:
        is_negative_unate = 0;
      break;
      
      // Negative unate.
      case NEGATIVE_UNATE:
        is_negative_unate = 1;
      break;
      
      // Assertion false.
      default:
        CHECK(false) << e->from_node_ptr()->pin_ptr()->name() << " -> " << e->to_node_ptr()->pin_ptr()->name();
      break;
    }
    
    // Go to the next tree node.
    _insert_ELHN(u, d, n);                          
    _traverse(u, e->to_node_ptr(), d + 1, n + is_negative_unate);            
  }

  _insert_ELHN(u, d, n);                
}

// Procedure: _insert_ELHN
void_t ClockTree::_insert_ELHN(node_pt node_ptr, int_ct depth, int_ct num_negations)
{
  if(node_ptr->clock_tree_node_idx() == OT_UNDEFINED_IDX) {
    node_ptr->set_clock_tree_node_idx((int_t)_E.size());
  }
  _E.push_back(node_ptr);
  _L.push_back(depth);
  _N.push_back(num_negations);
  _level = max(_level, depth);
}

// Function: lca_node_ptr
// Return the lowest common ancestor of u and v on the clock tree.
node_pt ClockTree::lca_node_ptr(node_pt u, node_pt v) const {
  
  if(u == nullptr || v == nullptr) return nullptr;

  if(!u->is_in_clock_tree() || !v->is_in_clock_tree()) return nullptr;

  if(u == v) return u;

  if(!(u->clock_tree_node_idx() <= v->clock_tree_node_idx())) {
    swap(u, v);
  }

  int_t c = log2(v->clock_tree_node_idx() - u->clock_tree_node_idx() + 1);
  int_t l = _ST[u->clock_tree_node_idx()][c];
  int_t r = _ST[v->clock_tree_node_idx()-(1<<c)+1][c];

  return _L[l] < _L[r] ? _E[l] : _E[r]; 
}

// Function: lca_node_ptr
node_pt ClockTree::lca_node_ptr(int urf, int vrf, node_pt u, node_pt v) const {
  
  node_pt lca = lca_node_ptr(u, v);  

  if(!lca) return nullptr;
  
  int_t num_negs_to_lca = num_negations(lca);
  int_t num_negs_to_u = num_negations(u);
  int_t num_negs_to_v = num_negations(v);
  int_t lca_rf_from_u = ((num_negs_to_u - num_negs_to_lca) & 1) ? !urf : urf;
  int_t lca_rf_from_v = ((num_negs_to_v - num_negs_to_lca) & 1) ? !vrf : vrf;

  return (lca_rf_from_u != lca_rf_from_v) ? nullptr : lca; 
}

// Function: root_node_ptr
// Return the node pointer to the root.
node_pt ClockTree::root_node_ptr() const {
  if(!primary_input_ptr()) return nullptr;
  return primary_input_ptr()->pin_ptr()->node_ptr();
}

// Function: root_pin_ptr
// Return the pin pointer to the clock tree root.
pin_pt ClockTree::root_pin_ptr() const {
  if(!primary_input_ptr()) return nullptr;
  return primary_input_ptr()->pin_ptr();
} 

// Procedure: _update_sparse_table
// Establish the sparse table for storing the LCA information.
void_t ClockTree::_update_sparse_table()
{
  unsigned_t sparse_table_len1 = _L.size();                      
  unsigned_t sparse_table_len2 = (unsigned_t)floor(log2(_L.size())) + 1;    
  _ST.resize(sparse_table_len1);              

  // Initialize ST for the intervals with length 1. 
  for(unsigned_t s=0; s < sparse_table_len1; s++) {
    _ST[s].resize(sparse_table_len2);                       // Resize the sublen
    _ST[s][0] = s;                                          // Self.
  }

  // Bottom-up dynamic programming.
  for(unsigned_t l=1; l<sparse_table_len2; l++) {           // Iterates through all sublen.
    for(unsigned_t i = 0; i+(1<<l)-1 < _ST.size(); i++) {   // Iterates through all elements.
      if(_L[_ST[i][l-1]] < _L[_ST[ i+(1<<(l-1))][l-1]]) {   // Compare left / right interval.
        _ST[i][l] = _ST[i][l-1];                            // Value in left interval is smaller.
      }
      else {
        _ST[i][l] = _ST[i+(1<<(l-1))][l-1];                 // Value in right interval is smaller.
      }
    }
  }
}

// Function: cppr_credit
// Return the cppr credit of two clock sink nodes in a given timing split "el" and transition
// status "rf" of the sink of the capturing node. For hold time credit, we consider all the delay 
// in the upstream path to the lowest common ancestor of nodes "capter" and "lncher". 
// For the setup time credit, we compute the same thing but subtract the arrival time credit at 
// the clock root. Normally hold time gets more credit than setup time.
float_t ClockTree::cppr_credit(int el, int lnch_rf, int capt_rf, node_pt lncher, node_pt capter) const {
  
  CHECK(capter != nullptr && lncher != nullptr);
  
  // Unconnected FF.
  if(!capter->is_in_clock_tree() || !lncher->is_in_clock_tree()) return OT_FLT_ZERO;

  // Undefined clock tree.
  if(root_pin_ptr() == nullptr) return OT_FLT_ZERO;

  // Obtain the LCA of the two nodes and the negation count from the lca node to the capt node. 
  // Also we need to retrieve the transition status from the sink node (capter) to the 
  // lca node. Note that if the transition status is different at the lca node from capturing node
  // and launching node, the common path does not exist.
  node_pt lca = lca_node_ptr(lncher, capter);
  int_t num_negs_to_lca = num_negations(lca);
  int_t num_negs_to_capt = num_negations(capter);
  int_t num_negs_to_lnch = num_negations(lncher);
  int_t lca_rf_from_capt = ((num_negs_to_capt - num_negs_to_lca) & 1) ? !capt_rf : capt_rf;
  int_t lca_rf_from_lnch = ((num_negs_to_lnch - num_negs_to_lca) & 1) ? !lnch_rf : lnch_rf;

  if(lca_rf_from_capt != lca_rf_from_lnch) {
    return OT_FLT_ZERO;
  }

  // Return the credit for the early (hold) test.  
  if(el == EARLY) lca->at_diff(LATE, lca_rf_from_capt, EARLY, lca_rf_from_capt);
  
  // Return the credit for the late (setup) test.
  auto root_rf = (num_negs_to_capt & 1) ? !capt_rf : capt_rf;
  auto root_credit = root_node_ptr()->at_diff(LATE, root_rf, EARLY, root_rf);

  return lca->at_diff(LATE, lca_rf_from_capt, EARLY, lca_rf_from_capt) - root_credit;
}


};  // End of OpenTimer namespace. -----------------------------------------------------------------






