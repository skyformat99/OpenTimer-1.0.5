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

#ifndef OT_IO_PORT_H_
#define OT_IO_PORT_H_

#include "ot_utility.h"
#include "ot_typedef.h"
#include "ot_pin.h"
#include "ot_test.h"

namespace OpenTimer {

// Class: PrimaryInput
class PrimaryInput {
  
  public:
    
    PrimaryInput(string_crt);                           // Constructor.
    ~PrimaryInput();                                    // Destructor.

    void_t clear();                                     // Clear the object.
    
    inline string_crt name() const;                     // Query the port name.
    inline pin_pt pin_ptr() const;                      // Query the poin pointer.
    inline mobility_e mobility() const;                 // Query the mobility.

    inline void_t set_pin_ptr(pin_pt);                  // Set the pin pointer.
    inline void_t set_at(int, int, float_ct);           // Set the at.
    inline void_t set_slew(int, int, float_ct);         // Set the slew.
    inline void_t set_mobility(mobility_ce);            // Set the port mobility.

    inline float_t at(int, int) const;                  // Query the arrival time.
    inline float_t slew(int, int) const;                // Query the slew.

  private:

    string_t _name;                                     // Name.

    mobility_e _mobility;                               // Mobility.

    float_t _at[2][2];                                  // Arrival time.
    float_t _slew[2][2];                                // Slew.

    pin_pt _pin_ptr;                                    // Pin pointer.
};

// Function: name
// Return the pin name.
inline string_crt PrimaryInput::name() const {
  return _name;
}

// Procedure: set_mobility
inline void_t PrimaryInput::set_mobility(mobility_ce mobility) {
  _mobility = mobility;
}

// Procedure: set_pin_ptr
// Set the pin pointer.
inline void_t PrimaryInput::set_pin_ptr(pin_pt pin_ptr) {
  _pin_ptr = pin_ptr;
}

// Procedure: set_at
inline void_t PrimaryInput::set_at(int el, int rf, float_ct at) {
  _at[el][rf] = at;
}

// Procedure: set_slew
inline void_t PrimaryInput::set_slew(int el, int rf, float_ct slew) {
  _slew[el][rf] = slew;
}

// Function: pin_ptr
inline pin_pt PrimaryInput::pin_ptr() const {
  return _pin_ptr;
}

// Function: at
inline float_t PrimaryInput::at(int el, int rf) const {
  return _at[el][rf];
}

// Function: slew
inline float_t PrimaryInput::slew(int el, int rf) const {
  return _slew[el][rf];
}

// Function: mobility
inline mobility_e PrimaryInput::mobility() const {
  return _mobility;
}

//-------------------------------------------------------------------------------------------------

// Class: PrimaryOutput
class PrimaryOutput {
  
  public:
    
    PrimaryOutput(string_crt);                              // Constructor.
    ~PrimaryOutput();                                       // Destructor.

    void_t set_rat(int, int, float_ct);                     // Set the rat to the test.

    float_t rat(int, int) const;                            // Query the rat.

    inline string_crt name() const;                         // Name.
    inline pin_pt pin_ptr() const;                          // Query the pin pointer.
    inline mobility_e mobility() const;                     // Query the mobility.
    
    inline float_t load(int, int) const;                    // Query the load capacitance.

    inline void_t set_pin_ptr(pin_pt);                      // Set the pin pointer.
    inline void_t set_load(int, int, float_ct);             // Set the load capacitance.
    inline void_t set_mobility(mobility_ce);                // Set the mobility.

  private:

    string_t _name;                                         // Name.

    mobility_e _mobility;                                   // Mobility.

    pin_pt _pin_ptr;                                        // Pin pointer.
    
    float_t _load[2][2];                                    // Load capacitance.
};

// Function: name
// Return the port name.
inline string_crt PrimaryOutput::name() const {
  return _name;
}

// Procedure: set_pin_ptr
inline void_t PrimaryOutput::set_pin_ptr(pin_pt pin_ptr) {
  _pin_ptr = pin_ptr;
}

// Procedure: set_mobility
inline void_t PrimaryOutput::set_mobility(mobility_ce mobility) {
  _mobility = mobility;
}

// Procedure: set_load
// Set the load capacitance.
inline void_t PrimaryOutput::set_load(int el, int rf, float_ct load) {
  _load[el][rf] = load;
}

// Function: pin_ptr
inline pin_pt PrimaryOutput::pin_ptr() const {
  return _pin_ptr;
}

// Function: load
// Query the load capacitance.
inline float_t PrimaryOutput::load(int el, int rf) const {
  return _load[el][rf];
}

// Function: mobility
// Query the mobility.
inline mobility_e PrimaryOutput::mobility() const {
  return _mobility;
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------


#endif



