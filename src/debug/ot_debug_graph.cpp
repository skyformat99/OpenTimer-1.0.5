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

#include "ot_debug_graph.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------
// Private Functions/Procedures

// Function: _is_dfs_cyclic
bool_t _is_dfs_cyclic(node_pt u, bool_pt visited, bool_pt instack) {
  
  if(!visited[u->idx()]) {

    visited[u->idx()] = true;
    instack[u->idx()] = true;
    node_pt v;
    for(auto& e : u->fanout()) {
      v = e->to_node_ptr();
      if(!visited[v->idx()] && _is_dfs_cyclic(v, visited, instack)) {
        return true; 
      }
      else if(instack[v->idx()]) return true;
    }
  }

  instack[u->idx()] = false;
  return false;
}

//-------------------------------------------------------------------------------------------------
// Header Functions/Procedures

// Function: is_timing_graph_acyclic
bool_t is_timing_graph_acyclic(timer_pt timer_ptr) {
  
  CHECK(timer_ptr != NULL);

  bool_pt visited = (bool_pt)calloc(sizeof(bool_t), timer_ptr->nodeset_ptr()->num_indices());
  bool_pt instack = (bool_pt)calloc(sizeof(bool_t), timer_ptr->nodeset_ptr()->num_indices());
  
  for(const auto& node_ptr : timer_ptr->nodeset()) {
    if(_is_dfs_cyclic(node_ptr, visited, instack)) return false;
  }

  free(instack);
  free(visited);

  return true;
}

// Function: is_timing_graph_cyclic
bool_t is_timing_graph_cyclic(timer_pt timer_ptr) {
  return !is_timing_graph_acyclic(timer_ptr);
}


};


