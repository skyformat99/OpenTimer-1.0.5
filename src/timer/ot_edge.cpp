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

#include "ot_edge.h"

namespace OpenTimer {

// Constructor: Edge
Edge::Edge() {
   
  for(int el=0; el<2; el++) {
    for(int from_rf=0; from_rf<2; from_rf++) {
      for(int to_rf=0; to_rf<2; to_rf++) {
        _delay[el][from_rf][to_rf] = OT_DEFAULT_EDGE_DELAY;
      }
    }
    _timing_arc_ptr[el] = NULL;
  }

  _from_node_ptr = NULL;
  _to_node_ptr = NULL;
  _net_ptr = NULL;
  _fanin_satellite = NULL;
  _fanout_satellite = NULL;
  _edgelist_satellite = NULL;
  _edge_type = UNDEFINED_EDGE_TYPE;
  _timing_sense = UNDEFINED_TIMING_SENSE;
}

// Destructor: Edge
Edge::~Edge() {
}

// Function: timing_ptr
timing_pt Edge::timing_ptr(int el, int irf, int orf) {
  if(timing_arc_ptr(el) == nullptr) return nullptr;
  return timing_arc_ptr(el)->timing_ptr(irf, orf);
}

//-------------------------------------------------------------------------------------------------

// Constructor.
Jump::Jump() {

  EL_RF_RF_ITER(el, irf, orf) {
    _delay[el][irf][orf] = OT_DEFAULT_EDGE_DELAY;
  }

  _idx = OT_UNDEFINED_IDX;
  _from_node_ptr = NULL;
  _to_node_ptr = NULL;
  _jumpin_satellite = NULL;
  _jumpout_satellite = NULL;

  _timing_sense = UNDEFINED_TIMING_SENSE;
}

// Destructor.
Jump::~Jump() {

}

};  // End of OpenTimer namespace. ----------------------------------------------------------------



