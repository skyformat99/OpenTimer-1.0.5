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

#include "ot_rctree.h"

namespace OpenTimer {

// Constructor
RCTreeNode::RCTreeNode(string_crt key):
  _name(key),
  _ures{ {OT_DEFAULT_RCTREE_NODE_URES, OT_DEFAULT_RCTREE_NODE_URES},
         {OT_DEFAULT_RCTREE_NODE_URES, OT_DEFAULT_RCTREE_NODE_URES} },
  _cap{ {OT_DEFAULT_RCTREE_NODE_CAP, OT_DEFAULT_RCTREE_NODE_CAP},
        {OT_DEFAULT_RCTREE_NODE_CAP, OT_DEFAULT_RCTREE_NODE_CAP} },
  _beta{ {OT_DEFAULT_RCTREE_NODE_BETA, OT_DEFAULT_RCTREE_NODE_BETA},
         {OT_DEFAULT_RCTREE_NODE_BETA, OT_DEFAULT_RCTREE_NODE_BETA} },
  _load{ {OT_DEFAULT_RCTREE_NODE_LCAP, OT_DEFAULT_RCTREE_NODE_LCAP},
         {OT_DEFAULT_RCTREE_NODE_LCAP, OT_DEFAULT_RCTREE_NODE_LCAP} },
  _delay{ {OT_DEFAULT_RCTREE_NODE_DELAY, OT_DEFAULT_RCTREE_NODE_DELAY},
         {OT_DEFAULT_RCTREE_NODE_DELAY, OT_DEFAULT_RCTREE_NODE_DELAY} },
  _ldelay{ {OT_DEFAULT_RCTREE_NODE_LDELAY, OT_DEFAULT_RCTREE_NODE_LDELAY},
         {OT_DEFAULT_RCTREE_NODE_LDELAY, OT_DEFAULT_RCTREE_NODE_LDELAY} },
  _impulse{ {OT_DEFAULT_RCTREE_NODE_IMPULSE, OT_DEFAULT_RCTREE_NODE_IMPULSE},
         {OT_DEFAULT_RCTREE_NODE_IMPULSE, OT_DEFAULT_RCTREE_NODE_IMPULSE} },
  _pin_ptr(nullptr),
  _fanin_ptr(new rctree_edgelist_t()),
  _fanout_ptr(new rctree_edgelist_t()) {
}


// Destructor
RCTreeNode::~RCTreeNode() {
  delete _fanin_ptr;
  delete _fanout_ptr;
}

// Procedure: insert_fanout
void_t RCTreeNode::insert_fanout(rctree_edge_pt edge_ptr) {
  rctree_edgelist_iter_t satellite = fanout_ptr()->push_back(edge_ptr);
  edge_ptr->set_fanout_satellite(satellite);
}

// Procedure: insert_fanin
void_t RCTreeNode::insert_fanin(rctree_edge_pt edge_ptr) {
  rctree_edgelist_iter_t satellite = fanin_ptr()->push_back(edge_ptr);
  edge_ptr->set_fanin_satellite(satellite);
}

// Function: parent_edge
// Query the parent edge.
rctree_edge_pt RCTreeNode::parent_edge() const {

  if(is_root()) return nullptr;

  if(num_fanins() != 1) {
    LOG(ERROR) << "Multiple fanins on a rctree node -> return nullptr";
    return nullptr;
  }

  return fanin_ptr()->head()->item();
}

// Function: parent
// Query the parent node.
rctree_node_pt RCTreeNode::parent() const {
  auto e = parent_edge();
  return e == nullptr ? nullptr : e->from_rctree_node_ptr();
}

//-------------------------------------------------------------------------------------------------

// Constructor
RCTreeEdge::RCTreeEdge():
  _res(OT_DEFAULT_RCTREE_EDGE_RES),
  _from_rctree_node_ptr(nullptr),
  _to_rctree_node_ptr(nullptr),
  _rctree_edgelist_satellite(nullptr),
  _fanin_satellite(nullptr),
  _fanout_satellite(nullptr)  
{
}

// Destructor
RCTreeEdge::~RCTreeEdge() {

}

//-------------------------------------------------------------------------------------------------

// Constructor
RCTree::RCTree():
  _root_ptr(nullptr),
  _is_rc_timing_updated(false),
  _rctree_node_dict_ptr(new rctree_node_dict_t()),
  _rctree_edgelist_ptr(new rctree_edgelist_t())
{
}

// Destructor
RCTree::~RCTree() {
  _clear_rctree_edges();
  delete _rctree_node_dict_ptr;
  delete _rctree_edgelist_ptr;  
}

// Procedure: remove_rc_timing
void_t RCTree::remove_rc_timing() {
  for(const auto& item : rctree_node_dict()) { 
    EL_RF_ITER(el, rf) {
      item.second->set_ures(el, rf, OT_DEFAULT_RCTREE_NODE_URES);
      item.second->set_beta(el, rf, OT_DEFAULT_RCTREE_NODE_BETA);
      item.second->set_load(el, rf, OT_DEFAULT_RCTREE_NODE_LCAP);
      item.second->set_delay(el, rf, OT_DEFAULT_RCTREE_NODE_DELAY);
      item.second->set_ldelay(el, rf, OT_DEFAULT_RCTREE_NODE_LDELAY);
      item.second->set_impulse(el, rf, OT_DEFAULT_RCTREE_NODE_IMPULSE);
    }
  }
}

// Procedure: _clear_rctree_edges
void_t RCTree::_clear_rctree_edges() {
  for(RCTreeEdgeIter i(this); i(); ++i) delete i.rctree_edge_ptr();
  rctree_edgelist_ptr()->clear();
}

// Procedure: clear
void_t RCTree::clear() {
  _rctree_node_dict_ptr->clear();
  _clear_rctree_edges();
}

// Function: num_rctree_nodes
size_t RCTree::num_rctree_nodes() const {
  return rctree_node_dict_ptr()->size();
}

// Function: num_rctree_edges
size_t RCTree::num_rctree_edges() const {
  return rctree_edgelist_ptr()->size();
}

// Function: load
// Query the total load capacitance.
float_t RCTree::load(int el, int rf) const {
  return (root_ptr() == nullptr) ? OT_FLT_ZERO : root_ptr()->load(el, rf);
}

// Function: slew
// Query the slew at a given rctree node.
float_t RCTree::slew(rctree_node_pt rctree_node_ptr, int el, int rf, float_ct si) const {
  CHECK(rctree_node_ptr != nullptr);
  return si < OT_FLT_ZERO ? -sqrt(OT_SQUARE(si) + rctree_node_ptr->impulse(el, rf)) : 
                             sqrt(OT_SQUARE(si) + rctree_node_ptr->impulse(el, rf));
}

// Function: slew
// Query the slew at a given rctree node.
float_t RCTree::slew(string_crt name, int el, int rf, float_ct si) const {
  return slew(rctree_node_dict()[name], el, rf, si);
}

// Function: delay
// Query the delay at a given rctree node.
float_t RCTree::delay(rctree_node_pt rctree_node_ptr, int el, int rf) const {
  CHECK(rctree_node_ptr != nullptr);
  return rctree_node_ptr->delay(el, rf);
}

// Function: delay
// Query the delay at a given rctree node.
float_t RCTree::delay(string_crt name, int el, int rf) const {
  return delay(rctree_node_dict()[name], el, rf);
}
    
// Procedure: set_cap
void_t RCTree::set_cap(rctree_node_pt ptr, int el, int rf, float_ct val) {
  CHECK(ptr != nullptr);
  ptr->set_cap(el, rf, val);
  set_is_rc_timing_updated(false);
}

// Procedure: set_cap
void_t RCTree::set_cap(string_crt name, int el, int rf, float_ct val) {
  set_cap(rctree_node_dict()[name], el, rf, val);
}

// Procedure: update_rc_timing
void_t RCTree::update_rc_timing() {

  CHECK(root_ptr() != nullptr);

  if(is_rc_timing_updated()) return;
  
  remove_rc_timing();
  
  _update_load(nullptr, root_ptr());
  _update_delay(nullptr, root_ptr());   
  _update_ldelay(nullptr, root_ptr());  
  _update_response(nullptr, root_ptr());

  set_is_rc_timing_updated(true);
}

// Function: insert_rctree_edge
rctree_edge_pt RCTree::insert_rctree_edge(string_crt from_node_name, string_crt to_node_name, float_ct res) {
  return insert_rctree_edge(rctree_node_ptr(from_node_name), rctree_node_ptr(to_node_name), res);
}

// Function: insert_rctree_edge
rctree_edge_pt RCTree::insert_rctree_edge(rctree_node_pt from_rctree_node_ptr, rctree_node_pt to_rctree_node_ptr, float_ct res) {

  CHECK(from_rctree_node_ptr != nullptr && to_rctree_node_ptr != nullptr);

  // Satellite of the edgelist in the rctree object.
  auto rctree_edge_ptr = new rctree_edge_t();
  rctree_edge_ptr->set_rctree_edgelist_satellite(rctree_edgelist_ptr()->push_back(rctree_edge_ptr));

  // Assign the resistance value.
  rctree_edge_ptr->set_res(res);

  // Connection to the node.
  rctree_edge_ptr->set_from_rctree_node_ptr(from_rctree_node_ptr);
  rctree_edge_ptr->set_to_rctree_node_ptr(to_rctree_node_ptr);
  from_rctree_node_ptr->insert_fanout(rctree_edge_ptr);
  to_rctree_node_ptr->insert_fanin(rctree_edge_ptr);

  return rctree_edge_ptr;
}

// Function: insert_rctree_segment
void_t RCTree::insert_rctree_segment(string_crt node_name_1, string_crt node_name_2, float_ct res) {
  auto rctree_node_ptr_1 = rctree_node_ptr(node_name_1);
  auto rctree_node_ptr_2 = rctree_node_ptr(node_name_2);
  if(rctree_node_ptr_1 == nullptr) rctree_node_ptr_1 = insert_rctree_node(node_name_1);
  if(rctree_node_ptr_2 == nullptr) rctree_node_ptr_2 = insert_rctree_node(node_name_2);
  insert_rctree_edge(rctree_node_ptr_1, rctree_node_ptr_2, res);
  insert_rctree_edge(rctree_node_ptr_2, rctree_node_ptr_1, res);
}

// Function: insert_rctree_node
rctree_node_pt RCTree::insert_rctree_node(string_crt name, float_ct cap) {
  auto rctree_node_ptr = rctree_node_dict_ptr()->insert(name, name);
  EL_RF_ITER(el, rf) {
    rctree_node_ptr->set_cap(el, rf, cap);
  }
  return rctree_node_ptr;
}

// Function: insert_rctree_node
rctree_node_pt RCTree::insert_rctree_node(string_crt name) {
  return rctree_node_dict_ptr()->insert(name, name);
}

// Function: rehash_rctree_node    
rctree_node_pt RCTree::rehash_rctree_node(string_crt old_key, string_crt new_key) {
  auto rctree_node_ptr = rctree_node_dict_ptr()->rehash(old_key, new_key);
  rctree_node_ptr->set_name(new_key);
  return rctree_node_ptr;
}

// Procedure: _update_load
// Compute the load capacitance of each rctree node along the downstream traversal of the rctree.
void_t RCTree::_update_load(rctree_node_pt parent, rctree_node_pt from_rctree_node_ptr) {

  // Add downstream capacitances.
  for(const auto& e : from_rctree_node_ptr->fanout()) {
    auto to_rctree_node_ptr = e->to_rctree_node_ptr();
    if(to_rctree_node_ptr != parent) {
      _update_load(from_rctree_node_ptr, to_rctree_node_ptr);
      EL_RF_ITER(el, rf) {
        from_rctree_node_ptr->add_load(el, rf, to_rctree_node_ptr->load(el, rf));
      }
    }
  }
  
  EL_RF_ITER(el, rf) {
    from_rctree_node_ptr->add_load(el, rf, from_rctree_node_ptr->cap(el, rf));
  }
}

// Procedure: _update_delay
// Compute the delay of each rctree node using the Elmore delay model.
void_t RCTree::_update_delay(rctree_node_pt parent, rctree_node_pt from_rctree_node_ptr) {
  
  for(const auto& e : from_rctree_node_ptr->fanout()) {
    auto to_rctree_node_ptr = e->to_rctree_node_ptr();
    if(to_rctree_node_ptr != parent) {
      EL_RF_ITER(el, rf) {
        // Update the delay.
        to_rctree_node_ptr->set_delay(
          el, 
          rf, 
          from_rctree_node_ptr->delay(el, rf) + e->res() * to_rctree_node_ptr->load(el, rf)
        );

        // Update the ures
        to_rctree_node_ptr->set_ures(el, rf, from_rctree_node_ptr->ures(el, rf) + e->res());
      }
      _update_delay(from_rctree_node_ptr, to_rctree_node_ptr);
    }
  }
}

// Procedure: _update_ldelay
// Compute the load delay of each rctree node along the downstream traversal of the rctree.
void_t RCTree::_update_ldelay(rctree_node_pt parent, rctree_node_pt from_rctree_node_ptr) {

  for(const auto& e : from_rctree_node_ptr->fanout()) {
    auto to_rctree_node_ptr = e->to_rctree_node_ptr();
    if(to_rctree_node_ptr != parent) {
      _update_ldelay(from_rctree_node_ptr, to_rctree_node_ptr);
      EL_RF_ITER(el, rf) {
        from_rctree_node_ptr->add_ldelay(el, rf, to_rctree_node_ptr->ldelay(el, rf));
      }
    }
  }

  EL_RF_ITER(el, rf) {
    from_rctree_node_ptr->add_ldelay(
      el, 
      rf, 
      from_rctree_node_ptr->cap(el, rf)*from_rctree_node_ptr->delay(el, rf)
    );
  }
}

// Procedure: _update_response
// Compute the impulse and second moment of the input response for each rctree node. 
void_t RCTree::_update_response(rctree_node_pt parent, rctree_node_pt from_rctree_node_ptr) {

  for(const auto& e : from_rctree_node_ptr->fanout()) {
    auto to_rctree_node_ptr = e->to_rctree_node_ptr();
    if(to_rctree_node_ptr != parent) {
      auto res = e->res();
      EL_RF_ITER(el, rf) {
        to_rctree_node_ptr->set_beta(
          el, 
          rf, 
          from_rctree_node_ptr->beta(el, rf) + res * to_rctree_node_ptr->ldelay(el, rf)
        );
      }
      _update_response(from_rctree_node_ptr, to_rctree_node_ptr);
    }
  }
  EL_RF_ITER(el, rf) {
    auto impulse = OT_FLT_TWO * from_rctree_node_ptr->beta(el, rf) - 
                   OT_SQUARE(from_rctree_node_ptr->delay(el, rf));
    from_rctree_node_ptr->set_impulse(el, rf, impulse);
  }
}

// Function: estimate_delay
// Estimate the new delay value if a new lump capacitance value, new_cap, is applied to this
// rc tree node. Notice that this function should be called only when the timing is up-to-date.
float_t RCTree::estimate_delay(rctree_node_pt ptr, int el, int rf, float_ct new_cap) {
  if(new_cap == ptr->cap(el, rf)) return ptr->delay(el, rf);
  return ptr->delay(el, rf) + ptr->ures(el, rf) * (new_cap - ptr->cap(el, rf));  
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------



