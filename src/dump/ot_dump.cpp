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

#include "ot_dump.h"

namespace OpenTimer {

// Function: dump_net
// Dump the net information into a string object and return it.
string_t dump_net(net_pt net_ptr) {

  stringstream ss;

  if(net_ptr == NULL) {
    ss << "Net N/A.\n";
  }
  else {
    ss << "Net '" << net_ptr->name() << "' {\n";
    ss << "  Pin count: " << net_ptr->pinlist_ptr()->size() << "\n";

    if(net_ptr->root_pin_ptr()) {
      ss << "  Root pin: '" << net_ptr->root_pin_ptr()->name() << "'\n";
    }
    else {
      ss << "  Root pin: N/A\n";
    }
    ss << "  Pinlist:";
    for(NetPinlistIter i(net_ptr); i(); ++i) {
      ss << " '" << i.pin_ptr()->name().c_str() << "'";
    }
    ss << "\n}\n";
  }

  return ss.str();
}

// Function: dump_pin
// Dump the pin information into a string object and return it.
string_t dump_pin(pin_pt pin_ptr) {
  
  stringstream ss;

  if(pin_ptr == NULL) {
    ss << "Pin N/A.\n";
  }
  else {
    ss << "Pin '" << pin_ptr->name() << "'{\n";

    switch (pin_ptr->direction()) {
      case PRIMARY_INPUT_PIN_DIRECTION: 
        ss << "  Direction: PRIMARY_INPUT_PIN_DIRECTION\n";
        break;
      case PRIMARY_OUTPUT_PIN_DIRECTION:
        ss << "  Direction: PRIMARY_OUTPUT_PIN_DIRECTION\n";
        break;
      case INPUT_CELLPIN_DIRECTION:
        ss << "  Direction: INPUT_CELLPIN_DIRECTION\n";
        break;
      case OUTPUT_CELLPIN_DIRECTION:
        ss << "  Direction: OUTPUT_CELLPIN_DIRECTION\n";
        break;
      case INOUT_CELLPIN_DIRECTION:
        ss << "  Direction: OUTPUT_CELLPIN_DIRECTION\n";
        break;
      case INTERNAL_CELLPIN_DIRECTION:
        ss << "  Direction: INTERNEL_CELLPIN_DIRECTION\n";
        break;
      case UNDEFINED_PIN_DIRECTION:
        ss << "  Direction: UNDEFINED_PIN_DIRECTION\n";
        break;
      default:
      break;
    };

    ss << "  Gate: " << (pin_ptr->gate_ptr() ? pin_ptr->gate_ptr()->name() : "N/A") << "\n";
    ss << "  PI  : " << (pin_ptr->primary_input_ptr() ? pin_ptr->primary_input_ptr()->name() : "N/A") << "\n";
    ss << "  PO  : " << (pin_ptr->primary_output_ptr() ? pin_ptr->primary_output_ptr()->name() : "N/A") << "\n";
    ss << "  Net : " << pin_ptr->net_ptr()->name() << "\n";

    ss << "}\n";
  }

  return ss.str();
}

// Function: dump_node
// Dump the node information into a string object and return it.
string_t dump_node(node_pt node_ptr) {

  stringstream ss;

  if(node_ptr == NULL) {
    ss << "Node N/A.\n";
  }
  else {
    ss << "Node {\n";
    ss << "  Attached pin: " << (node_ptr->pin_ptr() ? node_ptr->pin_ptr()->name() : "N/A") << "\n";
    ss << "}\n";
  }
  return ss.str();
}

// Function: dump_edge
// Dump the edge information into a string object and return it.
string_t dump_edge(edge_pt edge_ptr) {

  stringstream ss;

  if(edge_ptr == NULL) {   
    ss << "Edge N/A.\n";
  }
  else {
    ss << "Edge {\n";
    ss << "  From pin: " << edge_ptr->from_node_ptr()->pin_ptr()->name() << "\n";
    ss << "  To   pin: " << edge_ptr->to_node_ptr()->pin_ptr()->name() << "\n";
    ss << "  Net: " << (edge_ptr->net_ptr() ? edge_ptr->net_ptr()->name() : "N/A") << "\n";
    ss << "}\n";
  }
  return ss.str();
}


};  // End of namespace OpenTimer. ----------------------------------------------------------------





