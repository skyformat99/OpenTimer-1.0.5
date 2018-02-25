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

#include "ot_utility.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Function: __make_explorer
// Create an explorer object.
explorer_t __make_explorer(int rf, node_pt node_ptr) {
  explorer_t u;
  u.first = rf;
  u.second = node_ptr;
  return u;
}

// Function: __make_explorer
// Produce an explorer object.
void_t __make_explorer(explorer_rt u, int rf, node_pt node_ptr) {
  u.first = rf;
  u.second = node_ptr;
}

/*// Function: __to_numeric
// Convert a string to numeric value.
float_t __to_numeric(string_crt str) {
  std::stringstream stream_val(str);
  float_t tmp;
  stream_val >> tmp;
  if(!stream_val || stream_val.rdbuf()->in_avail()) {
    return OT_UNDEFINED_FLT;
  }
  return tmp; 
}*/

//-------------------------------------------------------------------------------------------------

// Function: __is_file_available
// Return true if the given file exists.
bool_t __is_file_available(char_cpt fpath) {
  struct stat buffer;
  return (stat(fpath, &buffer) == 0);
}

// Function: __is_file_available
// Return true if the given file exists.
bool_t __is_file_available(string_crt fpath) {
  return __is_file_available(fpath.c_str());
}

// Function: __max_fileline_size
size_t __max_fileline_size(string_crt fpath) {
  return __max_fileline_size(fpath.c_str());
}

// Function: __max_fileline_size
// Return the maximum line length of the file.
size_t __max_fileline_size(char_cpt fpath) {
  FILE *rfp = fopen(fpath, "rb");
  if(rfp == nullptr) {
    fprintf(stderr, "Error: cannot open the file\n");
    exit(1);
  }
  register char c;
  register size_t size = 0;
  register size_t max_size = 0;
  while(!feof(rfp)) {
    size = 0;
    while( (c=fgetc(rfp))!=EOF && c!='\n') size++;
    max_size = max(max_size, size);
  }
  fclose(rfp);
  return max_size;
}

// Function: __wfptr
// Return the file pointer for writing mode.
file_pt __wfptr(char_cpt fpath) {
  if(fpath == nullptr || strlen(fpath) == 0) return stdout;
  return fopen(fpath, "w");
}

// Function: __wfptr
file_pt __wfptr(string_crt fpath) {
  return __wfptr(fpath.c_str());
}

// Function: __rfptr
// Return the file pointer for reading mode.
file_pt __rfptr(char_cpt fpath) {
  if(fpath == nullptr || strlen(fpath) == 0) return stdout;
  return fopen(fpath, "r");
}

// Function: __rfptr
file_pt __rfptr(string_crt fpath) {
  return __rfptr(fpath.c_str());
}

//-------------------------------------------------------------------------------------------------

int_t __thread_id() {
  return omp_get_thread_num();
}

//-------------------------------------------------------------------------------------------------

string_t __time_info() {
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char_t buffer[100];
  strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", timeinfo);
  return buffer;
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------




