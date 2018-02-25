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

#include "ot_layer.h"

namespace OpenTimer {

// Constructor.
Layer::Layer(string_crt key):
  _name(key)
{

}

// Destructor.
Layer::~Layer() {

}

//-------------------------------------------------------------------------------------------------

// Constructor.
LayerGeometry::LayerGeometry(string_crt key):
  _name(key),
  _macropin_ptr(NULL),
  _rectangles_ptr(new rectangle_vt())
{

}

// Destructor.
LayerGeometry::~LayerGeometry() {
  delete _rectangles_ptr;
}

// Procedure: insert_rectangle
void_t LayerGeometry::insert_rectangle(rectangle_crt rect) {
  rectangles_ptr()->push_back(rect);
}

// Procedure: insert_rectangle
void_t LayerGeometry::insert_rectangle(point_crt llp, point_crt urp) {
  rectangles_ptr()->push_back(rectangle_t(llp, urp));
}

// Procedure: insert_rectangle
void_t LayerGeometry::insert_rectangle(point_value_ct llx, point_value_ct lly, point_value_ct urx, point_value_ct ury) {
  rectangles_ptr()->push_back(rectangle_t(llx, lly, urx, ury));
}


};
