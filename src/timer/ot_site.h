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

#ifndef OT_SITE_H_
#define OT_SITE_H_

#include "ot_enumdef.h"
#include "ot_typedef.h"
#include "ot_geometry.h"
#include "ot_macrodef.h"

namespace OpenTimer {

// Class: Site
class Site {

  public:

    Site(string_crt);                                       // Constructor.
    ~Site();                                                // Destructor.

    inline void_t set_class(site_class_e);                  // Set the site class.
    inline void_t set_width(float_ct);                      // Set the width.
    inline void_t set_height(float_ct);                     // Set the height.
    inline void_t set_is_x_symmetry(bool_ct);               // Set x symmetry.
    inline void_t set_is_y_symmetry(bool_ct);               // Set y symmetry.
    inline void_t set_is_r90_symmetry(bool_ct);             // Set r90 symmetry.

    inline string_crt name() const;                         // Query the site name.

    inline site_class_e site_class() const;                 // Query the site class.

    inline float_t width() const;                           // Query the width.
    inline float_t height() const;                          // Query the height.

    inline bool_t is_x_symmetry() const;                    // Query the x symmetry.
    inline bool_t is_y_symmetry() const;                    // Query the y symmetry.
    inline bool_t is_r90_symmetry() const;                  // Query the r90 symmetry.

  private:

    string_t _name;                                         // Site name.

    float_t _width;                                         // Site width.
    float_t _height;                                        // Site height.

    site_class_e _site_class;                               // Site class.

    bool_t _is_x_symmetry;                                  // Symmetry.
    bool_t _is_y_symmetry;                                  // Symmetry.
    bool_t _is_r90_symmetry;                                // Symmetry.
};

// Procedure: set_class
inline void_t Site::set_class(site_class_e site_class) {
  _site_class = site_class;
}

// Procedure: set_width
inline void_t Site::set_width(float_ct width) {
  _width = width;
}

// Procedure: set_height
inline void_t Site::set_height(float_ct height) {
  _height = height;
}

// Function: name
inline string_crt Site::name() const {
  return _name;
}

// Function: site_class
inline site_class_e Site::site_class() const {
  return _site_class;
}

// Function: width
inline float_t Site::width() const {
  return _width;
}

// Function: height
inline float_t Site::height() const {
  return _height;
}

// Procedure: set_is_x_symmetry
inline void_t Site::set_is_x_symmetry(bool_ct flag) {
  _is_x_symmetry = flag;
}

// Procedure: set_is_y_symmetry
inline void_t Site::set_is_y_symmetry(bool_ct flag) {
  _is_y_symmetry = flag;
}

// Procedure: set_is_r90_symmetry
inline void_t Site::set_is_r90_symmetry(bool_ct flag) {
  _is_r90_symmetry = flag;
}

// Procedure: is_x_symmetry
inline bool_t Site::is_x_symmetry() const {
  return _is_x_symmetry;
}

// Procedure: is_y_symmetry
inline bool_t Site::is_y_symmetry() const {
  return _is_y_symmetry;
}

// Procedure: is_r90_symmetry
inline bool_t Site::is_r90_symmetry() const {
  return _is_r90_symmetry;
}

};


#endif
