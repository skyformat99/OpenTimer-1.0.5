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

#ifndef OT_PATH_H_
#define OT_PATH_H_

#include "ot_typedef.h"
#include "ot_utility.h"
#include "ot_functor.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Class: Path
class Path {

  public:
    
    Path();                                                 // Constructor.
    ~Path();                                                // Destructor.
    
    inline int el() const;                                  // Query the split.
    inline int rf() const;                                  // Query the transition.

    inline float_t slack() const;                           // Query the slack.

    inline path_type_e path_type() const;                   // Query the path type.

    inline size_t size() const;                             // Trace length.

    inline void_t clear();                                  // Clear the trace.
    inline void_t push_back(explorer_crt);                  // Insert an explorer into the trace.
    inline void_t set_slack(float_ct);                      // Set the slack.
    inline void_t set_path_type(path_type_ce);              // Set the path type.
    inline void_t set_el(int);                              // Set the data path split.
    inline void_t set_rf(int);                              // Set the data path trans.

    inline path_iter_t begin();                             // Beginning position of the trace.
    inline path_iter_t end();                               // Ending position of the trace.
    inline path_riter_t rbegin();                           // Reversed beginning position.
    inline path_riter_t rend();                             // Reversed ending posision.
    inline explorer_crt back() const;                       // Last element.

  private:

    int _el;                                                // Early/Late split.
    int _rf;                                                // Rise/Fall split.

    float_t _slack;                                         // Path slack.

    path_type_e _path_type;                                 // Path type.

    explorer_vt _trace;                                     // Path trace.
};

// Function: el
// Query the timing split.
inline int Path::el() const {
  return _el;
}

// Function: rf
// Query the timing trans.
inline int Path::rf() const {
  return _rf;
}

// Function: size
// Return the size of the trace.
inline size_t Path::size() const {
  return _trace.size();
}

// Function: path_type
// Return the path type.
inline path_type_e Path::path_type() const {
  return _path_type;
}

// Procedure: set_el
// Set the timing split.
inline void_t Path::set_el(int el) {
  _el = el;
}

// Procedure: set_rf
// Set the timing trans.
inline void_t Path::set_rf(int rf) {
  _rf = rf;
}

// Procedure: set_slack
// Set the path slack.
inline void_t Path::set_slack(float_ct slack) {
  _slack = slack;
}

// Procedure: set_path_type
// Set the path type.
inline void_t Path::set_path_type(path_type_ce type) {
  _path_type = type;
}

// Procedure: clear
// Clear the trace.
inline void_t Path::clear() {
  _trace.clear();
}

// Procedure: push_back
// Insert an idx into the trace.
inline void_t Path::push_back(explorer_crt explorer) {
  _trace.push_back(explorer);
}

// Function: slack
// Query the slack value.
inline float_t Path::slack() const {
  return _slack;
}

// Function: begin
// Return the begin position.
inline path_iter_t Path::begin() {
  return _trace.begin();
}

// Function: end
// Return the end position.
inline path_iter_t Path::end() {
  return _trace.end();
}

// Function: rbegin
// Return the reversed beging position.
inline path_riter_t Path::rbegin() {
  return _trace.rbegin();
}

// Function: rend
// Return the reversed ending position.
inline path_riter_t Path::rend() {
  return _trace.rend();
}

// Function: back
// Return the last position.
inline explorer_crt Path::back() const {
  return _trace.back();
}

//-------------------------------------------------------------------------------------------------

// Class: PathMaxHeap
class PathMaxHeap {

  public:

    PathMaxHeap();                                          // Constructor.
    ~PathMaxHeap();                                         // Destructor.
    
    inline size_t size() const;                             // Return the size of PQ.
    inline path_pt top() const;                             // Return the maximum element.

    void_t insert(path_pt);                                 // Insert an item into the PQ.
    void_t pop();                                           // Extract the maximum element.
    void_t maintain_k_min(size_t k);                        // Maintain the k mininum elements.
    void_t emplace_path_ptrs(path_cache_rt);                // Emplace the path ptrs to cache.

  private:

    size_t _N;                                              // Heap size.

    path_ptr_vt _heap;                                      // Trace pointers.
};

// Function: size
// Return the size of min-max heap.
inline size_t PathMaxHeap::size() const {
  return _N;
}

// Function: top_max
inline path_pt PathMaxHeap::top() const {
  return (size() >= 1) ? _heap[1] : NULL;
}

//-------------------------------------------------------------------------------------------------

// Class: PathCache
// The class for caching paths for an endpoint. The cached path is stored in the pointer
// vector "path_ptrs" and will be automatically "freed" by the destructor.
class PathCache {
  
  public:

    PathCache();                                            // Constructor.
    ~PathCache();                                           // Destructor.
 
    void_t clear();                                         // Clear the path cache.
    
    inline size_t size() const;                             // Query the path count.

    inline void_t insert(path_pt);                          // Insert a given path pointer.

    void_t sort();                                          // Sort the paths.
    void_t maintain_k_min(size_t);                          // Maintain the K-minimun paths.

    inline path_ptr_cvrt path_ptrs() const;                 // Query the path ptrs.

  private:
    
    path_ptr_vt _path_ptrs;                                 // Path pointer vecotr.
};

// Function: size
// Query the size of pathcache.
inline size_t PathCache::size() const {
  return _path_ptrs.size();
}

// Function: insert
// Insert a given path and return this pointer.
inline void_t PathCache::insert(path_pt ptr) {
  _path_ptrs.push_back(ptr);
}

// Function: path_ptrs
inline path_ptr_cvrt PathCache::path_ptrs() const {
  return _path_ptrs;
}

}; // End of OpenTimer namespace ------------------------------------------------------------------


#endif




