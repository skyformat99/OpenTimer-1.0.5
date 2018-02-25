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

#include "ot_gate.h"

namespace OpenTimer {

// Constructor
Gate::Gate(string_crt key):
  _name(key),
  _weight(OT_FLT_ZERO),
  _cell_ptr{nullptr, nullptr},
  _mobility(UNDEFINED_MOBILITY),
  _orientation(UNDEFINED_ORIENTATION),
  _source(UNDEFINED_GATE_SOURCE)
{
}

// Destructor
Gate::~Gate() {
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------
