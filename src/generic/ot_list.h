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

#ifndef OT_LIST_H_
#define OT_LIST_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <memory>

namespace OpenTimer {

// Class: ListNode
// This class defines the structure of the list node.
template <class T>
class ListNode
{
  public:
   
    ListNode(const T &t): _item(t) {}
    ListNode(T&& t) : _item(t) {}
    
    inline T &item();                                       // Query the item.
    inline const T& item() const;                           // Query the const item.
    inline ListNode *next() const;                          // Query the next item.
    inline ListNode *prev() const;                          // Query the previous item.
    inline void set_next(ListNode*);                        // Set the next pointer.
    inline void set_prev(ListNode*);                        // Set the previous pointer.

    T _item;                                                // _item.
    ListNode *_next{nullptr};                               // Pointer to the next item.
    ListNode *_prev{nullptr};                               // Pointer to the prev item.  
};

// Function: item
// Return the reference to the item.
template <class T>
inline T& ListNode<T>::item() {
  return _item;
}

// Function: item
// Return the constant reference to the item.
template <class T>
inline const T& ListNode<T>::item() const {
  return _item;
}

// Function: next
// Return the next pointer.
template <class T>
inline ListNode<T>* ListNode<T>::next() const {
  return _next;
}

// Function: prev
// Return the prev pointer.
template <class T>
inline ListNode<T>* ListNode<T>::prev() const {
  return _prev;
}

// Procedure: set_next
template <class T>
inline void ListNode<T>::set_next(ListNode<T>* next) {
  _next = next;
}

// Procedure: set_prev
template <class T>
inline void ListNode<T>::set_prev(ListNode<T>* prev) {
  _prev = prev;
}

//--------------------------------------------------------------------------------------------------
// Class: List
// This class defines the general structure of the list.
template < class T, class Allocator = ::std::allocator<ListNode<T>> >
class List : public Allocator {

  public:

    using listnode_t = ListNode<T>;                         // Listnode type.
    using listnode_pt = ListNode<T>*;                       // Listnode ptr type.
    using listnode_rt = ListNode<T>&;                       // Listnode ref type.

    struct iterator {
      listnode_pt cursor;                                   // Cursor.
      inline iterator& operator ++ ();                      // Prefix operator.
      inline T& operator * ();                              // Deref operator.
      inline bool operator != (const iterator&);            // Comparator.
    };

    using iter_t = iterator;                                // Iterator type.
    using iter_pt = iterator*;                              // Iterator ptr type.
    using iter_rt = iterator&;                              // Iterator ref type.
    using iter_ct = const iterator;                         // Const iterator type.
    using iter_crt = const iterator&;                       // Const iterator ref type.

    inline List();                                          // Constructor.
    inline ~List();                                         // Destructor.

    inline void clear();                                    // Clear the list.
    inline void pop_back();                                 // Pop back.
    inline void pop_front();                                // Pop front.

    inline iter_t begin();                                  // Begin iterator.
    inline iter_t end();                                    // End iterator.

    inline void remove(ListNode<T> *);                      // Remove a node.
    template <typename... ArgsT>
    inline listnode_pt push_front(ArgsT&&...);              // Push front.
    template <typename... ArgsT>
    inline listnode_pt push_back(ArgsT&&...);               // Push back.
        
    inline size_t size() const;                             // Query the size.
    inline bool empty() const;                              // Query the size status.
    inline ListNode <T> *head() const;                      // Query the head ptr.
    inline ListNode <T> *tail() const;                      // Query the tail ptr.

    inline Allocator* allocator();                          // Query the allocator.

  private:

    listnode_pt _head;                                      // Head.
    listnode_pt _tail;                                      // Tail.
        
    size_t _size;                                           // Size.
};

// Mutable iterator. ------------------------------------------------------------------------------

// Operator: ++
template < class T, class Allocator >
typename List<T, Allocator>::iter_rt List<T, Allocator>::iterator::operator ++ () {
  cursor = cursor->next();
  return *this;
}

// Operator: *
template < class T, class Allocator >
T& List<T, Allocator>::iterator::operator * () {
  return cursor->item();
}

// Operator: !=
template < class T, class Allocator >
bool List<T, Allocator>::iterator::operator != (const iterator& rhs) {
  return cursor != rhs.cursor;
}

// List. ------------------------------------------------------------------------------------------

// Procedure: List
// Constructor of the class.
template < class T, class Allocator >
List<T, Allocator>::List():
  _head(nullptr),
  _tail(nullptr),
  _size(0) {
}

// Procedure: List
// Destructor of the class.
template < class T, class Allocator >
List<T, Allocator>::~List() {
  clear();
}

// Function: allocator
template < class T, class Allocator >
inline Allocator* List<T, Allocator>::allocator() {
  return static_cast <Allocator*> (this);   
}

// Procedure: clear
// Clear the list.
template < class T, class Allocator >
void List<T, Allocator>::clear() {
  while(!empty()) pop_back();
}

// Function: size
// Return the number of items in the list.
template < class T, class Allocator >
inline size_t List<T, Allocator>::size() const {
  return _size;
}

// Function: empty
// Return true for empty list, otherwise false.
template < class T, class Allocator >
inline bool List<T, Allocator>::empty() const {
  return !_size;
}

// Function: head
// Return the head pointer of the list.
template < class T, class Allocator >
inline ListNode <T>* List<T, Allocator>::head() const {
  return _head;
}

// Function: tail
// Return the tail pointer of the list.
template < class T, class Allocator >
inline ListNode <T>* List<T, Allocator>::tail() const {
  return _tail;
}

// Function: begin
// Return the beginning iterator.
template < class T, class Allocator >
inline typename List<T, Allocator>::iterator List<T, Allocator>::begin() {
  return { head() };
}

// Function: end
// Return the ending iterator.
template < class T, class Allocator >
inline typename List<T, Allocator>::iterator List<T, Allocator>::end() {
  return { nullptr }; 
}

// Function: push_back
// Inserting a new item from the back of list, and returning the address of the
// corresponding list node.
template < class T, class Allocator >
template < typename... ArgsT >
inline typename List<T, Allocator>::listnode_pt List<T, Allocator>::push_back(ArgsT&&...args) {

  auto node_ptr = allocator()->allocate(1);
  allocator()->construct(node_ptr, ::std::forward<ArgsT>(args)...);

  if(empty()) {
    _head = node_ptr;
    _tail = node_ptr;
  }
  else {
    _tail->set_next(node_ptr);
    node_ptr->set_prev(_tail);
    _tail = node_ptr;
  }
  ++_size;
  return node_ptr;
}

// Function: push_front
// Inserting a new item from the front of the list, then returning the address
// of the list node containing the item.
template < class T, class Allocator >
template < typename... ArgsT >
inline typename List<T, Allocator>::listnode_pt List<T, Allocator>::push_front(ArgsT&&...args) {
  
  auto node_ptr = allocator()->allocate(1);
  allocator()->construct(node_ptr, ::std::forward<ArgsT>(args)...);

  if(empty()) {
    _head = node_ptr;
    _tail = node_ptr;
  }
  else {
    _head->set_prev(node_ptr);
    node_ptr->set_next(_head);
    _head = node_ptr;
  }
  ++_size;
  return node_ptr;
}

// Procedure: pop_back
// Pop out a list node from the back with clearing its memory.
template < class T, class Allocator >
inline void List<T, Allocator>::pop_back() {
  
  if(empty()) return;

  ListNode <T> *ptr = _tail;    
  _tail = _tail->prev();

  if(_tail) _tail->set_next(nullptr);
  else _head = nullptr;

  allocator()->destroy(ptr);
  allocator()->deallocate(ptr, 1);
  --_size;
}

// Procedure: pop_front
// Pop out a list node from the beginning with clearing its memory.
template < class T, class Allocator >
inline void List<T, Allocator>::pop_front() {

  if(empty()) return;

  ListNode <T> *tmp = _head;
  _head = _head->next();

  if(_head) _head->set_prev(nullptr);
  else _tail = nullptr;
    
  allocator()->destroy(tmp);
  allocator()->deallocate(tmp, 1);

  --_size;
}

// Procedure: remove
// Erase a list node and clear its memory.
template < class T, class Allocator >
inline void List<T, Allocator>::remove(listnode_pt node_ptr) {
  
  // Unchecked, for the case node_ptr not in List.
  if(node_ptr == nullptr) return;
  
  if(node_ptr == _head) pop_front();
  else if(node_ptr==_tail) pop_back();
  else {
    node_ptr->prev()->set_next(node_ptr->next());
    node_ptr->next()->set_prev(node_ptr->prev());

    allocator()->destroy(node_ptr);
    allocator()->deallocate(node_ptr, 1);

    --_size;
  }
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif


