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

#ifndef OT_LAYER_H_
#define OT_LAYER_H_

#include "ot_enumdef.h"
#include "ot_typedef.h"

namespace OpenTimer {

// Class: Layer
class Layer {
  
  public:
    
    Layer(string_crt);                                      // Constructor.
    ~Layer();                                               // Destructor.
    
    inline string_crt name() const;                         // Query the site name.

    inline void_t set_type(layer_type_e);                   // Set the layer type.
    inline void_t set_direction(layer_direction_e);         // Set the layer direction.
    inline void_t set_width(float_ct);                      // Set the width.
    inline void_t set_xpitch(float_ct);                     // Set the pitch.
    inline void_t set_ypitch(float_ct);                     // Set the pitch.
    inline void_t set_xoffset(float_ct);                    // Set offset. 
    inline void_t set_yoffset(float_ct);                    // Set offset. 

    inline float_t width() const;                           // Query the width.
    inline float_t xpitch() const;                          // Query the pitch.
    inline float_t ypitch() const;                          // Query the pitch.
    inline float_t xoffset() const;                         // Query the offset.
    inline float_t yoffset() const;                         // Query the offset.

    inline layer_type_e layer_type() const;                 // Layer type.
    inline layer_direction_e layer_direction() const;       // Layer direction.

  private:

    string_t _name;                                         // name. 

    float_t _width;                                         // Width.
    float_t _xpitch;                                        // Pitch.
    float_t _ypitch;                                        // Pitch.
    float_t _xoffset;                                       // Offset.
    float_t _yoffset;                                       // Offset.

    layer_direction_e _layer_direction;                     // Layer direction.
    layer_type_e _layer_type;                               // Layer type.

};

// Function: name
inline string_crt Layer::name() const {
  return _name;
}

// Function: width
inline float_t Layer::width() const {
  return _width;
}

// Function: pitch
inline float_t Layer::xpitch() const {
  return _xpitch;
}

// Function: pitch
inline float_t Layer::ypitch() const {
  return _ypitch;
}

// Function: xoffset
inline float_t Layer::xoffset() const {
  return _xoffset;
}

// Function: yoffset
inline float_t Layer::yoffset() const {
  return _yoffset;
}

// Procedure: set_type
inline void_t Layer::set_type(layer_type_e layer_type) {
  _layer_type = layer_type;
}

// Procedure: set_width
inline void_t Layer::set_width(float_ct width) {
  _width = width;
}

// Procedure: set_xpitch
inline void_t Layer::set_xpitch(float_ct pitch) {
  _xpitch = pitch;
}

// Procedure: set_ypitch
inline void_t Layer::set_ypitch(float_ct pitch) {
  _ypitch = pitch;
}

// Procedure: set_xoffset
inline void_t Layer::set_xoffset(float_ct offset) {
  _xoffset = offset;
}

// Procedure: set_yoffset
inline void_t Layer::set_yoffset(float_ct offset) {
  _yoffset = offset;
}

// Procedure: set_direction
inline void_t Layer::set_direction(layer_direction_e layer_direction) {
  _layer_direction = layer_direction;
}

// Function: layer_type
inline layer_type_e Layer::layer_type() const {
  return _layer_type;
}

// Function: layer_direction
inline layer_direction_e Layer::layer_direction() const {
  return _layer_direction;
}

//-------------------------------------------------------------------------------------------------

// Class: LayerGeometry
class LayerGeometry {

  public:

    LayerGeometry(string_crt);
    ~LayerGeometry();

    inline void_t set_spacing(float_ct);                                                      // Set the float spacing.
    inline void_t set_width(float_ct);                                                        // Set the width.
    inline void_t set_macropin_ptr(macropin_pt);                                              // Set the macropin pointer.

    inline string_crt name() const;                                                           // Query the name.

    inline float_t width() const;                                                             // Query the width.
    inline float_t spacing() const;                                                           // Query the spacing.

    inline macropin_pt macropin_ptr() const;                                                  // Query the macropin pointer.

    inline rectangle_vpt rectangles_ptr() const;                                              // Query the rectangles.
    inline rectangle_vrt rectangles() const;                                                  // Query the rectangles.

    void_t insert_rectangle(rectangle_crt);                                                   // Insert a rectangle.
    void_t insert_rectangle(point_crt, point_crt);                                            // Insert a rectangle.
    void_t insert_rectangle(point_value_ct, point_value_ct, point_value_ct, point_value_ct);  // Insert a rectangle.

  private:

    string_t _name;
    
    float_t _width;
    float_t _spacing;

    macropin_pt _macropin_ptr;

    rectangle_vpt _rectangles_ptr;
};

// Procedure: set_width
inline void_t LayerGeometry::set_width(float_ct width) {
  _width = width;
}

// Procedure: set_spacing
inline void_t LayerGeometry::set_spacing(float_ct spacing) {
  _spacing = spacing;
}

// Function: name
inline string_crt LayerGeometry::name() const {
  return _name;
}

// Function: width
inline float_t LayerGeometry::width() const {
  return _width;
}

// Function: spacing
inline float_t LayerGeometry::spacing() const {
  return _spacing;
}

// Procedure: set_macropin_ptr
inline void_t LayerGeometry::set_macropin_ptr(macropin_pt ptr) {
  _macropin_ptr = ptr;
}

// Function: macropin_ptr
inline macropin_pt LayerGeometry::macropin_ptr() const {
  return _macropin_ptr;
}

// Function: rectangles_ptr
inline rectangle_vpt LayerGeometry::rectangles_ptr() const {
  return _rectangles_ptr;
}

// Function: rectangles
inline rectangle_vrt LayerGeometry::rectangles() const {
  return *_rectangles_ptr;
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------



#endif
