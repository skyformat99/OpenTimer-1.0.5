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

#include "ot_pipeline.h"

namespace OpenTimer {

// Constructor
Pipeline::Pipeline():
  _bucketlist_ptr(new node_bucketlist_t()),
  _num_nodes(0),
  _min_level(num_levels()),
  _max_level(-1)
{
  resize(65536);
}

// Destructor
Pipeline::~Pipeline() {
  for(unsigned_t i=0; i<_bucketlist_ptr->size(); i++) delete bucketlist()[i];
  delete _bucketlist_ptr;
}

// Function: empty
// Return true if the pipeline is empty.
bool_t Pipeline::empty() {
  return num_nodes() == 0;
}

// Function: empty
// Return true if the pipeline in a given level is empty.
bool_t Pipeline::empty(int_t l) {
  if(l >= (int_t)num_levels() || l < 0) return true;
  return bucketlist()[l]->empty();
}

// Procedure: remove_all_nodes
// Remove nodes from the pipeline.
void_t Pipeline::remove_all_nodes() {
  nodelist_pt _list;
  for(int_t i=min_level(); i<=max_level(); ++i) {
    _list = nodelist_ptr(i);
    if(_list != nullptr) {
      for(NodelistIter j(_list); j(); ++j) {
        j.node_ptr()->set_pipeline_satellite(nullptr);
      }
    }
    bucketlist()[i]->clear();
  }
  _num_nodes = 0;
  _min_level = num_levels();
  _max_level = -1;
}

// Procedure: remove
// Remove a node from the pipeline. If the node is not in hte pipeline, the procedure
// will do nothing. Otherwise, the pipeline removes the node from the nodelist in 
// accordance to the level of the node.
void_t Pipeline::remove(node_pt node_ptr) {

  CHECK(node_ptr != nullptr);

  if(node_ptr->is_in_pipeline()) {
    bucketlist()[node_ptr->level()]->remove(node_ptr->pipeline_satellite());
    node_ptr->set_pipeline_satellite(nullptr);
    --_num_nodes;
    
    while(_min_level < (int_t)num_levels() && empty(_min_level) == true) 
      ++_min_level;

    while(_max_level >= 0 && empty(_max_level) == true)
      --_max_level;
  }
}

// Procedure: resize
// Resize the pipeline level.
void_t Pipeline::resize(size_t size) {
  
  if(_min_level == (int_t)num_levels()) {
    _min_level = max(size, num_levels());
  }

  while(_bucketlist_ptr->size() < size) {
    bucketlist_ptr()->push_back(new nodelist_t());
  }
}

// Function: insert
// Insert a node into the pipeline. If the node is already in the pipeline, the procedure
// will do nothing. Otherwise, the node is inserted into the pipeline in accordance to the
// level of the node.
void_t Pipeline::insert(node_pt node_ptr) {

  CHECK(node_ptr != nullptr);

  if(!(node_ptr->is_in_pipeline())) {

    // Resize the level limit.
    if(node_ptr->level() >= (int_t)num_levels()) resize(node_ptr->level() + 1);

    // Insert the node into the pipeline.
    nodelist_iter_t satellite = bucketlist()[node_ptr->level()]->push_back(node_ptr);
    node_ptr->set_pipeline_satellite(satellite);
    ++_num_nodes;

    _min_level = min(_min_level, node_ptr->level());
    _max_level = max(_max_level, node_ptr->level());
  }
}

// Function: nodelist_ptr
// Return the nodelist to a given level in the pipeline.
nodelist_pt Pipeline::nodelist_ptr(int_t level) {
  if(level < 0) return nullptr;
  return (level < (int_t)num_levels()) ? bucketlist()[level] : nullptr;
}

// Function: node_ptrs
// Obtain the vector of node pointers that are stored in the nodelist to a given level
// of the pipeline.
void_t Pipeline::node_ptrs(int_t level, node_ptr_vrt vect) {
  vect.clear();
  nodelist_pt ptr = nodelist_ptr(level);
  if(ptr != nullptr) {
    for(NodelistIter i(ptr); i(); ++i) {
      vect.push_back(i.node_ptr());
    }
  }
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------



