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

#include "ot_macro.h"

namespace OpenTimer {

// Constructor.
Macro::Macro(string_crt key):
  _name(key),
  _macro_class(UNDEFINED_MACRO_CLASS),
  _is_fixed_mask(OT_DEFAULT_MACRO_IS_FIXED_MASK),
  _is_x_symmetry(OT_DEFAULT_MACRO_IS_X_SYMMETRY),
  _is_y_symmetry(OT_DEFAULT_MACRO_IS_Y_SYMMETRY),
  _is_r90_symmetry(OT_DEFAULT_MACRO_IS_R90_SYMMETRY),
  _site_ptr(nullptr),
  _macropin_dict_ptr(new macropin_dict_t())
{

}

// Destructor.
Macro::~Macro() {
  delete _macropin_dict_ptr;
}

// Function: insert_macropin
macropin_pt Macro::insert_macropin(string_crt name) {
  macropin_pt macropin_ptr = macropin_dict_ptr()->insert(name, name);
  macropin_ptr->set_macro_ptr(this);
  return macropin_ptr;
}

//-------------------------------------------------------------------------------------------------

// Constructor: macropin
Macropin::Macropin(string_crt key):
  _name(key),
  _direction(UNDEFINED_PIN_DIRECTION),
  _use(UNDEFINED_PIN_USE),
  _macro_ptr(nullptr),
  _layer_geometry_dict_ptr(new layer_geometry_dict_t())
{
  
}

// Destructor: ~macropin
Macropin::~Macropin() {
  delete _layer_geometry_dict_ptr;
}

// Function: insert_layer_geometry
layer_geometry_pt Macropin::insert_layer_geometry(string_crt name) {
  layer_geometry_pt lg_ptr = layer_geometry_dict_ptr()->insert(name, name);
  lg_ptr->set_macropin_ptr(this);
  return lg_ptr;
}



};



