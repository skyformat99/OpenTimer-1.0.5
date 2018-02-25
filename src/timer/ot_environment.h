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

#ifndef OT_ENVIRONMENT_H_
#define OT_ENVIRONMENT_H_

#include "ot_typedef.h"
#include "ot_macrodef.h"
#include "ot_utility.h"

namespace OpenTimer {

// Class: Environment
class Environment {
  
  public:

    Environment();                                          // Constructor.
    ~Environment();                                         // Destructor.
    
    void_t parse_environment_iccad2015(int, char **);       // Parse the environment from ICCAD15 contest.
    void_t parse_environment_tau2015(int, char **);         // Parse the environment from TAU2015 contest.
    void_t parse_environment_tau2016(int, char **);         // Parse the environment from TAU2016 contest.

    inline void_t set_rpt_fpath(string_crt);                // Set the report fpath.
    inline void_t set_lef_fpath(string_crt);                // Set the LEF fpath.
    inline void_t set_def_fpath(string_crt);                // Set the DEF fpath.
    inline void_t set_sdc_fpath(string_crt);                // Set the SDC fpath.
    inline void_t set_verilog_fpath(string_crt);            // Set the verilog fpath.
    inline void_t set_spef_fpath(string_crt);               // Set the spef fpath.
    inline void_t set_timing_fpath(string_crt);             // Set the timing fpath.
    inline void_t set_early_celllib_fpath(string_crt);      // Set the early celllib fpath.
    inline void_t set_late_celllib_fpath(string_crt);       // Set the late celllib fpath.
    inline void_t set_num_threads(int_ct);                  // Set the number of threads.
    inline void_t set_tau2016_macro_name(string_crt);       // Set the abs macro name.

    inline int num_threads() const;                         // Query the number of threads.

    inline float_t slack_cutoff() const;                    // Query the cutoff slack.

    inline string_crt tau2015_wrapper_fpath() const;        // Query the tau2015_wrapper file path.
    inline string_crt tau2015_wrapper_fdir() const;         // Query the tau2015_wrapper file dir.
    inline string_crt timing_fpath() const;                 // Query the timing file path.
    inline string_crt ops_fpath() const;                    // Query the ops file path.
    inline string_crt early_celllib_fpath() const;          // Query the early library file path.
    inline string_crt late_celllib_fpath() const;           // Query the late library file path.
    inline string_crt spef_fpath() const;                   // Query the spef file path.
    inline string_crt verilog_fpath() const;                // Query the verilog file path.
    inline string_crt sdc_fpath() const;                    // Query the sdf file path.
    inline string_crt lef_fpath() const;                    // Query the lef file path.
    inline string_crt def_fpath() const;                    // Query the def file path.
    inline string_crt tau2016_macro_name() const;           // Query the abs file path.

    inline file_pt rpt_fptr();                              // Query the report file ptr.

  private:

    string_vt _argv;                                        // Argument vector.

    float_t _slack_cutoff;                                  // Cutoff slack.
  
    string_t _tau2015_wrapper_fpath;                        // TAU15 wrapper file path.
    string_t _tau2015_wrapper_fdir;                         // TAU15 wrapper file dir.
    string_t _tau2016_wrapper_fpath;                        // TAU16 wrapper file path.
    string_t _tau2016_wrapper_fdir;                         // TAU16 wrapper file dir.
    string_t _tau2016_macro_name;                           // TAU16 model name.
    string_t _timing_fpath;                                 // Timing file path.
    string_t _sdc_fpath;                                    // SDC file path.
    string_t _ops_fpath;                                    // Operation file path.
    string_t _early_celllib_fpath;                          // Early celllib file path.
    string_t _late_celllib_fpath;                           // Late celllib file path.
    string_t _spef_fpath;                                   // Spef file path.
    string_t _verilog_fpath;                                // Verilog file path.
    string_t _lef_fpath;                                    // LEF file path.
    string_t _def_fpath;                                    // DEF file path.

    file_pt _rpt_fptr;                                      // Report file ptr.
};

// Procedure: set_lef_fpath
inline void_t Environment::set_lef_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _lef_fpath = fpath;
}

// Procedure: set_def_fpath
inline void_t Environment::set_def_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _def_fpath = fpath;
}

// Procedure: set_verilog_fpath
inline void_t Environment::set_verilog_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _verilog_fpath = fpath;
}

// Procedure: set_spef_fpath
inline void_t Environment::set_spef_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _spef_fpath = fpath;
}

// Procedure: set_early_celllib_fpath
inline void_t Environment::set_early_celllib_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _early_celllib_fpath = fpath;
}

// Procedure: set_verilog_fpath
inline void_t Environment::set_late_celllib_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _late_celllib_fpath = fpath;
}

// Procedure: set_timing_fpath
inline void_t Environment::set_timing_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _timing_fpath = fpath;
}

// Procedure: set_sdc_fpath
inline void_t Environment::set_sdc_fpath(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _sdc_fpath = fpath;
}

// Procedure: set_num_threads
inline void_t Environment::set_num_threads(int_ct n) {
  LOG(INFO) << string_t("Setting ") + to_string(n) << " thread(s)";
  omp_set_num_threads(min(n, OT_MAX_NUM_THREADS));
}

// Procedure: set_tau2016_macro_name
inline void_t Environment::set_tau2016_macro_name(string_crt fpath) {
  LOG(INFO) << string_t("Setting ") + Utility::realpath(fpath);
  _tau2016_macro_name = fpath;
}

// Function: tau2015_wrapper_fpath
// Return the file path of the warpper.
inline string_crt Environment::tau2015_wrapper_fpath() const {
  return _tau2015_wrapper_fpath;
}

// Function: tau2015_wrapper_fdir
// Return the file path of the warpper.
inline string_crt Environment::tau2015_wrapper_fdir() const {
  return _tau2015_wrapper_fdir;
}

// Function: timing_fpath
// Return the timing file path.
inline string_crt Environment::timing_fpath() const {
  return _timing_fpath;
}

// Function: ops_fpath
// Return the operation file path.
inline string_crt Environment::ops_fpath() const {
  return _ops_fpath;
}

// Function: sdc_fpath
// Return the sdc file path.
inline string_crt Environment::sdc_fpath() const {
  return _sdc_fpath;
}

// Function: lef_fpath
// Return the lef file path.
inline string_crt Environment::lef_fpath() const {
  return _lef_fpath;
}

// Function: def_fpath
// Return the def file path.
inline string_crt Environment::def_fpath() const {
  return _def_fpath;
}

// Function: early_celllib_fpath
// Return the early celllib file path.
inline string_crt Environment::early_celllib_fpath() const {
  return _early_celllib_fpath;
}

// Function: late_celllib_fpath
// Return the late celllib file path.
inline string_crt Environment::late_celllib_fpath() const {
  return _late_celllib_fpath;
}

// Function: spef_fpath
// Return the spef file path.
inline string_crt Environment::spef_fpath() const {
  return _spef_fpath;
}

// Function: verilog_fpath
// Return the verilog file path.
inline string_crt Environment::verilog_fpath() const {
  return _verilog_fpath;
}

// Function: tau2016_macro_name
// Return the abs file path.
inline string_crt Environment::tau2016_macro_name() const {
  return _tau2016_macro_name;
}

// Function: num_threads
// Return the number of threads.
inline int Environment::num_threads() const {
  return omp_get_num_threads();
}

// Function: slack_cutoff
// Return the slack cutoff value.
inline float_t Environment::slack_cutoff() const {
  return _slack_cutoff;
}

// Function: rpt_fptr
inline file_pt Environment::rpt_fptr() {
  return _rpt_fptr;
}

// Procedure: set_rpt_fpath
inline void_t Environment::set_rpt_fpath(string_crt fpath) {

  if(_rpt_fptr != stdout) {
    fclose(_rpt_fptr);
  }
  _rpt_fptr = fopen(fpath.c_str(), "w");

  // if fail to open, direct it to stdout.
  if(_rpt_fptr == nullptr) {
    LOG(WARNING) << string_t("Failed to open ") + Utility::realpath(fpath) + " (redirect to stdout)";
    _rpt_fptr = stdout;
  }
  else {
    LOG(INFO) << string_t("Setting report file to ") + Utility::realpath(fpath);
  }
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif

