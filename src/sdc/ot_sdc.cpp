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

#include "ot_sdc.h"

namespace OpenTimer {

// Constructor.
SDC::SDC():
  _sdc_fptr(nullptr),
  _user_data(nullptr),
  _create_clock(nullptr),
  _set_input_delay(nullptr),
  _set_output_delay(nullptr),
  _set_load(nullptr),
  _set_driving_cell(nullptr)
{
}

// Destructor.
SDC::~SDC() {

}

// Function: _is_sdc_delimiter
// Return true if a given char is the string delimiter.
bool SDC::_is_delimiter(const char c) const {
  // Delimiters of string parsing.
  static char sdc_delimiters[] = {
    '[', ']', 
    '{', '}'
  };
  for (unsigned i=0; i < sizeof(sdc_delimiters); ++i) {
    if (c == sdc_delimiters[i])
      return true;
  }
  return false;
}

// Procedure: _read_file_to_buffer
// Read the file into a buffer of main memory by a give file name to fpath. The procedure
// modifies the two pointers beg and end by pointing to the beginning address
// and ending address of the buffer.
void SDC::_read_file_to_buffer(const char* fpath, char* &beg, char* &end) {

  size_t file_size;
  std::ifstream ifs(fpath);                                   
  
  CHECK(ifs.good());                                       // Sanity check.

  ifs.seekg(0, std::ios::end);                              // Move to the end of file.
  file_size = ifs.tellg();                                  // Tell the file size.
  ifs.seekg(0, std::ios::beg);                              // Move back to the beg of file.
  beg = new char [file_size + 1];                           // Beg buffer.
  end = beg + file_size;                                    // End buffer.
  ifs.read(beg, file_size);                                 // Read the file into the buffer.
  beg[file_size] = '\0';                                    // End of buffer.
  ifs.close();
}

// Function: _read_next_token
// Read the next token and store it in the token. The current pointer is also redirected
// to the next token position.
bool SDC::_read_next_token(const char* beg, const char* end, char*& pre, char*& cursor, char* token) {
  pre = cursor; 
  int len = 0;
  for(; cursor < end; ++cursor) {

    // Comment line.
    if(*cursor == '#') {
      for(; *cursor != '\n'; ++cursor) ;
    }
    
    // Group delimiters.
    if(_is_delimiter(*cursor)) {
      if(!len) {
        token[len++] = *cursor++;
      }
      break;
    }
    
    // Normal delimiters.
    if(*cursor == ' ' || *cursor == '\n') {
      if(len) {
        ++cursor;
        break;
      }
    } 
    else token[len++] = *cursor;
  }
  token[len] = '\0';
  return len != 0;
}

// Procedure: _read_objects.
void SDC::_read_objects(std::vector<char*>&objects, const char* beg, const char* end, char*& pre, char*& cursor) {      
  char token[1024];
  _read_next_token(beg, end, pre, cursor, token);
  CHECK(_is_object_access_command(token));
  while(_read_next_token(beg, end, pre, cursor, token)) {
    if(token[0] == ']') break;
    if(_is_delimiter(token[0])) continue;
    objects.push_back(strdup(token));
  }
}

// Function: _is_object_access_command
// Return true if the given string is an object access command. The object access command could be
// 1) current_design 
// 2) get_clocks
// 3) all_clocks
// 4) get_port
// 5) all_inputs
// 6) all_outputs
// 7) get_cells
// 8) get_pins
// 9) get_nets
// 10) get_libs
// 11) get_lib_cells
// 12) get_lib_pins
// 13) all_registers
// 
bool SDC::_is_object_access_command(const char *token) const {
  if(!strcmp(token, "current_design")) return true;
  else if(!strcmp(token, "get_clocks")) return true;
  else if(!strcmp(token, "all_clocks")) return true;
  else if(!strcmp(token, "get_ports")) return true;
  else if(!strcmp(token, "all_inputs")) return true;
  else if(!strcmp(token, "all_outputs")) return true;
  else if(!strcmp(token, "get_cells")) return true;
  else if(!strcmp(token, "get_pins")) return true;
  else if(!strcmp(token, "get_lib_cells")) return true;
  else if(!strcmp(token, "get_lib_pins")) return true;
  else if(!strcmp(token, "get_libs")) return true;
  else if(!strcmp(token, "get_nets")) return true;
  else if(!strcmp(token, "all_registers")) return true;
  else return false;
}

// Procedure: _read_set_load
// [-min]
// [-max] 
// [-subtract_pin_load] 
// [-pin_load] 
// [-wire_load]
// value
// objects
void SDC::_read_set_load(const char* beg, const char* end, char*& pre, char*& cursor) {
  
  SetLoad reader;
  char token[1024];

  while(_read_next_token(beg, end, pre, cursor, token)) {
    if(strcmp(token, "-min") == 0) {
      reader.is_min = true;
    }
    else if(strcmp(token, "-max") == 0) {
      reader.is_max = true;
    }
    else if(strcmp(token, "-subtract_pin_load") == 0) {
      reader.is_subtract_pin_load = true;
    }
    else if(strcmp(token, "-pin_load") == 0) {
      reader.is_pin_load = true;
    }
    else if(strcmp(token, "-wire_load") == 0) {
      reader.is_wire_load = true;
    }
    else if(Utility::to_numeric(token) != FLT_MAX) {
      reader.value = Utility::to_numeric(token);
    } 
    else if(token[0] == '[') {
      _read_objects(reader.objects, beg, end, pre, cursor);
    }
    else {
      cursor = pre;
      break;
    }
  }
  
  // Callback function.
  if(_set_load != nullptr) {
    _set_load(&reader, _user_data);
  }
  
  // DEBUG:
  std::cout << dump_set_load(&reader);
}

// Procedure: _read_create_clock
// -period period_value 
// [-name clock_name] 
// [-waveform edge_list] 
// [-add]
// [-comment comment_string] 
// [source_objects]
void SDC::_read_create_clock(const char* beg, const char* end, char*& pre, char*& cursor) {

  CreateClock reader;  
  char token[1024];
  
  while(_read_next_token(beg, end, pre, cursor, token)) {
    if(strcmp(token, "-period") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.period = Utility::to_numeric(token);
    }
    else if(strcmp(token, "-name") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.clock_name = strdup(token);
    }
    else if(strcmp(token, "-add") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.is_add = true;
    }
    else if(strcmp(token, "-comment") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.comment_string = strdup(token);
    }
    else if(token[0] == '[') {
      _read_objects(reader.objects, beg, end, pre, cursor);
    }
    else {
      cursor = pre;
      break;
    }
  }
  
  // Call back function.
  if(_create_clock != nullptr) {
    _create_clock(&reader, _user_data);
  }

  // DEBUG:
  std::cout << dump_create_clock(&reader);
}

// Procedure: read_set_input_delay
// [-clock clock_name] 
// [-reference_pin pin_port_name] 
// [-clock_fall] 
// [-level_sensitive]
// [-rise]
// [-fall]
// [-max]
// [-min]
// [-add_delay] 
// [-network_latency_included] 
// [-source_latency_included] 
// delay_value
// port_pin_list
void SDC::_read_set_input_delay(const char* beg, const char* end, char*& pre, char*& cursor) {
  
  char token[1024];
  SetInputDelay reader; 

  while(_read_next_token(beg, end, pre, cursor, token)) {
    
    if(strcmp(token, "-clock") == 0) {
      _read_next_token(beg, end, pre, cursor, token);  
      reader.clock_name = strdup(token);
    }
    else if(strcmp(token, "-level_sensitive") == 0) {
      reader.is_level_sensitive = true;
    }
    else if(strcmp(token, "-rise") == 0) {
      reader.is_rise = true;
    }
    else if(strcmp(token, "-fall") == 0) {
      reader.is_fall = true;
    }
    else if(strcmp(token, "-max") == 0) {
      reader.is_max = true;
    }
    else if(strcmp(token, "-min") == 0) {
      reader.is_min = true;
    }
    else if(strcmp(token, "-add_delay") == 0) {
      reader.is_add_delay = true;
    }
    else if(strcmp(token, "-network_latency_included") == 0) {
      reader.is_network_latency_included = true;
    }
    else if(strcmp(token, "-source_latency_included") == 0) {
      reader.is_source_latency_included = true;
    }
    else if(Utility::to_numeric(token) != FLT_MAX) {
      reader.delay_value = Utility::to_numeric(token);
    }
    else if(token[0] == '[') {
      _read_objects(reader.objects, beg, end, pre, cursor);
    }
    else {
      cursor = pre;
      break;
    }
  }
  
  // Call back function.
  if(_set_input_delay != nullptr) {
    _set_input_delay(&reader, _user_data);
  }

  // DEBUG:
  std::cout << dump_set_input_delay(&reader);
}

// Procedure: read_set_output_delay
// [-clock clock_name] 
// [-reference_pin pin_port_name] 
// [-clock_fall] 
// [-level_sensitive]
// [-rise]
// [-fall]
// [-max]
// [-min]
// [-add_delay] 
// [-network_latency_included] 
// [-source_latency_included] 
// delay_value
// port_pin_list
void SDC::_read_set_output_delay(const char* beg, const char* end, char*& pre, char*& cursor) {
  
  char token[1024];
  SetOutputDelay reader; 

  while(_read_next_token(beg, end, pre, cursor, token)) {
    
    if(strcmp(token, "-clock") == 0) {
      _read_next_token(beg, end, pre, cursor, token);  
      reader.clock_name = strdup(token);
    }
    else if(strcmp(token, "-level_sensitive") == 0) {
      reader.is_level_sensitive = true;
    }
    else if(strcmp(token, "-rise") == 0) {
      reader.is_rise = true;
    }
    else if(strcmp(token, "-fall") == 0) {
      reader.is_fall = true;
    }
    else if(strcmp(token, "-max") == 0) {
      reader.is_max = true;
    }
    else if(strcmp(token, "-min") == 0) {
      reader.is_min = true;
    }
    else if(strcmp(token, "-add_delay") == 0) {
      reader.is_add_delay = true;
    }
    else if(strcmp(token, "-network_latency_included") == 0) {
      reader.is_network_latency_included = true;
    }
    else if(strcmp(token, "-source_latency_included") == 0) {
      reader.is_source_latency_included = true;
    }
    else if(Utility::to_numeric(token) != FLT_MAX) {
      reader.delay_value = Utility::to_numeric(token);
    }
    else if(token[0] == '[') {
      _read_objects(reader.objects, beg, end, pre, cursor);
    }
    else {
      cursor = pre;
      break;
    }
  }
  // Call back function.
  if(_set_output_delay != nullptr) {
    _set_output_delay(&reader, _user_data);
  }

  // DEBUG:
  std::cout << dump_set_output_delay(&reader);
}

// Procedure: set_driving_cell
// [-lib_cell lib_cell_name] 
// [-rise]
// [-fall]
// [-min]
// [-max]
// [-library lib_name] 
// [-pin pin_name] 
// [-from_pin from_pin_name] 
// [-multiply_by factor] 
// [-dont_scale] 
// [-no_design_rule]
// [-clock clock_name]
// [-clock_fall] 
// [-input_transition_rise rise_time] 
// [-input_transition_fall fall_time]
// port_list
void SDC::_read_set_driving_cell(const char* beg, const char* end, char*& pre, char*& cursor) {
  
  SetDrivingCell reader;
  char token[1024];
  
  while(_read_next_token(beg, end, pre, cursor, token)) {
    if(strcmp(token, "-lib_cell") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.lib_cell_name = strdup(token);
    }
    else if(strcmp(token, "-rise") == 0) {
      reader.is_rise = true;
    }
    else if(strcmp(token, "-fall") == 0) {
      reader.is_fall = true;
    }
    else if(strcmp(token, "-min") == 0) {
      reader.is_min = true;
    }
    else if(strcmp(token, "-max") == 0) {
      reader.is_max = true;
    }
    else if(strcmp(token, "-library") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.lib_name = strdup(token);
    }
    else if(strcmp(token, "-pin") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.pin_name = strdup(token);
    }
    else if(strcmp(token, "-from_pin_name") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.from_pin_name = strdup(token);
    }
    else if(strcmp(token, "-multiply_by") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.factor = Utility::to_numeric(token);
    }
    else if(strcmp(token, "-dont_scale") == 0) {
      reader.dont_scale = true; 
    }
    else if(strcmp(token, "-no_design_rule") == 0) {
      reader.no_design_rule = true;
    }
    else if(strcmp(token, "-clock") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.clock_name = strdup(token);
    }
    else if(strcmp(token, "-clock_fall") == 0) {
      reader.is_clock_fall = true;
    }
    else if(strcmp(token, "-input_transition_rise") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.rise_time = Utility::to_numeric(token);
    }
    else if(strcmp(token, "-input_transition_fall") == 0) {
      _read_next_token(beg, end, pre, cursor, token);
      reader.fall_time = Utility::to_numeric(token);
    }
    else if(token[0] == '[') {
      _read_objects(reader.objects, beg, end, pre, cursor);
    }
    else {
      cursor = pre;
      break;
    }
  }
  
  // Call back function.
  if(_set_driving_cell != nullptr) {
    _set_driving_cell(&reader, _user_data);
  }

  // DEBUG:
  std::cout << dump_set_driving_cell(&reader);
}

// Procedure: read
// The main procedure of reading the SDC file and execute a series of callback functions
// that are specified by the users.
void SDC::read(const char *fpath, void* user_data) {
  
  // Set the user data.
  set_user_data(user_data);
  
  // Read the file.
  char* pre(nullptr);
  char* beg;
  char* end; 
  char* cursor;
  char token[1024];

  _read_file_to_buffer(fpath, beg, end);
  cursor = beg;
  
  while(_read_next_token(beg, end, pre, cursor, token)) {

    printf("token is: %s\n", token);

    if(strcmp(token, "set_input_delay") == 0) {
      _read_set_input_delay(beg, end, pre, cursor);
    }
    else if(strcmp(token, "set_load") == 0) {
      _read_set_load(end, end, pre, cursor); 
    }
    else if(strcmp(token, "set_driving_cell") == 0) {
      _read_set_driving_cell(beg, end, pre, cursor);
    }
    else if(strcmp(token, "set_output_delay") == 0) {
      _read_set_output_delay(beg, end, pre, cursor);
    }
    else if(strcmp(token, "create_clock") == 0) {
      _read_create_clock(beg, end, pre, cursor);
    }
    else {
      CHECK(false);
    }
  }

  delete [] beg;
}


};


