/******************************************************************************
 *                                                                            *
 * Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#ifndef OC_ARRAYLIST_HPP_
#define OC_ARRAYLIST_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

namespace OpenCluster {

// Class: ArrayList
// An ordered set is a data structure consists of a collection of items that support random
// access. Each inserted item is referenced by an index and can be accessed through operator []
// on the ordered set.
template < typename T, typename Allocator = ::std::allocator<T>, size_t Cap = 8 >
class ArrayList {

  static_assert(Cap >= 1, "Failed to compile ArrayList (capacity should be at least 1)");
  
  struct DataBlock {
    T** block;                                              // Block memory.
    size_t size;                                            // Block capacity.
    size_t cursor;                                          // Cursor of the block.
  };

  struct RecyBlock {
    size_t* block;                                          // Block memory.
    size_t size;                                            // Block capacity.
    size_t cursor;                                          // Cursor of the block.
  };

  public:
  
    struct Iterator {                                       // Iterator type.
      T** cursor;                                           // Cursor.
      T** end;                                              // End cursor.
      inline Iterator& operator ++();                       // Prefix add.
      inline T* operator *();                               // Deref iter.
      inline bool operator != (const Iterator&);            // Comparator.
    };
    
    inline ArrayList();                                     // Constructor.
    inline ~ArrayList();                                    // Destructor.
    
    inline size_t size() const;                             // Query the size.
    inline size_t num_indices() const;                      // Query the index size.

    inline void remove(const size_t);                       // Remove an item.

    template <typename... ArgsT>
    inline size_t insert(ArgsT&&...);                       // Insert an item.

    inline Iterator begin();                                // Iterator status.
    inline Iterator end();                                  // Iterator status.

    inline T* operator [] (const size_t);                   // Operator.
    inline const T* operator [] (const size_t) const;       // Operator.

    inline Allocator& allocator();                          // Query the allocator.
    inline const Allocator& allocator() const;              // Query the allocator.
    
  private:

    size_t _size;                                           // Size.

    DataBlock _data;
    RecyBlock _recy;

    Allocator _allocator;
};  

// Mutable iterator. ------------------------------------------------------------------------------

// Operator: ++ ()
template <typename T, typename Allocator, size_t Cap>
inline typename ArrayList<T, Allocator, Cap>::Iterator&
ArrayList<T, Allocator, Cap>::Iterator::operator ++ () { 
  ++cursor;
  while(cursor != end && *cursor == nullptr) ++cursor;
  return *this;
}

// Operator: * ()
template <typename T, typename Allocator, size_t Cap>
inline T* ArrayList<T, Allocator, Cap>::Iterator::operator * () {
  return *cursor;
}

// Operator: !=
template <typename T, typename Allocator, size_t Cap>
inline bool ArrayList<T, Allocator, Cap>::Iterator::operator != (const Iterator& rhs) {
  return cursor != rhs.cursor;
}

// ArrayList. ------------------------------------------------------------------------------------

// Constructor.
template <typename T, typename Allocator, size_t Cap>
inline ArrayList<T, Allocator, Cap>::ArrayList() {

  // Set the number of elements to zero.
  _size = 0;

  // Initialize the data block.
  _data.block = static_cast<T**>(malloc(sizeof(T*)*Cap));
  _data.size = Cap;
  _data.cursor = 0;

  // Initialize the recy block.
  _recy.block = static_cast<size_t*>(malloc(sizeof(size_t)*Cap));
  _recy.size = Cap;
  _recy.cursor = 0;
}

// Destructor
template <typename T, typename Allocator, size_t Cap>
inline ArrayList<T, Allocator, Cap>::~ArrayList() {

  // Free the data block.
  for(size_t i=0; i<_data.cursor; ++i) {
    if(_data.block[i] != nullptr) {
      allocator().destroy(_data.block[i]);
      allocator().deallocate(_data.block[i], 1);
    }
  }

  free(_data.block);

  // Free the recyblock.
  free(_recy.block);
}

// Function: allocator
template < class T, class Allocator, size_t Cap >
inline Allocator& ArrayList<T, Allocator, Cap>::allocator() {
  return _allocator;
}

// Function: allocator
template < class T, class Allocator, size_t Cap >
inline const Allocator& ArrayList<T, Allocator, Cap>::allocator() const {
  return _allocator;
}

// Function: begin
// Return the begining position of the iterator in rvalue type.
template <typename T, typename Allocator, size_t Cap>
inline typename ArrayList<T, Allocator, Cap>::Iterator ArrayList<T, Allocator, Cap>::begin() {
  return {_data.block, _data.block + _data.cursor};
}

// Function: begin
// Return the ending position of the iterator in rvalue type.
template <typename T, typename Allocator, size_t Cap>
inline typename ArrayList<T, Allocator, Cap>::Iterator ArrayList<T, Allocator, Cap>::end() {
  return {_data.block + _data.cursor, _data.block + _data.cursor};
}

// Function: num_indices
template <typename T, typename Allocator, size_t Cap>
inline size_t ArrayList<T, Allocator, Cap>::num_indices() const {
  return _data.cursor;
}

// Operator
template <typename T, typename Allocator, size_t Cap>
inline T* ArrayList<T, Allocator, Cap>::operator [] (const size_t idx) {
  return idx < num_indices() ? _data.block[idx] : nullptr;
}

// Function: size
template <typename T, typename Allocator, size_t Cap>
inline size_t ArrayList<T, Allocator, Cap>::size() const {
  return _size;
}

// Procedure: remove
// Remove an item that is specified by the idx.
template <typename T, typename Allocator, size_t Cap>
inline void ArrayList<T, Allocator, Cap>::remove(const size_t idx) {

  auto ptr = operator [] (idx);
  if(ptr == nullptr) return;

  allocator().destroy(_data.block[idx]);
  allocator().deallocate(_data.block[idx], 1);

  _data.block[idx] = nullptr;
  --_size;

  // Reallocate the recycle idx array if it is full.
  if(_recy.cursor == _recy.size) {
    _recy.size = _recy.size << 1;
    _recy.block = static_cast<size_t*>(realloc(_recy.block, sizeof(size_t)*_recy.size));
  }

  // Insert the deallocated item to the end of the free entry array.
  _recy.block[_recy.cursor++] = idx;
}

// Procedure: insert
// Insert a new item.
template <typename T, typename Allocator, size_t Cap>
template <typename... ArgsT>
inline size_t ArrayList<T, Allocator, Cap>::insert(ArgsT&&... args) {

  size_t idx;

  if(_recy.cursor) {
    idx = _recy.block[--_recy.cursor];
  }
  else {
    if(_data.cursor == _data.size) {
      _data.size = _data.size << 1;
      _data.block = static_cast<T**>(realloc(_data.block, sizeof(T*)*_data.size));
    }
    idx = _data.cursor++;
  }
  
  ++_size;  
  allocator().construct(_data.block[idx] = allocator().allocate(1), ::std::forward<ArgsT>(args)...);
  return idx;
}

};  // End of namespace OpenCluster. --------------------------------------------------------------

#endif




