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

#ifndef OC_DEMUX_HPP_
#define OC_DEMUX_HPP_

#include "oc_ticker.hpp"
#include "oc_reactor.hpp"
#include "oc_utility.hpp"

namespace OpenCluster {

// Helper template: ExtractDemuxEventT
// Base definition.
template <typename T>
struct ExtractDemuxEventT {
  using type = T;
};

// Helper template: ExtractDemuxEventT
// Recursive definition.
template < template<typename...> class D, typename T, typename... RestT >
struct ExtractDemuxEventT< D<T, RestT...> > {
  using type = T;
};

// ------------------------------------------------------------------------------------------------

// Class: DemuxInterface
// The gateway class for demultiplexer.
template <typename DemuxT>
class DemuxInterface {
  
  using EventT = typename ExtractDemuxEventT<DemuxT>::type;
  using handler_t = void (*) (const int, EventT*);
    
  private:
    
    // Callback handler.
    handler_t _handler {nullptr};
  
  public:

    // Accessor.
    OC_CODEGEN_ACCESSOR(handler)
    
    // Mutator.
    OC_CODEGEN_MUTATOR(handler)
    
    // CRTP.
    inline DemuxT* demux();
    inline const DemuxT* demux() const;
    
    // Gateway call.
    inline void insert(EventT*);
    inline void remove(EventT*);
    template <typename DurationT>
    inline void poll(DurationT&&);

  private:

    // Default implementation.
    inline void insert_impl(EventT*);
    inline void remove_impl(EventT*);
    template <typename DurationT>
    inline void poll_impl(DurationT&&);

};

// Function: demux
template <typename DemuxT>
inline DemuxT* DemuxInterface<DemuxT>::demux() {
  return static_cast <DemuxT*> (this);
}

// Function: demux
template <typename DemuxT>
inline const DemuxT* DemuxInterface<DemuxT>::demux() const {
  return static_cast <const DemuxT*> (this);
}

// Procedure: insert
template <typename DemuxT>
inline void DemuxInterface<DemuxT>::insert(EventT* event) {
  return demux()->insert_impl(event);
}

// Procedure: remove
template <typename DemuxT>
inline void DemuxInterface<DemuxT>::remove(EventT* event) {
  return demux()->remove_impl(event);
}

// Procedure: poll
template <typename DemuxT>
template <typename DurationT>
inline void DemuxInterface<DemuxT>::poll(DurationT&& d) {
  demux()->poll_impl(::std::forward<DurationT>(d));
}

// Procedure: insert_impl
template <typename DemuxT>
inline void DemuxInterface<DemuxT>::insert_impl(EventT* event) {
  LOGW("insert method is not implemented in Demux class");
}

// Procedure: remove_impl
template <typename DemuxT>
inline void DemuxInterface<DemuxT>::remove_impl(EventT* event) {
  LOGW("remove method is not implemented in Demux class");
}

// Procedure: poll_impl
template <typename DemuxT>
template <typename DurationT>
inline void DemuxInterface<DemuxT>::poll_impl(DurationT&& d) {
  LOGW("poll method is not implemented in Demux class");
}


// ------------------------------------------------------------------------------------------------

// Class: Select
// Select allows a program to monitor multiple file descriptors, waiting until one or more of the
// file descriptors become "ready" for some class of IO operations. Below are function signarues:
//
// int select(int nfds, fd_set *rfds, fd_set *wfds, fd_set *exceptfds, struct timeval *timeout);
// void FD_CLR(int fd, fd_set *set);
// int  FD_ISSET(int fd, fd_set *set);
// void FD_SET(int fd, fd_set *set);
// void FD_ZERO(fd_set *set);
//
template <typename EventT>
class Select : public DemuxInterface<Select<EventT>> {

  using mask_t = fd_mask;

  public:

    Select();
    ~Select();

    constexpr size_t num_fds_per_mask() const;
    constexpr size_t num_masks(const size_t) const;

    void insert_impl(EventT*);
    void remove_impl(EventT*);
    
    template <typename DurationT>
    void poll_impl(DurationT&&);

  private:
    
    int _max_fd     {-1};
    size_t _cap     {0};
    fd_set* _R[2]   {nullptr, nullptr};
    fd_set* _W[2]   {nullptr, nullptr};
    EventT** _fd2ev {nullptr};
    
    void _recap(const int);
};

// Constructor
template <typename EventT>
Select<EventT>::Select() {
}

// Destructor
template <typename EventT>
Select<EventT>::~Select() {
  free(_R[0]);
  free(_R[1]);
  free(_W[0]);
  free(_W[1]);
  free(_fd2ev);
}

// Function: num_fds_per_mask
// Each byte can accommodate eight bits (eight file descriptors).
template <typename EventT>
constexpr size_t Select<EventT>::num_fds_per_mask() const {
  return sizeof(mask_t)*8;
}

// Function: num_masks
// Perform the ceil operation to find the number of required masks for a given
// number of file descriptors.
template <typename EventT>
constexpr size_t Select<EventT>::num_masks(const size_t num_fds) const {
  return (num_fds + num_fds_per_mask() - 1) / num_fds_per_mask();
}

// Procedure: poll
// Apply the IO demutiplexing using select method and extract active events.
template <typename EventT>
template <typename DurationT>
void Select<EventT>::poll_impl(DurationT&& d) {

  if(_max_fd == -1) return;

  timeval tv = duration_cast<timeval>(d);

  // fd  | bytes
  // -1 => 0
  // 0  => 1
  // 1  => 1
  // ...
  // 7  => 1
  // 8  => 2
  // 9  => 2
  // ...
  // 15 => 2
  // 16 => 3 
  size_t tgt_cap = (_max_fd) / 8 + 1;
  memcpy(_R[1], _R[0], tgt_cap);
  memcpy(_W[1], _W[0], tgt_cap);
  
  auto ret = select(_max_fd + 1, _R[1], _W[1], nullptr, &tv);

  if(ret == -1) {
    if(errno == EINTR) {
      return;
    }
    LOGE(strerror(errno));
  }

  // Invoke the handler for every active read/write event.
  if(this->handler()) {
    for(int fd=0; fd<=_max_fd; ++fd) {
      if(FD_ISSET(fd, _R[1])) {
        this->handler()(EventState::ACTIVE_READ, _fd2ev[fd]);
      }
      if(FD_ISSET(fd, _W[1])) {
        this->handler()(EventState::ACTIVE_WRITE, _fd2ev[fd]);
      }
    }
  }

}

// Procedure: _recap
// Adjust the capacity to accommodate the file descriptor fd.
template <typename EventT>
void Select<EventT>::_recap(const int fd) {
  
  size_t tgt_cap = num_masks(fd + 1) * sizeof(mask_t);

  // Nothing has to be done if the capacity can accommodate the file descriptor.
  if(_cap >= tgt_cap) return;
  
  // Adjust the cap to the next highest power of 2 larger than num_fds
  size_t new_cap = (_cap == 0) ? sizeof(mask_t) : _cap;
  while(new_cap < tgt_cap) new_cap *= 2;
  
  // Adjust and reset the memory chunk.
  for(int i=0; i<2; i++) {
    _R[i] = static_cast<fd_set*>(realloc(_R[i], new_cap));
    _W[i] = static_cast<fd_set*>(realloc(_W[i], new_cap));
  }
  _fd2ev = static_cast<EventT**>(realloc(_fd2ev, new_cap*8*sizeof(EventT*)));

  memset((unsigned char*)_R[0] + _cap, 0, new_cap - _cap);
  memset((unsigned char*)_W[0] + _cap, 0, new_cap - _cap);
  memset((unsigned char*)_fd2ev + _cap*8*sizeof(EventT*), 0, (new_cap - _cap)*8*sizeof(EventT*));

  _cap = new_cap;
}

// Procedure: insert_timpl
// Insert an event into the demux.
template <typename EventT>
void Select<EventT>::insert_impl(EventT* event) {
 
  // Invalid arguments.
  if(event == nullptr || event->fd() == -1) return;
  
  // Adjust the capacity in according to the input file descriptor.
  if(_max_fd < event->fd()) {
    _recap(event->fd());
    _max_fd = event->fd();
  }
  
  // Set the read file descriptor.
  if(event->is_read()) {
    FD_SET(event->fd(), _R[0]);
  }
  
  // Set the write file descriptor.
  if(event->is_write()) {
    FD_SET(event->fd(), _W[0]);
  }

  // Insert the event into the map.
  _fd2ev[event->fd()] = event;
}

// Procedure: remove_impl
// Remove an event from the demux.
template <typename EventT>
void Select<EventT>::remove_impl(EventT* event) {
  
  // Invalid arguments.
  if(event == nullptr || event->fd() == -1 || _max_fd < event->fd()) return;
  
  // Clear the read file descriptor.
  if(event->is_read()) {
    FD_CLR(event->fd(), _R[0]);
  }
  
  // Clear the write file decriptor.
  if(event->is_write()) {
    FD_CLR(event->fd(), _W[0]);
  }

  // Adjust the max_fd
  while(_max_fd >= 0 && !FD_ISSET(event->fd(), _R[0]) && !FD_ISSET(event->fd(), _W[0])) {
    --_max_fd;
  }

  // Remove the event from the map.
  _fd2ev[event->fd()] = nullptr;
}


};  // End of namespace OpenCluster. --------------------------------------------------------------

#endif






