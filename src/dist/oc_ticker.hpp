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

#ifndef OC_TICKER_HPP_
#define OC_TICKER_HPP_

#include "oc_headerdef.hpp"

namespace OpenCluster {

// Class: Ticker
class Ticker {
  
  private:

    ::std::chrono::steady_clock::time_point _timeout;
    ::std::chrono::steady_clock::time_point _interval;

  public:

    Ticker();
    Ticker(const auto&, const auto&);
    ~Ticker();

    OC_CODEGEN_ACCESSOR(timeout)
    OC_CODEGEN_ACCESSOR(interval)

    OC_CODEGEN_MUTATOR(timeout);
    OC_CODEGEN_MUTATOR(interval);
    
    template <typename DurationT>
    inline void set_interval(DurationT&&);
};

// Constructor
Ticker::Ticker() {
}

// Constructor
Ticker::Ticker(const auto& timeout, const auto& interval) :
  _timeout {timeout},
  _interval {interval} {
}

// Destructor
Ticker::~Ticker() {
}

// Procedure: set_interval
template <typename DurationT>
inline void Ticker::set_interval(DurationT&& d) {
  _interval = ::std::chrono::steady_clock::time_point(d);
}


};  // End of namespace OpenCluster. ---------------------------------------------------------------------


#endif








