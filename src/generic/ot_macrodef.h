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

#ifndef OT_MACRODEF_H_
#define OT_MACRODEF_H_

#define OT_MAX_NUM_THREADS 8

#define OT_INT_MAX INT_MAX 
#define OT_INT_MIN INT_MIN 
#define OT_SIZE_MAX SIZE_MAX
#define OT_SIZE_MIN 0
#define OT_FLT_INF FLT_MAX
#define OT_FLT_NINF -FLT_MAX
#define OT_FLT_MAX 1e16f
#define OT_FLT_MIN -1e16f
#define OT_FLT_ZERO 0.0f
#define OT_FLT_ONE 1.0f
#define OT_FLT_TWO 2.0f
#define TAU2015_FLT_MAX 987654.0f
#define TAU2015_FLT_MIN -987654.0f
#define OT_MAX_FPATH_LENGTH 4096

#define OT_DEFAULT_PIN_DICTIONARY_SIZE 786433
#define OT_DEFAULT_INSTANCE_DICTIONARY_SIZE 393241
#define OT_DEFAULT_NET_DICTIONARY_SIZE 393241

#define OT_UNDEFINED_FLT OT_FLT_INF
#define OT_UNDEFINED_IDX -1
#define OT_UNDEFINED_LEVEL -1
#define OT_UNDEFINED_COORDINATE OT_FLT_MAX

#define OT_DEFAULT_NODE_LEVEL 0

#define OT_DEFAULT_SITE_IS_X_SYMMETRY false
#define OT_DEFAULT_SITE_IS_Y_SYMMETRY false
#define OT_DEFAULT_SITE_IS_R90_SYMMETRY false

#define OT_DEFAULT_MACRO_IS_FIXED_MASK false
#define OT_DEFAULT_MACRO_IS_X_SYMMETRY false
#define OT_DEFAULT_MACRO_IS_Y_SYMMETRY false
#define OT_DEFAULT_MACRO_IS_R90_SYMMETRY false

#define OT_DEFAULT_CELL_LEAKAGE_POWER OT_FLT_ZERO
#define OT_DEFAULT_CELL_AREA OT_FLT_ZERO
#define OT_DEFAULT_CELL_TYPE COMBINATIONAL_CELL_TYPE

#define OT_DEFAULT_CELLPIN_CAPACITANCE OT_FLT_ZERO
#define OT_DEFAULT_CELLPIN_MAX_CAPACITANCE OT_FLT_MAX
#define OT_DEFAULT_CELLPIN_MIN_CAPACITANCE OT_FLT_ZERO
#define OT_DEFAULT_CELLPIN_MAX_TRANSITION OT_FLT_MAX
#define OT_DEFAULT_CELLPIN_MIN_TRANSITION OT_FLT_ZERO
#define OT_DEFAULT_CELLPIN_IS_CLOCK false

#define OT_DEFAULT_NET_LCAP OT_FLT_ZERO

#define OT_DEFAULT_EDGE_DELAY OT_FLT_ZERO

#define OT_DEFAULT_TEST_SLACK OT_FLT_ZERO

#define OT_DEFAULT_RCTREE_NODE_URES OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_CAP OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_LCAP OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_BETA OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_DELAY OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_LDELAY OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_NODE_IMPULSE OT_FLT_ZERO
#define OT_DEFAULT_RCTREE_EDGE_RES OT_FLT_ZERO

#define OT_DEFAULT_PRIMARY_OUTPUT_LCAP OT_FLT_ZERO
#define OT_DEFAULT_PRIMARY_OUTPUT_RAT OT_UNDEFINED_FLT
#define OT_DEFAULT_PRIMARY_INPUT_AT OT_FLT_ZERO
#define OT_DEFAULT_PRIMARY_INPUT_SLEW OT_FLT_ZERO

#define OT_DEFAULT_SPEFCAP OT_FLT_ZERO
#define OT_DEFAULT_SPEFRES OT_FLT_ZERO
#define OT_DEFAULT_SPEFLCAP OT_FLT_ZERO

#define OT_DEFAULT_CLOCK_SOURCE_PERIOD OT_FLT_ZERO

#define OT_DEFAULT_VERBOSE  0
#define OT_DEFAULT_LOG_FPTR stdout
#define OT_DEFAULT_RPT_FPTR stdout
#define OT_DEFAULT_ASSERT_WARN 0
#define OT_DEFAULT_ASSERT_ERROR 1

#define OT_COMMAND_MAX_LINE_SIZE 65536

#define OT_ANSI_COLOR_RED     "\x1b[31m"
#define OT_ANSI_COLOR_GREEN   "\x1b[32m"
#define OT_ANSI_COLOR_YELLOW  "\x1b[33m"
#define OT_ANSI_COLOR_BLUE    "\x1b[34m"
#define OT_ANSI_COLOR_MAGENTA "\x1b[35m"
#define OT_ANSI_COLOR_CYAN    "\x1b[36m"
#define OT_ANSI_COLOR_RESET   "\x1b[0m"

#define OT_MAX_TOKEN_LENGTH 1024

#define OT_SQUARE(X) (X)*(X)

#define EARLY 0
#define LATE  1
#define RISE  0
#define FALL  1
#define UNDEFINED_TRANS -1
#define UNDEFINED_SPLIT -1

#define EL_ITER(el) for(int el=0; el<2; ++el) 
#define RF_ITER(rf) for(int rf=0; rf<2; ++rf) 
#define RF_RF_ITER(irf, orf) for(int irf=0; irf<2; ++irf) \
                                     for(int orf=0; orf<2; ++orf)
#define EL_RF_ITER(el, rf) for(int el=0; el<2; ++el) \
                                   for(int rf=0; rf<2; ++rf)
#define EL_RF_RF_ITER(el, rf1, rf2) for(int el=0; el<2; ++el) \
                                            for(int rf1=0; rf1<2; ++rf1) \
                                              for(int rf2=0; rf2<2; ++rf2)

namespace OpenTimer {

};  // End of namespace OpenTimer. ----------------------------------------------------------------

#endif

