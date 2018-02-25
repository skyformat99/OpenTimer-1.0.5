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

#ifndef OT_DEFREADER_TIMER_H_
#define OT_DEFREADER_TIMER_H_

#include "ot_circuit.h"
#include "ot_typedef.h"
#include "defrReader.hpp"

//#define DEF_CALLBACK_VERBOSITY 1

namespace OpenTimer {

// Class: DEF
class DEFReader {
  
  public:

    DEFReader();                                                                      // Constructor.
    ~DEFReader();                                                                     // Destructor.

    void_t read(timer_pt, string_crt);                                                // Read.
    void_t read(timer_pt, char_cpt);                                                  // Read.

    inline timer_pt timer_ptr() const;                                                // Query the timer pointer.
    
    static orientation_e orientation_enum(int);                                       // Get the orientation.
    static char* orientStr(int);                                                      // Tokenize.

    static int die_area_cbk(defrCallbackType_e, void*, void*);                        // Die area.
    static int component_cbk(defrCallbackType_e, defiComponent*, void*);              // Component.
    static int pin_cbk(defrCallbackType_e, void*, void*);                             // PI/PO ports.

  private:

    string_t _fpath;                                                                  // File path.
    timer_pt _timer_ptr;                                                              // Timer pointer.
};

// Function: timer_ptr
// Return the timer pointer.
inline timer_pt DEFReader::timer_ptr() const {
  return _timer_ptr;
}


};






#endif
