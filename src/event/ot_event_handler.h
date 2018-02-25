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

#ifndef OT_EVENT_HANDLER_H_
#define OT_EVENT_HANDLER_H_

#include "ot_data_channel.h"

namespace OpenTimer {

// Class: EventHandlerAPI
class EventHandlerAPI {

  public:

    virtual void read() = 0;                                // read
    virtual void write() = 0;                               // write
    virtual void timeout() = 0;                             // timeout

  private:

};



};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif
