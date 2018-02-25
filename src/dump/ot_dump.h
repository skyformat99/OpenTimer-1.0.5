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

#ifndef OPEN_TIMER_DEBUG_H_
#define OPEN_TIMER_DEBUG_H_

#include "ot_typedef.h"
#include "ot_net.h"

namespace OpenTimer {

stringstream_t dump(timing_arc_pt);

// Function: dump_net
string_t dump_net(net_pt);

// Function: dump_pin
string_t dump_pin(pin_pt);

// Function: dump_node
string_t dump_node(node_pt);

// Function: dump_edge
string_t dump_edge(edge_pt);

// Function: dump_cell
string_t dump_cell(cell_pt);


};

#endif
