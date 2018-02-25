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

#include "ot_sdcdump.h"

namespace OpenTimer {

// Procedure: dump_objects
static void dump_objects(std::vector<char*>&objects, std::stringstream &ss) {
  for(unsigned i=0; i<objects.size(); i++) {
    ss << " " << (objects[i] ? objects[i] : "nullptr");
  }
  ss << std::endl;
}

// Function: dump_set_input_delay
std::string dump_set_input_delay(SetInputDelay* reader) {

  std::stringstream ss;
  
  ss << "----------------------------------------------------------------------------------------\n";
  if(reader == nullptr) {
    ss << "Command N/A\n"; 
  }
  else {
    ss << "Command                 : set_input_delay\n";
    ss << "Delay value             : " << reader->delay_value << std::endl;
    ss << "Rise                    : " << reader->is_rise << std::endl;
    ss << "Fall                    : " << reader->is_fall << std::endl;
    ss << "Clock fall              : " << reader->is_clock_fall << std::endl;
    ss << "Level sensitive         : " << reader->is_level_sensitive << std::endl;
    ss << "Min                     : " << reader->is_min << std::endl;
    ss << "Max                     : " << reader->is_max << std::endl;
    ss << "Add delay               : " << reader->is_add_delay << std::endl;
    ss << "Network latency included: " << reader->is_network_latency_included << std::endl;
    ss << "Source latency included : " << reader->is_source_latency_included << std::endl;
    ss << "Clock name              : " << (reader->clock_name ? reader->clock_name : "nullptr") << std::endl;
    ss << "Objects                 :";
    dump_objects(reader->objects, ss);
  }
  ss << "----------------------------------------------------------------------------------------\n";
  return ss.str();
}

// Function: dump_set_output_delay
std::string dump_set_output_delay(SetOutputDelay* reader) {

  std::stringstream ss;
  
  ss << "----------------------------------------------------------------------------------------\n";
  if(reader == nullptr) {
    ss << "Command N/A\n"; 
  }
  else {
    ss << "Command                 : set_output_delay\n";
    ss << "Delay value             : " << reader->delay_value << std::endl;
    ss << "Rise                    : " << reader->is_rise << std::endl;
    ss << "Fall                    : " << reader->is_fall << std::endl;
    ss << "Clock fall              : " << reader->is_clock_fall << std::endl;
    ss << "Level sensitive         : " << reader->is_level_sensitive << std::endl;
    ss << "Min                     : " << reader->is_min << std::endl;
    ss << "Max                     : " << reader->is_max << std::endl;
    ss << "Add delay               : " << reader->is_add_delay << std::endl;
    ss << "Network latency included: " << reader->is_network_latency_included << std::endl;
    ss << "Source latency included : " << reader->is_source_latency_included << std::endl;
    ss << "Clock name              : " << (reader->clock_name ? reader->clock_name : "nullptr") << std::endl;
    ss << "Objects                 :";
    dump_objects(reader->objects, ss);
  }
  ss << "----------------------------------------------------------------------------------------\n";
  return ss.str();
}

// Function: dump_set_load
std::string dump_set_load(SetLoad* reader) {
  
  std::stringstream ss;
  
  ss << "----------------------------------------------------------------------------------------\n";
  if(reader == nullptr) {
    ss << "Command N/A\n"; 
  }
  else {
    ss << "Command                 : set_load\n";
    ss << "Load capacitance        : " << reader->value << std::endl;
    ss << "Min                     : " << reader->is_min << std::endl;
    ss << "Max                     : " << reader->is_max << std::endl;
    ss << "Subtrace pin load       : " << reader->is_subtract_pin_load << std::endl;
    ss << "Pin load                : " << reader->is_pin_load << std::endl;
    ss << "Wire load               : " << reader->is_wire_load << std::endl;
    ss << "Objects                 :";
    dump_objects(reader->objects, ss);
  }
  ss << "----------------------------------------------------------------------------------------\n";
  return ss.str();
}

// Function: dump_create_clock
std::string dump_create_clock(CreateClock* reader) {
  
  std::stringstream ss;
  
  ss << "----------------------------------------------------------------------------------------\n";
  if(reader == nullptr) {
    ss << "Command N/A\n"; 
  }
  else {
    ss << "Command                 : create_clock\n";
    ss << "Period                  : " << reader->period << std::endl;
    ss << "add                     : " << reader->is_add << std::endl;
    ss << "Clock name              : " << (reader->clock_name ? reader->clock_name : "nullptr") << std::endl;
    ss << "Commen string           : " << (reader->comment_string ? reader->comment_string : "nullptr") << std::endl;
    ss << "Objects                 :";
    dump_objects(reader->objects, ss);
  }
  ss << "----------------------------------------------------------------------------------------\n";
  return ss.str();
}

// Function: dump_set_driving_cell
std::string dump_set_driving_cell(SetDrivingCell* reader) {
  
  std::stringstream ss;
  
  ss << "----------------------------------------------------------------------------------------\n";
  if(reader == nullptr) {
    ss << "Command N/A\n"; 
  }
  else {
    ss << "Command                 : set_driving_cell\n";
    ss << "Rise                    : " << reader->is_rise << std::endl;
    ss << "Fall                    : " << reader->is_fall << std::endl;
    ss << "Min                     : " << reader->is_min << std::endl;
    ss << "Max                     : " << reader->is_max << std::endl;
    ss << "Don't scale             : " << reader->dont_scale << std::endl;
    ss << "No desig rule           : " << reader->no_design_rule << std::endl;
    ss << "Clock fall              : " << reader->is_clock_fall << std::endl;
    ss << "Library name            : " << (reader->lib_name ? reader->lib_name : "nullptr") << std::endl;
    ss << "Library cell name       : " << (reader->lib_cell_name ? reader->lib_cell_name : "nullptr") << std::endl;
    ss << "Pin name                : " << (reader->pin_name ? reader->pin_name : "nullptr") << std::endl;
    ss << "From pin name           : " << (reader->from_pin_name ? reader->from_pin_name : "nullptr") << std::endl;
    ss << "Clock name              : " << (reader->clock_name ? reader->clock_name : "nullptr") << std::endl;
    ss << "Objects                 :";
    dump_objects(reader->objects, ss);
  }
  ss << "----------------------------------------------------------------------------------------\n";
  return ss.str();
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------









