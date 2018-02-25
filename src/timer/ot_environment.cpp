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

#include "ot_environment.h"

namespace OpenTimer {

// Constructor
Environment::Environment():
  _slack_cutoff(OT_FLT_ZERO),
  _rpt_fptr(OT_DEFAULT_RPT_FPTR)
{
}

// Destructor
Environment::~Environment() {
}


// Procedure: parse_environment_iccad2015
// The procedure parse the input environment in the form of tau2015 contest. 
void_t Environment::parse_environment_iccad2015(int argc, char **argv) {
  parse_environment_tau2015(argc, argv);  
}

// Procedure: parse_environment_tau2015
// The procedure parse the input environment in the form of tau2015 contest. 
void_t Environment::parse_environment_tau2015(int argc, char **argv) {
  
  LOG_IF(FATAL, argc != 5) << "Usage: ./executable <path>.tau2015 <path>.timing <path>.ops <path>.out";
  
  // For the tau2015_wrapper, timing, ops, and output file paths.
  _argv.clear();
  _argv.reserve(argc);
  for(int i=0; i<argc; i++) _argv.push_back(argv[i]);

  _tau2015_wrapper_fpath = _argv[1];
  _timing_fpath = _argv[2];
  _ops_fpath = _argv[3];
  set_rpt_fpath(_argv[4]);

  // For the lib, spef, and verilog file paths in the tau2015_wrapper.
  ifstream_t ifs(_tau2015_wrapper_fpath);

  CHECK(ifs.good());

  string line, token;
  getline(ifs, line);                                       // Read the line from file.
  
  _tau2015_wrapper_fdir = _tau2015_wrapper_fpath.substr(0, _tau2015_wrapper_fpath.find_last_of("/\\")); 

  // Add prefix and postfix.
  if(_tau2015_wrapper_fdir == _tau2015_wrapper_fpath) {
    _tau2015_wrapper_fdir = "./";
  }
  else {
    _tau2015_wrapper_fdir.push_back('/');
  }

  stringstream ss(line);                                   
   
  CHECK(ss.good());

  ss >> token;                                              // early celllib file path.
  _early_celllib_fpath = _tau2015_wrapper_fdir + token;

  CHECK(ss.good());

  ss >> token;                                              // late celllib file path.
  _late_celllib_fpath = _tau2015_wrapper_fdir + token;

  CHECK(ss.good());

  ss >> token;                                              // spef file path.
  _spef_fpath = _tau2015_wrapper_fdir + token;
  
  CHECK(ss.good());

  ss >> token;                                              // verilog file path.
  _verilog_fpath = _tau2015_wrapper_fdir + token;

  ifs.close();
}

// Procedure: parse_environment_tau2016
// The procedure parse the input environment in the form of tau2016 contest. The submitted binary 
// must adhere to the following format:
//
// ~$ myModeler <.tau2016> <.timing> <output name>
//
// <.tau2016>: input file that contains the benchmark information, including the netlist (.v), the
//             parasitics (.spef), and the cell libraries (.lib).
// <.timing>: input file that describes the initial timing conditions to each of the primary 
//            inputs of the design.
// <output name>: the name of the output abstraction macro, which should be <output name>_early.lib
//                and <output name>_late.lib
//
void_t Environment::parse_environment_tau2016(int argc, char **argv) {
  
  CHECK(argc == 4);
  
  // For the tau2016_wrapper, timing, ops, and output file paths.
  _argv.clear();
  _argv.reserve(argc);
  for(int i=0; i<argc; i++) _argv.push_back(argv[i]);

  _tau2016_wrapper_fpath = _argv[1];
  _timing_fpath = _argv[2];
  _tau2016_macro_name = _argv[3];

  // For the lib, spef, and verilog file paths in the tau2016_wrapper.
  ifstream_t ifs(_tau2016_wrapper_fpath);

  CHECK(ifs.good());

  string line, token;
  getline(ifs, line);                                       // Read the line from file.
  
  _tau2016_wrapper_fdir = _tau2016_wrapper_fpath.substr(0, _tau2016_wrapper_fpath.find_last_of("/\\")); 

  // Add prefix and postfix.
  if(_tau2016_wrapper_fdir == _tau2016_wrapper_fpath) {
    _tau2016_wrapper_fdir = "./";
  }
  else {
    _tau2016_wrapper_fdir.push_back('/');
  }

  stringstream ss(line);                                   
   
  CHECK(ss.good());

  ss >> token;                                              // early celllib file path.
  _early_celllib_fpath = _tau2016_wrapper_fdir + token;

  CHECK(ss.good());

  ss >> token;                                              // late celllib file path.
  _late_celllib_fpath = _tau2016_wrapper_fdir + token;

  CHECK(ss.good());

  ss >> token;                                              // spef file path.
  _spef_fpath = _tau2016_wrapper_fdir + token;
  
  CHECK(ss.good());

  ss >> token;                                              // verilog file path.
  _verilog_fpath = _tau2016_wrapper_fdir + token;

  ifs.close();
}



};  // End of namespace OpenTimer. ----------------------------------------------------------------

