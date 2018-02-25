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

#include "ot_techlib.h"

namespace OpenTimer {

// Constructor: TechLib
TechLib::TechLib():
  _site_dict_uptr(new site_dict_t()),
  _layer_dict_uptr(new layer_dict_t()),
  _macro_dict_uptr(new macro_dict_t())
{

}

// Destructor: TechLib
TechLib::~TechLib() {
}

// Function: insert_macro
// Insert a macro into the circuit.
macro_pt TechLib::insert_macro(string_crt name) const {
  macro_pt macro_ptr = macro_dict_ptr()->insert(name, name);
  return macro_ptr;
}

// Function: insert_layer
// Insert a layer into the circuit.
layer_pt TechLib::insert_layer(string_crt name) const {
  layer_pt layer_ptr = layer_dict_ptr()->insert(name, name);
  return layer_ptr;
}

// Function: insert_site
// Insert a site into the circuit.
site_pt TechLib::insert_site(string_crt name) const {
  site_pt site_ptr = site_dict_ptr()->insert(name, name);
  return site_ptr;
}


};
