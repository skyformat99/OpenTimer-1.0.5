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

#ifndef OT_SDCDEF_H_
#define OT_SDCDEF_H_

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "ot_sdcutil.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------
// Definition of timing constraint class.
//-------------------------------------------------------------------------------------------------

// Class: SetInputDelay
class SetInputDelay {

  public: 

    SetInputDelay();
    ~SetInputDelay();

    bool is_rise;
    bool is_fall;
    bool is_clock_fall;
    bool is_level_sensitive;
    bool is_min;
    bool is_max;
    bool is_add_delay; 
    bool is_network_latency_included;
    bool is_source_latency_included;
    char* clock_name;

    float delay_value;
    std::vector <char*> objects;
};

//-------------------------------------------------------------------------------------------------

// Class: SetOutputDelay
class SetOutputDelay {

  public: 

    SetOutputDelay();
    ~SetOutputDelay();
    
    bool is_rise;
    bool is_fall;
    bool is_clock_fall;
    bool is_level_sensitive;
    bool is_min;
    bool is_max;
    bool is_add_delay; 
    bool is_network_latency_included;
    bool is_source_latency_included;
    char* clock_name;

    float delay_value;
    std::vector <char*> objects;
};

//-------------------------------------------------------------------------------------------------

// Class: SetLoad
class SetLoad {
  
  public:

    SetLoad();
    ~SetLoad();

    bool is_min;
    bool is_max;
    bool is_subtract_pin_load;
    bool is_pin_load;
    bool is_wire_load;

    float value;
    std::vector <char*> objects;
};

//-------------------------------------------------------------------------------------------------

// Class: SetDrivingCell
class SetDrivingCell {

  public:

    SetDrivingCell();
    ~SetDrivingCell();

    bool is_rise;
    bool is_fall;
    bool is_min;
    bool is_max;
    bool dont_scale;
    bool no_design_rule;
    bool is_clock_fall;

    char* lib_name;
    char* lib_cell_name;
    char* pin_name;
    char* from_pin_name;
    char* clock_name;

    float factor;
    float rise_time;
    float fall_time;

    std::vector <char*> objects;
};

//-------------------------------------------------------------------------------------------------

// Class: CreateClock
class CreateClock {
  
  public:

    CreateClock();
    ~CreateClock();

    bool is_add;

    char* clock_name;
    char* comment_string;

    float period;

    std::vector <char*> objects;
};

//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Definition of timing exception class.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Definition of area constraint class.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Definition of multi-voltage/power optimization constraint class.
//-------------------------------------------------------------------------------------------------


};  // End of OpenTimer namespace. ----------------------------------------------------------------



#endif



