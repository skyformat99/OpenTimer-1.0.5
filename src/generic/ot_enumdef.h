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

#ifndef OT_ENUM_H_
#define OT_ENUM_H_

namespace OpenTimer {

typedef enum CapacitanceUnitBase {
  FARAD,
  MILLI_FARAD,
  MICRO_FARAD,
  NANO_FARAD,
  PICO_FARAD,
  FEMTO_FARAD,
  UNDEFINED_CAPACITANCE_UNIT
}capacitance_unit_base_e;

enum CurrentUnitBase {
  AMPERE,
  MILLI_AMPERE,
  UNDEFINED_CURRENT_UNIT
};

enum class LibraryType {
  EARLY_LIBRARY = 0,
  LATE_LIBRARY = 1,
  UNDEFINED
};

enum LibraryDelayModel {
  GENERIC_CMOS,
  TABLE_LOOKUP,
  PIECEWISE_CMOS,
  DELAY_CALCULATION_MODULE,
  POLYNOMIAL,
  UNDEFINED_LIBRARY_DELAY_MODEL
};

enum LibraryTechnology {
  CMOS_LIBRARY_TECHNOLOGY,
  FPGA_LIBRARY_TECHNOLOGY,
  UNDEFINED_LIBRARY_TECHNOLOGY
};

enum LookupTableVariable {
  TOTAL_OUTPUT_NET_CAPACITANCE = 0,
  INPUT_NET_TRANSITION = 1,
  CONSTRAINED_PIN_TRANSITION = 2,
  RELATED_PIN_TRANSITION = 3,
  UNDEFINED_LOOKUPTABLE_VARIABLE = 4
};

enum NodeTimingType {
  RISE_EARLY_NODE_TIMING_TYPE,
  RISE_LATE_NODE_TIMING_TYPE,
  FALL_EARLY_NODE_TIMING_TYPE,
  FALL_LATE_NODE_TIMING_TYPE,
  UNDEFINED_NODE_TIMING_TYPE
};

enum PathType {
  HOLD_PATH_TYPE,
  SETUP_PATH_TYPE,
  RAT_PATH_TYPE,
  UNDEFINED_PATH_TYPE
};

enum PinDirection {
  PRIMARY_INPUT_PIN_DIRECTION,
  PRIMARY_OUTPUT_PIN_DIRECTION,
  INPUT_CELLPIN_DIRECTION,
  OUTPUT_CELLPIN_DIRECTION,
  INOUT_CELLPIN_DIRECTION,
  INTERNAL_CELLPIN_DIRECTION,
  INPUT_MACROPIN_DIRECTION,
  OUTPUT_MACROPIN_DIRECTION,
  UNDEFINED_PIN_DIRECTION
};

enum PowerUnitBase {
  WATT,
  MILLI_WATT,
  MICRO_WATT,
  NANO_WATT,
  PICO_WATT,
  FEMTO_WATT,
  UNDEFINED_POWER_UNIT
};

enum ResistanceUnitBase {
  KILO_OHM,
  OHM,
  UNDEFINED_RESISTANCE_UNIT
};

enum TimingSense {
  POSITIVE_UNATE = 0,
  NEGATIVE_UNATE = 1,
  NON_UNATE = 2,
  UNDEFINED_TIMING_SENSE = 3
};

enum class NextStateType {
  DATA,
  PRESET, 
  CLEAR,
  LOAD,
  SCAN_IN,
  SCAN_ENABLE,
  UNDEFINED
};

enum class TimingType {
  COMBINATIONAL,
  COMBINATIONAL_RISE,
  COMBINATIONAL_FALL,
  THREE_STATE_DISABLE,
  THREE_STATE_DISABLE_RISE,
  THREE_STATE_DISABLE_FALL,
  THREE_STATE_ENABLE,
  THREE_STATE_ENABLE_RISE,
  THREE_STATE_ENABLE_FALL,
  RISING_EDGE,
  FALLING_EDGE,
  PRESET,
  CLEAR,
  HOLD_RISING,
  HOLD_FALLING,
  SETUP_RISING,
  SETUP_FALLING,
  RECOVERY_RISING,
  RECOVERY_FALLING,
  SKEW_RISING,
  SKEW_FALLING,
  REMOVAL_RISING,
  REMOVAL_FALLING,
  MIN_PULSE_WIDTH,
  MINIMUM_PERIOD,
  MAX_CLOCK_TREE_PATH,
  MIN_CLOCK_TREE_PATH,
  NON_SEQ_SETUP_RISING,
  NON_SEQ_SETUP_FALLING,
  NON_SEQ_HOLD_RISING,
  NON_SEQ_HOLD_FALLING,
  NOCHANGE_HIGH_HIGH,
  NOCHANGE_HIGH_LOW,
  NOCHANGE_LOW_HIGH,
  NOCHANGE_LOW_LOW,
  UNDEFINED
};

enum EdgeType {
  RCTREE_EDGE_TYPE,
  CONSTRAINT_EDGE_TYPE,
  COMBINATIONAL_EDGE_TYPE,
  UNDEFINED_EDGE_TYPE
};

enum TimeUnitBase {
  SECOND,
  MILLI_SECOND,
  MICRO_SECOND,
  NANO_SECOND,
  PICO_SECOND,
  UNDEFINED_TIME_UNIT
};

enum VoltageUnitBase {
  VOLT,
  UNDEFINED_VOLTAGE_UNIT
};

enum GateSource {
  DIST_GATE_SOURCE,
  NETLIST_GATE_SOURCE,
  TIMING_GATE_SOURCE,
  USER_GATE_SOURCE,
  UNDEFINED_GATE_SOURCE
};

enum MacroClass {
  PAD_MACRO_CLASS,
  CORE_MACRO_CLASS,
  BLOCK_MACRO_CLASS,
  UNDEFINED_MACRO_CLASS
};

enum PinUse {
  SIGNAL_PIN_USE,
  ANALOG_PIN_USE,
  POWER_PIN_USE,
  GROUND_PIN_USE,
  CLOCK_PIN_USE,
  TIEOFF_PIN_USE,
  SCAN_PIN_USE,
  RESET_PIN_USE,
  UNDEFINED_PIN_USE
};

enum SiteClass {
  PAD_SITE_CLASS,
  CORE_SITE_CLASS,
  UNDEFINED_SITE_CLASS
};

enum LayerDirection {
  HORIZONTAL_LAYER_DIRECTION,
  VERTICAL_LAYER_DIRECTION,
  UNDEFINED_LAYER_DIRECTION
};

enum LayerType {
  CUT_LAYER_TYPE,
  IMPLANT_LAYER_TYPE,
  OVERLAP_LAYER_TYPE,
  MASTERSLICE_LAYER_TYPE,
  ROUTING_LAYER_TYPE,
  UNDEFINED_LAYER_TYPE
};

enum Mobility {
  PLACED_MOBILITY,
  FIXED_MOBILITY,
  COVER_MOBILITY,
  UNPLACED_MOBILITY,
  UNDEFINED_MOBILITY
};

enum Orientation {
  N_ORIENTATION,
  W_ORIENTATION,
  S_ORIENTATION,
  E_ORIENTATION,
  FN_ORIENTATION,
  FW_ORIENTATION,
  FS_ORIENTATION,
  FE_ORIENTATION,
  UNDEFINED_ORIENTATION
};

};  // End of OpenTimer namespace. ----------------------------------------------------------------


#endif


