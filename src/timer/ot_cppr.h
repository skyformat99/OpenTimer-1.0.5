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

#ifndef OT_CPPR_H_
#define OT_CPPR_H_

#include "ot_pin.h"
#include "ot_node.h"
#include "ot_edge.h"
#include "ot_circuit.h"
#include "ot_path.h"
#include "ot_test.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Class: Prefix 
class Prefix {

  public:
    
    Prefix(int_t, int_t, edge_pt, Prefix*, float_t, float_t);                 // Constructor.
    ~Prefix();                                                                // Destructor.

    inline int_t from_idx() const;                                            // Query the from explorer.
    inline int_t to_idx() const;                                              // Query the to explorer.

    inline float_t cppr_credit() const;                                       // Query the cppr credit.
    inline float_t devi_credit() const;                                       // Query the path devi_credit.

    inline edge_pt deviator() const;                                          // Query the deviator.
    inline Prefix* parent() const;                                            // Query the parent.

    inline void_t set_from_idx(int_t);                                        // Set the explorer.
    inline void_t set_to_idx(int_t);                                          // Set the explorer.
    inline void_t set_cppr_credit(float_ct);                                  // Set the credit.
    inline void_t set_devi_credit(float_ct);                                  // Set the post-cppr devi_credit.
    inline void_t set_parent(prefix_pt);                                      // Set parent.
    inline void_t set_deviator(edge_pt);                                      // Set the deviator.

  private:

    int_t _from_idx;                                                          // The from explorer.
    int_t _to_idx;                                                            // The to explorer.

    edge_pt _deviator;                                                        // Deviator pointer.
    Prefix* _parent;                                                          // Parent.

    float_t _cppr_credit;                                                     // CPPR credit.
    float_t _devi_credit;                                                     // Post-CPPR devi_credit.
};

// Function: from_explorer
inline int_t Prefix::from_idx() const {
  return _from_idx;
}

// Function: to_explorer
inline int_t Prefix::to_idx() const {
  return _to_idx;
}

// Function: cppr_credit
// Query the cppr credit.
inline float_t Prefix::cppr_credit() const {
  return _cppr_credit;
}

// Function: devi_credit
// Query the post cppr devi_credit.
inline float_t Prefix::devi_credit() const {
  return _devi_credit;
}

// Function: deviator
// Query the deviator.
inline edge_pt Prefix::deviator() const {
  return _deviator;
}

// Function: parent
// Query the parent of the path prefix.
inline prefix_pt Prefix::parent() const {
  return _parent;
}

// Procedure: set_from_idx
// Set the from explorer
inline void_t Prefix::set_from_idx(int_t idx) {
  _from_idx = idx;
}

// Procedure: set_to_idx
// Set the to explorer
inline void_t Prefix::set_to_idx(int_t idx) {
  _to_idx = idx;
}

// Procedure: set_cppr_credit
// Set the cppr credit.
inline void_t Prefix::set_cppr_credit(float_ct credit) {
  _cppr_credit = credit;
}

// Procedure: set_devi_credit
// Set the post cppr devi_credit.
inline void_t Prefix::set_devi_credit(float_ct devi_credit) {
  _devi_credit = devi_credit;
}

// Procedure: set_parent
// Set the parent.
inline void_t Prefix::set_parent(prefix_pt parent) {
  _parent = parent;
}

// Procedure: set_deviator
// Set the deviator.
inline void_t Prefix::set_deviator(edge_pt e) {
  _deviator = e;
}

//-------------------------------------------------------------------------------------------------

// Class: PrefixHeap
// The is a subclass of the class PrefixTree. The PrefixHeap is majorly used as a priority queue 
// for the path peeling process. Each item in the heap is a pointer to a prefix.
class PrefixHeap {

  public:

    PrefixHeap();                                            // Constructor.
    ~PrefixHeap();                                           // Destructor.

    inline size_t size() const;                              // Return the size of PQ.

    void insert(prefix_pt);                                  // Insert an path prefix pointer.
    void pop();                                              // Extract the minimum key item.
    void clear();                                            // Clear the heap.

    prefix_pt top() const;                                   // The minimum key item.

  private:
    
    size_t _N;                                               // Size of the heap.
    prefix_ptr_vt _heap;                                     // Heap.
};

// Function: size
// Query the size of the heap.
inline size_t PrefixHeap::size() const {
  return _N;
}

//-------------------------------------------------------------------------------------------------

// Class: PrefixTree
// The class for the prefix tree, which contains 1) a heap array for path peeling and 2) an array
// storing the prefix pointers (ordered) popped from the heap during the peeling process.
class PrefixTree {
  
  public:

    PrefixTree();                                                     // Constructor.
    ~PrefixTree();                                                    // Destructor.
    
    inline size_t num_popped_prefix_ptrs() const;                     // Query the popped prefixes.
    inline size_t num_heaped_prefix_ptrs() const;                     // Query the heaped prefixes.
    
    void_t clear();                                                   // Clear tree.
    void_t pop();                                                     // Pop a prefix.
    void_t insert(int_t, int_t, edge_pt, Prefix*, float_t, float_t);  // Insert a prefix.            

    prefix_pt top();                                                  // Top heap item.

    inline prefix_pt back();                                          // Last popped prefix.
    inline prefix_pt operator [] (unsigned_t);                        // Query popped prefix.
    inline prefix_cpt operator [] (unsigned_t) const;                 // Query popped prefix.

  private:
  
    prefix_ptr_vpt _popper;                                           // Popped prefixes.
    prefix_heap_pt _heaper;                                           // Heaped prefixes.

};

// Function: num_popped_prefix_ptrs
// Return the number of popped prefixes.
inline size_t PrefixTree::num_popped_prefix_ptrs() const {
  return _popper->size();
}

// Function: num_heaped_prefix_ptrs
// Return the number of heaped prefixes.
inline size_t PrefixTree::num_heaped_prefix_ptrs() const {
  return _heaper->size();
}

// Operator: []
// Query the popped item.
inline prefix_pt PrefixTree::operator [] (unsigned_t idx) {
  return (idx >= num_popped_prefix_ptrs()) ? NULL : (*_popper)[idx];
}

// Operator: []
// Query the popped item.
inline prefix_cpt PrefixTree::operator [] (unsigned_t idx) const {
  return (idx >= num_popped_prefix_ptrs()) ? NULL : (*_popper)[idx];
}

// Function: back
// Return the last popped prefix pointer.
inline prefix_pt PrefixTree::back() {
  return _popper->back();
}

// Class: CPPR
// The class performs path-based tracing to discover the post-cppr critical paths for a given
// endpoint. The class majorly conducts the path peeling, which doesn't change any value of the
// given endpoint. Caller is responsible for calling the API to modify or cache any data they
// want. 
class CPPR {
  
  public:

    CPPR(endpoint_pt, pin_pt, clock_tree_pt, size_t K=1);                     // Constructor.
    ~CPPR();                                                                  // Destructor.

    inline endpoint_pt endpoint_ptr() const;                                  // Endpoint pointer.

    inline int D_el() const;                                                  // Early/Late of D.
    inline int D_rf() const;                                                  // Rise/Fall of D.
    inline int CK_rf() const;                                                 // Query the clock transition.
    inline int_t S_idx() const;                                               // Query the super source idx.

    inline node_pt through_node_ptr() const;                                  // Query the through node pointer.
    inline node_pt D() const;                                                 // Query the D pin node.
    inline node_pt CK() const;                                                // Query the CK pin node.
    inline nodeset_pt nodeset_ptr() const;                                    // Query the node set pointer.
    inline nodeset_rt nodeset() const;                                        // Query the node set reference.

    inline test_pt test_ptr() const;                                          // Query the endpointer.
    inline clock_tree_pt clock_tree_ptr() const;                              // Query the circuit pointer.

    size_t num_popped_prefix_ptrs() const;                                    // Query the number of ranked paths.    
    inline void_t set_cutoff_slack(float_ct);                                 // Set the cutoff slack.

    void_t report_worst_path(path_rt);                                        // Report the worst path.
    void_t report_worst_paths(size_t, path_vrt);                              // Report the K worst paths.
    void_t recover_path(prefix_pt, path_rt);                                  // Recover the path.
    void_t update_worst_paths(size_t, path_max_heap_rt);                      // Update the worst paths.
    
    float_t cutoff_slack() const;                                             // Query the cutoff slack.
    float_t get_slack(size_t K = 1);                                          // Query the kth slack.
    float_t report_worst_slack(size_t K = 1);                                 // Query the K'th worst path slack.

    inline prefix_tree_pt prefix_tree_ptr();                                  // Query the prefix tree pointer.
    inline prefix_tree_rt prefix_tree();                                      // Query the prefix tree.
    inline prefix_tree_cpt prefix_tree_ptr() const;                           // Query the prefix tree.
    inline prefix_tree_crt prefix_tree() const;                               // Query the prefix tree.

  private:
    
    endpoint_pt _endpoint_ptr;                                                // Endpoint.

    int _D_el;                                                                // Test timing split.
    int _D_rf;                                                                // Test timing trans.
    int _CK_rf;                                                               // Clock transition status.

    size_t _max_num_paths;                                                    // max_num_paths

    int_t _S_idx;                                                             // Super source idx.
    int_t _num_indices;                                                       // Number of indices;
    int_mpt _bucketlist;                                                      // Levellist.
    
    bool_pt _search_cone;                                                     // Fanout cone of the through pin.
    bool_pt _is_in_list;                                                      // Query if resides in bucketlist.
    
    test_pt _test_ptr;                                                        // Test pointer.
    clock_tree_pt _clock_tree_ptr;                                            // Circuit pointer.

    node_pt _D;                                                               // D pin.
    node_pt _CK;                                                              // CK pin.
    node_pt _through_node_ptr;                                                // Through node pointer.
    nodeset_pt _nodeset_ptr;                                                  // Pointer to the node set.

    float_t _cutoff_slack;                                                    // The cutoff value for slack.
    float_pt _dist;                                                           // Distance array type.

    int_pt _sffx;                                                             // Suffix array type.
    int_vt _path_src;                                                         // Data path sources.

    prefix_tree_pt _prefix_tree_ptr;                                          // Prefix tree pointer.

    int_t _clock_tree_parent(int_ct) const;                                   // Query the parent in clock tree.

    inline int _decode_rf(int_t) const;                                       // Query the transition of an idx.
    inline int_t _encode_idx(int, int_t) const;                               // Query the idx with rf encoded.
    inline int_t _decode_idx(int_t) const;                                    // Query the node idx.
    inline node_pt _decode_node_ptr(int_t) const;                             // Query the node pointer.
    
    bool_t _is_in_search_cone(node_pt) const;                                 // Query if feasibly through.
    bool_t _is_in_search_cone(int_t) const;                                   // Query if feasibly through.
    bool_t _is_prunable_from_suffix_tree(int_t) const;                        // Query if prunable.

    void_t _clear_thread_storage();                                           // Clear thread storage.
    void_t _relax(int_t, int_t, edge_pt, int_mrt);                            // Relaxation procedure.
    void_t _relax(int_t, int_t, jump_pt, int_mrt);                            // Relaxation procedure.
    void_t _relax_jumpin(int_t, int_mrt);                                     // Relaxation procedure.
    void_t _relax_fanin(int_t, int_mrt);                                      // Relaxation procedure.
    void_t _spur(prefix_pt);                                                  // Spur the search space.
    void_t _spur(prefix_pt, int_t, int_t, edge_pt);                           // Spur the search space.
    void_t _update_search_cone();                                             // Update the search cone.
    void_t _update_suffix_tree();                                             // Update the suffix tree.
    void_t _update_prefix_tree(size_t);                                       // Update the prefix tree. 
    void_t _update_prefix_tree(size_t, path_max_heap_rt);                     // Update the prefix tree. 
    void_t _update_worst_sources();                                           // Update the worst slack source.
    void_t _update_worst_source(int_t);                                       // Update the worst slack source.
    void_t _prefetch_cutoff_slack();                                          // Prefetch the cutoff slack.
    void_t _recover_worst_path(path_rt);                                      // Recover the first worst path.
    void_t _recover_jump_path(int_t, int_t, path_rt);                         // Recover the jump path.
    void_t _recover_data_path(prefix_pt, int_t, path_rt);                     // Recover the data path.
    void_t _recover_clock_path(prefix_pt, int_t, path_rt);                    // Recover the clock path.

    float_t _cppr_offset(int_t) const;                                        // Query the cppr offset.
    float_t _cppr_offset(int, node_pt) const;                                 // Query the cppr offset.
    float_t _cppr_credit(int_t) const;                                        // Query the cppr credit.
    float_t _cppr_credit(int, int, int, node_pt, node_pt) const;              // Query the cppr credit.
    float_t _devi_cost(int_t, int_t, edge_pt) const;                          // Query the deviation cost. 
};

// Function: endpoint_ptr
// Return the endpoint pointer.
inline endpoint_pt CPPR::endpoint_ptr() const {
  return _endpoint_ptr;
}

// Function: D_el
// Return the el status of the test.
inline int CPPR::D_el() const {
  return _D_el;
}

// Function: D_rf
// Return the rf status of the test.
inline int CPPR::D_rf() const {
  return _D_rf;
}

// Function: _decode_rf
// Return the transition of the given idx.
inline int CPPR::_decode_rf(int_t idx) const {
  return idx < _num_indices ? RISE : FALL;
}

// Function: _encode_idx
inline int_t CPPR::_encode_idx(int rf, int_t idx) const {
  return rf == RISE ? idx : idx + _num_indices;
}

// Function: _decode_idx
inline int_t CPPR::_decode_idx(int_t idx) const {
  return idx % _num_indices;
}

// Function: _decode_node_ptr
inline node_pt CPPR::_decode_node_ptr(int_t idx) const {
  return (*_nodeset_ptr)[_decode_idx(idx)];
}

// Function: CK_rf
// Query the clock transition status.
inline int_t CPPR::CK_rf() const {
  return _CK_rf;
}

// Function: test_ptr
// Return the pointer to the test.
inline test_pt CPPR::test_ptr() const {
  return _test_ptr;
}

// Procedure: set_cutoff_slack
// Set the cutoff slack.
inline void_t CPPR::set_cutoff_slack(float_ct val) {
  _cutoff_slack = val;
}

// Function: prefix_tree_ptr
// Return the pointer to the prefix tree.
inline prefix_tree_pt CPPR::prefix_tree_ptr() {
  return _prefix_tree_ptr;
}

// Function: prefix_tree_ptr
// Return the pointer to the prefix tree.
inline prefix_tree_cpt CPPR::prefix_tree_ptr() const {
  return _prefix_tree_ptr;
}

// Function: prefix_tree
// Return the reference to the prefix tree.
inline prefix_tree_rt CPPR::prefix_tree() {
  return *_prefix_tree_ptr;
}

// Function: prefix_tree
// Return the reference to the prefix tree.
inline prefix_tree_crt CPPR::prefix_tree() const {
  return *_prefix_tree_ptr;
}

// Function: nect
// Return the next node pointer.
inline node_pt CPPR::through_node_ptr() const {
  return _through_node_ptr;
}

// Function: D
// Return the D pin node.
inline node_pt CPPR::D() const {
  return _D;
}

// Function: CK
// Return the CK pin node.
inline node_pt CPPR::CK() const {
  return _CK;
}

// Function: S_idx
// Return the idx of the super source.
inline int_t CPPR::S_idx() const {
  return _S_idx;
}

// Function: nodeset_ptr
// Return the pointer to the node set.
inline nodeset_pt CPPR::nodeset_ptr() const {
  return _nodeset_ptr;
}

// Function: nodeset
// Return the reference to the node set.
inline nodeset_rt CPPR::nodeset() const {
  return *_nodeset_ptr;
}

// Function: clock_tree_ptr
// Return the pointer to the clock tree.
inline clock_tree_pt CPPR::clock_tree_ptr() const {
  return _clock_tree_ptr;
}

// Function: cutoff_slack
// Return the cutoff slack.
inline float_t CPPR::cutoff_slack() const {
  return _cutoff_slack;
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif


