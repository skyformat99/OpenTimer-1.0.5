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

#include "ot_freader.h"

namespace OpenTimer {

// Constructor.
FileReader::FileReader():
  _num_separators(0),
  _num_delimiters(0),
  _separators(nullptr),
  _delimiters(nullptr),
  _buffer_end(nullptr),
  _cursor(nullptr)
{
}

// Destructor.
FileReader::~FileReader() {
}

// Procedure: open
void_t FileReader::read(char_cpt fpath, char_pt separators, int_t ns, char_pt delimiters, int_t nd) {
  
  _separators = separators;
  _delimiters = delimiters;
  _num_separators = ns;
  _num_delimiters = nd; 

  _read_file_to_buffer(fpath);
}

// Function: _is_delimiter
// Delimiter is defined as a special character token to separate the regular token.
bool_t FileReader::_is_delimiter(char_ct c) {
  for(int_t i=0; i<_num_delimiters; i++) {
    if(_delimiters[i] == c) return true;
  }
  return false;
}

// Function: _is_separator
// Separator is defined as the character that separates the token and can be skipped.
bool_t FileReader::_is_separator(char_ct c) {
  for(int_t i=0; i<_num_separators; i++) {
    if(_separators[i] == c) return true;
  }
  return false;
}

// Procedure: _read_file_to_buffer
// Read the file to the buffer.
void_t FileReader::_read_file_to_buffer(char_cpt fpath) {

  size_t file_size;
  ifstream_t ifs(fpath);                                    // Open the file.
  
  LOG_IF(FATAL, !ifs.good()) << string_t("Failed to open ") + fpath;

  ifs.seekg(0, std::ios::end);                              
  file_size = ifs.tellg();                                  
  ifs.seekg(0, std::ios::beg);                             
  _buffer_beg.reset(new char_t [file_size + 1]);            
  _buffer_end = _buffer_beg.get() + file_size;              
  ifs.read(_buffer_beg.get(), file_size);                 
  _buffer_beg.get()[file_size] = '\0';                      
  ifs.close();

  _cursor = _buffer_beg.get();
}

// Function: next_token
// Get the next token from the current cursor pointer to the buffer position.
string_t FileReader::next_token() {

  string_t token;

  for(; _cursor < _buffer_end; ++_cursor) {
    
    // Skip comment line.
    if(*_cursor == '/' && *(_cursor + 1) == '/') {
      for(; *_cursor != '\n'; ++_cursor) ;
    }
    
    // Skip comment block.
    if(*_cursor == '/' && *(_cursor + 1) == '*') {
      for(_cursor = _cursor + 2; _cursor < _buffer_end; _cursor++) {
        if(*_cursor == '*' && *(_cursor + 1) == '/') {
          _cursor = _cursor + 2;
          break;
        }
      }
    }

    // Separator.
    if(_is_separator(*_cursor)) {
      if(token.length() != 0) {
        ++_cursor;
        break;
      }
    } 
    // Delimiter.
    else if(_is_delimiter(*_cursor)) {
      if(token.length() == 0) {
        token.push_back(*_cursor);
        _cursor++;
      }
      break;
    }
    // Token.
    else token.push_back(*_cursor);
  }
  return token; // _token_length == 0 ? nullptr : _token;
}



};  // End of OpenTimer namespace. ----------------------------------------------------------------





