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

#ifndef OT_TECHLIB_H_
#define OT_TECHLIB_H_

#include "ot_typedef.h"
#include "ot_site.h"
#include "ot_layer.h"
#include "ot_macro.h"

namespace OpenTimer {

// Class: OpenTimer
class TechLib {

  public:

    TechLib();                                                                  // Constuctor.
    ~TechLib();                                                                 // Destructor.
    
    inline double database_unit() const;                                        // Database unit.

    inline void_t set_database_unit(const double);                              // Set the database unit.

    inline size_t num_macros() const;                                           // Query the macro count.
    inline size_t num_layers() const;                                           // Query the layer count.
    inline size_t num_sites() const;                                            // Query the site count.
    
    inline site_dict_pt site_dict_ptr() const;                                  // Query the site dictionary pointer.
    inline site_pt site_ptr(string_crt) const;                                  // Query the site pointer.

    inline layer_dict_pt layer_dict_ptr() const;                                // Query the layer dictionary pointer.
    inline layer_pt layer_ptr(string_crt) const;                                // Query the layer pointer.

    inline macro_dict_pt macro_dict_ptr() const;                                // Query the macro dictionary pointer.
    inline macro_pt macro_ptr(string_crt) const;                                // Query the macro pointer.
    
    macro_pt insert_macro(string_crt) const;                                    // Insert a macro.

    site_pt insert_site(string_crt) const;                                      // Insert a site.

    layer_pt insert_layer(string_crt) const;                                    // Insert a layer.

  private:

    double _database_unit;                                                      // Database unit. 
    
    site_dict_upt _site_dict_uptr;                                              // Site dictionary.
    layer_dict_upt _layer_dict_uptr;                                            // Layer dictionary.
    macro_dict_upt _macro_dict_uptr;                                            // Macro dictionary.

};

// Function: site_dict_ptr
// Return the pointer to the site dictionary.
inline site_dict_pt TechLib::site_dict_ptr() const {
  return _site_dict_uptr.get();
}

// Function: layer_dict_ptr
// Return the pointer to the layer dictionary.
inline layer_dict_pt TechLib::layer_dict_ptr() const {
  return _layer_dict_uptr.get();
}

// Function: macro_dict_ptr
// Return the pointer to the macro dictionary.
inline macro_dict_pt TechLib::macro_dict_ptr() const {
  return _macro_dict_uptr.get();
}

// Function: site_ptr
inline site_pt TechLib::site_ptr(string_crt name) const {
  return (*site_dict_ptr())[name];
}

// Function: layer_ptr
inline layer_pt TechLib::layer_ptr(string_crt name) const {
  return (*layer_dict_ptr())[name];
}

// Function: macro_ptr
// Return the pointer to the macro.
inline macro_pt TechLib::macro_ptr(string_crt name) const {
  return (*macro_dict_ptr())[name];
}

// Procedure: set_database_unit
// Set the database unit.
inline void TechLib::set_database_unit(const double unit) {
  _database_unit = unit; 
}

// Function: database_unit
// Return the database unit.
inline double TechLib::database_unit() const {
  return _database_unit;
}

// Function: num_sites
// Query the number of sites.
inline size_t TechLib::num_sites() const {
  return site_dict_ptr()->size();
}

// Function: num_layers
// Query the number of layers.
inline size_t TechLib::num_layers() const {
  return layer_dict_ptr()->size();
}

// Function: num_macros
// Query the number of macros.
inline size_t TechLib::num_macros() const {
  return macro_dict_ptr()->size();
}


};


#endif
