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
  
  // Usage of OpenTimer.
  Timer::init_logging(argv[0], 1);
  Timer timer;
  
  // Initiate the design. At minimum, it requires one .v, two .lib (early and late),
  // one .timing, and one .spef.
  timer.set_num_threads(1); 
  timer.set_verilog_fpath("simple.v");
  timer.set_early_celllib_fpath("simple_Early.lib");
  timer.set_late_celllib_fpath("simple_Late.lib");
  timer.set_timing_fpath("simple.timing");
  timer.set_spef_fpath("simple.spef");

  // Initialize the timer. This should be callsed after the design had been
  // appropriately initiated.
  timer.init_timer();

  // Example: Report the TNS and WNS for the design.
  printf("TNS = %.4f, WNS = %.4f\n", timer.tns(), timer.wns());
  
  // Example: Run design modifiers.
  timer.repower_gate("u3", "INV_X1");
  timer.insert_gate("newbox", "INV_X3");
  timer.insert_net("newnet");
  timer.disconnect_pin("u3:o");
  timer.connect_pin("newbox:o", "out");
  timer.connect_pin("u3:o", "newnet");
  timer.connect_pin("newbox:a", "newnet");
  timer.read_spef("change1.spef");
  timer.repower_gate("u3", "INV_X2");
  timer.disconnect_pin("newbox:a");
  timer.disconnect_pin("newbox:o");
  timer.disconnect_pin("u3:o");
  timer.remove_net("newnet");
  timer.remove_gate("newbox");
  timer.connect_pin("u3:o", "out");
  timer.read_spef("change2.spef");

  // Example: Query the timing for a pin at any transition and split.
  printf("LATE/FALL slack at pin 'out' = %.4f\n", timer.slack("out", LATE, FALL)); 
  printf("LATE/RISE arrival time at pin 'out' = %.4f\n", timer.at("out", LATE, RISE)); 
  printf("EALRY/FALL required arrival time at pin 'out' = %.4f\n", timer.rat("out", EARLY, FALL)); 

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

  timer.write_early_celllib("output_Early.lib");

  return 0;
}



















