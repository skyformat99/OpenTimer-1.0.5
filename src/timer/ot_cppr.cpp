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

#include "ot_cppr.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Class: CPPRThreadStorage 
// The class defines the basic operations for accessing the memory storage that is defined
// for the CPPR class. These predefined static arrays are mainly used for shortest path
// finding algorithms.
class CPPRThreadStorage {

  public:
    
    inline static size_t dist_array_size(int);                        // Query the storage size.
    inline static size_t sffx_array_size(int);                        // Query the storage size.
    inline static size_t search_cone_size(int);                       // Query the storage size.
    inline static size_t bucketlist_size(int);                        // Query the storage size.
    inline static size_t is_in_list_size(int);                        // Query the storage size.
    
    inline static float_pt dist_array_ptr(int);                       // Query the status.
    inline static int_pt   sffx_array_ptr(int);                       // Query the status.
    inline static bool_pt  search_cone_ptr(int);                      // Query the status.
    inline static int_mpt  bucketlist_ptr(int);                       // Query the status.
    inline static bool_pt  is_in_list_ptr(int);                       // Query the status.

    static void_t free_dist_array(int);                               // Free the memory.
    static void_t free_sffx_array(int);                               // Free the memory.
    static void_t free_search_cone(int);                              // Free the memory.
    static void_t free_bucketlist(int);                               // Free the memory.
    static void_t free_is_in_list(int);                               // Free the memory.

    static float_pt dist_array_ptr(int, size_t);                      // Query the pointer.
    static int_pt   sffx_array_ptr(int, size_t);                      // Query the pointer. 
    static bool_pt  search_cone_ptr(int, size_t);                     // Query the pointer.
    static int_mpt  bucketlist_ptr(int, size_t);                      // Query the pointer.
    static bool_pt  is_in_list_ptr(int, size_t);                      // Query the pointer.

  private:
    
    static size_t _dist_array_size[OT_MAX_NUM_THREADS];               // Memory storage size.
    static size_t _sffx_array_size[OT_MAX_NUM_THREADS];               // Memory storage size.
    static size_t _search_cone_size[OT_MAX_NUM_THREADS];              // Memory storage size.
    static size_t _bucketlist_size[OT_MAX_NUM_THREADS];               // Memory storage size.
    static size_t _is_in_list_size[OT_MAX_NUM_THREADS];               // Memory storage size.

    static size_t _margin; 

    static int_pt   _sffx_array_ptr[OT_MAX_NUM_THREADS];              // Memory storage pointer.
    static int_mpt  _bucketlist_ptr[OT_MAX_NUM_THREADS];              // Memory storage pointer.
    static float_pt _dist_array_ptr[OT_MAX_NUM_THREADS];              // Memory storage pointer.
    static bool_pt  _search_cone_ptr[OT_MAX_NUM_THREADS];             // Memory storage pointer.
    static bool_pt  _is_in_list_ptr[OT_MAX_NUM_THREADS];              // Memory storage pointer.
};

size_t CPPRThreadStorage::_dist_array_size [OT_MAX_NUM_THREADS] = {0};
size_t CPPRThreadStorage::_sffx_array_size [OT_MAX_NUM_THREADS] = {0};
size_t CPPRThreadStorage::_search_cone_size [OT_MAX_NUM_THREADS] = {0};
size_t CPPRThreadStorage::_bucketlist_size [OT_MAX_NUM_THREADS] = {0};
size_t CPPRThreadStorage::_is_in_list_size [OT_MAX_NUM_THREADS] = {0};
size_t CPPRThreadStorage::_margin = 65536;

float_pt CPPRThreadStorage::_dist_array_ptr[OT_MAX_NUM_THREADS] = {nullptr};
int_pt   CPPRThreadStorage::_sffx_array_ptr[OT_MAX_NUM_THREADS] = {nullptr};
bool_pt  CPPRThreadStorage::_search_cone_ptr[OT_MAX_NUM_THREADS] = {nullptr};
int_mpt  CPPRThreadStorage::_bucketlist_ptr[OT_MAX_NUM_THREADS] = {nullptr};
bool_pt  CPPRThreadStorage::_is_in_list_ptr[OT_MAX_NUM_THREADS] = {nullptr};

// Function: dist_array_size
inline size_t CPPRThreadStorage::dist_array_size(int thread_idx) {
  return _dist_array_size[thread_idx];
}

// Function: sffx_array_size
inline size_t CPPRThreadStorage::sffx_array_size(int thread_idx) {
  return _sffx_array_size[thread_idx];
}

// Function: search_cone_size
inline size_t CPPRThreadStorage::search_cone_size(int thread_idx) {
  return _search_cone_size[thread_idx];
}

// Function: bucketlist_size
inline size_t CPPRThreadStorage::bucketlist_size(int thread_idx) {
  return _bucketlist_size[thread_idx];
}

// Function: is_in_list_size
inline size_t CPPRThreadStorage::is_in_list_size(int thread_idx) {
  return _is_in_list_size[thread_idx];
}

// Function: dist_array_ptr
inline float_pt CPPRThreadStorage::dist_array_ptr(int thread_idx) {
  return _dist_array_ptr[thread_idx];
}

// Function: sffx_array_ptr
inline int_pt CPPRThreadStorage::sffx_array_ptr(int thread_idx) {
  return _sffx_array_ptr[thread_idx];
}

// Function: search_cone_ptr
inline bool_pt CPPRThreadStorage::search_cone_ptr(int thread_idx) {
  return _search_cone_ptr[thread_idx];
}

// Function: bucketlist_ptr
inline int_mpt CPPRThreadStorage::bucketlist_ptr(int thread_idx) {
  return _bucketlist_ptr[thread_idx];
}

// Function: is_in_list_ptr
inline bool_pt CPPRThreadStorage::is_in_list_ptr(int thread_idx) {
  return _is_in_list_ptr[thread_idx];
}

// Procedure: free_dist_array
void_t CPPRThreadStorage::free_dist_array(int thread_idx) {
  free(_dist_array_ptr[thread_idx]);
  _dist_array_size[thread_idx] = 0;
  _dist_array_ptr [thread_idx] = nullptr;
}

// Procedure: free_sffx_array
void_t CPPRThreadStorage::free_sffx_array(int thread_idx) {
  free(_sffx_array_ptr[thread_idx]);
  _sffx_array_size[thread_idx] = 0;
  _sffx_array_ptr [thread_idx] = nullptr;
}

// Procedure: free_search_cone
void_t CPPRThreadStorage::free_search_cone(int thread_idx) {
  free(_search_cone_ptr[thread_idx]);
  _search_cone_size[thread_idx] = 0;
  _search_cone_ptr [thread_idx] = nullptr;
}

// Procedure: free_bucketlist
void_t CPPRThreadStorage::free_bucketlist(int thread_idx) {
  delete _bucketlist_ptr[thread_idx];
  _bucketlist_size[thread_idx] = 0;
  _bucketlist_ptr [thread_idx] = nullptr;
}

// Procedure: free_is_in_list
void_t CPPRThreadStorage::free_is_in_list(int thread_idx) {
  free(_is_in_list_ptr[thread_idx]);
  _is_in_list_size[thread_idx] = 0;
  _is_in_list_ptr [thread_idx] = nullptr;
}

// Function: dist_array_ptr
float_pt CPPRThreadStorage::dist_array_ptr(int thread_idx, size_t N) {
  if(_dist_array_size[thread_idx] < N) {
    free_dist_array(thread_idx);
    _dist_array_ptr [thread_idx] = (float_pt) malloc(sizeof(float_t) * (N + _margin));
    _dist_array_size[thread_idx] = (N + _margin);
    memset(_dist_array_ptr[thread_idx], 0x7f, sizeof(float_t) * (N + _margin));
  }
  return _dist_array_ptr[thread_idx];
}

// Function: sffx_array_ptr
int_pt CPPRThreadStorage::sffx_array_ptr(int thread_idx, size_t N) {
  if(_sffx_array_size[thread_idx] < N) {
    free_sffx_array(thread_idx);
    _sffx_array_ptr [thread_idx] = (int_pt) malloc(sizeof(int_t) * (N + _margin));
    _sffx_array_size[thread_idx] = (N + _margin);
    memset(_sffx_array_ptr[thread_idx], -1, sizeof(int_t) * (N + _margin));
  }
  return _sffx_array_ptr[thread_idx];
}

// Function: search_cone_ptr
bool_pt CPPRThreadStorage::search_cone_ptr(int thread_idx, size_t N) {
  if(_search_cone_size[thread_idx] < N) {
    free_search_cone(thread_idx);
    _search_cone_ptr [thread_idx] = (bool_pt) calloc((N + _margin), sizeof(bool_t));
    _search_cone_size[thread_idx] = (N + _margin);
  }
  return _search_cone_ptr[thread_idx];
}

// Function: bucketlist_ptr
int_mpt CPPRThreadStorage::bucketlist_ptr(int thread_idx, size_t N) {
  if(_bucketlist_size[thread_idx] < N) {
    free_bucketlist(thread_idx);
    _bucketlist_ptr [thread_idx] = new int_mt(N + _margin);
    _bucketlist_size[thread_idx] = N + _margin;
  }
  return _bucketlist_ptr[thread_idx];
}

// Function: is_in_list_ptr
bool_pt CPPRThreadStorage::is_in_list_ptr(int thread_idx, size_t N) {
  if(_is_in_list_size[thread_idx] < N) {
    free_is_in_list(thread_idx);
    _is_in_list_ptr [thread_idx] = (bool_pt) calloc((N + _margin), sizeof(bool_t));
    _is_in_list_size[thread_idx] = (N + _margin);
  }
  return _is_in_list_ptr[thread_idx];
}

//-------------------------------------------------------------------------------------------------

// Constructor: CPPR
CPPR::CPPR(
  endpoint_pt endpoint_ptr,
  pin_pt through_pin_ptr,
  clock_tree_pt clock_tree_ptr,
  size_t K
)
{
  CHECK(endpoint_ptr != nullptr);  

  _endpoint_ptr = endpoint_ptr;

  _D_el = endpoint_ptr->el();
  _D_rf = endpoint_ptr->rf();
  _test_ptr = endpoint_ptr->test_ptr();
  _through_node_ptr = through_pin_ptr ? through_pin_ptr->node_ptr() : nullptr;
  _clock_tree_ptr = clock_tree_ptr;
  _D = _test_ptr->constrained_node_ptr();
  _CK = _test_ptr->related_node_ptr();
  _nodeset_ptr = _D->nodeset_ptr();
  _num_indices = _nodeset_ptr->num_indices();
  _S_idx = (_num_indices << 1);
  _cutoff_slack = OT_FLT_ZERO;
  _max_num_paths = K;
  
  // Assign the clock transition status.
  if(CK() == nullptr) {
    _CK_rf = -1;
  }
  else {
    _CK_rf = endpoint_ptr->timing_arc_ptr()->is_rising_edge_triggered() ? RISE : FALL;
  }

  if(_max_num_paths > 1) {
    _prefix_tree_ptr = new prefix_tree_t();
  }
  else {
    _prefix_tree_ptr = nullptr;
  }

  _search_cone = nullptr;

  _update_suffix_tree();
}

// Destructor: ~CPPR
// Destructor of the CPPR object, which clear the path objects and those entires borrowed
// from the memory storage of the thread storage.
CPPR::~CPPR() {
  _clear_thread_storage();
  if(_prefix_tree_ptr) delete _prefix_tree_ptr;
}

// Procedure: _clear_thread_storage
// The procedure clears all the data from the memory storage that are used by this CPPR object.
// We should notice that the value in suffix array should be initialized to -1 and the value
// in the distance table should be initialized to inf. Also, the bucketlist should be cleared
// as well.
void_t CPPR::_clear_thread_storage() {
  for(int_t l=D()->level(); l>=0; --l) {
    for(unsigned_t i=0; i<(*_bucketlist)[l].size(); ++i) {
      _sffx[(*_bucketlist)[l][i]] = -1;
      _dist[(*_bucketlist)[l][i]] = OT_FLT_MAX; 
      _is_in_list[(*_bucketlist)[l][i]] = false;
    }
    (*_bucketlist)[l].clear();
  }
  _sffx[_S_idx] = -1;
  _dist[_S_idx] = OT_FLT_MAX;
  _is_in_list[_S_idx] = false;
}

// Function: num_popped_prefix_ptrs
// Return the number of ranked paths.
size_t CPPR::num_popped_prefix_ptrs() const {
  return prefix_tree_ptr()->num_popped_prefix_ptrs();
}

// Function: _clock_tree_parent
// Retutnr the parent explorer in the clock tree to a given explorer. This procedure is used
// in conjunction with the recover path procedure.
int_t CPPR::_clock_tree_parent(int_t v) const {

  int_t u;

  auto vnode_ptr = _decode_node_ptr(v);

  // Return undefined index if the given index is not well-defined along the clock tree.
  if(v == OT_UNDEFINED_IDX || !(vnode_ptr->is_in_clock_tree()) || vnode_ptr->num_fanins() == 0) {
    return OT_UNDEFINED_IDX;
  }
  
  // Assertion: any nodes in the clock should be connected in a tree fashion, that is, the
  // indegree should be either one or zero.
  CHECK(vnode_ptr->num_fanins() == 1);
  
  // Obtain the parent explorer along the clock tree, where we should be careful about the
  // transition. The transition is inferred by the transition of the given explorer node
  // and the edge between it and its parent. Notice that here the indegree of the given explorer
  // should be one.
  auto e = vnode_ptr->fanin_ptr()->head()->item();
  u = e->from_node_ptr()->idx();

  switch(e->timing_sense()) {
    // Positive unate.
    case POSITIVE_UNATE:
      u = _encode_idx(_decode_rf(v), u);
    break;
    
    // Negative unate.
    case NEGATIVE_UNATE:
      u = _encode_idx(!_decode_rf(v), u);
    break;

    // Non-unate failure.
    default:
      CHECK(false);
    break;
  }
  
  return u;
}

// Function: _is_in_search_cone
// Return true if the node resides in the search_cone of the search graph.
bool_t CPPR::_is_in_search_cone(int_t idx) const {
  return _search_cone ? _search_cone[_decode_idx(idx)] : true;
}

// Function: _is_in_search_cone
// Return true if the node resides in the search_cone of the search graph.
bool_t CPPR::_is_in_search_cone(node_pt node_ptr) const {
  return _search_cone ? _search_cone[node_ptr->idx()] : true;
}

// Function: _is_prunable_from_suffix_tree
// Return true if the ndoe is prunable from the suffix tree expansion.
bool_t CPPR::_is_prunable_from_suffix_tree(int_t v) const {

  switch(D_el()) {

    case EARLY:
      if(_dist[v] + _decode_node_ptr(v)->at(EARLY, _decode_rf(v)) > cutoff_slack()) {
        return true;
      }
    break;

    case LATE:
      if(_dist[v] - _decode_node_ptr(v)->at(LATE, _decode_rf(v)) > cutoff_slack()) {
        return true;
      }
    break;
    
    default:
      CHECK(false) << "Unexpected timing split";
    break;
  }

  return false;
}

// Function: _update_search_cone
// Update the search cone which is the node set that resides in the fanin and fanout cone 
// of the through node pointer. The search_cone is essentially the constrained search graph such that 
// all paths that coming from the source to the data pin of the test must pass through the through
// node. We can imagine that the search_cone is shapped like a sandglass.
void_t CPPR::_update_search_cone() {
  
  // Aall nodes are reachable and not restricted.
  if(through_node_ptr() == nullptr) return;

  // Initialize the search_cone table.
  _search_cone = CPPRThreadStorage::search_cone_ptr(omp_get_thread_num(), _S_idx + 1);
  memset(_search_cone, 0, sizeof(bool_t)*(_S_idx + 1));
  
  node_pt u, v;
  queue < node_pt > que;
  
  // BFS the downstream to obtain the search space.
  u = through_node_ptr();
  que.push(u);
  _search_cone[u->idx()] = true;
  while(!que.empty()) {
    u = que.front();
    que.pop();
    for(auto& e : (u->fanout())) {
      if(e->edge_type() == CONSTRAINT_EDGE_TYPE) continue;
      v = e->to_node_ptr();
      if(_search_cone[v->idx()] == false) {
        _search_cone[v->idx()] = true;
        que.push(v);
      }
    }
  }

  // BFS the upstream to obtain the search space.
  v = through_node_ptr();
  que.push(v);
  _search_cone[v->idx()] = true;
  while(!que.empty()) {
    v = que.front();
    que.pop();
    for(auto& e : v->fanin()) {
      if(e->edge_type() == CONSTRAINT_EDGE_TYPE) continue;
      u = e->from_node_ptr();
      if(_search_cone[u->idx()] == false) {
        _search_cone[u->idx()] = true;
        que.push(u);
      }
    }
  }
}

// Procedure: report_worst_path
// The procedure recovers the top worst path and stores the path into the given path object.
void_t CPPR::report_worst_path(path_rt path) {

  int_t v = _sffx[_S_idx];
  
  // Obtain the deviation cost and deviation credit.
  float_t dvcost = _dist[v] - _dist[_S_idx] + _cppr_offset(v);
  float_t credit = _cppr_credit(v);
  
  // Create a artificial prefix path.
  prefix_t prefix(_S_idx, v, nullptr, nullptr, credit, dvcost);

  recover_path(&prefix, path);
}

// Procedure: report_worst_paths
// The procedure compute the K most critical paths and store them into the given path vector.
// Notice that if the given K is larger than the actual number of paths that could be generated
// from the prefix tree construction, the program only report these paths.
void_t CPPR::report_worst_paths(size_t K, path_vrt paths) {
  
  // Initialization.
  paths.clear();
  
  if(K == 0) {
    LOG(ERROR) << "Failed to report worst path (zero path idx exception)";
    return;
  }

  // Case 1: the worst path slack is above the cutoff slack value.
  if(report_worst_slack(1) > cutoff_slack()) return;

  // Case 2: the user queries only the first critical path or the worst slack is already positive
  // in which case the prefix tree won't be constructed.
  if(K == 1) {
    paths.resize(1);
    report_worst_path(paths[0]);
    return;
  }

  // Case 3: multiple paths and we need to generate the prefix tree and obtain the K most cirtical
  // paths. Notice that if the number of generated paths is less then K the program will output
  // these paths only.
  _update_prefix_tree(K);

  CHECK(num_popped_prefix_ptrs() > 0);

  paths.resize(min(K, num_popped_prefix_ptrs()));
  for(unsigned_t k=0; k<paths.size(); ++k) recover_path(prefix_tree()[k], paths[k]);
}

// Procedure: update_worst_paths
// This procedure update and maintain the K worst paths from a given max heap. In particular,
// it iteratively generates a path and insert it into the heap, meanwhile maintaining the heap
// size by K.
void_t CPPR::update_worst_paths(size_t K, path_max_heap_rt max_heap) {
  
  if(K == 0) {
    LOG(ERROR) << "Fail to update worst paths (zero path idx exception)";
    return;
  }

  max_heap.maintain_k_min(K);

  // Case 1: the worst path slack is above the cutoff slack value.
  if(report_worst_slack(1) > cutoff_slack()) return;

  // Case 2: the worst path slack is larger than the kth min.
  if(max_heap.size() == K && report_worst_slack(1) >= max_heap.top()->slack()) return;

  // Case 3: only one critical have to be maintained.
  if(K == 1) {
    path_pt path_ptr = new path_t();
    report_worst_path(*path_ptr);
    max_heap.insert(path_ptr);
    max_heap.maintain_k_min(K);
    return;
  }

  // Case 4: we have to maintain the top k critical paths.
  _update_prefix_tree(K, max_heap);
}

// Function: get_slack
// Return the kth post-cppr slack value for the endpoint. The input K is supposed to be less than
// the configured maximum number of paths. The output slack should be less than the cutoff slack.
// The only special case is when K is equal to 1 the slack value is always returned.
float_t CPPR::get_slack(size_t K) {
  
  if(K == 0) {
    LOG(ERROR) << "Fail to get slack (zero path idx exception)";
    return OT_FLT_INF;
  }
  
  // Case 1: The prefix doesn't exist.
  if(prefix_tree_ptr() == nullptr) return _dist[_S_idx];

  // Case 2: The prefix tree exists. Notice that here the num_popped_prefix_ptrs might be zero 
  // simply because the worst slack is already non-negative in which case the program won't do 
  // anything and will just simply output the value of the worst slack.
  if(prefix_tree_ptr()->num_popped_prefix_ptrs() >= K) {
    return prefix_tree()[K-1]->devi_credit() + _dist[_S_idx];
  }
  else {
    if(prefix_tree_ptr()->num_popped_prefix_ptrs() == 0) return _dist[_S_idx];
    else return prefix_tree_ptr()->back()->devi_credit() + _dist[_S_idx];
  }
}

// Function: report_worst_slack
// Compute the worst post-CPPR slack of the given test at a given early/late timing split and
// a given rise/fall timing transition. The function must perform the "_update_suffix_tree" to
// find the shortest path tree that is rooted at the data pin.
float_t CPPR::report_worst_slack(size_t K) {
  
  if(K == 0) {
    LOG(ERROR) << "Fail to report worst slack (zero path idx exception)";
    return OT_FLT_INF;
  }

  // Case 1: The number of the path to be reported is one. In this case we can directly extract
  // the path from the suffix tree. No extra overhead of spurring procedure is needed.
  if(K == 1) return _dist[_S_idx];

  // Case 2: The number of the path to be reported is greater than one, where we have to perform
  // the path extraction in order to get the top K critical paths.
  _update_prefix_tree(K); 
  
  // Notice that here the num_popped_prefix_ptrs might be zero simply because the worst slack is
  // already non-negative in which case the program won't do anything and will just simply output 
  // the value of the worst slack.
  if(num_popped_prefix_ptrs() >= K) return prefix_tree()[K-1]->devi_credit() + _dist[_S_idx];
  return num_popped_prefix_ptrs() ? prefix_tree_ptr()->back()->devi_credit() + _dist[_S_idx] : _dist[_S_idx];
}

// Procedure: _update_prefix_tree
// Update the prefix tree. The prefix tree is a tree order of non-suffix-tree edges. Each node in
// the prefix tree represents a path implicitly using the concept of "deviation" edge. The path
// can be recovered by referring to the suffix tree.
void_t CPPR::_update_prefix_tree(size_t K) {
  
  if(prefix_tree_ptr() == nullptr) return;

  prefix_tree_ptr()->clear();

  float_t dvcost;
  float_t credit;
  float_t slack;

  prefix_pt prefix_ptr;

  // The prefix tree is not yet built. Generate the worst paths that originate from each of the 
  // data path source. For each path we need to get the credit and we should be careful about
  // the special case where the clock root feeds the data pin of the test.
  if(_dist[_S_idx] <= cutoff_slack()) {

    for(unsigned_t i=0; i!=_path_src.size(); ++i) {
    
      // Obtain the deviation cost, cppr credit, and path slack.
      dvcost = _dist[_path_src[i]] - _dist[_S_idx] + _cppr_offset(_path_src[i]);
      credit = _cppr_credit(_path_src[i]);
      slack = _dist[_S_idx] + dvcost;

      // Consider only paths with negative slack.
      if(slack > cutoff_slack()) continue;

      // Create a new path and insert it into the prefix tree.
      prefix_tree_ptr()->insert(_S_idx, _path_src[i], nullptr, nullptr, credit, dvcost);
    }
  }

  // Spur the path and expands the neighboring space. The procedure iteratively generates the
  // current most critical path and performs spur operation along this path to generate another
  // set of paths (neighboring expansion).
  while(prefix_tree_ptr()->num_heaped_prefix_ptrs()) {
    
    prefix_ptr = prefix_tree_ptr()->top();
    prefix_tree_ptr()->pop();

    if(num_popped_prefix_ptrs() >= K) break;
    
    _spur(prefix_ptr);
  }
}

// Procedure: _update_prefix_tree
// Update the prefix tree. The prefix tree is a tree order of non-suffix-tree edges. Each node in
// the prefix tree represents a path implicitly using the concept of "deviation" edge. The path
// can be recovered by referring to the suffix tree. In this procedure we have another input 
// argument, which is max_heap. The max_heap is used to maintain the globally top-k 
// critical paths among multiple tests.
void_t CPPR::_update_prefix_tree(size_t K, path_max_heap_rt max_heap) {
  
  if(prefix_tree_ptr() == nullptr) return;

  prefix_tree_ptr()->clear();

  float_t dvcost;
  float_t credit;
  float_t slack;

  prefix_pt prefix_ptr;

  // The prefix tree is not yet built. Generate the worst paths that originate from each of the 
  // data path source. For each path we need to get the credit and we should be careful about
  // the special case where the clock root feeds the data pin of the test.
  if(_dist[_S_idx] <= cutoff_slack()) {

    for(unsigned_t i=0; i!=_path_src.size(); ++i) {
    
      // Obtain the deviation cost, cppr credit, and path slack.
      dvcost = _dist[_path_src[i]] - _dist[_S_idx] + _cppr_offset(_path_src[i]);
      credit = _cppr_credit(_path_src[i]);
      slack = _dist[_S_idx] + dvcost;

      // Consider only paths with negative slack.
      if(slack > cutoff_slack()) continue;

      // Create a new path and insert it into the prefix tree.
      prefix_tree_ptr()->insert(_S_idx, _path_src[i], nullptr, nullptr, credit, dvcost);
    }
  }

  // Spur the path and expands the neighboring space. The procedure iteratively generates the
  // current most critical path and performs spur operation along this path to generate another
  // set of paths (neighboring expansion).
  while(prefix_tree_ptr()->num_heaped_prefix_ptrs()) {
    
    prefix_ptr = prefix_tree_ptr()->top();
    prefix_tree_ptr()->pop(); 
    slack = prefix_ptr->devi_credit() + _dist[_S_idx];
    
    // The current minimum is already larger than the kth max, so there is no need to generate
    // more paths from the spurring.
    if(max_heap.size() == K && slack >= max_heap.top()->slack()) break;

    // Otherwise we have to maintain the top k critical paths.
    path_pt path_ptr = new path_t();
    recover_path(prefix_ptr, *path_ptr);
    max_heap.insert(path_ptr);
    max_heap.maintain_k_min(K);

    if(num_popped_prefix_ptrs() >= K) break;
    
    _spur(prefix_ptr);
  }
}

// Procedure: _spur
// Spur is a procedure that takes a input prefix and perform all possible deviations on it.
// This procedures incorporates the lazy strategy, which only keeps track of the prune 
// values before we have seen all k prefixs. After k prefixs have been seen, the prune value
// is froezn and shall be used to prune the search space.
void_t CPPR::_spur(prefix_pt prefix_ptr) {

  int_t u = prefix_ptr->to_idx();
  int_t T = _encode_idx(D_rf(), D()->idx());
  int_t v;

  timing_arc_pt timing_arc_ptr;

  while(u != T) {

    for(const auto& e : _decode_node_ptr(u)->fanout()) {
      
      v = e->to_node_ptr()->idx();

      switch (e->edge_type()) {

        // RC tree connection.
        case RCTREE_EDGE_TYPE:
          v = _encode_idx(_decode_rf(u), v);
          _spur(prefix_ptr, u, v, e);
        break;

        // Combinational connection.
        case COMBINATIONAL_EDGE_TYPE:
          RF_ITER(vrf) {
            timing_arc_ptr = e->timing_arc_ptr(D_el());
            if(!timing_arc_ptr->is_transition_defined(_decode_rf(u), vrf)) continue;
            v = _encode_idx(vrf, v);
            _spur(prefix_ptr, u, v, e);
          }
        break;

        // Constraint edge.
        case CONSTRAINT_EDGE_TYPE:
        break;

        // Assertion false.
        default:
          CHECK(false);
        break;
      }
    }
    u = _sffx[u];
  }
}

// Procedure: _spur
// This is the subroutine of the biggest spur procedure. Here we are given a prefix and a targeted
// edge that is being spurred from the prefix. The newly spurred prefix is a children of the given
// path prefix path in the prefix tree.
void_t CPPR::_spur(prefix_pt prefix_ptr, int_t u, int_t v, edge_pt e) {
  
  if(v == _sffx[u] || _sffx[v] == OT_UNDEFINED_IDX || !_is_in_search_cone(v)) return;

  float_t devi_cost = _devi_cost(u, v, e);
  float_t devi_credit = prefix_ptr->devi_credit() + devi_cost;
  float_t slack = _dist[_S_idx] + devi_credit; 
  
  if(slack > cutoff_slack()) return;
  
  prefix_tree_ptr()->insert(u, v, e, prefix_ptr, prefix_ptr->cppr_credit(), devi_credit); 
}

// Function: _devi_cost
// Return the deviation cost of the path that is deviated by e.
float_t CPPR::_devi_cost(int_t u, int_t v, edge_pt e) const {
  if(D_el() == EARLY) return e->delay(EARLY, _decode_rf(u), _decode_rf(v)) + _dist[v] - _dist[u];
  return _dist[v] - _dist[u] - e->delay(LATE, _decode_rf(u), _decode_rf(v));
}

// Procedure: _prefetch_cutoff_slack
// The procedure prefetch a upper bound of the cutoff slack by traversing the worst pre-cppr 
// path and apply the cppr credit to this path in order to obtain the corresponding worst
// cppr slack. The cutoff slack is then set to the value of this post-cppr slack.
void_t CPPR::_prefetch_cutoff_slack() {

  if(prefix_tree_ptr()) return;

  int vrf(D_rf());
  int urf;
  node_pt v_ptr(D());
  node_pt u_ptr;
  
  // Obtain the pre-CPPR slack.
  auto slack = D_el() == EARLY ? D()->at(D_el(), D_rf()) - test_ptr()->rat(D_el(), D_rf()) :
                                 test_ptr()->rat(D_el(), D_rf()) - D()->at(D_el(), D_rf());

  // Starting at the D pin, backtrace through the worst pre-CPPR slack path, find the minimum 
  // credit and apply the credit to it.
  while(v_ptr) {

    if(!_is_in_search_cone(v_ptr)) return;

    if(v_ptr->is_data_path_source()) {
      slack += _cppr_credit(D_el(), vrf, CK_rf(), v_ptr, CK());
      break;
    }
    urf   = v_ptr->at_parent_rf(D_el(), vrf);
    u_ptr = v_ptr->at_parent_ptr(D_el(), vrf);
    vrf   = urf;
    v_ptr = u_ptr;
  }
  
  // Prefetch the cutoff slack value. Here we should add a threshold value to the cutoff value,
  // due to the floating difference.
  set_cutoff_slack(min(cutoff_slack(), slack + 1.0f)); 
}

// Procedure: _update_suffix_tree
// Build the suffix tree for the given test at a given early/late timing split and a given
// rise/fall timing transition. The suffix tree is a shortest path tree that is generated from
// the data pin.
void_t CPPR::_update_suffix_tree() {
  
  // Obtain the search space.
  _update_search_cone();

  // Data declaration.
  int_t v = _encode_idx(D_rf(), D()->idx());
  _dist = CPPRThreadStorage::dist_array_ptr(omp_get_thread_num(), _S_idx + 1);
  _sffx = CPPRThreadStorage::sffx_array_ptr(omp_get_thread_num(), _S_idx + 1);
  _bucketlist = CPPRThreadStorage::bucketlist_ptr(omp_get_thread_num(), _D->level() + 1);
  _is_in_list = CPPRThreadStorage::is_in_list_ptr(omp_get_thread_num(), _S_idx + 1);

  // If the v is not residing in the search_cone, nothing needs to be done.
  if(!_is_in_search_cone(v)) return;

  // Prefetch the cutoff slack from the post-cppr slack on the most critical pre-cppr slack path.
  // This procedure only works for reporting the top-most critical path.
  _prefetch_cutoff_slack();

  // Start at the D pin and perform the back BFS traversal until reaching the source FFs.
  // Notice that the source is either the sink of a FF or the node with zero indegree.
  _dist[v] = (D_el() == EARLY) ? -test_ptr()->rat(D_el(), D_rf()) : test_ptr()->rat(D_el(), D_rf());

  // Insert the target node of the D pin.
  (*_bucketlist)[D()->level()].push_back(v);
  _is_in_list[v] = true;

  // Iterate the level from the level of the data pin back to the lowest level. For each level, we
  // scan all nodes in that level and perform the relaxation on each node.
  for(auto l=D()->level(); l>=0; --l) {
    for(size_t i=0; i<(*_bucketlist)[l].size(); ++i) {
      v = (*_bucketlist)[l][i];

      // Prune the search space for non-necessary expansion.
      if(_is_prunable_from_suffix_tree(v)) continue;

      auto vnode_ptr = _decode_node_ptr(v);

      // Already reach the source of the data path. Update the artificial edge with slack offset
      // to adjust the suffix tree.
      if(vnode_ptr->is_data_path_source()) {
        _path_src.push_back(v);
        _update_worst_source(v);
        continue;
      }
      
      // Perform the relaxation from all nodes that coming from either the "jumpin" list 
      // or the "fanin" list of the node v.
      if(vnode_ptr->num_jumpins()) _relax_jumpin(v, *_bucketlist);
      else _relax_fanin(v, *_bucketlist);
    }
  }
}

// Procedure: _update_worst_sources
// The procedure update the slack offset of every suffix tree. The offset is the artificial
// weight between the super source nodes to each of the source nodes of the data path that are
// generated by the suffix building procedure.
void_t CPPR::_update_worst_sources() {
  for(unsigned_t i=0; i<_path_src.size(); ++i) {
    _update_worst_source(_path_src[i]);
  }
}

// Procedure: _update_worst_source
// The procedure update the slack offset of every suffix tree. The offset is the artificial
// weight between the super source nodes to each of the source nodes of the data path that are
// generated by the suffix building procedure.
void_t CPPR::_update_worst_source(int_t s) {
  float_t offset = _cppr_offset(s);
  if(_dist[s] + offset < _dist[_S_idx]) {
    _dist[_S_idx] = _dist[s] + offset;
    _sffx[_S_idx] = s;
    if(prefix_tree_ptr() == nullptr) {
      set_cutoff_slack(min(_dist[_S_idx], cutoff_slack()));
    }
  }
}

// Function: _relax
// The relax procedure for the SP routine. The function return true if a relaxation happens
// and false otherwise. Notice that this relaxation is taken in a backward manner, which means
// it only happens if dist[v] + d < dist[u].
void_t CPPR::_relax(int_t u, int_t v, edge_pt e, int_mrt levellist) {
  
  // Obtain the dealy from the explorer u to the explorer v.
  float_t d;

  if(D_el() == EARLY) d = e->delay(D_el(), _decode_rf(u), _decode_rf(v));
  else d = -(e->delay(D_el(), _decode_rf(u), _decode_rf(v)));
  
  // Relaxation incurs. Here we apply the smallest label first heuristic for the 
  // queue insertion.
  if(_dist[v] + d < _dist[u]) {
    _dist[u] = _dist[v] + d;
    _sffx[u] = v;
    if(!_is_in_list[u]) {
      levellist[_decode_node_ptr(u)->level()].push_back(u);
      _is_in_list[u] = true;
    }
  }
}

// Function: _relax
// The relax procedure for the SP routine. The function return true if a relaxation happens
// and false otherwise. Notice that this relaxation is taken in a backward manner, which means
// it only happens if dist[v] + d < dist[u].
void_t CPPR::_relax(int_t u, int_t v, jump_pt e, int_mrt levellist) {
  
  // Obtain the dealy from the explorer u to the explorer v.
  float_t d;

  if(D_el() == EARLY) d = e->delay(D_el(), _decode_rf(u), _decode_rf(v));
  else d = -(e->delay(D_el(), _decode_rf(u), _decode_rf(v)));
  
  // Relaxation incurs. Here we apply the smallest label first heuristic for the 
  // queue insertion.
  if(_dist[v] + d < _dist[u]) {
    _dist[u] = _dist[v] + d;
    _sffx[u] = v;
    if(!_is_in_list[u]) {
      levellist[_decode_node_ptr(u)->level()].push_back(u);
      _is_in_list[u] = true;
    }
  }
}

// Procedure: _relax_jumpin
// The relax procedure is mainly used for update_suffix_tree procedure. It takes a given node
// with transition sense encoded into an integer v and a given levellist and perform all relaxation
// on all input nodes that are reachable via the jumpping edges.
void_t CPPR::_relax_jumpin(int_t v, int_mrt levellist) {

  int_t u;

  for(auto& e : _decode_node_ptr(v)->jumpin()) {
    if(!_is_in_search_cone(e->from_node_ptr())) continue;
    u = e->from_node_ptr()->idx();
    switch(e->timing_sense()) {
      case POSITIVE_UNATE:
        u = _encode_idx(_decode_rf(v), u);
        _relax(u, v, e, levellist);
      break;

      case NEGATIVE_UNATE:
        u = _encode_idx(!_decode_rf(v), u);
        _relax(u, v, e, levellist);
      break;

      default:
        CHECK(false);
      break;
    }
  }
}

// Procedure: _relax_fanin
// The relax procedure is mainly used for update_suffix_tree procedure. It takes a given node
// with transition sense encoded into an integer v and a given levellist and perform all relaxation
// on all input nodes that are reachable via the fanin edges.
void_t CPPR::_relax_fanin(int_t v, int_mrt levellist) {
  
  int_t u;
  timing_arc_pt timing_arc_ptr;

  // Relax from normal edge.
  for(auto& e : _decode_node_ptr(v)->fanin()) {
    if(!_is_in_search_cone(e->from_node_ptr())) continue;

    u = e->from_node_ptr()->idx();

    switch (e->edge_type()) {
      // RC tree connection.
      case RCTREE_EDGE_TYPE:
        u = _encode_idx(_decode_rf(v), u);
        _relax(u, v, e, levellist);
      break;

      // Combinational connection.
      case COMBINATIONAL_EDGE_TYPE:
        RF_ITER(urf) {
          timing_arc_ptr = e->timing_arc_ptr(D_el());
          if(!timing_arc_ptr->is_transition_defined(urf, _decode_rf(v))) continue;
          u = _encode_idx(urf, u);
          _relax(u, v, e, levellist);
        }
      break;

      // Constraint edge.
      case CONSTRAINT_EDGE_TYPE:
      break;

      // Assertion false.
      default:
        CHECK(false);
      break;
    }
  }
}


// Function: _cppr_credit
// Return the cppr credit of a given data path source node to the D pin node of the test in a 
// given timing split "el" and transition status "rf" of the sink of the capturing node. 
// For hold time credit, we consider all the delay in the upstream path to the lowest common 
// ancestor of nodes "capter" and "lncher". For the setup time credit, we compute the same thing
// but subtract the arrival time credit at the clock root. Normally hold time gets more credit 
// than setup time.
float_t CPPR::_cppr_credit(int_t u) const {
  return _cppr_credit(D_el(), _decode_rf(u), CK_rf(), _decode_node_ptr(u), CK());
}

// Function: _cppr_credit
// Return the cppr credit of a given data path source node to the D pin node of the test in a 
// given timing split "el" and transition status "rf" of the sink of the capturing node. 
// For hold time credit, we consider all the delay in the upstream path to the lowest common 
// ancestor of nodes "capter" and "lncher". For the setup time credit, we compute the same thing
// but subtract the arrival time credit at the clock root. Normally hold time gets more credit 
// than setup time.
float_t CPPR::_cppr_credit(int el, int lnch_rf, int capt_rf, node_pt lncher, node_pt capter) const {
  
  if(lncher == nullptr) {
    LOG(ERROR) << "Fail to get cppr credit (nullptr exception)"; 
    return OT_FLT_INF;
  }
  
  // RAT test.
  if(capter == nullptr) return OT_FLT_ZERO;
   
  // Unconnected FF.
  if(!capter->is_in_clock_tree() || !lncher->is_in_clock_tree()) return OT_FLT_ZERO;

  // Undefined clock tree.
  if(clock_tree_ptr() == nullptr) return OT_FLT_ZERO;

  // Special case handling - the clock source might have data path injecting to the D pin. In 
  // this case we should use the D pin node as the launching node so that the LCA is obtained
  // appropriately.
  if(lncher == clock_tree_ptr()->root_node_ptr()) {
    lncher = D();
    lnch_rf = D_rf();
  }

  // Obtain the LCA of the two nodes and the negation count from the lca node to the capt node. 
  // Also we need to retrieve the transition status from the sink node (capter) to the 
  // lca node. Note that if the transition status is different at the lca node from capturing node
  // and launching node, the common path does not exist.
  node_pt lca = clock_tree_ptr()->lca_node_ptr(lncher, capter);
  auto num_negs_to_lca = clock_tree_ptr()->num_negations(lca);
  auto num_negs_to_capt = clock_tree_ptr()->num_negations(capter);
  auto num_negs_to_lnch = clock_tree_ptr()->num_negations(lncher);
  auto lca_rf_from_capt = ((num_negs_to_capt - num_negs_to_lca) & 1) ? !capt_rf : capt_rf;
  auto lca_rf_from_lnch = ((num_negs_to_lnch - num_negs_to_lca) & 1) ? !lnch_rf : lnch_rf;

  if(lca_rf_from_capt != lca_rf_from_lnch) return OT_FLT_ZERO;

  // Return the credit for the early (hold) test.  
  if(el == EARLY) lca->at_diff(LATE, lca_rf_from_capt, EARLY, lca_rf_from_capt);
  
  // Return the credit for the late (setup) test.
  auto root_rf = (num_negs_to_capt & 1) ? !capt_rf : capt_rf;
  auto root_credit = clock_tree_ptr()->root_node_ptr()->at_diff(LATE, root_rf, EARLY, root_rf);

  return lca->at_diff(LATE, lca_rf_from_capt, EARLY, lca_rf_from_capt) - root_credit;
}

// Function: _cppr_offset
// Return the cppr offset of a source pin with respect to the target test. The cppr offset is
// used as an artificial weight that is assigned to the artificial edges from the super source to
// all data path source nodes in the pessimism free graph.
float_t CPPR::_cppr_offset(int_t u) const {
  return _cppr_offset(_decode_rf(u), _decode_node_ptr(u));
}

// Function: _cppr_offset
// Return the cppr offset of a source pin with respect to the target test. The cppr offset is
// used as an artificial weight that is assigned to the artificial edges from the super source to
// all data path source nodes in the pessimism free graph.
float_t CPPR::_cppr_offset(int lnch_rf, node_pt lncher) const {

  if(lncher == nullptr) {
    LOG(ERROR) << "Fail to get cppr offset (nullptr exception)";
    return OT_FLT_INF;
  }
  
  // Obtain the CPPR credit between the source pin (node) and the target test.
  auto c = _cppr_credit(D_el(), lnch_rf, CK_rf(), lncher, CK());

  return (D_el() == EARLY) ? lncher->at(D_el(), lnch_rf) + c : -lncher->at(D_el(), lnch_rf) + c;
}

// Procedure: recover_path
// This procedure recovers a given path pointer from implicit representation
// to explicit form based on prefix path structure and parent array.
void_t CPPR::recover_path(prefix_pt prefix, path_rt path) {
  
  path.clear();
  path.set_slack(_dist[_S_idx] + prefix->devi_credit());
  path.set_el(D_el());
  path.set_rf(D_rf());
  
  if(CK() == nullptr) path.set_path_type(RAT_PATH_TYPE);
  else path.set_path_type(D_el() == EARLY ? HOLD_PATH_TYPE : SETUP_PATH_TYPE);

  _recover_data_path(prefix, _encode_idx(D_rf(), D()->idx()), path);  
}

// Procedure: _recover_jump_path
// A subroutine of the recover path. This procedure recovers the path that are passed via
// "jump" edge between two nodes. The path that is connected by a jumpping edge should be
// unate-definite.
void_t CPPR::_recover_jump_path(int_t beg, int_t end, path_rt path) {

  auto jumpper(_decode_node_ptr(beg));
  auto jumpper_rf(_decode_rf(beg));

  if(jumpper->num_jumpouts() == 0) return;
  
  CHECK(jumpper->num_jumpouts() == 1 && jumpper->num_fanouts() == 1);

  node_pt head = _decode_node_ptr(end);

  // Move forward by one step.
  edge_pt edge_ptr = jumpper->fanout_ptr()->head()->item();
  jumpper = edge_ptr->to_node_ptr();
  switch(edge_ptr->timing_sense()) {
    case POSITIVE_UNATE:
    break;

    case NEGATIVE_UNATE:
      jumpper_rf = !jumpper_rf;
    break;

    default:
      CHECK(false);
    break;
  }
  
  // Kepp moving forward by one step until we reach the head.
  while(jumpper != head) {
    path.push_back(__make_explorer(jumpper_rf, jumpper));

    // Move forward by one step.
    CHECK(jumpper->num_fanouts() == 1);

    edge_ptr = jumpper->fanout_ptr()->head()->item();
    jumpper = edge_ptr->to_node_ptr();

    switch(edge_ptr->timing_sense()) {
      case POSITIVE_UNATE:
      break;

      case NEGATIVE_UNATE:
        jumpper_rf = !jumpper_rf;
      break;

      default:
        CHECK(false);
      break;
    }
  }
  
  CHECK(jumpper_rf == _decode_rf(end));
}

// Procedure: _recover_data_path
// A subroutine of the recover path. This procedure recovers the path that resides in the data
// path we have found that injects to the data pin of the test.
void_t CPPR::_recover_data_path(prefix_pt prefix, int_t end, path_rt path) {

  auto beg(prefix->to_idx());

  if(prefix->parent() != nullptr) _recover_data_path(prefix->parent(), prefix->from_idx(), path);
  else _recover_clock_path(prefix, _clock_tree_parent(beg), path);
  
  while(beg != end) {
    path.push_back(__make_explorer(_decode_rf(beg), _decode_node_ptr(beg)));
    _recover_jump_path(beg, _sffx[beg], path);
    beg = _sffx[beg];
  }
  path.push_back(__make_explorer(_decode_rf(end), _decode_node_ptr(end)));
}

// Procedure: _recover_clock_path
// This procedure recovers the path that originates from the clock root to the source of a
// data path.
void_t CPPR::_recover_clock_path(prefix_pt prefix, int_t p, path_rt path) {
  if(p == OT_UNDEFINED_IDX) return;
  _recover_clock_path(prefix, _clock_tree_parent(p), path);
  path.push_back(__make_explorer(_decode_rf(p), _decode_node_ptr(p)));
}

//-------------------------------------------------------------------------------------------------

// Constructor
Prefix::Prefix(int_t u, int_t v, edge_pt e, Prefix *p, float_t c, float_t w):
  _from_idx(u),
  _to_idx(v),
  _deviator(e),
  _parent(p),
  _cppr_credit(c),
  _devi_credit(w)
{
}

// Destructor.
Prefix::~Prefix() {

}

//-------------------------------------------------------------------------------------------------

// Constructor.
PrefixHeap::PrefixHeap():
  _N(0),
  _heap(1, nullptr)
{
}

// Destructor.
PrefixHeap::~PrefixHeap() {
  clear();
}

// Procedure: clear
// Clear all memory space for all prefixes in the heap.
void_t PrefixHeap::clear() {
  while(size() > 0) {
    delete _heap.back();
    _heap.pop_back();
    --_N;
  }
}

// Function: top
// Return the minimum key element.
prefix_pt PrefixHeap::top() const {
  return (size() >= 1) ? _heap[1] : nullptr;
}

// Procedure: insert
// Insert a path prefix pointer to the heap.
void_t PrefixHeap::insert(prefix_pt prefix) {
    
  ++_N;
  _heap.push_back(prefix);

  // Heapify up.
  size_t i(size());
  size_t p(size()>>1);

  while(p && _heap[i]->devi_credit() < _heap[p]->devi_credit()) { 
    swap(_heap[i], _heap[p]); 
    i = p; 
    p >>= 1;
  }
}

// Procedure: pop
// Extract the top element out of the heap and then perform the downstream heapify.
void_t PrefixHeap::pop() {
  
  if(size() == 0) return;
            
  // Swap the first and last items.
  swap(_heap[1], _heap[size()]);

  // Pop the last item.
  _heap.pop_back();
  --_N;

  if(size() == 0) return;

  // Perform the heapify.
  size_t i(1), l(0), r(0), s(0);

  while(1) {
    l = i << 1;
    r = l |  1;
    s = (l<=size() && _heap[l]->devi_credit() < _heap[i]->devi_credit()) ? l : i;
    s = (r<=size() && _heap[r]->devi_credit() < _heap[s]->devi_credit()) ? r : s;
    if(s == i) break;
    swap(_heap[i], _heap[s]);
    i = s;
  }
}

//-------------------------------------------------------------------------------------------------

// Constructor.
PrefixTree::PrefixTree():
  _popper(new prefix_ptr_vt()),
  _heaper(new prefix_heap_t())
{

}

// Destructor.
PrefixTree::~PrefixTree() {
  clear();
  delete _popper;
  delete _heaper;
}

// Procedure: clear
// Delete all allocated prefix pointers in the prefix tree.
void_t PrefixTree::clear() {

  // Clear the popped prefixes.
  for(unsigned_t i=0; i<_popper->size(); ++i) {
    delete (*_popper)[i];
  }
  _popper->clear();

  // Clear the heaped prefixes.
  _heaper->clear();
}

// Procedure: insert
// Insert a new prefix into the prefix tree.
void_t PrefixTree::insert(int_t u, int_t v, edge_pt e, Prefix* ptr, float_t cc, float_t dc) {
  _heaper->insert(new prefix_t(u, v, e, ptr, cc, dc)); 
}

// Procedure: pop
// Pop an prefix pointer from the heap.
void_t PrefixTree::pop() {
  _popper->push_back(_heaper->top());
  _heaper->pop();
}

// Procedure: top
// Return the top prefix pointer from the heap.
prefix_pt PrefixTree::top() {
  return _heaper->top(); 
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

