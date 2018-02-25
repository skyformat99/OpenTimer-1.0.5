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

#ifndef OT_DICTIONARY_H_
#define OT_DICTIONARY_H_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <memory>

namespace OpenTimer {

// Class: Dictionary
// K: key type
// T: obj type
// The dictionary provides a similar interface to standard "unordered_map" except that
// (1) The object is stored using unique_ptr
// (2) Object is accessed through pointer.
// (3) Operator [] is not forced-to-insert.
template <
  class K, class V, 
  class Hash = ::std::hash<K>, 
  class KeyEqual = ::std::equal_to<K>,
  class Allocator = ::std::allocator< ::std::pair<const K, V*> >
>
class Dictionary : private ::std::unordered_map <K, V*, Hash, KeyEqual> {
  
  public:
  
    using base = ::std::unordered_map <K, V*, Hash, KeyEqual>;        // Base type.
    using iter_t = typename base::iterator;                           // Iter.
    using iter_ct = typename base::const_iterator;                    // Constant iter.
    
    template <typename... ArgsT>
    inline Dictionary(ArgsT&&...);                                    // Constructor.
    inline ~Dictionary();                                             // Destructor.

    inline bool empty() const;                                        // Query the size status.

    inline size_t size() const;                                       // Query the size.
    inline size_t max_size() const;                                   // Query the size.
    
    inline void clear();                                              // Clear the hash table.
    inline void rehash(size_t);                                       // Rehash the table.
    inline void remove(const K &);                                    // Erase an item.
    
    template <typename... ArgsT>
    inline V* insert(const K&, ArgsT&&...);                           // Insert an new item.
    inline V* rehash(const K&, const K&);                             // Rekey an item.

    inline iter_t begin();                                            // Query the iter_t.
    inline iter_ct begin() const;                                     // Query the iter_t.
    inline iter_t end();                                              // Query the iter_t.
    inline iter_ct end() const;                                       // Query the iter_t.

    inline V* operator [] (const K&);                                 // Operator [].
    inline const V* operator [] (const K&) const;                     // Operator [].

  private:
    
};

// Constructor
template <class K, class V, class Hash, class KeyEqual, class Allocator>
template <typename... ArgsT>
inline Dictionary<K, V, Hash, KeyEqual, Allocator>::Dictionary(ArgsT&&... args) :
  base(::std::forward<ArgsT>(args)...) {
}

// Destructor
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline Dictionary<K, V, Hash, KeyEqual, Allocator>::~Dictionary() {
  clear();
}

// Function: size
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline size_t Dictionary<K, V, Hash, KeyEqual, Allocator>::size() const {
  return base::size();
}

// Function: max_size
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline size_t Dictionary<K, V, Hash, KeyEqual, Allocator>::max_size() const {
  return base::max_size();
}

// Function: empty
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline bool Dictionary<K, V, Hash, KeyEqual, Allocator>::empty() const {
  return base::empty();
}

// Procedure: clear
// Clear the object.
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline void Dictionary<K, V, Hash, KeyEqual, Allocator>::clear() {
  for(auto itr = base::begin(); itr != base::end(); ++itr) {
    delete itr->second;
  }
  base::clear();
}

// Function: begin
template <class K, class V, class Hash, class KeyEqual, class Allocator>
typename Dictionary<K, V, Hash, KeyEqual, Allocator>::iter_t 
Dictionary<K, V, Hash, KeyEqual, Allocator>::begin() {
  return base::begin();
}

// Function: begin
template <class K, class V, class Hash, class KeyEqual, class Allocator>
typename Dictionary<K, V, Hash, KeyEqual, Allocator>::iter_ct 
Dictionary<K, V, Hash, KeyEqual, Allocator>::begin() const {
  return base::begin();
}

// Function: end
template <class K, class V, class Hash, class KeyEqual, class Allocator>
typename Dictionary<K, V, Hash, KeyEqual, Allocator>::iter_t 
Dictionary<K, V, Hash, KeyEqual, Allocator>::end() {
  return base::end();
}

// Function: end
template <class K, class V, class Hash, class KeyEqual, class Allocator>
typename Dictionary<K, V, Hash, KeyEqual, Allocator>::iter_ct 
Dictionary<K, V, Hash, KeyEqual, Allocator>::end() const {
  return base::end();
}

// Operator: []
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline const V* Dictionary<K, V, Hash, KeyEqual, Allocator>::operator [] (const K &key) const {
  auto itr = base::find(key);
  return (itr == base::end()) ? nullptr : itr->second;
}

// Operator: []
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline V* Dictionary<K, V, Hash, KeyEqual, Allocator>::operator [] (const K &key) {
  auto itr = base::find(key);
  return (itr == base::end()) ? nullptr : itr->second;
}

// Function: insert
template <class K, class V, class Hash, class KeyEqual, class Allocator>
template <typename ... ArgsT>
inline V* Dictionary<K, V, Hash, KeyEqual, Allocator>::insert(const K &key, ArgsT&&... args) {
  auto itr = base::find(key);
  if(itr == base::end()) {
    return base::operator[](key) = new V(args...);
  }
  return itr->second;
}

// Procedure: remove
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline void Dictionary<K, V, Hash, KeyEqual, Allocator>::remove(const K &key) {
  auto itr = base::find(key);
  if(itr != base::end()) {
    delete itr->second;
    base::erase(itr);
  }
}

// Procedure: rehash
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline void Dictionary<K, V, Hash, KeyEqual, Allocator>::rehash(size_t prime) {
  base::rehash(prime);
}

// Function: rehash
// Rehash an item with a new key value. Note that it's user's responsibility to change the
// object's key appropriately.
template <class K, class V, class Hash, class KeyEqual, class Allocator>
inline V* Dictionary<K, V, Hash, KeyEqual, Allocator>::rehash(const K &old_key, const K &new_key) {
  auto itr = base::find(old_key);
  if(itr == base::end()) return nullptr;
  remove(new_key);
  V* v = base::operator[](new_key) = itr->second;
  base::erase(itr);
  return v;
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif

