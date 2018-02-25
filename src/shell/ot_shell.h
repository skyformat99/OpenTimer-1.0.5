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

#ifndef OT_SHELL_H_
#define OT_SHELL_H_

#include "ot_timer.h"
#include "ot_typedef.h"

namespace OpenTimer {

// Class: Shell
class Shell final {

  public:

    Shell() = delete;                                                           // Disable constructor.
    Shell(const Shell&) = delete;                                               // Disable constructor.
    ~Shell() = delete;                                                          // Disable constructor.      

    static void_t run();                                                        // Run interactive timer.
    
    inline static file_pt fptr();                                               // Query the fptr.
    
    inline static string_crt info();                                            // Query the info.
    inline static string_crt help();                                            // Query the help info.
    inline static string_crt help_design_report();                              // Query the help info.
    inline static string_crt help_design_exec();                                // Query the help info.
    inline static string_crt help_design_modifier();                            // Query the help info.
    inline static string_crt help_design_initiate();                            // Query the help info.

    Shell& operator = (const Shell&) = delete;                                  // Disable copy assignment.

  private:

    static file_pt _fptr;                                                       // Fptr.
    
    static shell_reactor_dict_t _reactor_dict;                                  // Reactor dictionary.

    static string_ct _info;                                                     // Info.
    static string_ct _help;                                                     // Help info.
    static string_ct _help_design_report;                                       // Help info.
    static string_ct _help_design_exec;                                         // Help info.
    static string_ct _help_design_modifier;                                     // Help info.
    static string_ct _help_design_initiate;                                     // Help info.

    static bool_t _feed_command(timer_pt);                                      // Feed command.
    static bool_t _is_argc_valid(int, const int);                               // Sanity check.

    static void_t _initiate_reactor_dict();                                     // Initiate the reactor dict.

    static void_t _feed_help(int, char**, timer_pt);                            // Command reactor.
    static void_t _feed_set_report_fpath(int, char**, timer_pt);                // Command reactor.
    static void_t _feed_set_at(int, char**, timer_pt);                          // Command reactor.
    static void_t _feed_set_slew(int, char**, timer_pt);                        // Command reactor.
    static void_t _feed_set_load(int, char**, timer_pt);                        // Command reactor.
    static void_t _feed_set_rat(int, char**, timer_pt);                         // Command reactor.
    static void_t _feed_set_num_threads(int, char**, timer_pt);                 // Command reactor.
    static void_t _feed_set_lef_fpath(int, char**, timer_pt);                   // Command reactor.
    static void_t _feed_set_def_fpath(int, char**, timer_pt);                   // Command reactor.
    static void_t _feed_set_verilog_fpath(int, char**, timer_pt);               // Command reactor.
    static void_t _feed_set_spef_fpath(int, char**, timer_pt);                  // Command reactor.
    static void_t _feed_set_early_celllib_fpath(int, char**, timer_pt);         // Command reactor.
    static void_t _feed_set_late_celllib_fpath(int, char**, timer_pt);          // Command reactor.
    static void_t _feed_set_timing_fpath(int, char**, timer_pt);                // Command reactor.
    static void_t _feed_set_sdc_fpath(int, char**, timer_pt);                   // Command reactor.
    static void_t _feed_init_timer(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_report_timer(int, char**, timer_pt);                    // Command reactor.
    static void_t _feed_report_max_num_threads(int, char**, timer_pt);          // Command reactor.
    static void_t _feed_exec_ops(int, char**, timer_pt);                        // Command reactor.
    static void_t _feed_report_worst_paths(int, char**, timer_pt);              // Command reactor.
    static void_t _feed_report_tns(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_report_wns(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_report_at(int, char**, timer_pt);                       // Command reactor.
    static void_t _feed_report_rat(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_report_slack(int, char**, timer_pt);                    // Command reactor.
    static void_t _feed_report_slew(int, char**, timer_pt);                     // Command reactor.
    static void_t _feed_report_all_slacks(int, char**, timer_pt);               // Command reactor.
    static void_t _feed_insert_gate(int, char**, timer_pt);                     // Command reactor.
    static void_t _feed_repower_gate(int, char**, timer_pt);                    // Command reactor.
    static void_t _feed_remove_gate(int, char**, timer_pt);                     // Command reactor.
    static void_t _feed_insert_net(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_read_spef(int, char**, timer_pt);                       // Command reactor.
    static void_t _feed_remove_net(int, char**, timer_pt);                      // Command reactor.
    static void_t _feed_connect_pin(int, char**, timer_pt);                     // Command reactor.
    static void_t _feed_disconnect_pin(int, char**, timer_pt);                  // Command reactor.

};

// Function: fptr
inline file_pt Shell::fptr() {
  return _fptr;
}

// Function: info
inline string_crt Shell::info() {
  return _info; 
}

// Function: help
inline string_crt Shell::help() {
  return _help;
}

// Function: help_design_report
inline string_crt Shell::help_design_report() {
  return _help_design_report;
}

// Function: help_design_exec
inline string_crt Shell::help_design_exec() {
  return _help_design_exec;
}

// Function: help_design_modifier
inline string_crt Shell::help_design_modifier() {
  return _help_design_modifier;
}

// Function: help_design_initiate
inline string_crt Shell::help_design_initiate() {
  return _help_design_initiate;
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------





#endif
