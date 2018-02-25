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

#include "ot_verilog.h"

namespace OpenTimer {

// Constructor: Verilog
// Class constructor.
Verilog::Verilog() {
  _module_dict_ptr = new verilog_module_dict_t();
}

// Destructor: ~Verilog
// Class destructor.
Verilog::~Verilog() {
  delete _module_dict_ptr;
}
  
// Function: insert_module
// Insert a verilog module into the verilog class.
verilog_module_pt Verilog::insert_module(string_crt name) {
  verilog_module_pt module_ptr = _module_dict_ptr->insert(name, name);
  return module_ptr;
}

// Procedure: read_module
// Read a verilog module file.
void_t Verilog::read_module(string_crt fpath) {
  
  // [BEG INFO]
  LOG(INFO) << string_t("Loading ") + Utility::realpath(fpath);
  // [END INFO]
  
  char_t delimiters[] = {
    '(', ')', 
    ',', ':', ';', 
    '/', 
    '#', 
    '[', ']', 
    '{', '}', 
    '*', 
    '\"', '\\',
  };

  char_t separators[] = {
    ' ', '\n', ';', ','
  };

  freader_t freader; 
  freader.read(fpath.c_str(), separators, sizeof(separators), delimiters, sizeof(delimiters));

  string_t token;
  string_t gate_name;
  string_t cellpin_name;
  string_t wire_name;

  verilog_module_pt module_ptr(nullptr);

  // Parse the module group.
  while((token = freader.next_token()) != "") {
    if(token == "module") {
      module_ptr = insert_module(token = freader.next_token());
      break;
    }
  }
  while(token[0] != ')') {
    token = freader.next_token();
  }

  // Read the io ports and gates.
  LOG_IF(FATAL, module_ptr == nullptr) << "Failed to parse verilog module";
  
  while((token = freader.next_token()) != "") {

    if(token == "input") {
      module_ptr->insert_input(freader.next_token());
    }
    else if(token == "output") {
      module_ptr->insert_output(freader.next_token());
    }
    else if(token == "wire") {
      module_ptr->insert_wire(freader.next_token());
    }
    else if(token == "endmodule") {
      break;
    }
    else {
      gate_name = freader.next_token();
      verilog_gate_pt gate_ptr = module_ptr->insert_gate(gate_name, token);  
      token = freader.next_token();

      LOG_IF(FATAL, token[0] != '(') << "Failed to parse verilog gate";
      
      while((token = freader.next_token()) != "") {

        if(token[0] == ')') break;

        // Begin to read mapping in the form .a(wire)
        LOG_IF(FATAL, token[0] != '.') << "Failed to parse verilog net connection";
        
        // cellpin_name
        cellpin_name = token.substr(1);
        
        // group '('
        token = freader.next_token();
        LOG_IF(FATAL, token[0] != '(') << "Failed to parse verilog net connection";
        
        // Net name
        wire_name = freader.next_token();

        // group ')';
        token = freader.next_token();
        LOG_IF(FATAL, token[0] != ')') << "Failed to parse verilog net connection";

        gate_ptr->insert_cellpin_to_net(cellpin_name, wire_name);
      }
    }
  }
}

// Procedure: report
// Report the verilog object in the order of modules, gates to a given module, and then
// the cellpin to net mapping to a given gate.
void_t Verilog::report() { 

  printf("Report the verilog object\n");
  for(VerilogModuleIter i(this); i(); ++i) {
    printf("module %s\n", i.module_ptr()->name().c_str());
    for(VerilogModuleGateIter j(i.module_ptr()); j(); ++j) {
      printf("  gate %s\n", j.gate_ptr()->name().c_str());
      for(VerilogGateCellpinToNetMappingIter k(j.gate_ptr()); k(); ++k) {
        printf("    mapping: %s - %s\n", k.cellpin_name(), k.net_name());
      }
    }
  }
}

// Constructor: VerilogModule
VerilogModule::VerilogModule(string_crt key):
  _name(key)
{
  _inputs_ptr = new verilog_input_ptr_vt();
  _outputs_ptr = new verilog_output_ptr_vt();
  _wires_ptr = new verilog_wire_ptr_vt();
  _gates_ptr = new verilog_gate_ptr_vt();
}

// Destructor: ~VerilogModule
VerilogModule::~VerilogModule() {
  for(unsigned_t i=0; i<_inputs_ptr->size(); i++) delete (*_inputs_ptr)   [i];
  for(unsigned_t i=0; i<_outputs_ptr->size(); i++) delete (*_outputs_ptr)  [i];
  for(unsigned_t i=0; i<_wires_ptr->size() ; i++) delete (*_wires_ptr)    [i];
  for(unsigned_t i=0; i<_gates_ptr->size(); i++) delete (*_gates_ptr)[i];
  delete _inputs_ptr;
  delete _outputs_ptr;
  delete _wires_ptr;
  delete _gates_ptr;
}

// Function: insert_gate
// Insert an gate
verilog_gate_pt VerilogModule::insert_gate(string_crt gate_name, string_crt cell_name) {
  verilog_gate_pt gate_ptr = new verilog_gate_t();
  gate_ptr->set_name(gate_name);
  gate_ptr->set_cell_name(cell_name);
  gates_ptr()->push_back(gate_ptr); 
  return gate_ptr;
}

// Function: insert_input
verilog_input_pt VerilogModule::insert_input(string_crt name) {
  verilog_input_pt input_ptr = new verilog_input_t();
  input_ptr->set_name(name);
  inputs_ptr()->push_back(input_ptr);
  return input_ptr;
}

// Function: insert_output
verilog_output_pt VerilogModule::insert_output(string_crt name) {
  verilog_output_pt output_ptr = new verilog_output_t();
  output_ptr->set_name(name);
  outputs_ptr()->push_back(output_ptr);
  return output_ptr;
}

// Function: insert_wire
verilog_wire_pt VerilogModule::insert_wire(string_crt name) {
  verilog_wire_pt wire_ptr = new verilog_wire_t();
  wire_ptr->set_name(name);
  wires_ptr()->push_back(wire_ptr);
  return wire_ptr;
}

// Constructor: VerilogGate
VerilogGate::VerilogGate() {
}

// Destructor: ~VerilogGate
VerilogGate::~VerilogGate() {
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------
