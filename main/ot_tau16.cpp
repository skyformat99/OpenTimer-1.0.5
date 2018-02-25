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

#include "ot_tau16.h"

int main(int argc, char *argv[]) {

  OpenTimer::Timer::init_logging(argv[0], 1);  
  OpenTimer::timer_t timer;
  timer.set_num_threads(8);
  timer.exec_tau2015(argc, argv);

  return 0;
}

