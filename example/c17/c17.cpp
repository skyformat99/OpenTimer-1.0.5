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

#include "ot_timer.h"

// Function: main
int main(int argc, char *argv[]) {
  
  using namespace OpenTimer;

  // Logging options.
  Timer::init_logging(argv[0], 1);

  // Usage of OpenTimer.
  Timer timer;
  
  // Initiate the design. At minimum, it requires one .v, two .lib (early and late),
  // one .timing, and one .spef.
  timer.set_num_threads(8); 
  timer.set_verilog_fpath("c17.v");
  timer.set_early_celllib_fpath("c17_Early.lib");
  timer.set_late_celllib_fpath("c17_Late.lib");
  timer.set_timing_fpath("c17.timing");
  timer.set_spef_fpath("c17.spef");

  // Initialize the timer. This should be callsed after the design had been
  // appropriately initiated.
  timer.init_timer();

  // Example: Report the TNS and WNS for the design.
  printf("TNS = %.4f, WNS = %.4f\n", timer.tns(), timer.wns());
  
  // Example: Run design modifiers.
  timer.repower_gate("inst_0", "NAND2_X2");
  timer.insert_gate("TAUGATE_1", "BUF_X2");
  timer.insert_net("TAUNET_1");
  timer.disconnect_pin("nx6");
  timer.connect_pin("nx6", "TAUNET_1");
  timer.connect_pin("TAUGATE_1:A", "TAUNET_1");
  timer.connect_pin("TAUGATE_1:Z", "nx6");
  timer.read_spef("change_1.spef");

  // Example: Query the timing for a pin at any transition and split.
  printf("LATE/FALL slack at pin 'nx22' = %.4f\n", timer.slack("nx22", LATE, FALL)); 
  printf("LATE/RISE arrival time at pin 'nx22' = %.4f\n", timer.at("nx22", LATE, RISE)); 
  printf("EALRY/FALL required arrival time at pin 'nx22' = %.4f\n", timer.rat("nx22", EARLY, FALL)); 

  // Example: Report the top two critical paths in the design. 
  // The path trace is stored in the class object 'Path' which has been typedefed as follows:
  // In ot_typedef.h
  path_cache_t cache;
  printf("Top-two critical paths in the design.\n");
  timer.get_worst_paths("", 2, cache);
  for(unsigned i=0; i<cache.path_ptrs().size(); ++i) {
    path_rt p = *(cache.path_ptrs()[i]);
    switch(p.path_type()) {
      case RAT_PATH_TYPE:
        printf("Path %d: RAT %.3f %lu %c\n", i, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      case SETUP_PATH_TYPE:
        printf("Path %d: Setup %.3f %lu %c\n", i, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      case HOLD_PATH_TYPE:
        printf("Path %d: Hold %.3f %lu %c\n", i, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      default:
        assert(false);
      break;
    }
    for(PathIter i(p); i(); ++i) {
      printf("%s %c\n", i.pin_ptr()->name().c_str(), i.rf() ? 'F' : 'R');
    }
  }

  timer.report_timer();

  return 0;
}



















