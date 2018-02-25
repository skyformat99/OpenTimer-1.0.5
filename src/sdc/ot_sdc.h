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

#ifndef OT_SDC_H_
#define OT_SDC_H_

#include "ot_sdcdef.h"
#include "ot_sdcdump.h"

namespace OpenTimer {

// Class: SDC
// The class for reading the sdc 2.0 input file.
class SDC {
  
  public:
  
    typedef int (*callback_t)(void*, void*);                                            // Callback function type.
          
    SDC();                                                                              // Constructor.
    ~SDC();                                                                             // Destructor.

    inline void set_user_data(void*);                                                   // Set the user data.
    inline void* user_data() const;                                                     // Query the user data.

    void read(const char*, void*);                                                      // Read the sdc file.

  private:

    FILE* _sdc_fptr;                                                                    // File pointer to sdc file.
 
    void* _user_data;                                                                   // User data.
    
    callback_t _create_clock;                                                           // Callback function.
    callback_t _set_input_delay;                                                        // Callback function.
    callback_t _set_output_delay;                                                       // Callback function.
    callback_t _set_load;                                                               // Callback function.
    callback_t _set_driving_cell;                                                       // Callback function.
    
    void _read_objects(std::vector<char*>&, const char*, const char*, char*&, char*&);  // Read objects.
    void _read_file_to_buffer(const char*, char*&, char*&);                             // Buffer the file.
    void _read_create_clock(const char*, const char*, char*&, char*&);                  // Read create_clock.
    void _read_set_input_delay(const char*, const char*, char*&, char*&);               // Read set_input_delay.
    void _read_set_output_delay(const char*, const char*, char*&, char*&);              // Read set_out_delay.
    void _read_set_load(const char*, const char*, char*&, char*&);                      // Read set_load.
    void _read_set_driving_cell(const char*, const char*, char*&, char*&);              // Read set_driving_cell.
    
    bool _is_delimiter(const char) const;                                               // Check if the delimiter
    bool _is_object_access_command(const char*) const;                                  // Check if object accesser.
    bool _read_next_token(const char*, const char*, char*&, char*&, char*);             // Read the next token.
};

// Procedure: set_user_data
// Set the user data.
inline void SDC::set_user_data(void* user_data) {
  _user_data = user_data;
}

// Function: user_data
// Query the user data.
inline void* SDC::user_data() const {
  return _user_data;
}

};


#endif

