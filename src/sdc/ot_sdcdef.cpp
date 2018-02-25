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

#include "ot_sdcdef.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------
// Definition of timing constraint class.
//-------------------------------------------------------------------------------------------------

// Constructor.
SetInputDelay::SetInputDelay():
  is_rise(false),
  is_fall(false),
  is_clock_fall(false),
  is_level_sensitive(false),
  is_min(false),
  is_max(false),
  is_add_delay(false),
  is_network_latency_included(false),
  is_source_latency_included(false),
  clock_name(nullptr)
{
}

// Destructor.
SetInputDelay::~SetInputDelay() {
  free_sdc_objects(objects);  
  if(clock_name) free(clock_name);
}

//-------------------------------------------------------------------------------------------------

// Constructor.
SetOutputDelay::SetOutputDelay():
  is_rise(false),
  is_fall(false),
  is_clock_fall(false),
  is_level_sensitive(false),
  is_min(false),
  is_max(false),
  is_add_delay(false),
  is_network_latency_included(false),
  is_source_latency_included(false),
  clock_name(nullptr)
{
}

// Destructor.
SetOutputDelay::~SetOutputDelay() {
  free_sdc_objects(objects);
  if(clock_name) free(clock_name);
}

//-------------------------------------------------------------------------------------------------

// Constructor.
SetLoad::SetLoad():
  is_min(false),
  is_max(false),
  is_subtract_pin_load(false),
  is_pin_load(false),
  is_wire_load(false)
{
}

// Destructor.
SetLoad::~SetLoad() {
  free_sdc_objects(objects);
}

//-------------------------------------------------------------------------------------------------

// Constructor.
SetDrivingCell::SetDrivingCell():
  is_rise(false),
  is_fall(false),
  is_min(false),
  is_max(false),
  dont_scale(false),
  no_design_rule(false),
  is_clock_fall(false),
  lib_name(nullptr),
  lib_cell_name(nullptr),
  pin_name(nullptr),
  from_pin_name(nullptr),
  clock_name(nullptr)
{

}

// Destructor.
SetDrivingCell::~SetDrivingCell() {
  free_sdc_objects(objects);
  free_sdc_object(lib_name);
  free_sdc_object(lib_cell_name);
  free_sdc_object(pin_name);
  free_sdc_object(from_pin_name);
  free_sdc_object(clock_name);
}

//-------------------------------------------------------------------------------------------------

// Constructor.
CreateClock::CreateClock():
  is_add(false),
  clock_name(nullptr),
  comment_string(nullptr)
{
}

// Destructor.
CreateClock::~CreateClock() {
  free_sdc_objects(objects);
  free_sdc_object(clock_name);
  free_sdc_object(comment_string);
}

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

