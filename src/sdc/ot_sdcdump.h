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


#ifndef OT_SDCDUMP_H_
#define OT_SDCDUMP_H_

#include "ot_sdcdef.h"

namespace OpenTimer {

// Function: dump_set_input_delay
std::string dump_set_input_delay(SetInputDelay*);

// Function: dump_set_output_delay
std::string dump_set_output_delay(SetOutputDelay*);

// Function: dump_set_load
std::string dump_set_load(SetLoad*);

// Function: dump_set_driving_cell
std::string dump_set_driving_cell(SetDrivingCell*);

// Function: dump_create_clock
std::string dump_create_clock(CreateClock*);

};


#endif
