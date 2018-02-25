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

#include "ot_celllib.h"

namespace OpenTimer {

// Constructor.
CellLib::CellLib():
  _library_type(LibraryType::UNDEFINED)
{
}

// Destructor.
CellLib::~CellLib() {

}

// Procedure: read_file_to_tokens
void_t CellLib::_read_file_to_tokens(string_crt fpath, string_vrt tokens) {
  
  size_t file_size;
  ifstream_t ifs(fpath);                                    // Open the file.
  
  LOG_IF(FATAL, !ifs.good()) << "Failed to open " + Utility::realpath(fpath);

  ifs.seekg(0, std::ios::end);                              // Move to the end of file.
  file_size = ifs.tellg();                                  // Tell the file size.
  ifs.seekg(0, std::ios::beg);                              // Move back to the beg of file.
  char_pt buffer = new char_t [file_size + 1];
  ifs.read(buffer, file_size);                              // Read the file into the buffer.
  buffer[file_size] = '\0';                                 // End of buffer.
  ifs.close();
  
  // Mark out the comments.
  for(unsigned_t i=0; i<file_size; i++) {
    if(buffer[i] == '/' && buffer[i+1] == '*') {
      buffer[i] = buffer[i+1] = ' ';
      bool_t comments(true);
      for(i=i+2; i<file_size && comments; buffer[i++]=' ') {
        if(buffer[i] == '*' && buffer[i+1] == '/') {
          buffer[i++] = ' ';
          comments = false;
        }
      }
    }
  }
  
  // Parse the token.
  string_t token;
  tokens.clear();
  tokens.reserve(2000000);

  for(unsigned_t i=0; i<file_size; i++) {
    char_t curr_char = buffer[i];
    bool_t is_delimiter = _is_delimiter(curr_char);
    if (isspace (curr_char) || is_delimiter) {
      if (!token.empty()) {                                 // Add the current token.
        tokens.push_back(token) ;
        token.clear() ;
      }
      if (_is_group_delimiter(curr_char)) {                 // Include the stack char.
        tokens.push_back(string_t(1, curr_char));
      }
    } else token.push_back(curr_char);                      // Add the char to the current token.
  }
  if(!token.empty()) tokens.push_back(token);

  delete [] buffer;
}

// Function: _is_library_delimiter
// Return true if a given char is the string delimiter.
bool_t CellLib::_is_delimiter(char_ct c) const {
  // Delimiters of string parsing.
  static char_ct library_delimiters[] = {
    '(', ')', 
    ',', ':', ';', 
    '/', 
    '#', 
    '[', ']', 
    '{', '}', 
    '*', 
    '\"', '\\'
  };
  for (unsigned_t i=0; i < sizeof(library_delimiters); ++i) {
    if (c == library_delimiters[i])
      return true;
  }
  return false;
}

// Function: _is_group_delimiter
bool_t CellLib::_is_group_delimiter(char_ct c) const {
  return _is_beg_group_delimiter(c) || _is_end_group_delimiter(c);
}

// Function: _is_beg_group_delimiter
bool_t CellLib::_is_beg_group_delimiter(char_ct c) const {
  return c == '{';
}

// Function: _is_end_group_delimiter
bool_t CellLib::_is_end_group_delimiter(char_ct c) const {
  return c == '}';
}

// Function: _is_dummy_timing
// Return true if the given timing object is a dummy timing based on the library type.
bool_t CellLib::_is_dummy_timing(timing_pt timing_ptr) const {
  if(timing_ptr->is_hold_constraint() && library_type() != LibraryType::EARLY_LIBRARY) return true;
  if(timing_ptr->is_setup_constraint() && library_type() != LibraryType::LATE_LIBRARY) return true;
  return false;
}

// Function: insert_cell
// Insert a cell into this library.
cell_pt CellLib::insert_cell(string_crt name) {
  cell_pt cell_ptr = _cell_dict.insert(name, name);
  cell_ptr->set_celllib_ptr(this);
  return cell_ptr;
}

// Function: insert_lut_template
// Insert a lut template into this library.
lut_template_pt CellLib::insert_lut_template(string_crt name) {
  lut_template_pt lut_template_ptr = _lut_template_dict.insert(name, name);
  lut_template_ptr->set_celllib_ptr(this);
  return lut_template_ptr;
}

// Function: read
// Initialize this cell class by reading a given cell library file.
void_t CellLib::read(string_crt fpath) {
  
  // [BEG INFO]
  LOG(INFO) << "Loading " + Utility::realpath(fpath);
  // [END INFO]
  
  unsigned_t idx(0);
  int_t stack_lvl(1);
  int_t increment;
  string_vt tokens;
  _read_file_to_tokens(fpath, tokens);

  while(tokens[idx++] != "library");                        // Read library name.
  set_name(tokens[idx]);

  // BEG OF LIBRARY GROUP.
  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.
  
  while(stack_lvl && idx < tokens.size()) {

    increment = 1;

    if(tokens[idx] == "lu_table_template") {                // LUT template.
      lut_template_pt lut_template_ptr = insert_lut_template(tokens[++idx]);
      _read_lut_template(tokens, ++idx, lut_template_ptr);
      increment = 0;
    }
    else if(tokens[idx] == "cell") {                        // Read a new cell.
      cell_pt cell_ptr = insert_cell(tokens[++idx]);
      _read_cell(tokens, ++idx, cell_ptr);
      increment = 0;
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {
    }
    idx = idx + increment;
  }
  // END OF LIBRARY GROUP.
}

// Procedure: _read_lut_template
// Read the lut template to a give pointer to a lut template.
void_t CellLib::_read_lut_template(string_vrt tokens, unsigned_rt idx, lut_template_pt lut_template_ptr) {

  int_t stack_lvl(1);
  int_t increment;
  float_t val;
  
  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.

  // BEG OF LUT TEMPLATE GROUP.
  while(stack_lvl && idx < tokens.size()) {
    
    increment = 1;

    if(tokens[idx] == "variable_1") {                       // Read the variable.
      idx = idx + 1;
      if(tokens[idx] == "total_output_net_capacitance") {
        lut_template_ptr->set_variable1(TOTAL_OUTPUT_NET_CAPACITANCE);
      }
      else if(tokens[idx] == "input_net_transition") {
        lut_template_ptr->set_variable1(INPUT_NET_TRANSITION);
      }
      else if(tokens[idx] == "constrained_pin_transition") {
        lut_template_ptr->set_variable1(CONSTRAINED_PIN_TRANSITION);
      }
      else if(tokens[idx] == "related_pin_transition") {
        lut_template_ptr->set_variable1(RELATED_PIN_TRANSITION);
      }
      else {
        LOG(WARNING) << "Unexpected LUT variable \"" + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "variable_2") {                  // Read the variable.
      idx = idx + 1;
      if(tokens[idx] == "total_output_net_capacitance") {
        lut_template_ptr->set_variable2(TOTAL_OUTPUT_NET_CAPACITANCE);
      }
      else if(tokens[idx] == "input_net_transition") {
        lut_template_ptr->set_variable2(INPUT_NET_TRANSITION);
      }
      else if(tokens[idx] == "constrained_pin_transition") {
        lut_template_ptr->set_variable2(CONSTRAINED_PIN_TRANSITION);
      }
      else if(tokens[idx] == "related_pin_transition") {
        lut_template_ptr->set_variable2(RELATED_PIN_TRANSITION);
      }
      else {
        LOG(WARNING) << "Unexpected LUT variable \"" + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "index_1") {                     // Read the index.
      while((val=Utility::to_numeric(tokens[++idx])) != OT_UNDEFINED_FLT) {
        lut_template_ptr->insert_index1(val);
      }
      increment = 0;
    }
    else if(tokens[idx] == "index_2") {                     // Read the index.
      while((val=Utility::to_numeric(tokens[++idx])) != OT_UNDEFINED_FLT) {
        lut_template_ptr->insert_index2(val);
      }
      increment = 0;
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {
    }
    idx = idx + increment;
  }
  // END OF LUT TEMPLATE GROUP.
}

// Procedure: _read_cell
// Read the cell information of a given pointer to the cell.
void_t CellLib::_read_cell(string_vrt tokens, unsigned_rt idx, cell_pt cell_ptr) {
  
  cellpin_pt cellpin_ptr(nullptr);

  int_t stack_lvl;
  int_t increment;
  
  // BEG OF CELL GROUP.
  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.
  stack_lvl = 1;

  while(stack_lvl && idx < tokens.size()) {

    increment = 1;                                          // Increment of the index.

    if(tokens[idx] == "cell_leakage_power") {               // Read the leakage power.
      cell_ptr->set_leakage_power(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "cell_footprint") {              // Read the footprint.
      cell_ptr->set_footprint(tokens[++idx]);
    }
    else if(tokens[idx] == "area") {                        // Read the area.
      cell_ptr->set_area(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "pin") {                         // Read the cell pin group.
      cellpin_ptr = cell_ptr->insert_cellpin(tokens[++idx]);  
      _read_cellpin(tokens, ++idx, cellpin_ptr);
      increment = 0;
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {                                                  // Ignore other keywords.
    }
    idx = idx + increment;
  }
  // END OF CELL GROUP.
  
}

// Procedure: _read_lut
// Read the floating values into the timing lut.
void_t CellLib::_read_lut(string_vrt tokens, unsigned_rt idx, timing_lut_pt timing_lut_ptr) {

  int_t stack_lvl(1);
  int_t increment;
  size_t size1;
  size_t size2;

  timing_lut_ptr->set_lut_template_ptr(_lut_template_dict[tokens[idx]]);

  if(timing_lut_ptr->lut_template_ptr()) {                  // 2D LUT.
    size1 = timing_lut_ptr->lut_template_ptr()->size1();
    size2 = timing_lut_ptr->lut_template_ptr()->size2();
  }
  else {                                                    // 1D scalar.
    size1 = 1;
    size2 = 1;
  }

  timing_lut_ptr->resize(size1, size2);                     // Resize the lut.

  //printf("\t\tlut %s's size %lu %lu\n", tokens[idx].c_str(), size1, size2);

  while(tokens[idx++] != "{");

  // BEG OF TIMING LUT GROUP.
  while(stack_lvl && idx < tokens.size()) {
    
    increment = 1;

    if(tokens[idx] == "index_1") {                          // Read the index 1.
      for(unsigned_t i=0; i<size1; i++) {
        timing_lut_ptr->assign_index_1(i, atof(tokens[++idx].c_str()));
      }
    }
    else if(tokens[idx] == "index_2") {                     // Read the index 2.
      for(unsigned_t i=0; i<size2; i++) {
        timing_lut_ptr->assign_index_2(i, atof(tokens[++idx].c_str()));
      }
    }
    else if(tokens[idx] == "values") {                      // Read the lut values.
      for(unsigned_t i=0; i<size1; i++) {
        for(unsigned_t j=0; j<size2; j++) {
          timing_lut_ptr->assign_value(i, j, atof(tokens[++idx].c_str()));
        }
      }
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {
      LOG(WARNING) << "Unexpected LUT keyword " + tokens[idx];
    }
    idx = idx + increment;
  }
  // END OF TIMING LUT GROUP.
}

// Procedure: _read_timing
// Read the timing information to a given pointer to a timing object.
void_t CellLib::_read_timing(string_vrt tokens, unsigned_rt idx, timing_pt timing_ptr) {

  int_t stack_lvl;
  int_t increment;

  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.
  stack_lvl = 1;
  
  // BEG OF TIMING GROUP.
  while(stack_lvl && idx < tokens.size()) {
    
    increment = 1;

    if (tokens[idx] == "cell_fall") {                       // Fall delay.
      _read_lut(tokens, ++idx, timing_ptr->cell_fall_ptr());
      increment = 0;
    } 
    else if (tokens[idx] == "cell_rise") {                  // Rise delay.
      _read_lut(tokens, ++idx, timing_ptr->cell_rise_ptr());
      increment = 0;
    } 
    else if (tokens[idx] == "fall_transition") {            // Fall slew.
      _read_lut(tokens, ++idx, timing_ptr->fall_transition_ptr());
      increment = 0;
    } 
    else if (tokens[idx] == "rise_transition") {            // Rise slew.
      _read_lut(tokens, ++idx, timing_ptr->rise_transition_ptr());
      increment = 0;
    } 
    else if (tokens[idx] == "rise_constraint") {            // FF rise constraint.
      _read_lut(tokens, ++idx, timing_ptr->rise_constraint_ptr());
      increment = 0;
    }
    else if(tokens[idx] == "fall_constraint") {             // FF fall constraint.
      _read_lut(tokens, ++idx, timing_ptr->fall_constraint_ptr());
      increment = 0;
    }
    else if (tokens[idx] == "timing_sense") {               // Read the timing sense.
      idx = idx + 1;
      if(tokens[idx] == "negative_unate") {
        timing_ptr->set_timing_sense(NEGATIVE_UNATE);       // Negative unate.
      }
      else if(tokens[idx] == "positive_unate") {            // Positive unate.
        timing_ptr->set_timing_sense(POSITIVE_UNATE);
      }
      else if(tokens[idx] == "non_unate") {                 // Non unate.
        timing_ptr->set_timing_sense(NON_UNATE);
      }
      else {
        LOG(WARNING) << "Unexpected timing sense \"" + tokens[idx] + "\" in .lib file";
      }
    } 
    else if (tokens[idx] == "timing_type") {                // Timing type.
      idx = idx + 1;
      if(tokens[idx] == "combinational") {
        timing_ptr->set_timing_type(TimingType::COMBINATIONAL);
      }
      else if(tokens[idx] == "rising_edge") {
        timing_ptr->set_timing_type(TimingType::RISING_EDGE);
      }
      else if(tokens[idx] == "falling_edge") {
        timing_ptr->set_timing_type(TimingType::FALLING_EDGE);
      }
      else if(tokens[idx] == "hold_rising") {
        timing_ptr->set_timing_type(TimingType::HOLD_RISING);
      }
      else if(tokens[idx] == "hold_falling") {
        timing_ptr->set_timing_type(TimingType::HOLD_FALLING);
      }
      else if(tokens[idx] == "setup_rising") {
        timing_ptr->set_timing_type(TimingType::SETUP_RISING);
      }
      else if(tokens[idx] == "setup_falling") {
        timing_ptr->set_timing_type(TimingType::SETUP_FALLING);
      }
      else if(tokens[idx] == "three_state_disable") {
        timing_ptr->set_timing_type(TimingType::THREE_STATE_DISABLE);
      }
      else if(tokens[idx] == "three_state_enable") {
        timing_ptr->set_timing_type(TimingType::THREE_STATE_ENABLE);
      }
      else if(tokens[idx] == "recovery_rising") {
        timing_ptr->set_timing_type(TimingType::RECOVERY_RISING);
      }
      else if(tokens[idx] == "recovery_falling") {
        timing_ptr->set_timing_type(TimingType::RECOVERY_FALLING);
      }
      else if(tokens[idx] == "clear") {
        timing_ptr->set_timing_type(TimingType::CLEAR);
      }
      else if(tokens[idx] == "preset") {
        timing_ptr->set_timing_type(TimingType::PRESET);
      }
      else if(tokens[idx] == "min_pulse_width") {
        timing_ptr->set_timing_type(TimingType::MIN_PULSE_WIDTH);
      }
      else {
        LOG(WARNING) << "Unexpected celllib timing type \"" + tokens[idx] + "\"";
      }
    }
    else if (tokens[idx] == "related_pin") {                // From cellpin.
      timing_ptr->set_from_cellpin_name(tokens[++idx]);
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {
      // ignore unknown keyword.
      LOG(WARNING) << "Unexpected timing group token \"" + tokens[idx];
    }
    idx = idx + increment;
  }
}

// Procedure: _read_cellpin
// Read the cellpin information of a given pointer to the cellpin.
void_t CellLib::_read_cellpin(string_vrt tokens, unsigned_rt idx, cellpin_pt cellpin_ptr) {
  
  timing_arc_pt timing_arc_ptr;
  timing_pt timing_ptr;

  int_t stack_lvl;
  int_t increment;

  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.
  stack_lvl = 1;
  
  // BEG OF CELLPIN GROUP.
  while(stack_lvl && idx < tokens.size()) {
    
    increment = 1;

    if(tokens[idx] == "direction") {                        // Read the pin direction.
      idx = idx + 1;
      if(tokens[idx] == "input") 
        cellpin_ptr->set_direction(INPUT_CELLPIN_DIRECTION);
      else if(tokens[idx] == "output") 
        cellpin_ptr->set_direction(OUTPUT_CELLPIN_DIRECTION);
      else if(tokens[idx] == "inout")
        cellpin_ptr->set_direction(INOUT_CELLPIN_DIRECTION);
      else if(tokens[idx] == "internal")
        cellpin_ptr->set_direction(INTERNAL_CELLPIN_DIRECTION);
      else {
        LOG(WARNING) << "Unexpected cellpin direction \"" + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "capacitance") {                 // Read the pin capacitance.
      cellpin_ptr->set_capacitance(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "max_capacitance") {             // Read the max pin capacitance.
      cellpin_ptr->set_max_capacitance(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "min_capacitance") {             // Read the min pin capacitance.
      cellpin_ptr->set_min_capacitance(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "max_transition") {        
      cellpin_ptr->set_max_transition(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "min_transition") {          
      cellpin_ptr->set_min_transition(atof(tokens[++idx].c_str()));
    }
    else if(tokens[idx] == "clock") {                       // Read the clock status.
      idx = idx + 1;
      if(tokens[idx] == "true") {
        cellpin_ptr->set_is_clock(true);
      }
      else if(tokens[idx] == "false") {
        cellpin_ptr->set_is_clock(false);
      }
      else {
        LOG(WARNING) << "Unexpected cellpin clock flag \"" + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "timing") {                      // Read the timing arcs.
      
      timing_ptr = cellpin_ptr->insert_timing();
      timing_ptr->set_to_cellpin_name(cellpin_ptr->name());

      _read_timing(tokens, ++idx, timing_ptr);
      
      if(!_is_dummy_timing(timing_ptr)) {

        // Obtain the timing arc for the cell connection "from_cellpin" to "to_cellpin".
        timing_arc_ptr = cellpin_ptr->insert_timing_arc(timing_ptr->from_cellpin_name());
        timing_arc_ptr->set_to_cellpin_name(timing_ptr->to_cellpin_name());

        // Assign the transition timing object.
        RF_RF_ITER(irf, orf) {
          if(timing_ptr->is_transition_defined(irf, orf)) {
            timing_arc_ptr->set_timing_ptr(irf, orf, timing_ptr);
          }
        }
      }

      increment = 0;
    }
    else if(tokens[idx] == "original_pin") {
      cellpin_ptr->set_original_pin(tokens[++idx]);
    }
    else if(tokens[idx] == "nextstate_type") {
      idx = idx + 1;

      if(tokens[idx] == "data") {
        cellpin_ptr->set_nextstate_type(NextStateType::DATA);
      }
      else if(tokens[idx] == "preset") {
        cellpin_ptr->set_nextstate_type(NextStateType::PRESET);
      }
      else if(tokens[idx] == "clear") {
        cellpin_ptr->set_nextstate_type(NextStateType::CLEAR);
      }
      else if(tokens[idx] == "load") {
        cellpin_ptr->set_nextstate_type(NextStateType::LOAD);
      }
      else if(tokens[idx] == "scan_in") {
        cellpin_ptr->set_nextstate_type(NextStateType::SCAN_IN);
      }
      else if(tokens[idx] == "scan_enable") {
        cellpin_ptr->set_nextstate_type(NextStateType::SCAN_ENABLE);
      }
      else {
        LOG(WARNING) << "Unexpected nextstate_type " + tokens[idx];
      }
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {                                                  // Ignore other keywords. 
      LOG(WARNING) << "Unexpected token " + tokens[idx];
    }
    idx = idx + increment;
  }
  //END OF CELL PIN GROUP.
}

// Procedure: write
// Write the celllib into a given file.
void_t CellLib::write(string_crt fpath) const {
 
  FILE* fptr = fopen(fpath.c_str(), "w"); 
  
  if(fptr == nullptr) {
    LOG(ERROR) << "Failed to open " + Utility::realpath(fpath);
    return;
  }
  
  // Write the celllib.
  write(fptr);
  
  // Close the file pointer.
  fclose(fptr);
}

// Procedure: write
void_t CellLib::write(file_pt fptr) const {
  
  // Write the comment.
  fprintf(fptr, "/* Generated by %s - %s */\n\n", PACKAGE_STRING, __time_info().c_str());
  
  // Write library name.
  fprintf(fptr, "library (\"%s\") {\n", name().c_str());

  // Write the lut templates
  write_lut_templates(fptr);

  // Write all cells.
  write_cells(fptr);

  // Write library ending group symbol.
  fprintf(fptr, "}\n");
}

// Procedure: write
void_t CellLib::write(ostream& os) const {
  
  // Write the comment.
  os << "/* Generated by " << PACKAGE_STRING << " - " << __time_info() << " */\n\n";
  
  // Write library name.
  os << "library (\"" << name() << "\") {\n";

  // Write the lut templates
  write_lut_templates(os);

  // Write all cells.
  write_cells(os);

  // Write library ending group symbol.
  os << "}\n";
}

// Procedure: write_lut_templates
void_t CellLib::write_lut_templates(file_pt fptr) const {
  for(auto& item : _lut_template_dict) {
    item.second->write(fptr);
  }
}

// Procedure: write_lut_templates
void_t CellLib::write_lut_templates(ostream& os) const {
  for(auto& item : _lut_template_dict) {
    item.second->write(os);
  }
}

// Procedure: write_cells
void_t CellLib::write_cells(file_pt fptr) const {
  for(auto& item : _cell_dict) {
    item.second->write(fptr);
  }
}

// Procedure: write_cells
void_t CellLib::write_cells(ostream& os) const {
  for(auto& item : _cell_dict) {
    item.second->write(os);
  }
}

//-------------------------------------------------------------------------------------------------

// Constructor
Cell::Cell(string_crt key):
  _name(key),
  _leakage_power(OT_DEFAULT_CELL_LEAKAGE_POWER),
  _area(OT_DEFAULT_CELL_AREA),
  _celllib_ptr(nullptr)
{
}

// Destructor
Cell::~Cell() {
}

// Function: insert_cellpin
// Insert a cellpin into the cell and return the pointer to the corresponding memory address.
cellpin_pt Cell::insert_cellpin(string_crt name) {
  auto cellpin_ptr = _cellpin_dict.insert(name, name);
  cellpin_ptr->set_cell_ptr(this);
  return cellpin_ptr;
}

// Procedure: write
void_t Cell::write(file_pt fptr) const {
  // Write the cell name.
  fprintf(fptr, "  cell (\"%s\") {\n", name().c_str());

  // Write the cellpins.
  write_cellpins(fptr);
  
  // Write the ending group symbol.
  fprintf(fptr, "  }\n");
}

// Procedure: write
void_t Cell::write(ostream& os) const {
  // Write the cell name.
  os << "  cell (\"" << name() << "\") {\n";

  // Write the cellpins.
  write_cellpins(os);
  
  // Write the ending group symbol.
  os << "  }\n";
}

// Procedure: write_cellpins
void_t Cell::write_cellpins(file_pt fptr) const {
  for(auto& item : _cellpin_dict) {
    item.second->write(fptr);
  }
}

// Procedure: write_cellpins
void_t Cell::write_cellpins(ostream& os) const {
  for(auto& item : _cellpin_dict) {
    item.second->write(os);
  }
}

//-------------------------------------------------------------------------------------------------

// Constructor: Cellpin
Cellpin::Cellpin(string_crt key):
  _name(key),
  _direction(UNDEFINED_PIN_DIRECTION),
  _nextstate_type(NextStateType::UNDEFINED),
  _capacitance(OT_DEFAULT_CELLPIN_CAPACITANCE),
  _max_capacitance(OT_DEFAULT_CELLPIN_MAX_CAPACITANCE),
  _min_capacitance(OT_DEFAULT_CELLPIN_MIN_CAPACITANCE),
  _max_transition(OT_DEFAULT_CELLPIN_MAX_TRANSITION),
  _min_transition(OT_DEFAULT_CELLPIN_MIN_TRANSITION),
  _is_clock(OT_DEFAULT_CELLPIN_IS_CLOCK),
  _cell_ptr(nullptr),
  _timingset_uptr(new timingset_t()),
  _timing_arc_dict_uptr(new timing_arc_dict_t())
{
}

// Destructor: Cellpin
Cellpin::~Cellpin() {
}

// Function: insert_timing
timing_pt Cellpin::insert_timing() {
  int_t idx = timingset_ptr()->insert();
  timing_pt timing_ptr = timingset()[idx];
  timing_ptr->set_idx(idx);
  return timing_ptr;
}

// Function: insert_timing_arc
timing_arc_pt Cellpin::insert_timing_arc(string_crt from_cellpin_name) {
  return timing_arc_dict_ptr()->insert(from_cellpin_name, from_cellpin_name);
}

// Function: direction_name
string_t Cellpin::direction_name() const {
  return direction_name(direction());
}

// Function: direction_name
string_t Cellpin::direction_name(pin_direction_ce dir) {
  switch(dir) {
    case INPUT_CELLPIN_DIRECTION:
      return "input";
    break;

    case OUTPUT_CELLPIN_DIRECTION:
      return "output";
    break;

    case INOUT_CELLPIN_DIRECTION:
      return "inout";
    break;

    case INTERNAL_CELLPIN_DIRECTION:
      return "internal";
    break;
    
    default:
      return "undefined";
    break;
  }
}

// Function: clock_flag
string_t Cellpin::clock_flag(bool_ct flag) {
  return flag == true ? "true" : "false";
}

// Function: clock_flag
string_t Cellpin::clock_flag() const {
  return clock_flag(is_clock());
}

// Procedure: write_timings
void_t Cellpin::write_timings(file_pt fptr) const {
  for(const auto& timing_ptr : *(timingset_ptr())) {
    timing_ptr->write(fptr);  
  }
}

// Procedure: write_timings
void_t Cellpin::write_timings(ostream& os) const {
  for(const auto& timing_ptr : *(timingset_ptr())) {
    os << *timing_ptr;  
  }
}

// Procedure: write
void_t Cellpin::write(file_pt fptr) const {
  
  // Write the cellpin name.
  fprintf(fptr, "    pin (\"%s\") {\n", name().c_str());
    
  // Write the pin direction.
  fprintf(fptr, "      direction : %s;\n", direction_name().c_str());
  
  // Write the pin capacitance.
  fprintf(fptr, "      capacitance : %f;\n", capacitance());

  // Write the clock flag.
  fprintf(fptr, "      clock : %s;\n", clock_flag().c_str());

  // Write the timing.
  write_timings(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "    }\n");
}

// Procedure: write
void_t Cellpin::write(ostream& os) const {
  
  // Write the cellpin name.
  os << "    pin (\"" << name() << "\") {\n";
    
  // Write the pin direction.
  os << "      direction : " << direction_name() << ";\n";
  
  // Write the pin capacitance.
  os << "      capacitance : " << capacitance() << ";\n";

  // Write the clock flag.
  os << "      clock : " << clock_flag() << "\n";

  // Write the timing.
  write_timings(os);

  // Write the ending group symbol.
  os << "    }\n";
}

//-------------------------------------------------------------------------------------------------

// Constructor: LUTTemplate
LUTTemplate::LUTTemplate(string_crt key):
  _name(key),
  _variable1(UNDEFINED_LOOKUPTABLE_VARIABLE),
  _variable2(UNDEFINED_LOOKUPTABLE_VARIABLE),
  _celllib_ptr(nullptr)
{
}

// Destructor: ~LUTTemplate
LUTTemplate::~LUTTemplate() {
}

// Operator: <<
void LUTTemplate::write(ostream& os) const {

  // Write the lut template name.
  os << "  lu_table_template (" << name() << ") {\n";
  
  // Write variables.
  os << "    variable_1: " << variable_name(variable1()) << ";\n";
  os << "    variable_2: " << variable_name(variable2()) << ";\n";

  // Write indices.
  os << "    index_1 (\"";
  for(unsigned_t i=0; i<size1(); i++) {
    if(i) os << ", ";
    os << indices1()[i];
  }
  os << "\");\n";

  os << "    index_2 (\"";
  for(unsigned_t i=0; i<size2(); i++) {
    if(i) os << ", ";
    os << indices2()[i];
  }
  os << "\");\n";

  // Write the lut template ending group symbol.
  os <<"  }\n";
}

// Procedure: write
// Write the lut template.
void_t LUTTemplate::write(file_pt fptr) const {

  // Write the lut template name.
  fprintf(fptr, "  lu_table_template (%s) {\n", name().c_str());
  
  // Write variables.
  fprintf(fptr, "    variable_1 : %s;\n", variable_name(variable1()).c_str());
  fprintf(fptr, "    variable_2 : %s;\n", variable_name(variable2()).c_str());

  // Write indices.
  fprintf(fptr, "    index_1 (\"");
  for(unsigned_t i=0; i<_indices1.size(); i++) {
    if(i) fprintf(fptr, ", ");
    fprintf(fptr, "%f", _indices1[i]);
  }
  fprintf(fptr, "\");\n");
  fprintf(fptr, "    index_2 (\"");
  for(unsigned_t i=0; i<_indices2.size(); i++) {
    if(i) fprintf(fptr, ", ");
    fprintf(fptr, "%f", _indices2[i]);
  }
  fprintf(fptr, "\");\n");

  // Write the lut template ending group symbol.
  fprintf(fptr, "  }\n");
}

// Operator: copy assignment 
LUTTemplate& LUTTemplate::operator = (const LUTTemplate &rhs) {
  _name = rhs._name;
  _variable1 = rhs._variable1;
  _variable2 = rhs._variable2;
  _indices1 = rhs._indices1;
  _indices2 = rhs._indices2;
  _celllib_ptr = rhs._celllib_ptr;
  return *this;
}

// Procedure: clear
// Clear this object.
void_t LUTTemplate::clear() {

  _name.clear();                                            // Name.
  _indices1.clear();                                        // Indices for index 1.
  _indices2.clear();                                        // Indices for index 2.

  _variable1 = UNDEFINED_LOOKUPTABLE_VARIABLE;              // LUT variable.
  _variable2 = UNDEFINED_LOOKUPTABLE_VARIABLE;              // LUT variable. 

  _celllib_ptr = nullptr;                                      // Pointer to celllib.
}
    
// Function: variable_name
// Return string version of the variable name.
string_t LUTTemplate::variable_name(lut_variable_ce var) const {
  
  switch (var) {
    
    case TOTAL_OUTPUT_NET_CAPACITANCE:
      return "total_output_net_capacitance";
    break;

    case INPUT_NET_TRANSITION:
      return "input_net_transition";
    break;

    case CONSTRAINED_PIN_TRANSITION:
      return "constrained_pin_transition";
    break;

    case RELATED_PIN_TRANSITION:
      return "related_pin_transition";
    break;

    default:
      return "undefined";
    break;
  }
}

// Function: is_transition_variable
// Return true if the given LUT template variable is a transition field or false.
bool_t LUTTemplate::is_transition_variable(lut_variable_ce var) {

  switch (var) {
    case TOTAL_OUTPUT_NET_CAPACITANCE:
      return false;
    break;

    case INPUT_NET_TRANSITION:
      return true;
    break;

    case CONSTRAINED_PIN_TRANSITION:
      return true;
    break;

    case RELATED_PIN_TRANSITION:
      return true;
    break;

    default:
      LOG(WARNING) << "Unexpected LUT template variable";
      return false;
    break;
  }
}

// Function: is_capacitance_variable
// Return true if the given LUT template variable is a capacitance field or false.
bool_t LUTTemplate::is_capacitance_variable(lut_variable_ce var) {
  
  switch (var) {
    case TOTAL_OUTPUT_NET_CAPACITANCE:
      return true;
    break;

    case INPUT_NET_TRANSITION:
      return false;
    break;

    case CONSTRAINED_PIN_TRANSITION:
      return false;
    break;

    case RELATED_PIN_TRANSITION:
      return false;
    break;

    default:
      LOG(WARNING) << "Unexpected LUT template variable";
      return false;
    break;
  }
}



/*
// Procedure: read
// Read the lut template from a given tokenizer object.
void_t LUTTemplate::read(tokenizer_rt tokenizer) {

  int_t stack_lvl(1);
  int_t increment;
  float_t val;
  
  while(!_is_beg_group_delimiter(tokens[idx++][0]));        // Beg of the group.

  // BEG OF LUT TEMPLATE GROUP.
  while(stack_lvl && idx < tokens.size()) {
    
    increment = 1;

    if(tokens[idx] == "variable_1") {                       // Read the variable.
      idx = idx + 1;
      if(tokens[idx] == "total_output_net_capacitance") {
        lut_template_ptr->set_variable1(TOTAL_OUTPUT_NET_CAPACITANCE);
      }
      else if(tokens[idx] == "input_net_transition") {
        lut_template_ptr->set_variable1(INPUT_NET_TRANSITION);
      }
      else if(tokens[idx] == "constrained_pin_transition") {
        lut_template_ptr->set_variable1(CONSTRAINED_PIN_TRANSITION);
      }
      else if(tokens[idx] == "related_pin_transition") {
        lut_template_ptr->set_variable1(RELATED_PIN_TRANSITION);
      }
      else {
        LOG(WARNING) << string_t("Unexpected LUT variable \"") + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "variable_2") {                  // Read the variable.
      idx = idx + 1;
      if(tokens[idx] == "total_output_net_capacitance") {
        lut_template_ptr->set_variable2(TOTAL_OUTPUT_NET_CAPACITANCE);
      }
      else if(tokens[idx] == "input_net_transition") {
        lut_template_ptr->set_variable2(INPUT_NET_TRANSITION);
      }
      else if(tokens[idx] == "constrained_pin_transition") {
        lut_template_ptr->set_variable2(CONSTRAINED_PIN_TRANSITION);
      }
      else if(tokens[idx] == "related_pin_transition") {
        lut_template_ptr->set_variable2(RELATED_PIN_TRANSITION);
      }
      else {
        LOG(WARNING) << string_t("Unexpected LUT variable \"") + tokens[idx] + "\" in .lib file";
      }
    }
    else if(tokens[idx] == "index_1") {                     // Read the index.
      while((val=Utility::to_numeric(tokens[++idx])) != OT_UNDEFINED_FLT) {
        lut_template_ptr->insert_index1(val);
      }
      increment = 0;
    }
    else if(tokens[idx] == "index_2") {                     // Read the index.
      while((val=Utility::to_numeric(tokens[++idx])) != OT_UNDEFINED_FLT) {
        lut_template_ptr->insert_index2(val);
      }
      increment = 0;
    }
    else if(_is_beg_group_delimiter(tokens[idx][0])) {      // Inc the stack level.
      ++stack_lvl;
    }
    else if(_is_end_group_delimiter(tokens[idx][0])) {      // Dec the stack level.
      --stack_lvl; 
    }
    else {
    }
    idx = idx + increment;
  }
  // END OF LUT TEMPLATE GROUP.
}
*/

//-------------------------------------------------------------------------------------------------

// Constructor
TimingLUT::TimingLUT() {
  _lut_template_ptr = nullptr;
}

// Destructor
TimingLUT::~TimingLUT() {
}

// Operator: = (copy assignment)
TimingLUT& TimingLUT::operator = (const TimingLUT& rhs) {
  _indices1 = rhs._indices1;
  _indices2 = rhs._indices2;
  _table = rhs._table;                             
  _lut_template_ptr = rhs._lut_template_ptr;
  return *this;
}

// Procedure: write
void_t TimingLUT::write(ostream& os) const {
  
  // Write the indices1.
  if(!is_scalar()) {
    os << "          index_1 (\"";
    for(unsigned_t i=0; i<size1(); ++i) {
      if(i) os << ", ";
      os << indices1(i);
    }
    os << "\");\n";
  }
         
  // Write the indices2.
  if(!is_scalar()) {
    os << "          index_2 (\"";
    for(unsigned_t i=0; i<size2(); ++i) {
      if(i) os << ", ";
      os << indices2(i);
    }
    os << "\");\n";
  }

  // Write the values.
  os << "          values (\n";
  for(unsigned_t i=0; i<size1(); ++i) {
    os << "            \"";
    for(unsigned_t j=0; j<size2(); ++j) {
      if(j) os << ", ";
      os << table(i, j);
    }
    os << "\",\n";
  }
  os << "          );\n";
}

// Procedure: write
void_t TimingLUT::write(file_pt fptr) const {
  
  // Write the indices1.
  if(!is_scalar()) {
    fprintf(fptr, "          index_1 (\"");
    for(unsigned_t i=0; i<size1(); ++i) {
      if(i) fprintf(fptr, ", ");
      fprintf(fptr, "%f", indices1(i));
    }
    fprintf(fptr, "\");\n");
  }
         
  // Write the indices2.
  if(!is_scalar()) {
    fprintf(fptr, "          index_2 (\"");
    for(unsigned_t i=0; i<size2(); ++i) {
      if(i) fprintf(fptr, ", ");
      fprintf(fptr, "%f", indices2(i));
    }
    fprintf(fptr, "\");\n");
  }

  // Write the values.
  fprintf(fptr, "          values (\n");
  for(unsigned_t i=0; i<size1(); ++i) {
    fprintf(fptr, "            \"");
    for(unsigned_t j=0; j<size2(); ++j) {
      if(j) fprintf(fptr, ", ");
      fprintf(fptr, "%f", table(i, j));
    }
    fprintf(fptr, "\",\n");
  }
  fprintf(fptr, "          );\n");

}

// Procedure: clear
// Clear this timing lut object.
void_t TimingLUT::clear() {

  _indices1.clear();                                        // Output load capacitance indices.
  _indices2.clear();                                        // Input slew indices.

  // Clear the 2D lut.
  for(unsigned_t i=0; i<_table.size(); ++i)
    for(unsigned_t j=0; j<_table[i].size(); ++j)
      _table[i].clear();
  _table.clear();

  _lut_template_ptr = nullptr;
}

// Procedure: resize
// Resize the table
void_t TimingLUT::resize(int_ct num_rows, int_ct num_cols) {
  _indices1.resize(num_rows);
  _indices2.resize(num_cols);
  _table.resize(num_rows);
  for(int_t i=0; i<num_rows; i++) _table[i].resize(num_cols);
}

// Function: template_name
string_t TimingLUT::template_name() const {
  if(is_scalar()) return "scalar";
  return lut_template_ptr()->name();
}

// Function: lut
// Return the linear inter-extra polation given two input numerics on indices1 and indices2.
// Note the x value corresponds to the dimension in indices1 array and y value corresponds to 
// the dimension in indices2 array.
float_t TimingLUT::lut_polation(float_ct val1, float_ct val2) const {
  
  CHECK(size1() >= 1 && size2() >= 1);

  // Case 1: scalar
  if(size1() == 1 && size2() == 1) return _table[0][0];     // Scalar.

  int_t idx1[2], idx2[2];

  idx1[1] = lower_bound(_indices1.begin(), _indices1.end(), val1) - _indices1.begin();
  idx2[1] = lower_bound(_indices2.begin(), _indices2.end(), val2) - _indices2.begin();
  
  // Case 2: linear inter/extra polation.
  idx1[1] = max(1, min(idx1[1], (int_t)(size1() - 1)));
  idx2[1] = max(1, min(idx2[1], (int_t)(size2() - 1)));
  idx1[0] = idx1[1] - 1;
  idx2[0] = idx2[1] - 1;

  //printf("Perform the linear interpolation on val1=%.5f (%d %d) and val2=%.5f (%d %d)\n", 
  //        val1, idx1[0], idx1[1], val2, idx2[0], idx2[1]);

  if(size1() == 1) {                                        // 1 x N array (N>=2)
    return linear_polation(
      val2, 
      _indices2[idx2[0]], 
      _indices2[idx2[1]], 
      _table[0][idx2[0]], 
      _table[0][idx2[1]]
    );
  }
  else if(size2() == 1) {                                   // N x 1 array (N>=2)
    return linear_polation(
      val1, 
      _indices1[idx1[0]], 
      _indices1[idx1[1]], 
      _table[idx1[0]][0], 
      _table[idx1[1]][0]
    );
  }
  else {                                                    // N x N array (N>=2)
    float_t numeric[2];
    
    /*numeric[0] = linear_polation(
      val2, 
      _indices2[idx2[0]], 
      _indices2[idx2[1]], 
      _table[idx1[0]][idx2[0]], 
      _table[idx1[0]][idx2[1]]
    );
    numeric[1] = linear_polation(
      val2,
       _indices2[idx2[0]], 
       _indices2[idx2[1]], 
       _table[idx1[1]][idx2[0]], 
       _table[idx1[1]][idx2[1]]
    );

    return linear_polation(val1, _indices1[idx1[0]], _indices1[idx1[1]], numeric[0], numeric[1]);*/

    //cout << _table[idx1[0]][idx2[0]] << " " << _table[idx1[1]][idx2[0]] << endl;
    //cout << _table[idx1[0]][idx2[1]] << " " << _table[idx1[1]][idx2[1]] << endl;

    numeric[0] = linear_polation(
      val1, 
      _indices1[idx1[0]], 
      _indices1[idx1[1]], 
      _table[idx1[0]][idx2[0]], 
      _table[idx1[1]][idx2[0]]
    );

    numeric[1] = linear_polation(
      val1, 
      _indices1[idx1[0]], 
      _indices1[idx1[1]], 
      _table[idx1[0]][idx2[1]], 
      _table[idx1[1]][idx2[1]]
    );

    return linear_polation(val2, _indices2[idx2[0]], _indices2[idx2[1]], numeric[0], numeric[1]);
  }
}

// Function: linear_polation
// Performs the linear inter/extra polation between a segment (x1, x2) which satisfies the 
// function f(x1) = y1 and f(x2) = y2. There are five cases: 1) x < x1, 2) x = x1,
// 3) x1 < x < x2, 4) x = x2, and 5) x > x2. For cases 1) and 5), extra-polation is needed.
// Cases 2) and 4) are boundary cases. Case 3) requires the inter-polation.
float_t TimingLUT::linear_polation(float_ct x, float_ct x1, float_ct x2, float_ct y1, float_ct y2) const {

  CHECK(x1 < x2) << "(x1, x2) = (" + to_string(x1) + ", " + to_string(x2) + ")";

  if(x >= OT_FLT_MAX || x <= OT_FLT_MIN) return x;

  float_t slope = (y2 - y1) / (x2 - x1);

  if(x < x1) return y1 - (x1 - x) * slope;                  // Extrapolation.
  else if(x > x2)  return y2 + (x - x2) * slope;            // Extrapolation.
  else if(x == x1) return y1;                               // Boundary case.
  else if(x == x2) return y2;                               // Boundary case.
  else return y1 + (x - x1) * slope;                        // Interpolation.
}



//-------------------------------------------------------------------------------------------------

// Constructor: Timing
Timing::Timing():
  _idx(OT_UNDEFINED_IDX),
  _timing_sense(UNDEFINED_TIMING_SENSE),
  _timing_type(TimingType::UNDEFINED),
  _cell_rise_uptr(new timing_lut_t()),
  _cell_fall_uptr(new timing_lut_t()),
  _rise_transition_uptr(new timing_lut_t()),
  _fall_transition_uptr(new timing_lut_t()),
  _rise_constraint_uptr(new timing_lut_t()),
  _fall_constraint_uptr(new timing_lut_t()) {

}

// Destructor: ~Timing
Timing::~Timing() {
}

// Operator: = (copy assignment)
Timing& Timing::operator = (const Timing& rhs) {
    
  _idx = rhs._idx;
  _from_cellpin_name = rhs._from_cellpin_name;
  _to_cellpin_name = rhs._to_cellpin_name;
  _timing_sense = rhs._timing_sense;
  _timing_type = rhs._timing_type;   
  cell_rise() = rhs.cell_rise();                              
  cell_fall() = rhs.cell_fall();
  rise_transition() = rhs.rise_transition();
  fall_transition() = rhs.fall_transition();
  rise_constraint() = rhs.rise_constraint();
  fall_constraint() = rhs.fall_constraint();

  return *this;
}

// Function: is_transition_defined
bool_t Timing::is_transition_defined(int irf, int orf) const {

  if(!is_input_transition_defined(irf)) return false;
  
  switch(timing_sense()) {
    case POSITIVE_UNATE:
      if(irf != orf) return false;
    break;

    case NEGATIVE_UNATE:
      if(irf == orf) return false;
    break;

    default:
    break;
  }

  return true;
}

// Function: is_input_transition_defined
bool_t Timing::is_input_transition_defined(int irf) const {
  if(is_rising_edge_triggered() && irf != RISE) return false;
  if(is_falling_edge_triggered() && irf != FALL) return false;
  return true;
}

// Function: is_input_transition_defined
bool_t Timing::is_input_transition_defined() const {
  return (is_falling_edge_triggered() || is_rising_edge_triggered());
}

// Function: is_constraint
bool_t Timing::is_constraint() const {
  return is_hold_constraint() || is_setup_constraint();
}

// Function: is_hold_constraint
bool_t Timing::is_hold_constraint() const {
  switch (timing_type()) {
    case TimingType::HOLD_RISING:
      return true;
    break;
    case TimingType::HOLD_FALLING:
      return true;
    break;
    default:
      return false;
    break;
  };
}

// Function: is_setup_constraint
bool_t Timing::is_setup_constraint() const {
  switch (timing_type()) {
    case TimingType::SETUP_RISING:
      return true;
    break;
    case TimingType::SETUP_FALLING:
      return true;
    break;
    default:
      return false;
    break;
  };
}

// Function: is_falling_edge_triggered
bool_t Timing::is_falling_edge_triggered() const {
  switch (timing_type()) {
    case TimingType::SETUP_FALLING:
      return true;
    break;

    case TimingType::HOLD_FALLING:
      return true;
    break;

    case TimingType::FALLING_EDGE:
      return true;
    break;

    default:
      return false;
    break;
  };
}

// Function: is_rising_edge_triggered
bool_t Timing::is_rising_edge_triggered() const {
  switch (timing_type()) {
    case TimingType::SETUP_RISING:
      return true;
    break;

    case TimingType::HOLD_RISING:
      return true;
    break;

    case TimingType::RISING_EDGE:
      return true;
    break;

    default:
      return false;
    break;
  };
}

// Function: timing_sense_name      
string_t Timing::timing_sense_name() const {

  switch(timing_sense()) {

    case NEGATIVE_UNATE:
      return "negative_unate";
    break;

    case POSITIVE_UNATE:
      return "positive_unate";
    break;

    case NON_UNATE:
      return "non_unate";
    break;

    default:
      return "undefined";
    break;
  }
}

// Function: timing_type_name
string_t Timing::timing_type_name() const { 

  switch (timing_type()) {

    case TimingType::COMBINATIONAL:
      return "combinational";
    break;

    case TimingType::RISING_EDGE:
      return "rising_edge";
    break;

    case TimingType::FALLING_EDGE:
      return "falling_edge";
    break;

    case TimingType::HOLD_RISING:
      return "hold_rising";
    break;

    case TimingType::HOLD_FALLING:
      return "hold_falling"; 
    break;

    case TimingType::SETUP_RISING:
      return "setup_rising";
    break;

    case TimingType::SETUP_FALLING:
      return "setup_falling";
    break;

    case TimingType::THREE_STATE_DISABLE:
      return "three_state_disable";
    break;

    case TimingType::THREE_STATE_ENABLE:
      return "three_state_enable";
    break;

    case TimingType::RECOVERY_RISING:
      return "recovery_rising";
    break;

    case TimingType::RECOVERY_FALLING:
      return "recovery_falling";
    break;

    case TimingType::CLEAR:
      return "clear";
    break;

    case TimingType::PRESET:
      return "preset";
    break;

    case TimingType::MIN_PULSE_WIDTH:
      return "min_pulse_width";
    break;

    default:
      return "undefined";
    break;
  }
}

// Procedure: write_cell_rise
void_t Timing::write_cell_rise(file_pt fptr) const {

  // Ignore the empty table.
  if(cell_rise_ptr()->empty()) return;

  // Write the lut name.
  fprintf(fptr, "        cell_rise (\"%s\") {\n", cell_rise_ptr()->template_name().c_str());

  // Write the lut content.
  cell_rise_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_cell_rise
void_t Timing::write_cell_rise(ostream& os) const {

  // Ignore the empty table.
  if(cell_rise_ptr()->empty()) return;

  // Write the lut name.
  os << "        cell_rise (\"" << cell_rise_ptr()->template_name() << "\") {\n"; 

  // Write the lut content.
  os << cell_rise();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write_cell_fall
void_t Timing::write_cell_fall(file_pt fptr) const {
  
  // Ignore the empty table.
  if(cell_fall_ptr()->empty()) return;

  // Write the lut name.
  fprintf(fptr, "        cell_fall (\"%s\") {\n", cell_fall_ptr()->template_name().c_str());

  // Write the lut content.
  cell_fall_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_cell_fall
void_t Timing::write_cell_fall(ostream& os) const {
  
  // Ignore the empty table.
  if(cell_fall_ptr()->empty()) return;

  // Write the lut name.
  os << "        cell_fall (\"" << cell_fall_ptr()->template_name() << "\") {\n";

  // Write the lut content.
  os << cell_fall();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write_rise_transion
void_t Timing::write_rise_transition(file_pt fptr) const {
   
  // Ignore the empty table.
  if(rise_transition_ptr()->empty()) return;
  
  // Write the lut name.
  fprintf(fptr, "        rise_transition (\"%s\") {\n", rise_transition_ptr()->template_name().c_str());

  // Write the lut content.
  rise_transition_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_rise_transion
void_t Timing::write_rise_transition(ostream& os) const {
   
  // Ignore the empty table.
  if(rise_transition_ptr()->empty()) return;
  
  // Write the lut name.
  os << "        rise_transition (\"" << rise_transition_ptr()->template_name() << "\") {\n";

  // Write the lut content.
  os << rise_transition();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write_fall_transition
void_t Timing::write_fall_transition(file_pt fptr) const {
  
  // Ignore the empty table.
  if(fall_transition_ptr()->empty()) return;
  
  // Write the lut name.
  fprintf(fptr, "        fall_transition (\"%s\") {\n", fall_transition_ptr()->template_name().c_str());

  // Write the lut content.
  fall_transition_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_fall_transion
void_t Timing::write_fall_transition(ostream& os) const {
   
  // Ignore the empty table.
  if(fall_transition().empty()) return;
  
  // Write the lut name.
  os << "        fall_transition (\"" << fall_transition_ptr()->template_name() << "\") {\n";

  // Write the lut content.
  os << fall_transition();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write_rise_constraint
void_t Timing::write_rise_constraint(file_pt fptr) const {

  // Ignore the empty table.
  if(rise_constraint().empty()) return;
  
  // Write the lut name.
  fprintf(fptr, "        rise_constraint (\"%s\") {\n", rise_constraint_ptr()->template_name().c_str());

  // Write the lut content.
  rise_constraint_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_rise_constraint
void_t Timing::write_rise_constraint(ostream& os) const {
   
  // Ignore the empty table.
  if(rise_constraint().empty()) return;
  
  // Write the lut name.
  os << "        rise_constraint (\"" << rise_constraint_ptr()->template_name() << "\") {\n";

  // Write the lut content.
  os << rise_constraint();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write_fall_constraint
void_t Timing::write_fall_constraint(file_pt fptr) const {
  
  // Ignore the empty table.
  if(fall_constraint().empty()) return;
  
  // Write the lut name.
  fprintf(fptr, "        fall_constraint (\"%s\") {\n", fall_constraint_ptr()->template_name().c_str());

  // Write the lut content.
  fall_constraint_ptr()->write(fptr);

  // Write the ending group symbol.
  fprintf(fptr, "        }\n");
}

// Procedure: write_fall_constraint
void_t Timing::write_fall_constraint(ostream& os) const {
   
  // Ignore the empty table.
  if(fall_constraint().empty()) return;
  
  // Write the lut name.
  os << "        fall_constraint (\"" << fall_constraint_ptr()->template_name() << "\") {\n";

  // Write the lut content.
  os << fall_constraint();

  // Write the ending group symbol.
  os << "        }\n";
}

// Procedure: write
void_t Timing::write(file_pt fptr) const {
  
  // Write the timing.
  fprintf(fptr, "      timing () {\n");

  // Write the related pin (from cellpin).
  fprintf(fptr, "        related_pin : \"%s\";\n", from_cellpin_name().c_str());

  // Write the timing sense.
  fprintf(fptr, "        timing_sense : %s;\n", timing_sense_name().c_str());

  // Write the timing type.
  fprintf(fptr, "        timing_type : %s;\n", timing_type_name().c_str());

  // Write cell_fall
  write_cell_fall(fptr);
  
  // Write cell_rise
  write_cell_rise(fptr);
  
  // Write fall_transition
  write_fall_transition(fptr);
  
  // Write rise_transition
  write_rise_transition(fptr);
  
  // Write rise_constraint
  write_rise_constraint(fptr);
  
  // Write fall_constraint
  write_fall_constraint(fptr);
  
  // Write the ending group symbol.
  fprintf(fptr, "      }\n");
}

// Operator: <<
void_t Timing::write(ostream& os) const {
  
  // Write the timing.
  os << "      timing () {\n";

  // Write the related pin (from cellpin).
  os << "        related_pin : \"" << from_cellpin_name() << "\";\n";

  // Write the timing sense.
  os << "        timing_sense : " << timing_sense_name() << ";\n";

  // Write the timing type.
  os << "        timing_type : " << timing_type_name() << ";\n";

  // Write cell_fall
  write_cell_fall(os);
  
  // Write cell_rise
  write_cell_rise(os);
  
  // Write fall_transition
  write_fall_transition(os);
  
  // Write rise_transition
  write_rise_transition(os);
  
  // Write rise_constraint
  write_rise_constraint(os);
  
  // Write fall_constraint
  write_fall_constraint(os);
  
  // Write the ending group symbol.
  os << "      }\n";
}

//-------------------------------------------------------------------------------------------------

// Constructor: TimingArc
TimingArc::TimingArc(string_crt key):
  _from_cellpin_name(key)
{
  RF_RF_ITER(irf, orf) {
    _timing_ptr[irf][orf] = nullptr;
  }
}

// Destructor: ~TimingArc
TimingArc::~TimingArc() {
}

// Procedure: set_from_cellpin_name
void_t TimingArc::set_to_cellpin_name(string_crt name) {
  _to_cellpin_name = name;
}

// Function: is_positive_unate
bool_t TimingArc::is_positive_unate() const {
  if(timing_ptr(RISE, FALL) || timing_ptr(FALL, RISE)) return false;
  return true;
}

// Function: is_negative_unate
bool_t TimingArc::is_negative_unate() const {
  if(timing_ptr(RISE, RISE) || timing_ptr(FALL, FALL)) return false;
  return true;
}

// Function: is_non_unate
bool_t TimingArc::is_non_unate() const {
  if(!is_positive_unate() && !is_negative_unate()) return true;
  return false;
}

// Function: timing_sense
timing_sense_e TimingArc::timing_sense() const {
  if(is_positive_unate()) return POSITIVE_UNATE;
  if(is_negative_unate()) return NEGATIVE_UNATE;
  return NON_UNATE;
}

// Function: is_transition_defined
bool_t TimingArc::is_transition_defined(int irf, int orf) const {
  return timing_ptr(irf, orf) != nullptr;
}

// Function: is_constraint
bool_t TimingArc::is_constraint() const {
  RF_RF_ITER(irf, orf) {
    if(timing_ptr(irf, orf) && timing_ptr(irf, orf)->is_constraint()) return true;
  }
  return false;
}

// Function: is_falling_edge_triggered
bool_t TimingArc::is_falling_edge_triggered() const {
  RF_ITER(orf) {
    if(timing_ptr(FALL, orf) && timing_ptr(FALL, orf)->is_falling_edge_triggered()) return true;
  }
  return false;
}

// Function: is_rising_edge_triggered
bool_t TimingArc::is_rising_edge_triggered() const {
  RF_ITER(orf) {
    if(timing_ptr(RISE, orf) && timing_ptr(RISE, orf)->is_rising_edge_triggered()) return true;
  }
  return false;
}

// Function: delay
// Query the delay which is referenced by the output transition status, input slew, and driving 
// load. The output transition status indicates the type of lut that should be used during the
// linear interpolation or linear extrapolation.
float_t TimingArc::delay(int irf, int orf, float_ct slew, float_ct lcap) const {
  
  timing_lut_pt lut_ptr;

  CHECK(timing_ptr(irf, orf) != nullptr);

  switch(orf) {
    case RISE:
      lut_ptr = timing_ptr(irf, orf)->cell_rise_ptr();
    break;
    case FALL:
      lut_ptr = timing_ptr(irf, orf)->cell_fall_ptr();
    break;
    default:
      lut_ptr = nullptr;
    break;
  };
  
  CHECK(lut_ptr != nullptr);
  
  // Case 1: scalar.
  if(lut_ptr->lut_template_ptr() == nullptr) {     
    CHECK(lut_ptr->is_scalar() == true);
    return lut_ptr->table(0, 0);
  }

  // Case 2: non-scalar table.
  float_t val1, val2;
  
  // - obtain the input numerics
  switch(lut_ptr->lut_template_ptr()->variable1()) {
    case TOTAL_OUTPUT_NET_CAPACITANCE:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == INPUT_NET_TRANSITION);
      val1 = lcap;
      val2 = slew;
    break;
    case INPUT_NET_TRANSITION:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == TOTAL_OUTPUT_NET_CAPACITANCE);
      val1 = slew;
      val2 = lcap;
    break;
    default:
      CHECK(false);
      val1 = OT_UNDEFINED_FLT;
      val2 = OT_UNDEFINED_FLT;
    break;
  };
  
  // - perform the linear inter/extro-polation on indices1 and indices2
  return lut_ptr->lut_polation(val1, val2); 
}

// Function: slew
// Query the slew which is referenced by the output transition status, input slew, and driving 
// load. The output transition status indicates the type of lut that should be used during the
// linear interpolation or linear extrapolation.
float_t TimingArc::slew(int irf, int orf, float_ct slew, float_ct lcap) const {
  
  timing_lut_pt lut_ptr;

  CHECK(timing_ptr(irf, orf) != nullptr);

  // Obtain the pointer to the lut.
  switch(orf) {
    case RISE:
      lut_ptr = timing_ptr(irf, orf)->rise_transition_ptr();
    break;
    case FALL:
      lut_ptr = timing_ptr(irf, orf)->fall_transition_ptr();
    break;
    default:
      lut_ptr = nullptr;
    break;
  };

  CHECK(lut_ptr != nullptr);

  // Case 1: scalar.
  if(lut_ptr->lut_template_ptr() == nullptr) {     
    CHECK(lut_ptr->is_scalar() == true);
    return lut_ptr->table(0, 0);
  }

  // Case 2: non-scalar table.
  float_t val1, val2;
  
  // - obtain the input numerics
  switch(lut_ptr->lut_template_ptr()->variable1()) {
    case TOTAL_OUTPUT_NET_CAPACITANCE:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == INPUT_NET_TRANSITION);
      val1 = lcap;
      val2 = slew;
    break;
    case INPUT_NET_TRANSITION:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == TOTAL_OUTPUT_NET_CAPACITANCE);
      val1 = slew;
      val2 = lcap;
    break;
    default:
      CHECK(false);
      val1 = OT_UNDEFINED_FLT;
      val2 = OT_UNDEFINED_FLT;
    break;
  }
  
  // - perform the linear inter/extro-polation on indices1 and indices2
  return lut_ptr->lut_polation(val1, val2); 
}

// Function: constraint
// Query the constraint which is referenced by the output transition status, input slew, and
// output slew. The output transition status indicates the type of lut that should be used 
// during the linear interpolation or linear extrapolation.
float_t TimingArc::constraint(int irf, int orf, float_ct related_slew, float_ct constrained_slew) const {
  
  timing_lut_pt lut_ptr;
  
  CHECK(timing_ptr(irf, orf) != nullptr);

  // Obtain the pointer to the lut.
  switch(orf) {
    case RISE:
      lut_ptr = timing_ptr(irf, orf)->rise_constraint_ptr();
    break;
    case FALL:
      lut_ptr = timing_ptr(irf, orf)->fall_constraint_ptr();
    break;
    default:
      lut_ptr = nullptr;
    break;
  };

  CHECK(lut_ptr != nullptr);

  // Case 1: scalar.
  if(lut_ptr->lut_template_ptr() == nullptr) {     
    CHECK(lut_ptr->is_scalar() == true);
    return lut_ptr->table(0, 0);
  }

  // Case 2: non-scalar table.
  float_t val1, val2;
  
  // - obtain the input numerics
  switch(lut_ptr->lut_template_ptr()->variable1()) {
    case CONSTRAINED_PIN_TRANSITION:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == RELATED_PIN_TRANSITION);
      val1 = constrained_slew;
      val2 = related_slew;
    break;
    case RELATED_PIN_TRANSITION:
      CHECK(lut_ptr->lut_template_ptr()->variable2() == CONSTRAINED_PIN_TRANSITION);
      val1 = related_slew;
      val2 = constrained_slew;
    break;
    default:
      CHECK(false);
      val1 = OT_UNDEFINED_FLT;
      val2 = OT_UNDEFINED_FLT;
    break;
  };
  
  // - perform the linear inter/extro-polation on indices1 and indices2
  return lut_ptr->lut_polation(val1, val2); 
}



};  // End of OpenTimer namespace. ----------------------------------------------------------------



