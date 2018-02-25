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

#include "ot_io_port.h"

namespace OpenTimer {

// Constructor
PrimaryInput::PrimaryInput(string_crt key):
  _name(key)
{
  EL_RF_ITER(el, rf) {
    _at  [el][rf] = OT_DEFAULT_PRIMARY_INPUT_AT;
    _slew[el][rf] = OT_DEFAULT_PRIMARY_INPUT_SLEW;
  }
  _pin_ptr = nullptr;
  _mobility = UNDEFINED_MOBILITY;
}

// Destructor.
PrimaryInput::~PrimaryInput() {
}

//-------------------------------------------------------------------------------------------------

// Constructor
PrimaryOutput::PrimaryOutput(string_crt key):
  _name(key)
{
  EL_RF_ITER(el, rf) {
    _load[el][rf] = OT_DEFAULT_PRIMARY_OUTPUT_LCAP;
  }
  _pin_ptr = nullptr;
  _mobility = UNDEFINED_MOBILITY;
}

// Destructor.
PrimaryOutput::~PrimaryOutput() {
}

// Procedure: set_rat
// Set the rat to the test.
void_t PrimaryOutput::set_rat(int el, int rf, float_ct rat) {
  CHECK(pin_ptr() != nullptr);
  CHECK(pin_ptr()->test_ptr() != nullptr);
  pin_ptr()->test_ptr()->set_rat(el, rf, rat);
}

// Function: rat
// Query the rat of the test.
float_t PrimaryOutput::rat(int el, int rf) const {
  CHECK(pin_ptr() != nullptr);
  CHECK(pin_ptr()->test_ptr() != nullptr);
  return pin_ptr()->test_ptr()->rat(el, rf);
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------



