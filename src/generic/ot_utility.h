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

#ifndef OT_UTILITY_H_
#define OT_UTILITY_H_

#include "ot_headerdef.h"
#include "ot_macrodef.h"
#include "ot_typedef.h"

namespace OpenTimer {

// Integer pair struct.
struct IntPair {
  int_t first;
  int_t second;
};

//-------------------------------------------------------------------------------------------------

// Explorer struct.
struct Explorer {
  int first;
  node_pt second;
};

// Function: __make_explorer
explorer_t __make_explorer(int_t, node_pt);

// Procedure: __make_explorer
void_t __make_explorer(explorer_rt, int_t, node_pt);

//-------------------------------------------------------------------------------------------------

// Class: Utility
class Utility {

  public:

    inline static float_t to_numeric(string_crt);                     // Numeric conversion.

    static constexpr float_t undefined_at(int);                       // Query the undefined AT.
    static constexpr float_t undefined_rat(int);                      // Query the undefined RAT.
    static constexpr float_t undefined_slew(int);                     // Query the undefined slew.
    static constexpr float_t undefined_slack(int);                    // Query the undefined slack.
    
    inline static string_t realpath(string_crt);                      // Query the realpath.
    inline static string_t getcwd();                                  // Query the current directory.

    static constexpr bool_t is_finite_numeric(float_ct);              // Query the numeric status.
};

// Function: to_numeric
inline float_t Utility::to_numeric(string_crt str) {
  try {
    auto res = stof(str);
    return res;
  }
  catch (...) {
    return OT_UNDEFINED_FLT;
  }
}

// Function: undefined_at
constexpr float_t Utility::undefined_at(int el) {
  return el == EARLY ? OT_FLT_MAX : OT_FLT_MIN;
}

// Function: undefined_slew
constexpr float_t Utility::undefined_rat(int el) {
  return el == EARLY ? OT_FLT_MIN : OT_FLT_MAX;
}

// Function: undefined_slew
constexpr float_t Utility::undefined_slew(int el) {
  return el == EARLY ? OT_FLT_MAX : OT_FLT_MIN;
}

// Function: undefined_slack
constexpr float_t Utility::undefined_slack(int el) {
  return (el == EARLY) ? undefined_at(el) - undefined_rat(el) : undefined_rat(el) - undefined_at(el);
}

// Function: is_numeric_finite
constexpr bool_t Utility::is_finite_numeric(float_ct val) {
  return !(val >= OT_FLT_MAX || val <= OT_FLT_MIN);
}

// Static function: realpath
inline string_t Utility::realpath(string_crt path) {
  auto tmp = ::realpath(path.c_str(), nullptr);
  string_t resolved = tmp;
  free(tmp);
  return resolved;
}

// Static function: getcwd
inline string_t Utility::getcwd() {
  auto tmp = ::getcwd(nullptr, 0);
  string_t resolved = tmp;
  free(tmp);
  return resolved;
}

//-------------------------------------------------------------------------------------------------

// Guard the numeric value between max/min floating range.
inline void_t __numeric_guard(float_rt val) {
  val = min(OT_FLT_MAX, max(OT_FLT_MIN, val));
}

// Procedure: __tau2015_numeric_guard
// Guard the numeric value using the TAU2015 floating limit.
inline void_t __tau2015_numeric_guard(float_rt val) {
  val = min(TAU2015_FLT_MAX, max(TAU2015_FLT_MIN, val)); 
}

// Function: __is_valid_split
inline bool_t __is_valid_split(int el) {
  return (el == EARLY || el == LATE);
}

// Function: __is_valid_trans
inline bool_t __is_valid_trans(int rf) {
  return (rf == RISE || rf == FALL);
}

//-------------------------------------------------------------------------------------------------

// Function: __max_fileline_size.
size_t __max_fileline_size(string_crt);

// Function: max_filelinesize.
size_t __max_fileline_size(char_cpt);

// Function: __is_file_available
bool_t __is_file_available(char_cpt);

// Function: __is_file_available.
bool_t __is_file_available(string_crt);

// Function: __wfptr
file_pt __wfptr(char_cpt);

// Function: __wfptr
file_pt __wfptr(string_crt);

// Function: __rfptr
file_pt __rfptr(char_cpt);

// Function: __rfptr
file_pt __rfptr(string_crt);

//-------------------------------------------------------------------------------------------------

// Function: __time_info
string_t __time_info();

//-------------------------------------------------------------------------------------------------

// Function: __thread_id
int_t __thread_id();

//-------------------------------------------------------------------------------------------------

// Procedure: __clear_vector
template <class T>
inline void_t __clear_vector(vector <T> & vec) {
  vec.clear();
}

// Procedure: __clear_matrix
template <class T>
inline void_t __clear_matrix(vector < vector<T> > &mat) {
  for(unsigned_t i=0; i<mat.size(); ++i) {
    mat[i].clear();
  }
  mat.clear();
}




};  // End of OpenTimer namespace. ----------------------------------------------------------------



#endif


