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

#ifndef OT_DEBUG_GRAPH_H_
#define OT_DEBUG_GRAPH_H_

#include "ot_timer.h"

namespace OpenTimer {

// Function: is_timing_graph_acyclic
bool_t is_timing_graph_acyclic(timer_pt);

// Function: is_timing_graph_cyclic
bool_t is_timing_graph_cyclic(timer_pt);

};

#endif
