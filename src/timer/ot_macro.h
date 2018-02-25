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

#ifndef OT_MACRO_H_
#define OT_MACRO_H_

#include "ot_enumdef.h"
#include "ot_typedef.h"
#include "ot_geometry.h"
#include "ot_macrodef.h"
#include "ot_layer.h"

namespace OpenTimer {

// Class: Macro
class Macro {

  public:

    Macro(string_crt);                                                      // Constructor.
    ~Macro();                                                               // Destructor.

    inline void_t set_class(macro_class_e);                                 // Set the macro class.
    inline void_t set_width(float_ct);                                      // Set the width.
    inline void_t set_height(float_ct);                                     // Set the height.
    inline void_t set_origin(point_crt);                                    // Set the origin.
    inline void_t set_origin(point_t::value_t, point_t::value_t);           // Set the origin.
    inline void_t set_site_ptr(site_pt);                                    // Set the site pointer.
    inline void_t set_is_fixed_mask(bool_ct);                               // Set the fixed mask.
    inline void_t set_is_x_symmetry(bool_ct);                               // Set x symmetry.
    inline void_t set_is_y_symmetry(bool_ct);                               // Set y symmetry.
    inline void_t set_is_r90_symmetry(bool_ct);                             // Set r90 symmetry.

    inline string_crt name() const;                                         // Query the macro name.

    inline macro_class_e macro_class() const;                               // Query the macro class.
    
    inline float_t width() const;                                           // Query the width.
    inline float_t height() const;                                          // Query the height.

    inline point_crt origin() const;                                        // Query the origin.

    inline site_pt site_ptr() const;                                        // Query the site pointer.
    
    inline bool_t is_fixed_mask() const;                                    // Query if the fixed mask.
    inline bool_t is_x_symmetry() const;                                    // Query if the x symmetry.
    inline bool_t is_y_symmetry() const;                                    // Query if the y symmetry.
    inline bool_t is_r90_symmetry() const;                                  // Query if the r90 symmetry.

    inline macropin_dict_pt macropin_dict_ptr() const;                      // Query the macropin dict pointer.
    
    macropin_pt insert_macropin(string_crt);                                // Insert a macropin.

  private:

    string_t _name;                                                         // Macro name.

    point_t _origin;                                                        // Origin point.

    float_t _height;                                                        // Macro height.
    float_t _width;                                                         // Macro width.
    
    macro_class_e _macro_class;                                             // Macro class. 

    bool_t _is_fixed_mask;                                                  // Fixed mask flag.
    bool_t _is_x_symmetry;                                                  // x symmetry flag.
    bool_t _is_y_symmetry;                                                  // y symmetry flag.
    bool_t _is_r90_symmetry;                                                // r90 symmetry flag.

    site_pt _site_ptr;                                                      // Macro pointer.
    
    macropin_dict_pt _macropin_dict_ptr;                                    // Macropin dictionary.
};

// Function: name
inline string_crt Macro::name() const {
  return _name;
}

// Procedure: set_width
inline void_t Macro::set_width(float_ct width) {
  _width = width;
}

// Procedure: set_height
inline void_t Macro::set_height(float_ct height) {
  _height = height;
}

// Procedure: set_origin
inline void_t Macro::set_origin(point_crt origin) {
  _origin = origin;
}

// Procedure: set_origin
inline void_t Macro::set_origin(point_t::value_t x, point_t::value_t y) {
  _origin.set_x(x);
  _origin.set_y(y);
}

// Procedure: set_class
inline void_t Macro::set_class(macro_class_e macro_class) {
  _macro_class = macro_class;
}

// Procedure: set_site_ptr
inline void_t Macro::set_site_ptr(site_pt site_ptr) {
  _site_ptr = site_ptr;
}

// Function: macro_class
inline macro_class_e Macro::macro_class() const {
  return _macro_class;
}

// Function: width
inline float_t Macro::width() const {
  return _width;
}

// Function: height
inline float_t Macro::height() const {
  return _height;
}

// Function: origin
inline point_crt Macro::origin() const {
  return _origin;
}

// Function: site_ptr
inline site_pt Macro::site_ptr() const {
  return _site_ptr;
}

// Function: macro_dict_ptr
inline macropin_dict_pt Macro::macropin_dict_ptr() const {
  return _macropin_dict_ptr;
}

// Procedure: set_is_fixed_mask
inline void_t Macro::set_is_fixed_mask(bool_ct flag) {
  _is_fixed_mask = flag;
}

// Procedure: set_is_x_symmetry
inline void_t Macro::set_is_x_symmetry(bool_ct flag) {
  _is_x_symmetry = flag;
}

// Procedure: set_is_y_symmetry
inline void_t Macro::set_is_y_symmetry(bool_ct flag) {
  _is_y_symmetry = flag;
}

// Procedure: set_is_r90_symmetry
inline void_t Macro::set_is_r90_symmetry(bool_ct flag) {
  _is_r90_symmetry = flag;
}

// Procedure: is_fixed_mask
inline bool_t Macro::is_fixed_mask() const {
  return _is_fixed_mask;
}

// Procedure: is_x_symmetry
inline bool_t Macro::is_x_symmetry() const {
  return _is_x_symmetry;
}

// Procedure: is_y_symmetry
inline bool_t Macro::is_y_symmetry() const {
  return _is_y_symmetry;
}

// Procedure: is_r90_symmetry
inline bool_t Macro::is_r90_symmetry() const {
  return _is_r90_symmetry;
}

// Class: Macropin
class Macropin {

  public:

    Macropin(string_crt);                                               // Constructor.
    ~Macropin();                                                        // Destructor.

    inline string_crt name() const;                                     // Query the name.

    inline pin_direction_e direction() const;                           // Query the pin direction.
    inline pin_use_e use() const;                                       // Query the use.

    inline macro_pt macro_ptr() const;                                  // Query the macro pointer.

    inline layer_geometry_dict_pt layer_geometry_dict_ptr() const;      // Query the layer geometry dict.
    inline layer_geometry_dict_rt layer_geometry_dict() const;          // Query the layer geometry dict.

    inline void_t set_direction(pin_direction_ce);                      // Set the pin direction.
    inline void_t set_macro_ptr(macro_pt);                              // Set the macro pointer.
    inline void_t set_use(pin_use_ce);                                  // Set the macropin use.
    
    layer_geometry_pt insert_layer_geometry(string_crt);                // Insert a layer geometry.

  private:
    
    string_t _name;                                                     // Pin name.
    
    pin_direction_e _direction;                                         // Pin direction. 
    pin_use_e _use;                                                     // Pin use type.

    macro_pt _macro_ptr;                                                // Macro pointer.

    layer_geometry_dict_pt _layer_geometry_dict_ptr;                    // Layer geometry dictionary pointer.
};

// Function: name
inline string_crt Macropin::name() const {
  return _name;
}

// Procedure: set_use
inline void_t Macropin::set_use(pin_use_ce use) {
  _use = use;
}

// Procedure: set_macro_ptr
inline void_t Macropin::set_macro_ptr(macro_pt macro_ptr) {
  _macro_ptr = macro_ptr;
}

// Procedure: set_pin_direction
inline void_t Macropin::set_direction(pin_direction_e direction) {
  _direction = direction;
}

// Function: direction
inline pin_direction_e Macropin::direction() const {
  return _direction;
}

// Function: macro_ptr
inline macro_pt Macropin::macro_ptr() const {
  return _macro_ptr;
}

// Function: use
inline pin_use_e Macropin::use() const {
  return _use;
}

// Function: layer_geometry_dict_ptr
inline layer_geometry_dict_pt Macropin::layer_geometry_dict_ptr() const {
  return _layer_geometry_dict_ptr;
}

// Function: layer_geometry_dict
inline layer_geometry_dict_rt Macropin::layer_geometry_dict() const {
  return *_layer_geometry_dict_ptr;
}

};


#endif



