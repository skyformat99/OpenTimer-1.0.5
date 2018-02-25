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

#ifndef OC_REACTOR_HPP_
#define OC_REACTOR_HPP_

#include "oc_event.hpp"
#include "oc_demux.hpp"

namespace OpenCluster {

// Struct: ReactorPolicy
struct ReactorPolicy {

};

// Class: Reactor
// The gateway class for event-driven programming.
template <typename EventT>
class Reactor {
    
  // Event comparator.
  struct EventTimeoutLessT {
    constexpr bool operator () (const EventT& lhs, const EventT& rhs) const {
      return lhs.ticker()->timeout() < rhs.ticker()->timeout();
    }
  };

  private:

    bool _loop_break {false};

    EventT* _head_active_event {nullptr};
    
    ::std::chrono::steady_clock::time_point _sync_time_point {now()};

    ArrayList <EventT, SingularAllocator<EventT>> _eventlist;
    EventHeap <EventT, EventTimeoutLessT> _timeoutpq;
    
    // TODO: user configure to automatically choose the best demux.
    Select <EventT> _demux;

  public:
    
    // Constructor and destructor.
    Reactor();                                          
    ~Reactor();                                          
    
    // Accessor.
    OC_CODEGEN_ACCESSOR(sync_time_point)
    OC_CODEGEN_ACCESSOR(eventlist)
    OC_CODEGEN_ACCESSOR(timeoutpq)
    OC_CODEGEN_ACCESSOR(head_active_event)
    OC_CODEGEN_ACCESSOR(demux);
    OC_CODEGEN_ACCESSOR(loop_break);
    
    // Mutator.
    OC_CODEGEN_MUTATOR(head_active_event)        
    OC_CODEGEN_MUTATOR(loop_break);        
    
    // Size query.
    OC_CODEGEN_SIZE_QUERY(eventlist, event)               
    OC_CODEGEN_SIZE_QUERY(timeoutpq, ticked_event)    
    
    // Disable member functions.
    OC_CODEGEN_DISABLE_COPY(Reactor);
    OC_CODEGEN_DISABLE_MOVE(Reactor);
    
    // Reactor routines.
    void dispatch();                                        
    void remove(EventT*);                                   
    void commit(EventT*);                                   
    void uncommit(EventT*);

    template <typename... ArgsT>
    EventT* insert(ArgsT&&...);
    
    template <typename... ArgsT>
    EventT* insert_io_event(const int, typename EventT::handler_t, ArgsT&&...);

    template <typename D, typename... ArgsT>
    EventT* insert_timeout_event(const D&, const bool, typename EventT::handler_t, ArgsT&&...);

  private:

    void _activate_timeout_events();
    void _activate_io_events();
    void _process_active_events();
    void _process_active_timeout_event(EventT*);
    void _process_active_read_event(EventT*);
    void _process_active_write_event(EventT*);

    static void on_active_event(const int, EventT*);
};

// Constructor
template <typename EventT>
Reactor<EventT>::Reactor() {
  demux().set_handler(on_active_event);
}

// Destructor
template <typename EventT>
Reactor<EventT>::~Reactor() {
}

// Procedure: on_active_event
template <typename EventT>
void Reactor<EventT>::on_active_event(const int active_state, EventT* event) {

  if(event == nullptr || event->reactor() == nullptr) {
    LOGE("Failed to handle demuxed event (nullptr exception)");
    return;
  }

  auto reactor = event->reactor();

  // Insert the callback flag.
  event->insert_states(active_state);
  
  // Insert the event into the active eventlist.
  if(!event->has_state(EventState::IN_ACTIVELIST)) {

    // Insert the event into the head of the active event list.
    if(reactor->head_active_event() != nullptr) {
      event->set_next_active_event(reactor->head_active_event());
    }
    reactor->set_head_active_event(event);

    // Mark the event active status.
    event->insert_states(EventState::IN_ACTIVELIST);
  }
}
    
// Function: insert_io_event
// Insert an event and make it an IO event with respect to a given file descriptor.
template <typename EventT>
template <typename... ArgsT>
EventT* Reactor<EventT>::insert_io_event(
  const int fd, 
  typename EventT::handler_t handler, 
  ArgsT&&... args
) {

  if(!is_valid_fd(fd)) return nullptr;

  auto event = insert(::std::forward<args>...);

  event->set_fd(fd);
  event->set_state(
    EventState::WRITE | 
    EventState::READ  | 
    EventState::PERSISTENT
  );
  event->set_handler(handler);
  event->make_istream();
  event->make_ostream();

  return event;
}

// Function: insert_timeout_event
// Insert an event and make it a timeout event with respect to a given interval.
template <typename EventT>
template <typename D, typename... ArgsT>
EventT* Reactor<EventT>::insert_timeout_event(
  const D& d, 
  const bool p, 
  typename EventT::handler_t handler, 
  ArgsT&&... args
) {
  auto event = insert(::std::forward<args>...);
  event->set_state(p ? EventState::TIMEOUT | EventState::PERSISTENT : EventState::TIMEOUT);
  event->set_handler(handler);
  event->make_ticker();
  event->ticker()->set_interval(d);
  return event;
}
    
// Procedure: dispatch 
template <typename EventT>
void Reactor<EventT>::dispatch() {

  while(1) {

    // Terminated the loop?
    if(loop_break()) break;

    // Activate the io events.
    _activate_io_events();

    // Synchronize the time point.
    _sync_time_point = now();

    // Activate timeout events.
    _activate_timeout_events();
    
    // TODO
    // Idle?

    // Process all active events and invoke callbacks appropriately.
    _process_active_events();

  }

}
    
// Function: insert
// Insert an event into the reactor and return the pointer to the event.
template <typename EventT>
template <typename... ArgsT>
EventT* Reactor<EventT>::insert(ArgsT&&... args) {
  auto idx = eventlist().insert(::std::forward<ArgsT>(args)...);
  auto event = eventlist()[idx];
  event->set_idx(idx);
  event->set_reactor(this);
  return event;
}
    
// Procedure: remove 
// Remove a given event from the reactor.
template <typename EventT>
void Reactor<EventT>::remove(EventT* event) {

  // Nil event.
  if(event == nullptr || event->reactor() != this) return;
  
  // Remove the event.
  // TODO
  eventlist().remove(event->idx());
}
    
// Procedure: commit 
// Commit the event. An event won't actually function before the commit call.
template <typename EventT>
void Reactor<EventT>::commit(EventT* event) {

  // Nil event or does not belong to the reactor.
  if(event == nullptr || event->reactor() != this) return;
  
  // Case 1:
  // Timeout event should be inserted to the priority queue.
  if(event->is_timeout() && event->pq_satellite() == -1) {

    // Set the timeout of the event in relative to the last sync point and insert the event
    // into the priority queue (keyed on timeout).
    event->ticker()->set_timeout(sync_time_point() + event->ticker()->interval().time_since_epoch());
    timeoutpq().insert(event);
  }
  
  // Case 2:
  // IO events should be processed by the back-end de-multiplexer.
  if(event->is_io()) {
    demux().insert(event);
  }

  // Case 3:
  // Signal events - TODO
}

// Procedure: uncommit
// Uncommit the event. The event will be removed from the IO demux and priority queue.
template <typename EventT>
void Reactor<EventT>::uncommit(EventT* event) {

  if(event == nullptr || event->reactor() != this) return;

  // Remove all runtime status (only possible after commit).
  event->remove_runtime_states();

  // Remove event from timeout priority queue.
  timeoutpq().remove(event);

  // Remove event from the select.
  demux().remove(event);

}

// Procedure: _activate_timeout_events 
// The procedure removes activate timeout events (those events with timeout value passing the
// last synchronization point) from the priority queue.
template <typename EventT>
void Reactor<EventT>::_activate_timeout_events() {

  while(!timeoutpq().empty()) {

    // Stop if the minimum timeout is not yet reached.
    if(timeoutpq().top()->ticker()->timeout() > sync_time_point()) break;

    // Remove the timeout event.
    auto event = timeoutpq().pop();
  
    // Persistent timeout event should be re-inserted into the priority queue.
    if(event->is_persistent()) {
      event->ticker()->set_timeout(sync_time_point() + event->ticker()->interval().time_since_epoch());
      timeoutpq().insert(event);
    }

    // Insert the event into the active list.
    on_active_event(EventState::ACTIVE_TIMEOUT, event);
  }
}

// Procedure: _activate_io_events
// The procedure extracts the activate events from the demux and insert them into the active event 
// list through the procedure on_active_event.
template <typename EventT>
void Reactor<EventT>::_activate_io_events() {
  
  // Obtain the waiting time for this round.
  auto event = timeoutpq().top();

  ::std::chrono::milliseconds duration(0); 

  if(event == nullptr) {
    duration = make_duration<::std::chrono::milliseconds>(100);
  }
  else {
    if(event->ticker()->timeout() > sync_time_point()) {
      duration = duration_cast<::std::chrono::milliseconds>(event->ticker()->timeout() - sync_time_point());
    }
  }

  // Poll.
  demux().poll(duration);
}

// _process_active_timeout_event
// The procedure handle the timeout event.
template <typename EventT>
void Reactor<EventT>::_process_active_timeout_event(EventT* event) {
  if(!event->has_state(EventState::ACTIVE_TIMEOUT)) return;

  event->insert_states(EventState::ON_TIMEOUT);
  event->handler()(event);
  event->remove_states(EventState::ON_TIMEOUT);

  event->remove_states(EventState::ACTIVE_TIMEOUT);
}

// _process_active_read_event
// Handle the read event, where we have to synchronize the input stream with the underlying
// device specified by the file descriptor of the event and then invoke the event handler.
template <typename EventT>
void Reactor<EventT>::_process_active_read_event(EventT* event) {
      
  if(!event->has_state(EventState::ACTIVE_READ)) return;

  // TODO: check the iostate
  if(event->istream()) {
    event->istream()->sync(event->fd());
  }
  event->insert_states(EventState::ON_READ);
  event->handler()(event);
  event->remove_states(EventState::ON_READ);

  event->remove_states(EventState::ACTIVE_READ);
}

// Procedure: _process_active_write_event    
// Handle the write event, where we have to flush the output stream out to the underlying
// device specified by the file descriptor of the event and then invoke the event handler.
template <typename EventT>
void Reactor<EventT>::_process_active_write_event(EventT* event) {
  
  if(!event->has_state(EventState::ACTIVE_WRITE)) return;

  event->insert_states(EventState::ON_WRITE);
  event->handler()(event);
  // TODO: check the iostate?
  if(event->ostream()) {
    event->ostream()->flush(event->fd());
  }
  event->remove_states(EventState::ON_WRITE);

  event->remove_states(EventState::ACTIVE_WRITE);
}
    
// Procedure: _process_active_events 
// The procedure processes all active events from the active eventlist and invoke the callback
// that is associated with each other (user-defined EventHandler).
template <typename EventT>
void Reactor<EventT>::_process_active_events() {

  auto acev = head_active_event();

  while(acev) {
    
    // Stop if the event is accidentally removed from the activelist.
    if(!acev->has_state(EventState::IN_ACTIVELIST)) {
      continue;
    }

    _process_active_timeout_event(acev);
    _process_active_read_event(acev);
    _process_active_write_event(acev);
    
    // Proceed to the next active event and de-link the list to deactivate the event.
    auto previous = acev;
    acev = acev->next_active_event();
    previous->set_next_active_event(nullptr);
    previous->remove_states(EventState::IN_ACTIVELIST);
  }

  set_head_active_event(nullptr);
}


};  // End of namespace OpenCluster. --------------------------------------------------------------




#endif




