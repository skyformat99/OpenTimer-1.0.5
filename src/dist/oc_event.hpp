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

#ifndef OC_EVENT_HPP_
#define OC_EVENT_HPP_

#include "oc_classdef.hpp"
#include "oc_ticker.hpp"
#include "oc_iostream.hpp"

namespace OpenCluster {

// Class: EventState
// The class defines the state of an event. The event state can be categorized to (1) static
// event state and (2) runtime event state. Static refers to the "fixed" states that should 
// be bound to the event and Dynamic refers to the "changeable" states that might be assigned
// during the event loop.
class EventState {

  private: 
    
    int _state {UNDEFINED};

    static_assert(
      sizeof(_state >= 4), 
      "Fail to compile EventState (int32_t is not supported)"
    );

  public:

    // Static states.
    constexpr static int UNDEFINED      {0x0};
    constexpr static int TIMEOUT        {0x1};                  
    constexpr static int READ           {0x2};                  
    constexpr static int WRITE          {0x4};                  
    constexpr static int SIGNAL         {0x8};                  
    constexpr static int PERSISTENT     {0x10};                 

    // Runtime states: reactor monitored status.
    constexpr static int IN_ACTIVELIST  {0x20};
    constexpr static int ACTIVE_TIMEOUT {0x40};
    constexpr static int ACTIVE_READ    {0x80};
    constexpr static int ACTIVE_WRITE   {0x100};

    // Runtime states: on callback status.
    constexpr static int ON_TIMEOUT     {0x200};
    constexpr static int ON_READ        {0x400};
    constexpr static int ON_WRITE       {0x800};

    inline int state() const;
    
    inline bool has_state(const int&) const;
    inline bool is_undefined() const;
    inline bool is_timeout() const;
    inline bool is_read() const;
    inline bool is_write() const;
    inline bool is_io() const;
    inline bool is_persistent() const;
    inline bool on_timeout() const;
    inline bool on_read() const;
    inline bool on_write() const;
    
    inline void clear_state();
    inline void set_state(const int&);
    inline void insert_states(const int&);
    inline void remove_states(const int&);
    inline void remove_runtime_states();

    template <typename... ArgsT>
    inline void insert_states(const int&, ArgsT&&...);
    
    template <typename... ArgsT>
    inline void remove_states(const int&, ArgsT&&...);
};

// Static member declaration.
constexpr int EventState::UNDEFINED;                  
constexpr int EventState::TIMEOUT;                  
constexpr int EventState::READ;                  
constexpr int EventState::WRITE;                  
constexpr int EventState::SIGNAL;                  
constexpr int EventState::PERSISTENT;                 
constexpr int EventState::IN_ACTIVELIST;
constexpr int EventState::ACTIVE_TIMEOUT;
constexpr int EventState::ACTIVE_READ;
constexpr int EventState::ACTIVE_WRITE;
constexpr int EventState::ON_TIMEOUT;
constexpr int EventState::ON_READ;
constexpr int EventState::ON_WRITE;

// Function: state
inline int EventState::state() const {
  return _state;
}

// Procedure: set_state
inline void EventState::set_state(const int& s) {
  _state = s;
}

// Function: has_state
inline bool EventState::has_state(const int& s) const {
  return _state & s;
}

// Procedure: insert
inline void EventState::insert_states(const int& s) {
  _state |= s;
}

// Procedure: insert
template <typename... ArgsT>
inline void EventState::insert_states(const int& s, ArgsT&&... args) {
  _state |= s;
  insert_states(::std::forward<ArgsT>(args)...); 
}

// Procedure: remove_states
inline void EventState::remove_states(const int& s) {
  _state &= ~s;
}

// Procedure: remove_states
template <typename... ArgsT>
inline void EventState::remove_states(const int& s, ArgsT&&... args) {
  _state &= ~s;
  remove_states(::std::forward<ArgsT>(args)...); 
}

// Procedure: remove_runtime
inline void EventState::remove_runtime_states() {
  remove_states(
    IN_ACTIVELIST, ACTIVE_TIMEOUT, ACTIVE_READ, ACTIVE_WRITE, 
    ON_TIMEOUT, ON_READ, ON_WRITE
  );
}

// Procedure: clear
inline void EventState::clear_state() {
  _state = UNDEFINED;
}

// Function: is_undefined
inline bool EventState::is_undefined() const {
  return _state == UNDEFINED;
}
    
// Function: is_timeout
inline bool EventState::is_timeout() const {
  return has_state(TIMEOUT);
}

// Function: is_read
inline bool EventState::is_read() const {
  return has_state(READ);
}

// Function: is_write
inline bool EventState::is_write() const {
  return has_state(WRITE);
}

// Function: is_io
inline bool EventState::is_io() const {
  return has_state(READ) || has_state(WRITE);
}

// Function: is_persistent
inline bool EventState::is_persistent() const {
  return has_state(PERSISTENT);
}

// Function: on_timeout
inline bool EventState::on_timeout() const {
  return has_state(ON_TIMEOUT);
}

// Function: on_read
inline bool EventState::on_read() const {
  return has_state(ON_READ);
}

// Function: on_write
inline bool EventState::on_write() const {
  return has_state(ON_WRITE);
}

//-------------------------------------------------------------------------------------------------

// Class: BasicEvent
// The basic event class.
template <typename EventT>
class BasicEvent : public EventState {

  public:

    // Handler type.
    using handler_t = void (*) (EventT*);
  
  private:
    
    int _idx                   {-1};
    int _fd                    {-1};    
    int _pq_satellite          {-1};
    handler_t _handler         {nullptr};
    EventT* _next_active_event {nullptr};
    Reactor<EventT>* _reactor  {nullptr};

    ::std::unique_ptr<Ticker> _ticker_uptr;
    ::std::unique_ptr<InputStream<StreamBuffer>> _istream_uptr;
    ::std::unique_ptr<OutputStream<StreamBuffer>> _ostream_uptr;

  public:
    
    // Accessor.
    OC_CODEGEN_ACCESSOR(idx)
    OC_CODEGEN_ACCESSOR(fd)
    OC_CODEGEN_ACCESSOR(reactor)
    OC_CODEGEN_ACCESSOR(pq_satellite)
    OC_CODEGEN_ACCESSOR(handler)
    OC_CODEGEN_ACCESSOR(next_active_event)
    OC_CODEGEN_ACCESSOR(ticker_uptr)
    OC_CODEGEN_ACCESSOR(istream_uptr)
    OC_CODEGEN_ACCESSOR(ostream_uptr)
    OC_CODEGEN_ACCESSOR_UPTR(ticker)
    OC_CODEGEN_ACCESSOR_UPTR(istream)
    OC_CODEGEN_ACCESSOR_UPTR(ostream)

    // Mutator.
    OC_CODEGEN_MUTATOR(reactor)
    OC_CODEGEN_MUTATOR(idx);
    OC_CODEGEN_MUTATOR(fd);
    OC_CODEGEN_MUTATOR(pq_satellite);
    OC_CODEGEN_MUTATOR(handler);
    OC_CODEGEN_MUTATOR(next_active_event);
    OC_CODEGEN_MUTATOR_UPTR(ticker);
    OC_CODEGEN_MUTATOR_UPTR(istream);
    OC_CODEGEN_MUTATOR_UPTR(ostream);
}; 

//-------------------------------------------------------------------------------------------------

// Class: EventHeap
template <typename EventT, typename LessT>
class EventHeap {

  private:

    ::std::vector<EventT*> _array;

  public:

    void clear();
    void remove(EventT*);
    void insert(EventT*);

    constexpr bool empty() const;
    
    constexpr size_t size() const;

    EventT* pop();
    constexpr EventT* top() const;

  private:
    
    void _bubble_up(size_t, EventT*);
    void _bubble_down(size_t, EventT*);
};

// Function: empty
template <typename EventT, typename LessT>
constexpr bool EventHeap<EventT, LessT>::empty() const {
  return _array.empty();
}

// Function: top 
template <typename EventT, typename LessT>
constexpr EventT* EventHeap<EventT, LessT>::top() const {
  return empty() ? nullptr : _array[0];
}

// Function: size
template <typename EventT, typename LessT>
constexpr size_t EventHeap<EventT, LessT>::size() const {
  return _array.size();
}

// Procedure: clear 
template <typename EventT, typename LessT>
void EventHeap<EventT, LessT>::clear() {

  // Clear all heap-related field for the event.
  for(auto e : _array) {
    e->set_pq_satellite(-1);
  }
  _array.clear();
}

// Procedure: remove   
template <typename EventT, typename LessT>
void EventHeap<EventT, LessT>::remove(EventT* e) {

  // Invalid removal.
  if(e == nullptr || e->pq_satellite() == -1) return;

  // Event exists in the heap.
  auto last = _array.back();
  _array.pop_back();

  if(e->pq_satellite() && LessT()(*last, *_array[(e->pq_satellite()-1) >> 1])) {
    _bubble_up(e->pq_satellite(), last);
  }
  else {
    _bubble_down(e->pq_satellite(), last);
  }
  e->set_pq_satellite(-1);
}

// Function: insert 
template <typename EventT, typename LessT>
void EventHeap<EventT, LessT>::insert(EventT* e) {
  if(e == nullptr || e->pq_satellite() != -1) return;
  _array.push_back(e);
  _bubble_up(_array.size()-1, e);
}

// Function: pop 
template <typename EventT, typename LessT>
EventT* EventHeap<EventT, LessT>::pop() {
  if(_array.size()) {
    auto e = _array[0]; 
    auto last = _array.back();
    _array.pop_back();
    _bubble_down(0, last);
    e->set_pq_satellite(-1);
    return e;
  }
  return nullptr;
}

// Procedure: _bubble_up
template <typename EventT, typename LessT>
void EventHeap<EventT, LessT>::_bubble_up(size_t idx, EventT* e) {
  while(idx) {
    size_t parent = (idx - 1) >> 1;
    if(!LessT()(*e, *_array[parent])) {
      break;
    }
    (_array[idx] = _array[parent])->set_pq_satellite(idx);
    idx = parent;
  }
  (_array[idx] = e)->set_pq_satellite(idx);
}

// Procedure: _bubble_down 
template <typename EventT, typename LessT>
void EventHeap<EventT, LessT>::_bubble_down(size_t idx, EventT* e) {
  while(1) {
    size_t min_child = (idx + 1) << 1;
    if(min_child >= _array.size() || LessT()(*_array[min_child-1], *_array[min_child])) {
      --min_child;
    }
    if(min_child >= _array.size() || !(LessT()(*_array[min_child], *e))) break;
    (_array[idx] = _array[min_child])->set_pq_satellite(idx);
    idx = min_child;
  }
  (_array[idx] = e)->set_pq_satellite(idx);
}


};  // End of namespace OpenCluster. --------------------------------------------------------------







#endif



