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

#include "ot_path.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Constructor.
Path::Path() {
  _slack = OT_FLT_MAX;
  _path_type = UNDEFINED_PATH_TYPE;
}


// Destructor.
Path::~Path() {

}

//-------------------------------------------------------------------------------------------------

// Constructor.
PathMaxHeap::PathMaxHeap():
  _N(0),
  _heap(1, nullptr)
{
}

// Destructor.
PathMaxHeap::~PathMaxHeap() {
  //for(unsigned_t i=1; i<=_N; ++i) delete _heap[i];
  for(const auto& ptr : _heap) delete ptr;
}

// Procedure: insert
void_t PathMaxHeap::insert(path_pt ptr) {
    
  ++_N;
  _heap.push_back(ptr);

  // Heapify up.
  auto i(size());
  auto p(size()>>1);

  while(p && _heap[i]->slack() > _heap[p]->slack()) { 
    swap(_heap[i], _heap[p]); 
    i = p; 
    p >>= 1;
  }
}

// Procedure: pop
// Pop the top path (the one with the largest slack values in the heap) from the heap.
void_t PathMaxHeap::pop() {
  
  if(size() == 0) return;
            
  // Swap the first and last items.
  swap(_heap[1], _heap[size()]);

  // Pop the last item.
  delete _heap.back();
  _heap.pop_back();
  --_N;

  if(size() == 0) return;

  // Perform the heapify.
  size_t i(1), l(0), r(0), s(0);

  while(1) {
    l = i << 1;
    r = l |  1;
    s = (l<=size() && _heap[l]->slack() > _heap[i]->slack()) ? l : i;
    s = (r<=size() && _heap[r]->slack() > _heap[s]->slack()) ? r : s;
    if(s == i) break;
    swap(_heap[i], _heap[s]);
    i = s;
  }
}

// Procedure: maintain_k_min
void_t PathMaxHeap::maintain_k_min(size_t k) {
  while(_N > k) pop();
}

// Procedure: emplace_path_ptrs
void_t PathMaxHeap::emplace_path_ptrs(path_cache_rt path_cache) {
  for(unsigned_t i=1; i<=_N; ++i) {
    path_cache.insert(_heap[i]);
  }
  _heap.resize(1);
  _N = 0;
}

//-------------------------------------------------------------------------------------------------

// Constructor.
PathCache::PathCache()
{

}

// Destructor.
PathCache::~PathCache() {
  clear();
}

// Procedure: clear
// Clear the path cache.
void_t PathCache::clear() {
  for(const auto& ptr : _path_ptrs) delete ptr;
  _path_ptrs.clear();
}

// Procedure: maintain_k_min
void_t PathCache::maintain_k_min(size_t K) {
  sort();
  while(_path_ptrs.size() > K) {
    delete _path_ptrs.back();
    _path_ptrs.pop_back();
  }
}

// Procedure: sort
// Sort the paths in the increasing order of slack.
void_t PathCache::sort() {

  auto ComparePath = [] (path_pt i, path_pt j) -> bool_t {
    return i->slack() < j->slack();
  };

  std::sort(_path_ptrs.begin(), _path_ptrs.end(), ComparePath);
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------


