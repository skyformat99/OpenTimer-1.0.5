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

#ifndef OT_LEFREADER_TIMER_H_
#define OT_LEFREADER_TIMER_H_

#include "ot_timer.h"
#include "ot_circuit.h"
#include "ot_typedef.h"
#include "lefrReader.hpp"
#include "lefwWriter.hpp"
#include "lefiDebug.hpp"
#include "lefiEncryptInt.hpp"
#include "lefiUtil.hpp"

//#define LEF_CALLBACK_VERBOSITY 1

namespace OpenTimer {

// Class: LEFReader
class LEFReader
{
  public:

    LEFReader();                                                          // Constructor.
    ~LEFReader();                                                         // Destructor.

    void_t read(timer_pt, string_crt);                                    // Read the lef.
    void_t read(timer_pt, char_cpt);                                      // Read the lef.

    inline timer_pt timer_ptr() const;                                    // Query the timer pointer.
    inline site_pt site_cursor() const;                                   // Query the site cursor.
    inline macro_pt macro_cursor() const;                                 // Query the macro cursor.
    inline layer_pt layer_cursor() const;                                 // Query the layer cursor.
    inline macropin_pt macropin_cursor() const;                           // Query the macropin cursor.
    inline layer_geometry_pt layer_geometry_cursor() const;               // Query the layer geometry cursor.
    
    inline bool_t parse65nm() const;                                      // Query the parser status.
    inline bool_t parseLef58Type() const;                                 // Query the parser status.

    static double chkNum(double);                                         // Round off numbers.
    static char* orientStr(int);                                          // Tokenize the orientation.
    void prtGeometry(lefiGeometries*);                                    // Print the geometry.

    static int units_cbk(lefrCallbackType_e, lefiUnits*, void*);          // Unit callback function.
    static int site_cbk(lefrCallbackType_e, lefiSite*, void*);            // Site callback function.
    static int layer_cbk(lefrCallbackType_e, lefiLayer*, void*);          // Layer callback function.
    static int macro_begin_cbk(lefrCallbackType_e, const char*, void*);   // Macro begin callback function.
    static int macro_cbk(lefrCallbackType_e, lefiMacro*, void*);          // Macro block callback function.
    static int macro_end_cbk(lefrCallbackType_e, const char*, void*);     // Macro end callback function.
    static int pin_cbk(lefrCallbackType_e, lefiPin*, void*);              // Pin callback function.

  private:

    string_t _fpath;
    timer_pt _timer_ptr;
    
    site_pt _site_cursor;                                                 // Site cursor.
    macro_pt _macro_cursor;                                               // Macro cursor.
    layer_pt _layer_cursor;                                               // Layer cursor.
    macropin_pt _macropin_cursor;                                         // Macropin cursor.
    layer_geometry_pt _layer_geometry_cursor;                             // Layer geometry cursor.

    bool_t _parse65nm;                                                    // Parser status.
    bool_t _parseLef58Type;                                               // Parser status.
};

// Function: site_cursor
inline site_pt LEFReader::site_cursor() const {
  return _site_cursor;
}

// Function: timer_ptr
inline timer_pt LEFReader::timer_ptr() const {
  return _timer_ptr;
}

// Function: macro_cursor
inline macro_pt LEFReader::macro_cursor() const {
  return _macro_cursor;
}

// Function: layer_cursor
inline layer_pt LEFReader::layer_cursor() const {
  return _layer_cursor;
}

// Function: macropin_cursor
inline macropin_pt LEFReader::macropin_cursor() const {
  return _macropin_cursor;
}

// Function: layer_geometry_cursor
inline layer_geometry_pt LEFReader::layer_geometry_cursor() const {
  return _layer_geometry_cursor;
}

// Function: parse65nm
inline bool_t LEFReader::parse65nm() const {
  return _parse65nm;
}

// Function: parseLef58Type
inline bool_t LEFReader::parseLef58Type() const {
  return _parseLef58Type;
}

};


#endif
