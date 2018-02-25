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

#ifndef OT_RCTREE_H_
#define OT_RCTREE_H_

#include "ot_typedef.h"
#include "ot_pin.h"
#include "ot_net.h"

namespace OpenTimer {

// Class RCTreeNode
// The class defines the node structure of the rc network. All the timing field of the node is 
// set to zero.
class RCTreeNode {

  public:

    RCTreeNode(string_crt);                                 // Constructor.
    ~RCTreeNode();                                          // Destructor.
    
    inline void_t set_name(string_crt);                     // Set the name. 
    inline void_t set_ures(int, int, float_ct);             // Set the upstream res.
    inline void_t set_cap(int, int, float_ct);              // Set the capacitance.
    inline void_t set_load(int, int, float_ct);             // Set the load capacitance.
    inline void_t set_beta(int, int, float_ct);             // Set the second moment.
    inline void_t set_delay(int, int, float_ct);            // Set the delay.
    inline void_t set_ldelay(int, int, float_ct);           // Set the load delay.
    inline void_t set_impulse(int, int, float_ct);          // Set the impulse.
    inline void_t add_cap(int, int, float_ct);              // Add the capacitance.
    inline void_t add_load(int, int, float_ct);             // Add the load capacitance.
    inline void_t add_beta(int, int, float_ct);             // Add the second moment.
    inline void_t add_delay(int, int, float_ct);            // Add the delay.
    inline void_t add_ldelay(int, int, float_ct);           // Add the load delay.
    inline void_t add_impulse(int, int, float_ct);          // Add the impulse.
    inline void_t set_pin_ptr(pin_pt);                      // Set pin pointer.

    inline string_crt name() const;                         // Query the name.

    inline float_t ures(int, int) const;                    // Query the upstream res.
    inline float_t cap(int, int) const;                     // Query the capacitance.
    inline float_t load(int, int) const;                    // Query the load capacitance.
    inline float_t beta(int, int) const;                    // Query the second moment.
    inline float_t slew(int, int, float_ct) const;          // Query the slew.
    inline float_t delay(int, int) const;                   // Query the delay.
    inline float_t ldelay(int, int) const;                  // Query the load delay.
    inline float_t impulse(int, int) const;                 // Query the impulse.
    
    inline pin_pt pin_ptr() const;                          // Query the pin pointer.

    inline rctree_edgelist_pt fanin_ptr() const;            // Query the fanin ptr.
    inline rctree_edgelist_rt fanin() const;                // Query the fanin.
    inline rctree_edgelist_pt fanout_ptr() const;           // Query the fanout ptr.
    inline rctree_edgelist_rt fanout() const;               // Query the fanout.

    inline size_t num_fanins() const;                       // Query the fanin size.
    inline size_t num_fanouts() const;                      // Query the fanout size.

    inline bool_t is_leaf() const;                          // Query the node status.
    inline bool_t is_root() const;                          // Query the node status.

    void_t insert_fanout(rctree_edge_pt);                   // Insert a fanout edge.
    void_t insert_fanin(rctree_edge_pt);                    // Insert a fanin edge.

    rctree_edge_pt parent_edge() const;                     // Query the parent edge.
    rctree_node_pt parent() const;                          // Query the parent node.

  private:

    string _name;                                           // Name.

    float_t _ures[2][2];                                    // Upstream res.
    float_t _cap[2][2];                                     // Capacitance.
    float_t _beta[2][2];                                    // Beta.
    float_t _load[2][2];                                    // Load capacitance.
    float_t _delay[2][2];                                   // Delay.
    float_t _ldelay[2][2];                                  // Load delay.
    float_t _impulse[2][2];                                 // Impulse.

    pin_pt _pin_ptr;                                        // Pin pointer.

    rctree_edgelist_pt _fanin_ptr;                          // Fanin.
    rctree_edgelist_pt _fanout_ptr;                         // Fanout.
};

// Procedure: set_name
inline void_t RCTreeNode::set_name(string_crt name) {
  _name = name;
}

// Procedure: set_ures
inline void_t RCTreeNode::set_ures(int el, int rf, float_ct ures) {
  _ures[el][rf] = ures;
}

// Procedure: set_cap
inline void_t RCTreeNode::set_cap(int el, int rf, float_ct cap) {
  _cap[el][rf] = cap;
}

// Procedure: set_load
inline void_t RCTreeNode::set_load(int el, int rf, float_ct load) {
  _load[el][rf] = load;
}

// Procedure: set_beta
inline void_t RCTreeNode::set_beta(int el, int rf, float_ct beta) {
  _beta[el][rf] = beta;
}

// Procedure: set_delay
inline void_t RCTreeNode::set_delay(int el, int rf, float_ct delay) {
  _delay[el][rf] = delay;
}

// Procedure: set_ldelay
inline void_t RCTreeNode::set_ldelay(int el, int rf, float_ct ldelay) {
  _ldelay[el][rf] = ldelay;
}

// Procedure: set_impulse
inline void_t RCTreeNode::set_impulse(int el, int rf, float_ct impulse) {
  _impulse[el][rf] = impulse;
}

// Procedure: add_cap
inline void_t RCTreeNode::add_cap(int el, int rf, float_ct cap) {
  _cap[el][rf] += cap;
}

// Procedure: add_load
inline void_t RCTreeNode::add_load(int el, int rf, float_ct load) {
  _load[el][rf] += load;
}

// Procedure: add_beta
inline void_t RCTreeNode::add_beta(int el, int rf, float_ct beta) {
  _beta[el][rf] += beta;
}

// Procedure: add_delay
inline void_t RCTreeNode::add_delay(int el, int rf, float_ct delay) {
  _delay[el][rf] += delay;
}

// Procedure: add_ldelay
inline void_t RCTreeNode::add_ldelay(int el, int rf, float_ct ldelay) {
  _ldelay[el][rf] += ldelay;
}

// Procedure: add_impulse
inline void_t RCTreeNode::add_impulse(int el, int rf, float_ct impulse) {
  _impulse[el][rf] += impulse;
}

// Procedure: set_pin_ptr
inline void_t RCTreeNode::set_pin_ptr(pin_pt pin_ptr) {
  _pin_ptr = pin_ptr;
}

// Function: name
// Query the node name.
inline string_crt RCTreeNode::name() const {
  return _name;
}

// Function: pin_ptr
// Query the pin pointer.
inline pin_pt RCTreeNode::pin_ptr() const {
  return _pin_ptr;
}

// Function: fanin_ptr
inline rctree_edgelist_pt RCTreeNode::fanin_ptr() const {
  return _fanin_ptr;
}

// Function: fanin
inline rctree_edgelist_rt RCTreeNode::fanin() const {
  return *_fanin_ptr;
}

// Function: fanout_ptr
inline rctree_edgelist_pt RCTreeNode::fanout_ptr() const {
  return _fanout_ptr;
}

// Function: fanout
inline rctree_edgelist_rt RCTreeNode::fanout() const {
  return *_fanout_ptr;
}

// Function: ures
inline float_t RCTreeNode::ures(int el, int rf) const {
  return _ures[el][rf];
}

// Function: cap
inline float_t RCTreeNode::cap(int el, int rf) const {
  return _cap[el][rf];
}

// Function: load
inline float_t RCTreeNode::load(int el, int rf) const {
  return _load[el][rf];
}

// Function: beta
inline float_t RCTreeNode::beta(int el, int rf) const {
  return _beta[el][rf];
}

// Function: set_delay
inline float_t RCTreeNode::delay(int el, int rf) const {
  return _delay[el][rf];
}

// Function: set_ldelay
inline float_t RCTreeNode::ldelay(int el, int rf) const {
  return _ldelay[el][rf];
}

// Function: set_impulse
inline float_t RCTreeNode::impulse(int el, int rf) const {
  return _impulse[el][rf];
}

// Function: slew
inline float_t RCTreeNode::slew(int el, int rf, float_ct input_slew) const {
  return sqrt( OT_SQUARE(input_slew) + impulse(el, rf) );
}

// Function: num_fanouts
inline size_t RCTreeNode::num_fanouts() const {
  return fanout_ptr()->size();
}

// Function: num_fanins
inline size_t RCTreeNode::num_fanins() const {
  return fanin_ptr()->size();
}

// Function: is_leaf
inline bool_t RCTreeNode::is_leaf() const {
  return num_fanouts() == 0;
}

// Function: is_root
inline bool_t RCTreeNode::is_root() const {
  return num_fanins() == 0;
}

//-------------------------------------------------------------------------------------------------

// Class RCTreeEdge
// The class defines the edge structure of the rc network.
class RCTreeEdge {
  
  public:

    RCTreeEdge();                                                           // Constructor.
    ~RCTreeEdge();                                                          // Destructor.

    inline float_t res() const;                                             // Query the resistance.

    inline void_t set_res(float_ct);                                        // Set the resistance.
    inline void_t set_from_rctree_node_ptr(rctree_node_pt);                 // Set the from node.
    inline void_t set_to_rctree_node_ptr(rctree_node_pt);                   // Set the to node.
    inline void_t set_fanin_satellite(rctree_edgelist_iter_t);              // Set the fanin satellite.
    inline void_t set_fanout_satellite(rctree_edgelist_iter_t);             // Set the fanout satellite.
    inline void_t set_rctree_edgelist_satellite(rctree_edgelist_iter_t);    // Set the edgelist satellite.
    
    inline rctree_node_pt from_rctree_node_ptr() const;                     // Query the from node.
    inline rctree_node_pt to_rctree_node_ptr() const;                       // Query the to node.
    inline rctree_edgelist_iter_t rctree_edgelist_satellite() const;        // Query the edgelist satellite.
    inline rctree_edgelist_iter_t fanin_satellite() const;                  // Query the fanin satellite.
    inline rctree_edgelist_iter_t fanout_satellite() const;                 // Query the fanout satellite.

  private:

    float_t _res;                                                           // Resistance.
    
    rctree_node_pt _from_rctree_node_ptr;                                   // From rctree node.
    rctree_node_pt _to_rctree_node_ptr;                                     // To rctree node.
    
    rctree_edgelist_iter_t _rctree_edgelist_satellite;                      // Edgelist satellite.
    rctree_edgelist_iter_t _fanin_satellite;                                // Fanin satellite.
    rctree_edgelist_iter_t _fanout_satellite;                               // Fanout satellite.
};

// Procedure: set_res
inline void_t RCTreeEdge::set_res(float_ct res) {
  _res = res;
}

// Procedure: set_from_rctree_node_ptr
inline void_t RCTreeEdge::set_from_rctree_node_ptr(rctree_node_pt rctree_node_ptr) {
  _from_rctree_node_ptr = rctree_node_ptr;
}

// Procedure: set_from_rctree_node_ptr
inline void_t RCTreeEdge::set_to_rctree_node_ptr(rctree_node_pt rctree_node_ptr) {
  _to_rctree_node_ptr = rctree_node_ptr;
}

// Procedure: set_rctree_edgelist_satellite
inline void_t RCTreeEdge::set_rctree_edgelist_satellite(rctree_edgelist_iter_t satellite) {
  _rctree_edgelist_satellite = satellite;
}

// Procedure: set_fanin_satellite
inline void_t RCTreeEdge::set_fanin_satellite(rctree_edgelist_iter_t satellite) {
  _fanin_satellite = satellite;
}

// Procedure: set_fanout_satellite
inline void_t RCTreeEdge::set_fanout_satellite(rctree_edgelist_iter_t satellite) {
  _fanout_satellite = satellite;
}

// Function: res
inline float_t RCTreeEdge::res() const {
  return _res;
}

// Function: rctree_edgelist_satellite
inline rctree_edgelist_iter_t RCTreeEdge::rctree_edgelist_satellite() const {
  return _rctree_edgelist_satellite;
}

// Function: fanin_satellite
inline rctree_edgelist_iter_t RCTreeEdge::fanin_satellite() const {
  return _fanin_satellite;
}

// Function: fanout_satellite
inline rctree_edgelist_iter_t RCTreeEdge::fanout_satellite() const {
  return _fanout_satellite;
}

// Function: from_rctree_node_ptr
inline rctree_node_pt RCTreeEdge::from_rctree_node_ptr() const {
  return _from_rctree_node_ptr;
}

// Function: to_rctree_node_ptr
inline rctree_node_pt RCTreeEdge::to_rctree_node_ptr() const {
  return _to_rctree_node_ptr;
}

//-------------------------------------------------------------------------------------------------

// Class: RCTree
// The class defines the rc-tree of a net which contains the parasitics of the net
// network. If a connection does not have parasitics, then that the connection has
// 0 delay and the output slew is equivalent to the input slew.
class RCTree {

  public:

    RCTree();                                                                    // Constructor.
    ~RCTree();                                                                   // Destructor.

    void_t clear();                                                              // Clear the RC tree.
    void_t update_rc_timing();                                                   // Update the timing data.
    void_t remove_rc_timing();                                                   // Remove all timing data.
    void_t insert_rctree_segment(string_crt, string_crt, float_ct);              // Insert a RC tree segm.
    void_t set_cap(rctree_node_pt, int, int, float_ct);                          // Change the cap at a node.
    void_t set_cap(string_crt, int, int, float_ct);                              // Change the cap at a node.

    inline void_t set_root_ptr(rctree_node_pt);                                  // Set the root rctree node.
    inline void_t set_is_rc_timing_updated(bool_ct);                             // Set the rc timing status.

    inline bool_t is_rc_timing_updated() const;                                  // Query the rc timing status.

    rctree_edge_pt insert_rctree_edge(string_crt, string_crt, float_ct);         // Insert a RC tree edge.
    rctree_edge_pt insert_rctree_edge(rctree_node_pt, rctree_node_pt, float_ct); // Insert a RC tree edge.
    rctree_node_pt insert_rctree_node(string_crt);                               // Insert a RC tree node.
    rctree_node_pt insert_rctree_node(string_crt, float_ct);                     // Insert a RC tree node.
    rctree_node_pt rehash_rctree_node(string_crt, string_crt);                   // Rehash a RC tree node.

    inline rctree_node_pt rctree_node_ptr(string_crt) const;                     // Query the rctree node ptr.
    inline rctree_node_pt root_ptr() const;                                      // Query the root pointer.
    inline rctree_node_dict_pt rctree_node_dict_ptr() const;                     // Query the ptr.
    inline rctree_node_dict_rt rctree_node_dict() const;                         // Query the ref.
    inline rctree_edgelist_pt rctree_edgelist_ptr() const;                       // Query the rctree edgelist ptr.

    float_t load(int, int) const;                                                // Query the net capacitance.
    float_t slew(rctree_node_pt, int, int, float_ct) const;                      // Query the node slew.
    float_t slew(string_crt, int, int, float_ct) const;                          // Query the node slew.
    float_t delay(rctree_node_pt, int, int) const;                               // Query the node delay.
    float_t delay(string_crt, int, int) const;                                   // Query the node delay.
    float_t estimate_delay(rctree_node_pt, int, int, float_ct);                  // Estimate the delay.

    size_t num_rctree_nodes() const;                                             // Query the node count.
    size_t num_rctree_edges() const;                                             // Query the edge count.

  private:
   
    rctree_node_pt _root_ptr;                                                    // Root of the rc tree.
    
    void_t _clear_rctree_edges();                                                // Clear the RC tree edges.
    void_t _update_load(rctree_node_pt, rctree_node_pt);                         // Update the load capacitance.
    void_t _update_delay(rctree_node_pt, rctree_node_pt);                        // Update the delay.
    void_t _update_ldelay(rctree_node_pt, rctree_node_pt);                       // Update the load delay.
    void_t _update_response(rctree_node_pt, rctree_node_pt);                     // Update the moment and impulse.

    bool_t _is_rc_timing_updated;                                                // RC timing status.
    rctree_node_dict_pt _rctree_node_dict_ptr;                                   // RC node dictionary.
    rctree_edgelist_pt _rctree_edgelist_ptr;                                     // RC tree edge list.
};

// Procedure: set_root_ptr
inline void_t RCTree::set_root_ptr(rctree_node_pt root_ptr) {
  _root_ptr = root_ptr;
}

// Procedure: set_is_rc_timing_updated
inline void_t RCTree::set_is_rc_timing_updated(bool_ct flag) {
  _is_rc_timing_updated = flag;
}

// Function: is_rc_timing_updated
inline bool_t RCTree::is_rc_timing_updated() const {
  return _is_rc_timing_updated;
}

// Function: rctree_node_dict_ptr
inline rctree_node_dict_pt RCTree::rctree_node_dict_ptr() const {
  return _rctree_node_dict_ptr;
}

// Function: rctree_node_dict
inline rctree_node_dict_rt RCTree::rctree_node_dict() const {
  return *_rctree_node_dict_ptr;
}

// Function: rctree_edgelist_ptr
inline rctree_edgelist_pt RCTree::rctree_edgelist_ptr() const {
  return _rctree_edgelist_ptr;
}

// Function: root_ptr
inline rctree_node_pt RCTree::root_ptr() const {
  return _root_ptr;
}

// Function: rctree_node_ptr
inline rctree_node_pt RCTree::rctree_node_ptr(string_crt name) const {
  return rctree_node_dict()[name]; 
}


};  // End of namespace OpenTimer. ----------------------------------------------------------------


#endif
