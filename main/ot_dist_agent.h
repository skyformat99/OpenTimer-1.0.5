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

#ifndef OT_DIST_AGENT_H_
#define OT_DIST_AGENT_H_

#include "ot_timer.h"
#include "oc_reactor.hpp"

namespace OpenTimer {

using os_t = OpenCluster::OutputStream<OpenCluster::StreamBuffer>;
using is_t = OpenCluster::InputStream<OpenCluster::StreamBuffer>;

struct Packet {
  size_t size {0};
  string key;
  string value;
};
  
inline Packet make_packet(const string& K, const string& V) {
  Packet pkt;
  pkt.key = K;
  pkt.value = V;
  pkt.size = sizeof(size_t) + K.size() + V.size() + 2;
  return pkt;
}

os_t& operator << (os_t& os, const Packet& pkt) {
  os << pkt.size;
  os << pkt.key.c_str();
  os << pkt.value.c_str();
  return os;
}

is_t& operator >> (is_t& is, Packet& pkt) {
    
  if(is.rdbuf()->in_avail() >= sizeof(size_t)) {

    size_t sz{0};
    is.rdbuf()->copy(&sz, sizeof(size_t));
    
    if(is.rdbuf()->in_avail() >= sz) {
      is >> pkt.size;
      is >> pkt.key;
      is >> pkt.value;
    }

  }
  return is;
}

class Agent;

// AgentEvent
class AgentEvent : public OpenCluster::BasicEvent <AgentEvent> {
  
  
  public:

    Agent* agent {nullptr};

};

// Class: Agent
// An agent is a client program that runs the timer and monitors all message passing.
class Agent {
    
  private:

    string _hostname; 
    string _port;
    bool _is_registered {false};

    timer_upt _timer_uptr;

  public:

    Agent();
    ~Agent();

    inline timer_pt timer_ptr();
    inline timer_cpt timer_ptr() const;

    OC_CODEGEN_ACCESSOR(hostname);
    OC_CODEGEN_ACCESSOR(port);
    OC_CODEGEN_ACCESSOR(is_registered);

    OC_CODEGEN_MUTATOR(hostname);
    OC_CODEGEN_MUTATOR(port);
    OC_CODEGEN_MUTATOR(is_registered);
    
  private:



    static void on(AgentEvent*);

};  

// Function: timer_ptr
inline timer_pt Agent::timer_ptr() {
  return _timer_uptr.get();
}

// Function: timer_ptr
inline timer_cpt Agent::timer_ptr() const {
  return _timer_uptr.get();
}


};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif
