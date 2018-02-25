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

#ifndef OT_DIST_MASTER_H_
#define OT_DIST_MASTER_H_

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

// Master class.
class Master;

// Master
class MasterEvent : public OpenCluster::BasicEvent <MasterEvent> {
  
  
  public:
    
    Master* master {nullptr};

};



class Master {
    
    bool _is_locked {false};

  public:

    
    // 
    unordered_map < string, double > boundary_timing;
    

    OC_CODEGEN_ACCESSOR(is_locked)

    OC_CODEGEN_MUTATOR(is_locked);
};


};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif









