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

#ifndef OT_LOG_H_
#define OT_LOG_H_

#include "ot_typedef.h"

namespace OpenTimer {

// Class: Log
class Log {

  public:

    Log() = delete;                                                   // Disable constructor.
    Log(const Log&) = delete;                                         // Disable copy constructor.
    ~Log() = delete;                                                  // Disable destructor.

    inline static void_t init_logging(char_cpt, bool_ct);             // Initialize the logging.
    inline static void_t init_logging(char_cpt);                      // Initialize the logging.
    inline static void_t set_logtostderr(bool_ct);                    // Set the log status.
    inline static void_t set_colorlogtostderr(bool_ct);               // Set the color flag.

    Log& operator = (const Log&) = delete;                            // Disable copy assignment.

  private:

};

// Procedure: init_logging
inline void_t Log::init_logging(char_cpt argv_0) {
  google::InitGoogleLogging(argv_0);
}

// Procedure: init_logging
inline void_t Log::init_logging(char_cpt argv_0, bool_ct flag) {
  init_logging(argv_0);
  set_logtostderr(flag);
  set_colorlogtostderr(true);
}

// Procedure: set_logtostderr
inline void_t Log::set_logtostderr(bool_ct flag) {
  FLAGS_logtostderr = flag;
}

// Procedure: set_colorlogtostderr
inline void_t Log::set_colorlogtostderr(bool_ct flag) {
  FLAGS_colorlogtostderr = flag;
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------


#endif

