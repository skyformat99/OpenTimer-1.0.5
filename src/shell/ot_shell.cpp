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

#include "ot_shell.h"
#include <wordexp.h>

namespace OpenTimer {

// Static member: _fptr
file_pt Shell::_fptr = stdout;

// Static member: _reactor_dict
shell_reactor_dict_t Shell::_reactor_dict;
  
// Static member: _info
string_ct Shell::_info = string_t("\
  ____              _______              \n\
 / __ \\___  ___ ___/_  __(_)_ _  ___ ____\n\
/ /_/ / _ \\/ -_) _ \\/ / / /  ' \\/ -_) __/\n\
\\____/ .__/\\__/_//_/_/ /_/_/_/_/\\__/_/       version " + string_t(PACKAGE_VERSION) + "\n\
    /_/                                     \n")                                      +
  "Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,\n"                      +
  "University of Illinois at Urbana-Champaign (UIUC), IL, USA.\n"                     +
  "All rights reserved.\n"                                                            +
  "This program is free software: you can redistribute it and/or modify\n"            +
  "it under the terms of the GNU General Public License as published by\n"            +
  "the Free Software Foundation, either version 3 of the License, or\n"               +
  "(at your option) any later version.\n"                                             +
  "For bug reporting instructions and manual for OpenTimer, please see:\n"            +
  string_t(PACKAGE_URL) + "\n"                                                        +
  "For help, type \"help\".\n"                                                        ;

// Static member: _help
string_ct Shell::_help = string_t("\
List of classes of commands:\n\n\
design-initiate -- commands that initiate the design\n\
design-modifier -- commands that modify the design topology\n\
design-report   -- commands that generate the design report\n\
design-exec     -- commands that execute the design operation\n\n\
Type \"help\" followed by a class name for a list of commands in that class.\n\
Type \"help\" followed by command name for full documentation.\n");

// Static member: _help_design_initiate
string_ct Shell::_help_design_initiate = string_t("\
List of design-initiate commands:\n\n\
set_early_celllib_fpath <file_path.lib>\n\
set_late_celllib_fpath <file_path.lib>\n\
set_verilog_fpath <file_path.v>\n\
set_spef_fpath <file_path.spef>\n\
set_timing_fpath <file_path.timing>\n\
It is the user's responsibility to make each command legal.\n\
Illegal commands might cause unexpected program errors.\n");

// Static member: _help_design_report
string_ct Shell::_help_design_report = string_t("\
List of design-report commands:\n\n\
report_worst_paths -numPaths <integer> [-pin <pin_name>]\n\
report_at -pin <pin_name> -early/-late -rise/-fall\n\
report_rat -pin <pin_name> -early/-late -rise/-fall\n\
report_slack -pin <pin_name> -early/-late -rise/-fall\n\
report_slew -pin <pin_name> -early/-late -rise/-fall\n\
report_tns\n\
report_wns\n\
report_timer\n\n\
It is the user's responsibility to make each command legal.\n\
Illegal commands might cause unexpected program errors.\n");

// Static member: _help_design_modifier
string_ct Shell::_help_design_modifier = string_t("\
List of design-modifier commands:\n\n\
insert_gate <gate_name> <cell_name>\n\
remove_gate <gate_name>\n\
repower_gate <gate_name> <cell_name>\n\
insert_net <net_name>\n\
remove_net <net_name>\n\
disconnect_pin <pin_name>\n\
connect_pin <pin_name> <net_name>\n\n\
read_spef <file_path.spef>\n\
set_at -pin <pi_name> -early/-late -rise/-fall <value>\n\
set_rat -pin <po_name> -early/-late -rise/-fall <value>\n\
set_load -pin <po_name> <value>\n\
set_slew -pin <pi_name> -early/-late -rise/-fall <value>\n\
It is the user's responsibility to make each command legal.\n\
Illegal commands might cause unexpected program errors.\n");

// Static member: _help_design_exec
string_ct Shell::_help_design_exec = string_t("\
List of design-exec commands:\n\n\
init_timer\n\n\
exec_ops <file_path.ops>\n\
It is the user's responsibility to make each command legal.\n\
Illegal commands might cause unexpected program errors.\n");

// Function: _is_argc_valid
bool_t Shell::_is_argc_valid(int argc, const int target_argc) {
  if(argc == target_argc) return true;
  return false;
}

// Procedure: _feed_help
void_t Shell::_feed_help(int argc, char **argv, timer_pt timer) {
  if(argc == 1) {
    fprintf(fptr(), "%s", help().c_str());
  }
  else if(argc == 2) {
    if(strcmp(argv[1], "design-report") == 0) {
      fprintf(fptr(), "%s", help_design_report().c_str()); 
    }
    else if(strcmp(argv[1], "design-modifier") == 0) {
      fprintf(fptr(), "%s", help_design_modifier().c_str());
    }
    else if(strcmp(argv[1], "design-initiate") == 0) {
      fprintf(fptr(), "%s", help_design_initiate().c_str());
    }
    else if(strcmp(argv[1], "design-exec") == 0) {
      fprintf(fptr(), "%s", help_design_exec().c_str());
    }
  }
}

// Procedure: _feed_set_num_threads
void_t Shell::_feed_set_num_threads(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_num_threads(atoi(argv[1]));
  }
}

// Procedure: _feed_set_lef_fpath
void_t Shell::_feed_set_lef_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_lef_fpath(argv[1]);
  }
}

// Procedure: _feed_set_def_fpath
void_t Shell::_feed_set_def_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_def_fpath(argv[1]);
  }
}

// Procedure: _feed_set_verilog_fpath
void_t Shell::_feed_set_verilog_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_verilog_fpath(argv[1]);
  }
}

// Procedure: _feed_set_spef_fpath
void_t Shell::_feed_set_spef_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_spef_fpath(argv[1]);
  }
}

// Procedure: _feed_set_early_celllib_fpath
void_t Shell::_feed_set_early_celllib_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_early_celllib_fpath(argv[1]);
  }
}

// Procedure: _feed_set_late_celllib_fpath
void_t Shell::_feed_set_late_celllib_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_late_celllib_fpath(argv[1]);
  }
}

// Procedure: _feed_set_timing_fpath
void_t Shell::_feed_set_timing_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_timing_fpath(argv[1]);
  }
}

// Procedure: _feed_set_sdc_fpath
void_t Shell::_feed_set_sdc_fpath(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->set_sdc_fpath(argv[1]);
  }
}

// Procedure: _feed_init_timer
void_t Shell::_feed_init_timer(int argc, char **argv, timer_pt timer) {
  timer->init_timer();
}

// Procedure: _feed_report_timer
void_t Shell::_feed_report_timer(int argc, char **argv, timer_pt timer) {
  timer->report_timer();
}

// Procedure: _feed_report_max_num_threads
void_t Shell::_feed_report_max_num_threads(int argc, char **argv, timer_pt timer) {
  timer->report_max_num_threads();
}

// Procedure: _feed_exec_ops
void_t Shell::_feed_exec_ops(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->exec_ops(argv[1]);
  }
}

// Procedure: _feed_report_worst_paths
void_t Shell::_feed_report_worst_paths(int argc, char **argv, timer_pt timer) {
  timer->report_worst_paths(argc, argv);
}

// Procedure: _feed_report_tns
void_t Shell::_feed_report_tns(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 1)) {
    timer->report_tns();
  }
}

// Procedure: _feed_report_wns
void_t Shell::_feed_report_wns(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 1)) {
    timer->report_wns();
  }
}

// Procedure: _feed_report_at
void_t Shell::_feed_report_at(int argc, char **argv, timer_pt timer) {
  timer->report_at(argc, argv);
}

// Procedure: _feed_report_rat
void_t Shell::_feed_report_rat(int argc, char **argv, timer_pt timer) {
  timer->report_rat(argc, argv);
}

// Procedure: _feed_report_slack
void_t Shell::_feed_report_slack(int argc, char **argv, timer_pt timer) {
  timer->report_slack(argc, argv);
}

// Procedure: _feed_report_slew
void_t Shell::_feed_report_slew(int argc, char **argv, timer_pt timer) {
  timer->report_slew(argc, argv);
}
    
// Procdure: _feed_report_all_slacks
void_t Shell::_feed_report_all_slacks(int argc, char **argv, timer_pt timer) {
  timer->report_all_slacks();
}

// Procedure: _feed_insert_gate
void_t Shell::_feed_insert_gate(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 3)) {
    timer->insert_gate(argv[1], argv[2]);
  }
}

// Procedure: _feed_repower_gate
void_t Shell::_feed_repower_gate(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 3)) {
    timer->repower_gate(argv[1], argv[2]);
  }
}

// Procedure: _feed_remove_gate
void_t Shell::_feed_remove_gate(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->remove_gate(argv[1]);
  }
}

// Procedure: _feed_insert_net
void_t Shell::_feed_insert_net(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->insert_net(argv[1]);
  }
}

// Procedure: _feed_read_spef
void_t Shell::_feed_read_spef(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
     timer->read_spef(argv[1]);
  }
}

// Procedure: _feed_remove_net
void_t Shell::_feed_remove_net(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->remove_net(argv[1]);
  }
}

// Procedure: _feed_connect_pin
void_t Shell::_feed_connect_pin(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 3)) {
    timer->connect_pin(argv[1], argv[2]);
  }
}

// Procedure: _feed_disconnect_pin
void_t Shell::_feed_disconnect_pin(int argc, char **argv, timer_pt timer) {
  if(_is_argc_valid(argc, 2)) {
    timer->disconnect_pin(argv[1]);
  }
}

// Procedure: _feed_set_at
void_t Shell::_feed_set_at(int argc, char **argv, timer_pt timer) {
  timer->set_at(argc, argv);
}

// Procedure: _feed_set_rat
void_t Shell::_feed_set_rat(int argc, char **argv, timer_pt timer) {
  timer->set_rat(argc, argv);
}

// Procedure: _feed_set_slew
void_t Shell::_feed_set_slew(int argc, char **argv, timer_pt timer) {
  timer->set_slew(argc, argv);
}

// Procedure: _feed_set_load
void_t Shell::_feed_set_load(int argc, char **argv, timer_pt timer) {
  timer->set_load(argc, argv);
}

// Procedure: _feed_set_report_fpath
void_t Shell::_feed_set_report_fpath(int argc, char **argv, timer_pt timer) {
  timer->set_rpt_fpath(argv[1]);
}

// Procedure: _initiate_reactor_dict
void_t Shell::_initiate_reactor_dict() {
  
  if(_reactor_dict.size() != 0) return;

  _reactor_dict["help"] = _feed_help;
  _reactor_dict["set_report_fpath"] = _feed_set_report_fpath;
  _reactor_dict["set_lef_fpath"] = _feed_set_lef_fpath;
  _reactor_dict["set_def_fpath"] = _feed_set_def_fpath;
  _reactor_dict["set_sdc_fpath"] = _feed_set_sdc_fpath;
  _reactor_dict["set_verilog_fpath"] = _feed_set_verilog_fpath;
  _reactor_dict["set_spef_fpath"] = _feed_set_spef_fpath;
  _reactor_dict["set_early_celllib_fpath"] = _feed_set_early_celllib_fpath;
  _reactor_dict["set_late_celllib_fpath"] = _feed_set_late_celllib_fpath;
  _reactor_dict["set_timing_fpath"] = _feed_set_timing_fpath;
  _reactor_dict["set_num_threads"] = _feed_set_num_threads;
  _reactor_dict["set_load"] = _feed_set_load;
  _reactor_dict["set_at"] = _feed_set_at;
  _reactor_dict["set_slew"] = _feed_set_slew;
  _reactor_dict["set_rat"] = _feed_set_rat;
  _reactor_dict["init_timer"] = _feed_init_timer;
  _reactor_dict["exec_ops"] = _feed_exec_ops;
  _reactor_dict["report_worst_paths"] = _feed_report_worst_paths;
  _reactor_dict["report_tns"] = _feed_report_tns;
  _reactor_dict["report_wns"] = _feed_report_wns;
  _reactor_dict["report_at"] = _feed_report_at;
  _reactor_dict["report_rat"] = _feed_report_rat;
  _reactor_dict["report_slew"] = _feed_report_slew;
  _reactor_dict["report_slack"] = _feed_report_slack;
  _reactor_dict["report_all_slacks"] = _feed_report_all_slacks;
  _reactor_dict["report_timer"] = _feed_report_timer;
  _reactor_dict["report_max_num_threads"] = _feed_report_max_num_threads;
  _reactor_dict["insert_gate"] = _feed_insert_gate;
  _reactor_dict["repower_gate"] = _feed_repower_gate;
  _reactor_dict["remove_gate"] = _feed_remove_gate;
  _reactor_dict["read_spef"] = _feed_read_spef;
  _reactor_dict["insert_net"] = _feed_insert_net;
  _reactor_dict["remove_net"] = _feed_remove_net;
  _reactor_dict["connect_pin"] = _feed_connect_pin;
  _reactor_dict["disconnect_pin"] = _feed_disconnect_pin;
}

// Procedure: _feed_command
// The major procedure that handles the command for the timer.
bool_t Shell::_feed_command(timer_pt timer) {

  wordexp_t p;

  char cmd[OT_COMMAND_MAX_LINE_SIZE];
  char *keyword;
  bool keep_going(true);

  // Interactive mode.
  fprintf(fptr(), "OpenTimer> ");
  if(!fgets(cmd, OT_COMMAND_MAX_LINE_SIZE, stdin)) {
    return keep_going = false;
  } 

  fprintf(fptr(), "%s", cmd);

  // Parse the keyword.
  cmd[strlen(cmd) - 1] = '\0';

  // Parser the command into argc/argv pair.
  wordexp(cmd, &p, 0);
  keyword = (p.we_wordc == 0) ? nullptr : p.we_wordv[0];

  // Handle the keyword and invoke the corresponding shell functions.
  if(keyword == nullptr) {
  }
  else if(strcmp(keyword, "quit") == 0 || strcmp(keyword, "exit") == 0) {
    keep_going = false;
  }
  else {
    if(_reactor_dict.find(keyword) != _reactor_dict.end()) {
      _reactor_dict[keyword](p.we_wordc, p.we_wordv, timer);
    }
    else {
      LOG(WARNING) << string_t("Undefined command keyword \"") + keyword + "\"";
    }
  }
  
  wordfree(&p);

  return keep_going;
}

// Procedure: run
// The procedure runs OpenTimer in interactive mode. 
void_t Shell::run() {

  // Display the shell info.
  fprintf(fptr(), "%s", info().c_str());

  // Register the commands for the shell.
  _initiate_reactor_dict();

  // Create a timer object and goes into the interactive shell.
  Timer timer;

  while(_feed_command(&timer));
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------


