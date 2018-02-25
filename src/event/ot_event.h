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

#ifndef OT_EVENT_H_
#define OT_EVENT_H_

#include "ot_typedef.h"
#include "ot_data_channel.h"
#include "ot_event_handler.h"

namespace OpenTimer {

// Class: Event
class Event {

  public:

    Event();                                                // Constructor.
    ~Event();                                               // Destructor.

    inline int fd() const;                                  // Query the file descriptor.


  private:

    int _fd;                                                // File descriptor.

};

// Function: fd
inline int Event::fd() const {
  return _fd;
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------


#endif
