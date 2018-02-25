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

#ifndef OT_TOKENIZER_H_
#define OT_TOKENIZER_H_

#include "ot_macrodef.h"
#include "ot_utility.h"
#include "ot_enumdef.h"
#include "ot_typedef.h"

namespace OpenTimer {

// Class: FileReader
class FileReader {

  public:
    
    FileReader();                                                      // Constructor.
    ~FileReader();                                                     // Destructor.

    void_t read(char_cpt, char_pt, int_t, char_pt, int_t);            // Read a file.

    string_t next_token();                                            // Query the next token.

    inline char_pt cursor() const;                                    // Query the cursor.
    inline char_pt buffer_beg() const;                                // Query the ptr to the buffer beg.
    inline char_pt buffer_end() const;                                // Query the ptr to the buffer end.

    inline void_t set_cursor(char_pt);                                // Set the cursor.
    
  private:

    int _num_separators;                                              // Number of skipped chars.
    int _num_delimiters;                                              // Number of delimiters.

    char_pt _separators;                                              // Separators.
    char_pt _delimiters;                                              // Delimiters.
    char_pt _buffer_end;                                              // End pointer of the buffer.
    char_pt _cursor;                                                  // Cursor.

    unique_ptr <char_t[]> _buffer_beg;                                // Unique pointer of the buffer.
    
    bool_t _is_delimiter(char_ct);                                    // Query the char status.
    bool_t _is_separator(char_ct);                                    // Query the char status.

    void_t _read_file_to_buffer(char_cpt);                            // Read the file to the buffer.
};

// Function: cursor
inline char_pt FileReader::cursor() const {
  return _cursor;
}

// Function: buffer_beg
inline char_pt FileReader::buffer_beg() const {
  return _buffer_beg.get();
}

// Function: buffer_end
inline char_pt FileReader::buffer_end() const {
  return _buffer_end;
}

// Function: set_cursor
inline void_t FileReader::set_cursor(char_pt cursor) {
  _cursor = cursor;
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------


#endif
