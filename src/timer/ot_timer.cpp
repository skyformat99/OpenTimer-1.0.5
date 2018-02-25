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

namespace OpenTimer {

// Constructor
Timer::Timer():
  _name(PACKAGE_STRING),
  _environment_uptr (new environment_t()),
  _techlib_uptr (new techlib_t()),
  _circuit_uptr (new circuit_t()),
  _pipeline_uptr (new pipeline_t()),
  _nodeset_uptr (new nodeset_t()),
  _jumpset_uptr (new jumpset_t()),
  _endpoint_minheap_uptr (new endpoint_minheap_t())
{
  _celllib_uptr[EARLY].reset(new celllib_t());
  _celllib_uptr[LATE].reset(new celllib_t());
  celllib_ptr(EARLY)->set_library_type(LibraryType::EARLY_LIBRARY);
  celllib_ptr(LATE)->set_library_type(LibraryType::LATE_LIBRARY);

  _edgelist_ptr = new edgelist_t();

  EL_RF_ITER(el, rf) {
    _tns[el][rf] = OT_FLT_ZERO;
    _wns[el][rf] = OT_FLT_ZERO;
    _is_tns_updated[el][rf] = false;
    _is_wns_updated[el][rf] = false;
    _cutoff_slack[el][rf] = OT_FLT_ZERO;
  }
  
  //LOG(INFO) << string("An OpenTimer object created ") + Utility::getcwd();
  omp_set_num_threads(OT_MAX_NUM_THREADS);
  omp_set_nested(1);
}

// Destructor.
Timer::~Timer() {
  _clear_edgelist();
  delete _edgelist_ptr;
}

// Procedure: init_logging
void_t Timer::init_logging(char_cpt argv_0, bool_ct flag) {
  ::OpenTimer::Log::init_logging(argv_0, flag);
}

// Procedure: run
void_t Timer::run() {
  ::OpenTimer::Shell::run();
}

// Procedure: _clear_edgelist
// Clear the edge objects in the edgelist.
void_t Timer::_clear_edgelist() {
  for(const auto& e : *edgelist_ptr()) delete e;
  edgelist_ptr()->clear();
}

// Procedure: _remove_all_frontiers
// Clear the pipeline by removing all nodes from the pipeline.
void_t Timer::_remove_all_frontiers() {
  pipeline_ptr()->remove_all_nodes();
}

// Function: num_pins
// Query the number of pins.
size_t Timer::num_pins() const {
  return circuit_ptr()->num_pins();
}

// Function: num_nets
// Query the number of nets.
size_t Timer::num_nets() const {
  return circuit_ptr()->num_nets();
}

// Function: num_gates
// Query the number of gates.
size_t Timer::num_gates() const {
  return circuit_ptr()->num_gates();
}

// Function: num_tests
// Query the number of tests.
size_t Timer::num_tests() const {
  return circuit_ptr()->num_tests();
}

// Function: num_jump
// Query the number of jump.
size_t Timer::num_jumps() const {
  return jumpset_ptr()->size();
}

// Function: num_primary_inputs
// Query the number of primary inputs.
size_t Timer::num_primary_inputs() const {
  return circuit_ptr()->num_primary_inputs();
}

// Function: num_primary_outputs
// Query the number of primary outputs
size_t Timer::num_primary_outputs() const {
  return circuit_ptr()->num_primary_outputs();
}

// Function: num_nodes
// Return the node count.
size_t Timer::num_nodes() const {
  return nodeset_ptr()->size();
}

// Function: num_edges
// Return the edge count.
size_t Timer::num_edges() const {
  return _edgelist_ptr->size();
}

// Procedure: set_num_threads
void_t Timer::set_num_threads(int_ct num_threads) {
  environment_ptr()->set_num_threads(num_threads);
}

// Function: num_threads
size_t Timer::num_threads() const {
  return environment_ptr()->num_threads();
}

// Procedure: set_rpt_fpath
void_t Timer::set_rpt_fpath(string_crt fpath) {
  return environment_ptr()->set_rpt_fpath(fpath);
}

// Procedure: set_slew
// Assert a new slew value to a pin.
void_t Timer::set_slew(int argc, char **argv) {
  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  float_t val = .0f;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    else val = atof(argv[i]);
    ++i;
  }
  set_slew(pin_name, el, rf, val);
}

// Procedure: set_slew
// Assert a new slew value to a pin.
void_t Timer::set_slew(string_crt name, int el, int rf, float_ct slew) {
  set_slew(pin_ptr(name), el, rf, slew);
}

// Procedure: set_slew
// Assert a slew value to a pin.
void_t Timer::set_slew(pin_pt pin_ptr, int el, int rf, float_ct slew) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Fail to set slew (nullptr exception)";
    return;  
  }
  
  // Case 1: the pin is binding to a PI port assertion.
  if(pin_ptr->is_primary_input()) {
    pin_ptr->primary_input_ptr()->set_slew(el, rf, slew); 
  }
  // Case 2: regular pin at the gate.
  else {
    LOG(WARNING) << "Assert slew on a non-PI pin (may be ignored)";
    pin_ptr->node_ptr()->set_slew(el, rf, slew);
  }

  // Enable the timing propagation.
  _insert_frontier(pin_ptr->node_ptr());
}

// Procedure: set_at
// Assert a new arrival time value to a pin.
void_t Timer::set_at(int argc, char **argv) {
  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  float_t val = .0f;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    else val = atof(argv[i]);
    ++i;
  }
  set_at(pin_name, el, rf, val);
}

// Procedure: set_at
// Assert a at value to a pin.
void_t Timer::set_at(string_crt name, int el, int rf, float_ct at) {
  set_at(pin_ptr(name), el, rf, at);
}

// Procedure: set_at
// Assert a at value to a pin.
void_t Timer::set_at(pin_pt pin_ptr, int el, int rf, float_ct at) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Fail to set arrival time (nullptr exception)";
    return;  
  }
  
  // Case 1: the pin is binding to a PI port assertion.
  if(pin_ptr->is_primary_input()) {
    pin_ptr->primary_input_ptr()->set_at(el, rf, at); 
  }
  // Case 2: regular pin at the gate.
  else {
    LOG(WARNING) << "Assert arrival time on a non-PI pin (may be ignored)";
    pin_ptr->node_ptr()->set_at(el, rf, at);
  }

  // Enable the timing propagation.
  _insert_frontier(pin_ptr->node_ptr());
}

// Procedure: set_rat
// Assert a new required arrival time value to a pin.
void_t Timer::set_rat(int argc, char **argv) {
  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  float_t val = .0f;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    else val = atof(argv[i]);
    ++i;
  }
  set_rat(pin_name, el, rf, val);
}

// Procedure: set_rat
// Assert a rat value to a pin.
void_t Timer::set_rat(string_crt name, int el, int rf, float_ct rat) {
  set_rat(pin_ptr(name), el, rf, rat);
}

// Procedure: set_rat
// Assert a rat value to a pin.
void_t Timer::set_rat(pin_pt pin_ptr, int el, int rf, float_ct rat) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Fail to set required arrival time (nullptr exception)";
    return;  
  }
  
  // Case 1: the pin is binding to a PI port assertion.
  if(pin_ptr->is_primary_output()) {
    pin_ptr->primary_output_ptr()->set_rat(el, rf, rat); 
  }
  // Case 2: regular pin at the gate.
  else {
    LOG(WARNING) << "Assert required arrival time on a non-PO pin (may be ignored)";
    pin_ptr->node_ptr()->set_rat(el, rf, rat);
  }

  // Enable the timing propagation.
  _insert_frontier(pin_ptr->node_ptr());
}

// Procedure: set_load
// Assert a new load capacitance to a pin.
void_t Timer::set_load(int argc, char **argv) {

  int i = 0;
  float_t val = .0f;
  string_t po_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) po_name = argv[++i];
    else val = atof(argv[i]);
    ++i;
  }

  EL_RF_ITER(el, rf) {
    set_load(po_name, el, rf, val);
  }
}

// Procedure: set_load
// Assert a load value to a pin.
void_t Timer::set_load(string_crt name, int el, int rf, float_ct load) {
  set_load(circuit_ptr()->primary_output_ptr(name), el, rf, load);
}

// Procedure: set_load
// Assert a load value to a pin.
void_t Timer::set_load(primary_output_pt po_ptr, int el, int rf, float_ct load) {
  
  if(po_ptr == nullptr) {
    LOG(ERROR) << "Fail to set load (nullptr exception)";
    return;  
  }

  // Adjust the delta capacitance.
  auto delta = load - po_ptr->load(el, rf);

  // Assert a new primary output load.
  po_ptr->set_load(el, rf, load); 
    
  // Leaf capacitance of the rctree.
  if(po_ptr->pin_ptr()->rctree_node_ptr()) {
    po_ptr->pin_ptr()->rctree_node_ptr()->add_cap(el, rf, delta);
  }
  
  // Update the leaf capacitance of the net and enable the rc timing update.
  if(po_ptr->pin_ptr()->net_ptr()) {
    po_ptr->pin_ptr()->net_ptr()->dupdate_total_leaf_cap(el, rf, delta);
  }

  // Enable the timing propagation.
  for(auto& e : po_ptr->pin_ptr()->node_ptr()->fanin()) {
    _insert_frontier(e->from_node_ptr());
  }

}

// Procedure: set_sdc_fpath
// Set the file path of the sdc for initialization.
void_t Timer::set_sdc_fpath(char_cpt fpath) {
  environment_ptr()->set_sdc_fpath(fpath);
}

// Procedure: set_lef_fpath
// Set the file path of the lef for initialization.
void_t Timer::set_lef_fpath(char_cpt fpath) {
  environment_ptr()->set_lef_fpath(fpath);
}

// Procedure: set_def_fpath
// Set the file path of the def for initialization.
void_t Timer::set_def_fpath(char_cpt fpath) {
  environment_ptr()->set_def_fpath(fpath);
}

// Procedure: set_spef_fpath
// Set the file path of the spef for initialization.
void_t Timer::set_spef_fpath(char_cpt fpath) {
  environment_ptr()->set_spef_fpath(fpath);
}

// Procedure: set_verilog_fpath
// Set the file path of the verilog for initialization.
void_t Timer::set_verilog_fpath(char_cpt fpath) {
  environment_ptr()->set_verilog_fpath(fpath);
}

// Procedure: set_timing_fpath
// Set the file path of the timing for initialization.
void_t Timer::set_timing_fpath(char_cpt fpath) {
  environment_ptr()->set_timing_fpath(fpath);
}

// Procedure: set_early_celllib_fpath
// Set the file path of the early celllib for initialization.
void_t Timer::set_early_celllib_fpath(char_cpt fpath) {
  environment_ptr()->set_early_celllib_fpath(fpath);
}

// Procedure: set_late_celllib_fpath
// Set the file path of the late celllib for initialization.
void_t Timer::set_late_celllib_fpath(char_cpt fpath) {
  environment_ptr()->set_late_celllib_fpath(fpath);
}

// Function: cell_ptr
// Query the cell pointer to a given timing split and name.
cell_pt Timer::cell_ptr(int el, string_crt name) const {
  return celllib_ptr(el)->cell_ptr(name);
}

// Function: pin_ptr
// Query the pointer to a given name of pin in the design.
pin_pt Timer::pin_ptr(string_crt name) const {
  return circuit_ptr()->pin_ptr(name);
}

// Function: primary_input_ptr
// Query the pointer to a given name of primary input port.
primary_input_pt Timer::primary_input_ptr(string_crt name) const {
  return circuit_ptr()->primary_input_ptr(name);
}

// Function: _clock_tree_root_node_ptr
// Return the clock tree root node pointer.
node_pt Timer::_clock_tree_root_node_ptr() const {
  pin_pt pin_ptr = circuit_ptr()->clock_tree_root_pin_ptr();
  return pin_ptr ? pin_ptr->node_ptr() : nullptr;
}

// Procedure: _read_def
void_t Timer::_read_def(string_crt fpath) {
  DEFReader def_reader;
  def_reader.read(this, fpath);
}

// Procedure: _read_def
void_t Timer::_read_lef(string_crt fpath) {
  LEFReader lef_reader;
  lef_reader.read(this, fpath);
}

// Procedure: _enable_lazy_evaluation
void_t Timer::_enable_lazy_evaluation() {
  EL_RF_ITER(el, rf) {
    _is_tns_updated[el][rf] = false;
    _is_wns_updated[el][rf] = false;
  }
}

// Procedure: _parse_verilog
void_t Timer::_parse_verilog(string_crt fpath, verilog_pt verilog_ptr) {
  
  if(verilog_ptr == nullptr) {
    LOG(WARNING) << "Ignore parsing verilog (nullptr exception)";
    return;
  }

  verilog_ptr->read_module(fpath);
}

// Procedure: _parse_spef
void_t Timer::_parse_spef(string_crt fpath, spef_pt spef_ptr) {
  spef_ptr->read_spef(fpath);
}

// Procedure: _read_celllib
void_t Timer::_read_celllib(string_crt fpath, celllib_pt celllib_ptr) {
  
  if(celllib_ptr == nullptr) {
    LOG(WARNING) << "Ignore reading celllib (nullptr exception)";
    return;
  }

  celllib_ptr->read(fpath);
}

// Procedure: _write_celllib
void_t Timer::_write_celllib(string_crt fpath, celllib_pt celllib_ptr) {

  if(celllib_ptr == nullptr) {
    LOG(WARNING) << "Ignore writing celllib (nullptr exception)";
    return;
  }

  celllib_ptr->write(fpath);
}

// Procedure: write_early_celllib
void_t Timer::write_early_celllib(char_cpt fpath) {
  _write_celllib(fpath, celllib_ptr(EARLY));
}

// Procedure: write_late_celllib
void_t Timer::write_late_celllib(char_cpt fpath) {
  _write_celllib(fpath, celllib_ptr(LATE));
}

// Procedure: _init_circuit_from_verilog
void_t Timer::_init_circuit_from_verilog(verilog_pt verilog_ptr) {

  if(verilog_ptr == nullptr) {
    LOG(WARNING) << "Ignore circuit initiation (null verilog object)";
    return;
  }
  
  pin_pt pin_ptr;
  net_pt net_ptr;

  // Step 1: Scan the primary input and create a pin for each primary input port.
  // Each primary input is automatically connected to a net whose name is the same
  // as the name of the primary input.
  for(VerilogModuleIter i(verilog_ptr); i(); ++i) {
    for(VerilogModuleInputIter j(i.module_ptr()); j(); ++j) {
      insert_primary_input(j.input_ptr()->name()); 
    }
  }
  
  // Step 2: Scan the primary output and create a pin for each primary output port.
  // Each primary output is automatically connected to a net whose name is the same
  // as the name of the primary output.
  for(VerilogModuleIter i(verilog_ptr); i(); ++i) {
    for(VerilogModuleOutputIter j(i.module_ptr()); j(); ++j) {
      insert_primary_output(j.output_ptr()->name()); 
    }
  }

  // Step 3: Scan the wires and insert a net for each wire. Each wire is automatically
  // attached to a net. The connection of each net is specified by the gates.
  for(VerilogModuleIter i(verilog_ptr); i(); ++i) {
    for(VerilogModuleWireIter j(i.module_ptr()); j(); ++j) {
      insert_net(j.wire_ptr()->name());
    }
  }

  // Step 4: Scan the gate through verilog file. Insert a new gate for each gate
  // being iterated and a set of pins corresponding to each cellpin of the gate. Then,
  // for each pin-net mapping specified in the gate, connect the pin to the net.
  for(VerilogModuleIter i(verilog_ptr); i(); ++i) {
    for(VerilogModuleGateIter j(i.module_ptr()); j(); ++j) {
      insert_gate(j.gate_ptr()->name(), j.gate_ptr()->cell_name());
      for(VerilogGateCellpinToNetMappingIter k(j.gate_ptr()); k(); ++k) {
        net_ptr = insert_net(k.net_name());                                     
        pin_ptr = circuit_ptr()->pin_ptr(j.gate_ptr()->name() + ":" + k.cellpin_name());
        connect_pin(pin_ptr, net_ptr);
      }
    }
  }
}

// Procedure: _init_io_timing
// This procedure reads the relevant timing information which is needed to propagate the timing
// from the primary inputs and primary outputs.
void_t Timer::_init_io_timing(string_crt fpath) {

  LOG(INFO) << "Loading " + Utility::realpath(fpath);
  
  ifstream_t ifs(fpath);

  LOG_IF(FATAL, !ifs.good()) << "Failed to open " + Utility::realpath(fpath);

  string_t line, token, pi, po, clock;
  float_t at[2][2], slew[2][2], rat[2][2], load, period; 
  
  while (getline(ifs, line)) {
    stringstream ss(line);
    ss >> token;

    if(token == "clock") {
      ss >> pi >> period;
      auto pi_ptr = circuit_ptr()->primary_input_ptr(pi);

      if(pi_ptr == nullptr) {
        LOG(WARNING) << string("Ignore setting clock timing on non-existing PI ") + pi;
        continue;
      }

      circuit_ptr()->clock_tree_ptr()->set_period(period);
      circuit_ptr()->clock_tree_ptr()->set_primary_input_ptr(pi_ptr);
    }
    else if(token == "at") {
      ss >> pi >> at[EARLY][RISE] >> at[EARLY][FALL] >> at[LATE][RISE] >> at[LATE][FALL];
      auto pi_ptr = circuit_ptr()->primary_input_ptr(pi);

      if(pi_ptr == nullptr) {
        LOG(WARNING) << string("Ignore setting arrival time on non-existing PI ") + pi;
        continue;
      }

      set_at(pi_ptr->pin_ptr(), EARLY, RISE, at[EARLY][RISE]);
      set_at(pi_ptr->pin_ptr(), EARLY, FALL, at[EARLY][FALL]);
      set_at(pi_ptr->pin_ptr(), LATE, RISE, at[LATE][RISE]);
      set_at(pi_ptr->pin_ptr(), LATE, FALL, at[LATE][FALL]);
    }
    else if(token == "slew") {
      ss >> pi >> slew[EARLY][RISE] >> slew[EARLY][FALL] >> slew[LATE][RISE] >> slew[LATE][FALL];
      auto pi_ptr = circuit_ptr()->primary_input_ptr(pi);

      if(pi_ptr == nullptr) {
        LOG(WARNING) << string("Ignore setting slew on non-existing PI ") + pi; 
        continue;
      }

      set_slew(pi_ptr->pin_ptr(), EARLY, RISE, slew[EARLY][RISE]);
      set_slew(pi_ptr->pin_ptr(), EARLY, FALL, slew[EARLY][FALL]);
      set_slew(pi_ptr->pin_ptr(), LATE, RISE, slew[LATE][RISE]);
      set_slew(pi_ptr->pin_ptr(), LATE, FALL, slew[LATE][FALL]);
    }
    else if(token == "rat") {
      ss >> po >> rat[EARLY][RISE] >> rat[EARLY][FALL] >> rat[LATE][RISE] >> rat[LATE][FALL];
      auto po_ptr = circuit_ptr()->primary_output_ptr(po);
      
      if(po_ptr == nullptr) {
        LOG(WARNING) << string("Ignore setting required arrival time on non-existing PO ") + po; 
        continue;
      }

      set_rat(po_ptr->pin_ptr(), EARLY, RISE, rat[EARLY][RISE]);
      set_rat(po_ptr->pin_ptr(), EARLY, FALL, rat[EARLY][FALL]);
      set_rat(po_ptr->pin_ptr(), LATE, RISE, rat[LATE][RISE]);
      set_rat(po_ptr->pin_ptr(), LATE, FALL, rat[LATE][FALL]);
    }
    else if(token == "load") {
      ss >> po >> load;
      
      auto po_ptr = circuit_ptr()->primary_output_ptr(po);

      if(po_ptr == nullptr) {
        LOG(WARNING) << string("Ignore setting load on non-existing PO ") + po; 
        continue;
      }

      set_load(po_ptr, EARLY, RISE, load);
      set_load(po_ptr, EARLY, FALL, load);
      set_load(po_ptr, LATE, RISE, load);
      set_load(po_ptr, LATE, FALL, load);
    }
    else {
      LOG(WARNING) << "Unexpected keyword \"" + token + "\" in .timing file (skipped)";
    }
  }

  ifs.close();
}

// Procedure: _update_clock_tree
// Initialize the clock tree.
void_t Timer::_update_clock_tree() {
  circuit_ptr()->clock_tree_ptr()->update_clock_tree();
}

// Procedure: _update_node_level
// Update the level of a node. The procedure automatically adjust the position where the node
// resides in the pipeline in accordence to the new level index.
void_t Timer::_update_node_level(node_pt node_ptr, int_ct level) const {
  
  // Case 1: No change needed.
  if(node_ptr->level() == level) return;

  // Case 2: the node doesn't reside in the pipeline.
  if(!node_ptr->is_in_pipeline()) {
    node_ptr->set_level(level);
    return;
  }

  // Case 3: adjust the node's level and corresponding position in the pipeline.
  _remove_frontier(node_ptr);
  node_ptr->set_level(level);
  _insert_frontier(node_ptr);
}

// Procedure: _update_all_node_levels
// The procedure performs the offline algorithm for levelizing levels of all nodes. The
// offline levelization produces a "compact" level index for each node. If any of the node
// resides in the pipeline, the levelization process will automatically adjust its level
// and reassign it to the correct pipeline location.
void_t Timer::_update_all_node_levels() const {

  int_pt indegs = new int_t [nodeset_ptr()->num_indices()];
  queue <node_pt> que; 
  node_pt u;

  // Initialize the data storage.
  for(const auto& node_ptr : nodeset()) {
    indegs[node_ptr->idx()] = node_ptr->num_fanins();
    if(node_ptr->num_fanins() == 0) {
      _update_node_level(node_ptr, 0);
      que.push(node_ptr);
    }
  }

  // Levelization.
  while(!que.empty()) {
    u = que.front();
    que.pop();
    for(const auto& e : u->fanout()) {
      --indegs[e->to_node_ptr()->idx()];
      if(!indegs[e->to_node_ptr()->idx()]) {
        _update_node_level(e->to_node_ptr(), u->level() + 1);
        que.push(e->to_node_ptr());
      }
    }
  }

  delete [] indegs;
}

// Procedure: _init_full_timing_update
// In order to perform the full timing update, the initialization needs to 1) remove the 
// timing values on every node, 2) remove every node from the pipeline, and 3) insert
// those nodes having zero indegree into the corresponding pipeline.
void_t Timer::_init_full_timing_update() {

  for(const auto& node_ptr : nodeset()) {

    // Remove the node from the pipeline.
    _remove_frontier(node_ptr);

    // Insert the starter to the pipeline.
    if(node_ptr->num_fanins() == 0) {
      node_ptr->set_level(0);
      _insert_frontier(node_ptr);
    }
  }
  
  // Revelize all node levels.
  _update_all_node_levels();
}

// Procedure: update_timing
// Update the timing data on nodes and edges in pipeline fashion. The input flag "is_incremental"
// indicates if the full timing update is needed. If the full timing update is asked, then all
// timing on nodes are removed and only those nodes with zero indegree are inserted into the 
// frontmost pipeline level for propagation. By default, the flag "is_incremental" is true.
void_t Timer::update_timing(bool_t is_incremental) {
  
  // Initialize the data structure and perform the full timing update.
  if(!is_incremental) {
    _init_full_timing_update();
  }

  // The incremental timing is performed only if there exists at least one node in the pipeline.
  if(pipeline().empty()) return;

  // Update the clock tree for any changes on the clock tree.
  _update_clock_tree();

  // Pipeline construction stage. The pipeline is constructed by those nodes that are expanded 
  // from the fanout of thoses nodes residing in the lowest pipeline.
  _update_forward_propagate_pipeline();

  // Forward propagation stage. The forward propagation consists of five major tasks:
  // 1) propagate the rc timing, 2) propagate the slew, 3) propagate the delay, and 4) propagate 
  // the arrival time.
  #pragma omp parallel
  {
    #pragma omp single
    {
      for(auto l=pipeline().min_level(); l<=pipeline().max_level()+4; ++l) {

        // Task 1: RC timing propagation at the level "l"
        #pragma omp task firstprivate(l)
        { _forward_propagate_rc_timing(l); }
    
        // Task 2: Slew propagation at the level "l-1".
        #pragma omp task firstprivate(l)
        { _forward_propagate_slew(l-1); }

        // Task 3: Delay propagation at the level "l-1".
        #pragma omp task firstprivate(l)
        { _forward_propagate_delay(l-1); }
    
        // Task 4: Arrival time propagation at the level "l-2".
        #pragma omp task firstprivate(l)
        { _forward_propagate_at(l-2); }
        
        // Task 5: Jump propagation at the level "l-3".
        #pragma omp task firstprivate(l)
        { _forward_propagate_jump(l-3); }
        
        // Task 6: CPPR credit propagation at the level "l-4".
        #pragma omp task firstprivate(l)
        { _forward_propagate_test(l-4); }

        #pragma omp taskwait
      }
    } 
  } // Implicit synchronization barrier. ----------------------------------------------------------

  // Update test stage. After the forward propagation we have already had the timing data for
  // slew, arrival time, and all rc delay. On the basis of these timing information, we can 
  // correctly infer the tests, which consist of a set of setup and hold time checks. Here we
  // shall only update those tests that are pipelined by the forward propagation.
  //_update_pipelined_tests();

  // Backward propagation stage. The backward propagation consists of two major tasks:
  // 1) propagate the level (inserting all nodes in the fanin cone) and then 2) propagate the
  // required arrival time.
  #pragma omp parallel 
  {
    #pragma omp single 
    {
      for(auto l=pipeline().max_level(); l>=pipeline().min_level(); --l) {

        // Task 1: Insert the fanin cone.
        #pragma omp task firstprivate(l)
        { _backward_propagate_fanin(l); }

        // Task 2: Required arrival time propagation.
        #pragma omp task firstprivate(l)
        { _backward_propagate_rat(l); }
        
        // Synchronization.
        #pragma omp taskwait
      }
    }
  } // Implicit synchronization barrier. ----------------------------------------------------------

  // Trigger lazy evaluation flag.
  _enable_lazy_evaluation();

  // The timing has been up-to-date. We then remove all nodes from the pipeline.
  _remove_all_frontiers();

  CHECK(pipeline_ptr()->num_nodes() == 0);
}

// Procedure: _update_tns
// Update the total negative slack for any transition and timing split. The procedure applies
// the parallel reduction to compute the value.
void_t Timer::_update_tns() {
  EL_RF_ITER(el, rf) {
    _update_tns(el, rf);
  }
}

// Procedure: _update_tns
// Update the total negative slack for a given transition and a timing split. The procedure
// applies the parallel reduction to obtain the vlaue.
void_t Timer::_update_tns(int el, int rf) {
  
  if(_is_tns_updated[el][rf]) return;

  float_t tns(OT_FLT_ZERO);

  #pragma omp parallel for reduction(+ : tns)
  for(unsigned_t i=0; i<nodeset().num_indices(); ++i) {
    if(nodeset()[i] == nullptr || nodeset()[i]->slack(el, rf) >= OT_FLT_ZERO) continue;
    tns = tns + nodeset()[i]->slack(el, rf);
  } // Implicit synchronization barrier. ----------------------------------------------------------

  _tns[el][rf] = tns;
  _is_tns_updated[el][rf] = true;
}

// Procedure: _update_wns
// Update the total negative slack for any transition and timing split. The procedure apply
// the parallel reduction to compute the value.
void_t Timer::_update_wns() {
  EL_RF_ITER(el, rf) {
    _update_wns(el, rf);
  }
}

// Procedure: _update_wns
// The procedure updates the worst negative slack at a given transition and a timing split
// using parallel reduction algorithm.
void_t Timer::_update_wns(int el, int rf) {

  if(_is_wns_updated[el][rf]) return;

  float_t wns(OT_FLT_ZERO);
  
  #pragma omp parallel for reduction(min : wns)
  for(unsigned_t i=0; i<nodeset().num_indices(); ++i) {
    if(nodeset()[i] == nullptr || nodeset()[i]->slack(el, rf) >= OT_FLT_ZERO) continue;
    if(nodeset()[i]->slack(el, rf) < wns) {
      wns = nodeset()[i]->slack(el, rf);
    }
  } // Implicit synchronization barrier. ----------------------------------------------------------

  _wns[el][rf] = wns;
  _is_wns_updated[el][rf] = true;
}

// Function: insert_primary_input
// Create a new primary input port in the design. The function return the pointer to the port.
// Each primary input is associated with a pin and node.
primary_input_pt Timer::insert_primary_input(string_crt name) {

  // Insert a new primary input port into the circuit.
  primary_input_pt pi_ptr = circuit_ptr()->insert_primary_input(name);

  // Create a new pin corresponding to the primary input port.
  pin_pt pin_ptr = circuit_ptr()->insert_pin(name);       // Get the pin pointer.
  pin_ptr->set_direction(PRIMARY_INPUT_PIN_DIRECTION);    // Set the pin direction.
  pin_ptr->set_primary_input_ptr(pi_ptr);                 // Set the pi pointer.
  pi_ptr->set_pin_ptr(pin_ptr);                           // Set the pin pointer.
  node_pt node_ptr = _insert_node(pin_ptr);               // Attach a node to the pin.
  _insert_frontier(node_ptr);                             // Insert the frontier node.

  // Create a new net for the primary input pin.
  net_pt net_ptr = insert_net(name);
  connect_pin(pin_ptr, net_ptr);

  return pi_ptr;
}

// Procedure: splice_driver
// This procedure replace a primary input with a driver. The original primary input will be 
// isolated (not being removed) and the corresponding RC network, if any, is reattached to the
// output pin of the driver. In other words, the output pin of the driver can be conceptually
// viewed as the original primary input.
void_t Timer::splice_driver(string_crt pi_name, string_crt gate_name, string_crt cell_name) {
  
  primary_input_pt pi_ptr = circuit_ptr()->primary_input_ptr(pi_name);

  if(pi_ptr == nullptr) {
    LOG(ERROR) << "Fail to splice driver (non-PI exception)";
    return;
  }
  
  // Insert the gate for the driver.
  gate_pt gate_ptr = insert_gate(gate_name, cell_name); 
  
  // Obtain the driver's output pin.
  string_t driver_output_name; 
  for(CellpinIter i(gate_ptr->cell_ptr()); i(); ++i) {
    if(i.cellpin_ptr()->direction() == OUTPUT_CELLPIN_DIRECTION) {
      driver_output_name = gate_name + ":" + i.cellpin_ptr()->name();
      break;
    }
  }
  
  CHECK(driver_output_name != "");

  // If the primary input pin is already isolated (no net and thus no RC), nothing to be done.
  if(pi_ptr->pin_ptr()->net_ptr() == nullptr) return;

  // Storage declaration.
  pin_pt driver_output_pin_ptr = circuit_ptr()->pin_ptr(driver_output_name);
  net_pt pi_net_ptr = pi_ptr->pin_ptr()->net_ptr();
  rctree_node_pt rctree_node_ptr = pi_ptr->pin_ptr()->rctree_node_ptr();
  rctree_pt rctree_ptr = pi_ptr->pin_ptr()->net_ptr()->rctree_ptr();

  // Net/Pin-level modification. Here we disconnect the PI from its net. Notice that the PI will
  // be disconnected from the RC network as well.
  disconnect_pin(pi_ptr->pin_ptr());
  connect_pin(driver_output_pin_ptr, pi_net_ptr);

  // RC-level modification.
  driver_output_pin_ptr->set_rctree_node_ptr(rctree_node_ptr);
  if(rctree_node_ptr) {
    rctree_node_ptr->set_pin_ptr(driver_output_pin_ptr);

    CHECK(rctree_ptr != nullptr);

    rctree_ptr->rehash_rctree_node(rctree_node_ptr->name(), driver_output_name);
  }
  
  CHECK(pi_ptr->pin_ptr()->node_ptr()->num_fanouts() == 0);
}

// Function: primary_output_ptr
// Query the pointer to a given name of primary output port.
primary_output_pt Timer::primary_output_ptr(string_crt name) const {
  return circuit_ptr()->primary_output_ptr(name);
}

// Function: insert_primary_output
// Create a new primary output port in the design. The function return the pointer to the port.
// Each primary output is associated with a pin and node.
primary_output_pt Timer::insert_primary_output(string_crt name) {

  // Insert a new primary output port into the circuit.
  primary_output_pt po_ptr = circuit_ptr()->insert_primary_output(name);

  // Create a new pin corresponding to the primary output port.
  pin_pt pin_ptr = circuit_ptr()->insert_pin(name);       // Get the pin pointer.
  pin_ptr->set_direction(PRIMARY_OUTPUT_PIN_DIRECTION);   // Set the pin direction.
  pin_ptr->set_primary_output_ptr(po_ptr);                // Set the po pointer.
  po_ptr->set_pin_ptr(pin_ptr);                           // Set the pin pointer.
  _insert_node(pin_ptr);                                  // Attach a node to the pin.

  // Create a new net for the primary output pin.
  net_pt net_ptr = insert_net(name);
  connect_pin(pin_ptr, net_ptr);

  // Create a test for the primary output.
  circuit_ptr()->insert_test(pin_ptr);

  return po_ptr;
}

// Function: gate_ptr
// Query the gate pointer by a given name.
gate_pt Timer::gate_ptr(string_crt gate_name) const {
  return circuit_ptr()->gate_ptr(gate_name);  
}

// Function: insert_gate
// Create a new gate in the design. This newly-created gate is "not yet" connected to
// any other gates or wires. The new gate name is guaranteed not to conflict with any
// exisiting names in the current design.
gate_pt Timer::insert_gate(string_crt gate_name, string_crt cell_name) {
  
  // Insert a new gate.
  gate_pt gate_ptr = circuit_ptr()->insert_gate(gate_name);

  // Assign the cell pointer.
  cell_pt early_cell_ptr = celllib_ptr(EARLY)->cell_ptr(cell_name);
  cell_pt late_cell_ptr = celllib_ptr(LATE)->cell_ptr(cell_name);

  // No cell found for the gate. The gate might be a blockage with cell_name defined
  // in LEF file (optional). 
  if(early_cell_ptr == nullptr) early_cell_ptr = celllib_ptr(EARLY)->insert_cell(cell_name);
  if(late_cell_ptr == nullptr) late_cell_ptr = celllib_ptr(LATE)->insert_cell(cell_name);
  
  gate_ptr->set_cell_ptr(EARLY, early_cell_ptr);            // Gate's cell.
  gate_ptr->set_cell_ptr(LATE, late_cell_ptr);              // Gate's cell.
  
  // Insert a new pin for each cell pin of the gate's cell.
  // Attach a new node for each pin.
  string_t from_pin_name, to_pin_name, pin_name;
  pin_pt from_pin_ptr, to_pin_ptr, pin_ptr;
  timing_arc_pt timing_arc_ptr[2];

  for(CellpinEarlyLateIter i(early_cell_ptr, late_cell_ptr); i(); ++i) {
    pin_name = gate_name + ":" + i.cellpin_name();
    pin_ptr = circuit_ptr()->insert_pin(pin_name);          // Get the cellpin pointer.
    pin_ptr->set_cellpin_ptr(EARLY, i.cellpin_ptr(EARLY));  // Set the cellpin.
    pin_ptr->set_cellpin_ptr(LATE, i.cellpin_ptr(LATE));    // Set the cellpin.
    pin_ptr->set_gate_ptr(gate_ptr);                        // Set the gate.
    pin_ptr->set_direction(i.cellpin_direction());          // Set the direction.
    _insert_node(pin_ptr);                                  // Attach a node to the pin.
  }
  
  // Iterate the timing arc of the cell and insert an edge for each timing arc. Notice here
  // we iterate the cellpin in the cell first and then iterate all timing arcs that connect
  // this cellpin that is being iterated.
  for(CellpinEarlyLateIter i(early_cell_ptr, late_cell_ptr); i(); ++i) {

    // Obtain the "to_pin".
    to_pin_name = gate_ptr->name() + ":" + i.cellpin_name();
    to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);

    // Iterate the "from_pin" that has edge connecting to "to_pin".
    for(TimingArcIter j(i.cellpin_ptr(EARLY), i.cellpin_ptr(LATE)); j(); ++j) {
      timing_arc_ptr[EARLY] = j.timing_arc_ptr(EARLY);
      timing_arc_ptr[LATE ] = j.timing_arc_ptr(LATE);
      from_pin_name = gate_ptr->name() + ":" + j.from_cellpin_name();
      from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);

      _insert_edge(from_pin_ptr, to_pin_ptr, timing_arc_ptr);

      // Insert a test for each constraint arc.
      if(j.is_constraint()) {
        circuit_ptr()->insert_test(from_pin_ptr, to_pin_ptr, timing_arc_ptr);
      }
    }
  }

  return gate_ptr;
}

// Procedure: repower_gate
// Change the size or level of an existing gate, e.g., NAND2_X2 to NAND2_X3. The gate's
// logic function and topology is guaranteed to be the same, along with the currently-connected
// nets. However, the pin capacitances of the new cell type could be different. 
void_t Timer::repower_gate(string_crt gate_name, string_crt cell_name) {
  
  gate_pt gate_ptr = circuit_ptr()->gate_ptr(gate_name);
  cell_pt early_cell_ptr = celllib_ptr(EARLY)->cell_ptr(cell_name);
  cell_pt late_cell_ptr = celllib_ptr(LATE)->cell_ptr(cell_name);

  if(gate_ptr == nullptr || late_cell_ptr == nullptr || early_cell_ptr == nullptr) {
    LOG(ERROR) << "Failed to repower gate (nullptr exception)";
    return;
  }

  // Dummy repower operation - he newly inserted cell is identical to the old one.
  if(early_cell_ptr == gate_ptr->cell_ptr(EARLY) && late_cell_ptr == gate_ptr->cell_ptr(LATE)) return;

  // Data declaration.
  string_t from_pin_name, to_pin_name, pin_name;
  pin_pt from_pin_ptr, to_pin_ptr, pin_ptr;
  edge_pt edge_ptr;
  test_pt test_ptr;
  
  // Modify the pointer fields of the gate.
  gate_ptr->set_cell_ptr(EARLY, early_cell_ptr);            // Gate's cell.
  gate_ptr->set_cell_ptr(LATE, late_cell_ptr);              // Gate's cell.
  
  // Iterate the cellpin of the gate and reassign the mapping between cellpins
  // and pins. The corresponding pin capacitance should be modified either.
  for(CellpinEarlyLateIter i(early_cell_ptr, late_cell_ptr); i(); ++i) {

    // Obtain the pin that is attached to the gate.
    pin_name = gate_name + ":" + i.cellpin_name();
    pin_ptr = circuit_ptr()->pin_ptr(pin_name);             // Get the pin pointer.

    CHECK(pin_ptr != nullptr);
    
    // Remap the cellpin.
    _remap_cellpin(EARLY, pin_ptr, i.cellpin_ptr(EARLY));
    _remap_cellpin(LATE, pin_ptr, i.cellpin_ptr(LATE));
    
    // Insert all fanins into the pipeline. 
    for(auto& e : pin_ptr->node_ptr()->fanin()) {
      _insert_frontier(e->from_node_ptr());
    }
  }

  // Iterate the timing arc of the gate and reassign the mapping between timing arcs
  // and edges. Notice that these internal edges should exist due to the prior insertion
  // of the gate.
  for(CellpinEarlyLateIter i(early_cell_ptr, late_cell_ptr); i(); ++i) {

    // Obtain the "to_pin".
    to_pin_name = gate_ptr->name() + ":" + i.cellpin_name();
    to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);

    // Iterate the edge connecting to "to_pin".
    for(TimingArcIter j(i.cellpin_ptr(EARLY), i.cellpin_ptr(LATE)); j(); ++j) {

      // Obtain the "from_pin" and the edge between "from_pin" and "to_pin".
      from_pin_name = gate_ptr->name() + ":" + j.from_cellpin_name();
      from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);
      edge_ptr = _find_edge_ptr(from_pin_ptr, to_pin_ptr);

      CHECK(edge_ptr != nullptr);
      
      // Remap the timing arc.
      edge_ptr->set_timing_arc_ptr(EARLY, j.timing_arc_ptr(EARLY));
      edge_ptr->set_timing_arc_ptr(LATE, j.timing_arc_ptr(LATE));
      
      // Remap the constraint arc.
      if(j.is_constraint()) {
        test_ptr = to_pin_ptr->test_ptr();
        
        CHECK(test_ptr != nullptr);

        test_ptr->set_timing_arc_ptr(EARLY, j.timing_arc_ptr(EARLY));
        test_ptr->set_timing_arc_ptr(LATE, j.timing_arc_ptr(LATE));
      }
    }
  }
}

// Procedure: remove_gate
// Remove a gate from the current design. This is guaranteed to be called after the gate has 
// been disconnected from the design using pin-level operations. The procedure iterates all 
// pins in the cell to which the gate was attached. Each pin that is being iterated is either
// a cell input pin or cell output pin. In the former case, the pin might have constraint arc
// while in the later case, the ot_pin.has no output connections and all fanin edges should be 
// removed here.
void_t Timer::remove_gate(string_crt gate_name) {
  remove_gate(circuit_ptr()->gate_ptr(gate_name));
}

// Procedure: remove_gate
// Remove a gate from the current design. This is guaranteed to be called after the gate has 
// been disconnected from the design using pin-level operations. The procedure iterates all 
// pins in the cell to which the gate was attached. Each pin that is being iterated is either
// a cell input pin or cell output pin. In the former case, the pin might have constraint arc
// while in the later case, the ot_pin.has no output connections and all fanin edges should be 
// removed here.
void_t Timer::remove_gate(gate_pt gate_ptr) {

  CHECK(gate_ptr != nullptr);

  string_t pin_name;
  pin_pt pin_ptr;

  // Iterate all pins that are attached to the gate/cell and remove the corresponding
  // cell edges. Notice that the gate is assumed to be disconnected from the design.
  for(CellpinIter i(gate_ptr->cell_ptr()); i(); ++i) {
    pin_name = gate_ptr->name() + ":" + i.cellpin_ptr()->name();
    pin_ptr = circuit_ptr()->pin_ptr(pin_name);

    edge_pt edge_ptr;

    switch (pin_ptr->direction()) {
      // Case 1: the pin is an input pin of the cell.
      case INPUT_CELLPIN_DIRECTION:
        if(pin_ptr->node_ptr()->num_fanins() != 0) {
          edge_ptr = pin_ptr->node_ptr()->fanin_ptr()->head()->item();
          
          CHECK(pin_ptr->test_ptr() != nullptr);
          CHECK(pin_ptr->node_ptr()->num_fanins() == 1);
          CHECK(edge_ptr->edge_type() == CONSTRAINT_EDGE_TYPE);

          _remove_edge(edge_ptr);                                       // Remove cell edge.
          circuit_ptr()->remove_test(pin_ptr->test_ptr());              // Remove test.
        }
      break;

      // Case 2: the pin is an output pin of the cell.
      case OUTPUT_CELLPIN_DIRECTION:

        CHECK(pin_ptr->node_ptr()->num_fanouts() == 0);;
        
        // Remove all cell edges injecting to the cell output.
        for(NodeFaninIter j(pin_ptr->node_ptr()); j();) {
          edge_ptr = j.edge_ptr();
          ++j;
          _remove_edge(edge_ptr);
        }
      break;
      
      // Case 3: unexpected keywork.
      default:
        CHECK(false);
      break;
    }
  }

  // Iterate all pins of the gate and remove the pins and nodes. Notice in this stage
  // the pins and nodes are all isolated, i.e., no edges connecting to them.
  for(CellpinIter i(gate_ptr->cell_ptr()); i(); ++i) {
    pin_name = gate_ptr->name() + ":" + i.cellpin_ptr()->name();
    pin_ptr = circuit_ptr()->pin_ptr(pin_name);
    _remove_node(pin_ptr->node_ptr());
    circuit_ptr()->remove_pin(pin_name);
  }

  // Remove the gate from the circuit.
  circuit_ptr()->remove_gate(gate_ptr->name());
}

// Function: net_ptr
// Query the net pointer to a given net name.
net_pt Timer::net_ptr(string_crt net_name) const {
  return circuit_ptr()->net_ptr(net_name);
}

// Procedure: insert_net
// Creates an empty net object with the input "net_name". By default, it will not be connected 
// to any pins and have no parasitics (.spef). This net will be connected to existing pins in 
// the design by the environment "connect_pin" and parasitics will be loaded by the environment
// "read_spef".
net_pt Timer::insert_net(string_crt net_name) {
  net_pt net_ptr = circuit_ptr()->insert_net(net_name);
  return net_ptr;
}

// Procedure: read_spef
// Reads in a .spef file which includes a set of net parasitics. If the spef file contains a
// net that already has parasitics in the design, it should be overwritten. Any .spef files
// will be located in the same directory as the tau2015_wrapper (.tau2015).
void_t Timer::read_spef(string_crt fpath) {
  spef_pt spef_ptr = new spef_t();
  _parse_spef(fpath, spef_ptr);
  _update_spef(spef_ptr);
  delete spef_ptr;  
}

// Procedure: _update_spef
// A subprocedure of the procedure "read_spef". Given a spef object which stores the spef 
// information, this procedure updates the parasitics and timing values based on the spef
// object.
void_t Timer::_update_spef(spef_pt spef_ptr) {

  spefnet_ptr_vrt spefnets = *(spef_ptr->spefnets_ptr());
  
  #pragma omp parallel for schedule(dynamic, 1)
  for(unsigned_t i=0; i<spefnets.size(); ++i) {
    net_pt net_ptr = circuit_ptr()->net_ptr(spefnets[i]->name());

    CHECK(net_ptr != nullptr);

    //cout << "\nnet " << net_ptr->name() << endl;

    // Step 1: Create a new rctree object for this net.
    net_ptr->create_new_rctree();
    rctree_pt rctree_ptr = net_ptr->rctree_ptr();

    // Step 2: Insert the rctree node and assign capacitance value from the CAP section
    // of the .spef file.
    for(SpefCapIter j(spefnets[i]); j(); ++j) {     
      //cout << j.spefcap_ptr()->name() << " " << j.spefcap_ptr()->cap() << endl;
      rctree_ptr->insert_rctree_node(j.spefcap_ptr()->name(), j.spefcap_ptr()->cap());
    }

    // Step 3: Insert the rctree root/leaves and assign the pointer of the rctree node from
    // the PIN section of the .spef file. For each specified pin, the rc node is connected
    // to the pin pointer.
    for(SpefPinIter j(spefnets[i]); j(); ++j) {
      //cout << j.spefpin_ptr()->name() << endl;
      pin_pt pin_ptr = circuit_ptr()->pin_ptr(j.spefpin_ptr()->name());
      
      CHECK(pin_ptr != nullptr);

      rctree_node_pt rctree_node_ptr = rctree_ptr->rctree_node_ptr(j.spefpin_ptr()->name());
      if(rctree_node_ptr == nullptr) {
        rctree_node_ptr = rctree_ptr->insert_rctree_node(j.spefpin_ptr()->name());
      }

      // Attach the pin to the rctree node and vice versa. Also, add the pin capacitance to
      // the rctree node to which the pin is attached. Notice that we only consider the pin 
      // capacitance on taps pins. The pin capacitance on root pin is ignored.
      rctree_node_ptr->set_pin_ptr(pin_ptr);
      pin_ptr->set_rctree_node_ptr(rctree_node_ptr);
      
      if(pin_ptr->is_rctree_root()) rctree_ptr->set_root_ptr(rctree_node_ptr);
      else {
        EL_RF_ITER(el, rf) {
          rctree_node_ptr->add_cap(el, rf, pin_ptr->cap(el, rf));
        }
      }
    }

    // Step 4: Insert the rctree segment and assign the resistance value. Notice that the 
    // default connection of the rctree segment is bidirectional.
    for(SpefResIter j(spefnets[i]); j(); ++j) {
      rctree_ptr->insert_rctree_segment(j.spefres_ptr()->endpoint_name_1(), 
                                        j.spefres_ptr()->endpoint_name_2(), 
                                        j.spefres_ptr()->res());
    }

    // Step 5: Update the Elmore timing of the RC tree.
    rctree_ptr->update_rc_timing();

    // Step 6: Insert the root pin into the pipeline.
    #pragma omp critical
    { _insert_frontier(net_ptr->root_pin_ptr()->node_ptr()); }
  } // Implicit synchronization barrier. ----------------------------------------------------------
}

// Procedure: remove_net
// Remove a net from the current design. By default, if a net is connected to pins, the pins 
// should be automatically disconnected from the net.
void_t Timer::remove_net(string_crt net_name) {
  remove_net(circuit_ptr()->net_ptr(net_name));
}

// Procedure: remove_net
// Remove a net from the current design. By default, if a net is connected to pins, the pins 
// should be automatically disconnected from the net.
void_t Timer::remove_net(net_pt net_ptr) {

  CHECK(net_ptr != nullptr && net_ptr->num_pins() == 0);

  // Disconnect all pins in the net.
  if(net_ptr->num_pins() > 0) {
  vector <pin_pt> pin_ptrs;
    for(NetPinlistIter i(net_ptr); i(); ++i) {
      pin_ptrs.push_back(i.pin_ptr());
    }
    for(unsigned_t i=0; i<pin_ptrs.size(); ++i) {
      disconnect_pin(pin_ptrs[i]);
    }
  }

  // Remove the net from the current design.
  circuit_ptr()->remove_net(net_ptr->name());
}


// Procedure: disconnect_pin
// Disconnect the pin from the net it is connected to. The pin_name will either have the 
// <gate name>:<cell pin name> syntax (e.g., u4:ZN) or be a primary input.
void_t Timer::disconnect_pin(string_crt pin_name) {
  disconnect_pin(circuit_ptr()->pin_ptr(pin_name));
}

// Procedure: disconnect_pin
// Disconnect the pin from the net it is connected to. The pin_name will either have the 
// <gate name>:<cell pin name> syntax (e.g., u4:ZN) or be a primary input.
void_t Timer::disconnect_pin(pin_pt pin_ptr) {

  CHECK(pin_ptr != nullptr);
  
  // Obtain the net to which the pin is connected.
  net_pt net_ptr = pin_ptr->net_ptr();

  if(net_ptr == nullptr) return;

  // Remove edge connections that are associated with the pin and disconnect the pin
  // from the net. Notice we have two cases, either the pin is a root of the net where we
  // need to remove all fanout edges of the root or the pin is a leave of the net where
  // we need to remove the only one edge that connects to the pin.
  pin_pt root_pin_ptr = net_ptr->root_pin_ptr();
  
  // Case 1: the pin is a root of the net (i.e., root of the rctree)
  if(pin_ptr == root_pin_ptr) {

    // Iterate the pinlist and delete the corresponding edge. Notice here we cannot iterate
    // fanout of the node, because the list is not static.
    for(NetPinlistIter i(net_ptr); i(); ++i) {
      if(i.pin_ptr() != root_pin_ptr) {
        _remove_edge(_find_edge_ptr(root_pin_ptr, i.pin_ptr()));
      }
    }
  }
  // Case 2: the pin is not a root of the net.
  else {
    if(root_pin_ptr != nullptr) {
      _remove_edge(_find_edge_ptr(root_pin_ptr, pin_ptr));
    }
  }

  // Enable the clock tree update.
  if(pin_ptr->node_ptr()->is_in_clock_tree()) {
    circuit_ptr()->clock_tree_ptr()->enable_clock_tree_update();
  }
  
  // Remove the pin from the net and enable the rc timing update.
  net_ptr->remove_pin_ptr(pin_ptr);       
}

// Procedure: connect_pin
// Connect the pin to the corresponding net. The pin_name will either have the 
// <gate name>:<cell pin name> syntax (e.g., u4:ZN) or be a primary input. The net name
// will match an existing net read in from a .spef file.
void_t Timer::connect_pin(string_crt pin_name, string_crt net_name) {
  connect_pin(circuit_ptr()->pin_ptr(pin_name), circuit_ptr()->net_ptr(net_name));
}

// Procedure: connect_pin
// Connect the pin to the corresponding net. The pin_name will either have the 
// <gate name>:<cell pin name> syntax (e.g., u4:ZN) or be a primary input. The net name
// will match an existing net read in from a .spef file.
void_t Timer::connect_pin(string_crt pin_name, net_pt net_ptr) {
  connect_pin(circuit_ptr()->pin_ptr(pin_name), net_ptr);
}

// Procedure: connect_pin
// Connect the pin to the corresponding net. The pin_name will either have the 
// <gate name>:<cell pin name> syntax (e.g., u4:ZN) or be a primary input. The net name
// will match an existing net read in from a .spef file.
void_t Timer::connect_pin(pin_pt pin_ptr, net_pt net_ptr) {
   
  CHECK(pin_ptr != nullptr && net_ptr != nullptr);

  if(pin_ptr->net_ptr()) {
    LOG(ERROR) << "Failed to connect a pin to a net (non-disconnected pin exception)";
    return;
  }

  // Connect the pin to the net and construct the edge connections that are associated
  // with this net connection. Notice that are two cases - the given pin is either a root of
  // the net where we need to scan all other pins that remaines in the net and construct
  // the edge connections, or the given pin is a leave of the net where we need to construct 
  // only one edge that connects it to the root of the net.
  net_ptr->insert_pin_ptr(pin_ptr);
  
  // Case 1: the pin is the root of the net.
  if(pin_ptr == net_ptr->root_pin_ptr()) {
    for(NetPinlistIter i(net_ptr); i(); ++i) {
      if(i.pin_ptr() != net_ptr->root_pin_ptr()) {
        _insert_edge(net_ptr->root_pin_ptr(), i.pin_ptr(), net_ptr); 
      }
    }
  }
  // Case 2: the pin is not a root of the net.
  else {
    if(net_ptr->root_pin_ptr() != nullptr) {
      _insert_edge(net_ptr->root_pin_ptr(), pin_ptr, net_ptr);
    }
  }

  // Enable the clock tree update.
  if(pin_ptr->node_ptr()->is_in_clock_tree()) {
    circuit_ptr()->clock_tree_ptr()->enable_clock_tree_update();
  }
}

// Procedure: _remap_cellpin
// Remap the cellpin to which a give pin is mapped. This procedure is called inside the routine
// of repower_gate, where each ot_pin.has to be remapped to the new cellpin that is specified
// by the new cell.
void_t Timer::_remap_cellpin(int el, pin_pt pin_ptr, cellpin_pt cellpin_ptr) {

  CHECK(pin_ptr != nullptr);
  
  // Compute the capacitance difference between the old cellpin and the new cellpin, 
  // and then remap the cellpin to which the pin was mapped.
  float_t delta = cellpin_ptr->capacitance() - pin_ptr->cellpin_ptr(el)->capacitance();
  pin_ptr->set_cellpin_ptr(el, cellpin_ptr);

  // Update the pin capacitance that is pipelined by the new mapping.
  if(!pin_ptr->is_rctree_root()) {
    
    // Leaf capacitance of the rctree.
    if(pin_ptr->rctree_node_ptr()) {
      RF_ITER(rf) {
        pin_ptr->rctree_node_ptr()->add_cap(el, rf, delta);
      }
    }
    
    // Update the leaf capacitance of the net and enable the rc timing update.
    if(pin_ptr->net_ptr()) {
      RF_ITER(rf) {
        pin_ptr->net_ptr()->dupdate_total_leaf_cap(el, rf, delta);
      }
    }
  }
}

// Procedure: _insert_frontier
// Insert a frontier node into the pipeline. The frontier node is the source
// of the incremental timing propagation.
void_t Timer::_insert_frontier(node_pt node_ptr) const {
  pipeline_ptr()->insert(node_ptr); 
}

// Procedure: _remove_frontier
// Remove a frontier node from the pipeline. The frontier node is the source
// of the incremental timing propagation.
void_t Timer::_remove_frontier(node_pt node_ptr) const {
  pipeline_ptr()->remove(node_ptr); 
}

// Function: _insert_node
// Insert a node and attach it to a given pin pointer.
node_pt Timer::_insert_node(pin_pt pin_ptr) {

  if(pin_ptr == nullptr) return nullptr;

  // Case 1: avoid duplicate attachment.
  if(pin_ptr->node_ptr()) {
    LOG(ERROR) << "Failed to attach a node to a pin (duplicate attachment exception)";
    return nullptr;
  }

  // Case 2: attach a node to the pin and vice versa.
  int_t idx = nodeset_ptr()->insert();
  node_pt node_ptr = nodeset()[idx];
  node_ptr->set_idx(idx); 
  node_ptr->set_pin_ptr(pin_ptr);
  node_ptr->set_nodeset_ptr(nodeset_ptr());
  pin_ptr->set_node_ptr(node_ptr);

  return node_ptr;
}

// Function: _insert_edge (cell arc)
// Insert an edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively. 
edge_pt Timer::_insert_edge(string_crt from_pin_name, string_crt to_pin_name, timing_arc_pt timing_arc_ptr[]) {

  pin_pt from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);
  pin_pt to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);
  
  return _insert_edge(from_pin_ptr, to_pin_ptr, timing_arc_ptr);
}

// Function: _insert_edge (cell arc)
// Insert an edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively. 
edge_pt Timer::_insert_edge(pin_pt from_pin_ptr, pin_pt to_pin_ptr, timing_arc_pt timing_arc_ptr[]) {

  if(!from_pin_ptr || !to_pin_ptr) {
    return nullptr;
  }
 
  return _insert_edge(from_pin_ptr->node_ptr(), to_pin_ptr->node_ptr(), timing_arc_ptr);
}

// Function: _insert_edge (cell arc)
// Insert an edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively.
edge_pt Timer::_insert_edge(node_pt from_node_ptr, node_pt to_node_ptr, timing_arc_pt timing_arc_ptr[]) {

  // Case 1: illegal nodes and arc
  if(!from_node_ptr || !to_node_ptr || !timing_arc_ptr[EARLY] || !timing_arc_ptr[LATE]) {
    return nullptr;
  }
    
  // Case 2: create a new edge.
  edge_pt edge_ptr = new edge_t();
  edge_ptr->set_edgelist_satellite(_edgelist_ptr->push_back(edge_ptr));
  edge_ptr->set_from_node_ptr(from_node_ptr);
  edge_ptr->set_to_node_ptr(to_node_ptr);
  EL_ITER(el) {
    edge_ptr->set_timing_arc_ptr(el, timing_arc_ptr[el]);
  }
  from_node_ptr->insert_fanout(edge_ptr);
  to_node_ptr->insert_fanin(edge_ptr);
  
  if(timing_arc_ptr[EARLY]->is_constraint()) edge_ptr->set_edge_type(CONSTRAINT_EDGE_TYPE);
  else edge_ptr->set_edge_type(COMBINATIONAL_EDGE_TYPE);
  
  edge_ptr->set_timing_sense(timing_arc_ptr[EARLY]->timing_sense());

  // Insert the from node into the pipeline as the frontier and remove 
  // the to node from the frontier list.
  _remove_frontier(to_node_ptr);
  _insert_frontier(from_node_ptr);
  to_node_ptr->set_level(max(from_node_ptr->level() + 1, to_node_ptr->level()));

  return edge_ptr;
}

// Function: _insert_edge (cell arc)
// Insert an constraint edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively. The timing arc is referenced by the constraint object.
edge_pt Timer::_insert_edge(string_crt from_pin_name, string_crt to_pin_name, test_pt test_ptr) {
  pin_pt from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);
  pin_pt to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);
  return _insert_edge(from_pin_ptr, to_pin_ptr, test_ptr);
}

// Function: _insert_edge (cell arc)
// Insert an constraint edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively. The timing arc is referenced by the constraint object.
edge_pt Timer::_insert_edge(pin_pt from_pin_ptr, pin_pt to_pin_ptr, test_pt test_ptr) {
  
  if(!from_pin_ptr || !to_pin_ptr) {
    return nullptr;
  }

  return _insert_edge(from_pin_ptr->node_ptr(), to_pin_ptr->node_ptr(), test_ptr);
}

// Function: _insert_edge (cell arc)
// Insert an constraint edge into the design. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively. The timing arc is referenced by the constraint object.
edge_pt Timer::_insert_edge(node_pt from_node_ptr, node_pt to_node_ptr, test_pt test_ptr) {

  if(!from_node_ptr || !to_node_ptr || !test_ptr) {
    return nullptr;
  }

  timing_arc_pt timing_arc_ptr[2] = {test_ptr->timing_arc_ptr(EARLY), test_ptr->timing_arc_ptr(LATE)};
  return _insert_edge(from_node_ptr, to_node_ptr, timing_arc_ptr);
}

// Function: _insert_edge (net arc)
// Insert an edge into the dictionary. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively.
edge_pt Timer::_insert_edge(string_crt from_pin_name, string_crt to_pin_name, net_pt net_ptr) {
  pin_pt from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);
  pin_pt to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);
  return _insert_edge(from_pin_ptr, to_pin_ptr, net_ptr);
}

// Function: _insert_edge (net arc)
// Insert an edge into the dictionary. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively.
edge_pt Timer::_insert_edge(pin_pt from_pin_ptr, pin_pt to_pin_ptr, net_pt net_ptr) {
  if(!from_pin_ptr || !to_pin_ptr) return nullptr;
  return _insert_edge(from_pin_ptr->node_ptr(), to_pin_ptr->node_ptr(), net_ptr);
}

// Function: _insert_edge (net arc)
// Insert an edge into the dictionary. The edge is indexed by the name, which is equal to
// 'from_node_name->to_node_name', where 'from_node_name' and 'to_node_name' are the name objects
// of the node objects, respectively.
edge_pt Timer::_insert_edge(node_pt from_node_ptr, node_pt to_node_ptr, net_pt net_ptr) {

  // Case 1: illegal nodes and arc
  if(!from_node_ptr || !to_node_ptr || !net_ptr) return nullptr;
    
  // Case 2: create a new edge.
  edge_pt edge_ptr = new edge_t();
  edge_ptr->set_edgelist_satellite(_edgelist_ptr->push_back(edge_ptr));
  edge_ptr->set_from_node_ptr(from_node_ptr);
  edge_ptr->set_to_node_ptr(to_node_ptr);
  edge_ptr->set_net_ptr(net_ptr);
  edge_ptr->set_edge_type(RCTREE_EDGE_TYPE);
  edge_ptr->set_timing_sense(POSITIVE_UNATE);
  from_node_ptr->insert_fanout(edge_ptr);
  to_node_ptr->insert_fanin(edge_ptr);

  // Pipeline the node.
  _remove_frontier(to_node_ptr);
  _insert_frontier(from_node_ptr);
  to_node_ptr->set_level(max(from_node_ptr->level() + 1, to_node_ptr->level()));

  return edge_ptr;
}

// Function: _insert_jump
// Insert a jump into the jump set. The jump is indexed by the output index. The 
// jump is the "express pass" from a condensed tree leave to the root.
jump_pt Timer::_insert_jump(pin_pt from_ptr, pin_pt to_ptr, int_t num_negs, float_t d[][2]) {
  if(!from_ptr || !to_ptr) return nullptr;
  return _insert_jump(from_ptr->node_ptr(), to_ptr->node_ptr(), num_negs, d);
}

// Function: _insert_jump
// Insert a jump into the jump set. The jump is indexed by the output index. The 
// jump is the "express pass" from a condensed tree leave to the root.
jump_pt Timer::_insert_jump(node_pt from_ptr, node_pt to_ptr, int_t num_negs, float_t d[][2]) {

  if(!from_ptr || !to_ptr) return nullptr;

  int_t idx = jumpset_ptr()->insert();
  jump_pt jump_ptr = jumpset()[idx];
  jump_ptr->set_idx(idx);
  jump_ptr->set_from_node_ptr(from_ptr);
  jump_ptr->set_to_node_ptr(to_ptr);
  jump_ptr->set_timing_sense((num_negs & 1) ? NEGATIVE_UNATE : POSITIVE_UNATE);
  
  int lrf;

  EL_RF_ITER(el, rf) {
    lrf = (jump_ptr->timing_sense() == NEGATIVE_UNATE) ? !rf : rf;
    jump_ptr->set_delay(el, lrf, rf, d[el][rf]);
  }

  from_ptr->insert_jumpout(jump_ptr);
  to_ptr->insert_jumpin(jump_ptr);

  return jump_ptr;
}

// Procedure: _remove_jump
// Remove all jumps from/to a given node in the timing graph. 
void_t Timer::_remove_jump(node_pt node_ptr) {

  jump_pt jump_ptr;

  // Fanin jump.
  if(node_ptr->num_jumpins()) {
    for(NodeJumpinIter i(node_ptr); i();) {
      jump_ptr = i.jump_ptr();
      ++i;
      _remove_jump(jump_ptr);
    }
  }

  // Fanout jump.
  if(node_ptr->num_jumpouts()) {
    for(NodeJumpoutIter i(node_ptr); i();) {
      jump_ptr = i.jump_ptr();
      ++i;
      _remove_jump(jump_ptr);
    }
  }
}

// Procedure: _remove_jump
// Remove a jump from the jump set and the timing graph. The procedure disconnects the 
// two endpoint nodes and then deletes the jump.
void_t Timer::_remove_jump(jump_pt jump_ptr) {
  if(jump_ptr == nullptr) return;
  jump_ptr->from_node_ptr()->remove_jumpout(jump_ptr);
  jump_ptr->to_node_ptr()->remove_jumpin(jump_ptr);
  jumpset_ptr()->remove(jump_ptr->idx());
}

// Function: _find_edge_ptr
// The function finds the edge that connects the given two endpoints and returns the pointer
// pointing to this edge. The two endpoints can be speficied by name, pin pointer, or node
// pointer.
edge_pt Timer::_find_edge_ptr(string_crt from_pin_name, string_crt to_pin_name) {
  pin_pt from_pin_ptr = circuit_ptr()->pin_ptr(from_pin_name);
  pin_pt to_pin_ptr = circuit_ptr()->pin_ptr(to_pin_name);
  return _find_edge_ptr(from_pin_ptr, to_pin_ptr);
}

// Function: _find_edge_ptr
// The function finds the edge that connects the given two endpoints and returns the pointer
// pointing to this edge. The two endpoints can be speficied by name, pin pointer, or node
// pointer.
edge_pt Timer::_find_edge_ptr(pin_pt from_pin_ptr, pin_pt to_pin_ptr) {
  if(!from_pin_ptr || !to_pin_ptr) return nullptr;
  return _find_edge_ptr(from_pin_ptr->node_ptr(), to_pin_ptr->node_ptr());
}

// Function: _find_edge_ptr
// The function finds the edge that connects the given two endpoints and returns the pointer
// pointing to this edge. The two endpoints can be speficied by name, pin pointer, or node
// pointer.
edge_pt Timer::_find_edge_ptr(node_pt from_node_ptr, node_pt to_node_ptr) {

  if(!from_node_ptr || !to_node_ptr) return nullptr;

  edgelist_pt edgelist_ptr;
  
  // Obtain the edgelist with smaller size, which could be better for later search.
  if(from_node_ptr->num_fanouts() > to_node_ptr->num_fanins()) {
    edgelist_ptr = from_node_ptr->fanout_ptr();
  }
  else {
    edgelist_ptr = to_node_ptr->fanin_ptr();
  }
  
  // Search the edge.
  for(const auto& e : *edgelist_ptr) {
    if(e->from_node_ptr() == from_node_ptr && e->to_node_ptr() == to_node_ptr)  {
      return e;
    }
  }

  return nullptr;
}

// Procedure: remove_node
// Remove a node from the design. The node must be disconnected from the design using the
// procedure "_remove_edge" before it can be removed and deleted. Notice that if the node
// is deleted from the design, it should be removed from the pipeline if it was inserted 
// to the pipeline before. By default, the node is isolated.
void_t Timer::_remove_node(node_pt node_ptr) {
  _remove_frontier(node_ptr);
  nodeset_ptr()->remove(node_ptr->idx());
}

// Procedure: remove_edge
// Remove an edge from the design. The procedure first disconnects the edge from its two ending
// nodes, "from_node" and "to_node". Then it removes the edge from the design and insert both
// "from_node" and "to_node" into the pipeline.
void_t Timer::_remove_edge(edge_pt edge_ptr) {

  _remove_jump(edge_ptr->from_node_ptr());
  _remove_jump(edge_ptr->to_node_ptr());
  edge_ptr->from_node_ptr()->remove_fanout(edge_ptr);
  edge_ptr->to_node_ptr()->remove_fanin(edge_ptr);
  _insert_frontier(edge_ptr->to_node_ptr());
  _insert_frontier(edge_ptr->from_node_ptr());
  edgelist_ptr()->remove(edge_ptr->edgelist_satellite());

  delete edge_ptr;
}

// Procedure: _update_forward_propagate_pipeline
// The procedure builds the pipeline for forward propagation. Starting from nodes in the lowest
// level, the procedure iteratively updates the level of these nodes based on the fanin level.
// Then the node insert all fanout of each node that was being iterated. In other words, in the
// end of the procedure, the pipelins consists of all the fanout nodes of those initial node 
// residing in the lowest starting level.
void_t Timer::_update_forward_propagate_pipeline() {
  
  for(int_t l=pipeline_ptr()->min_level(); l<=pipeline_ptr()->max_level(); ++l) {
    _forward_propagate_fanout(l);
  }
}

// Procedure: _forward_propagate_fanout
// Iterate the nodelist in a given level and levelize the fanout of each node in the list.
// Meanwhile, such fanout nodes are inserted into the pipeline after it has been levelized.
void_t Timer::_forward_propagate_fanout(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  // Iterate the node in the nodelist.
  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    _forward_propagate_fanout(i.node_ptr()); 
  }
}

// Procedure: _forward_propagate_fanout
// Levelize the fanout of a given node pointer. Once the levels of fanout nodes are levelized, 
// they are inserted into the pipeline for the forward timing propagation.
void_t Timer::_forward_propagate_fanout(node_pt node_ptr) {

  if(node_ptr == nullptr) return;
  
  int_t next_level = node_ptr->level() + 1;

  // Insert the fanout of the node and adjust the level of these fanout nodes. Notice that
  // we should be careful about the case where the fanout node resides in the same level
  // and overlaps the iterator.
  for(auto& e : node_ptr->fanout()) {
    if(next_level > e->to_node_ptr()->level()) {
      _remove_frontier(e->to_node_ptr());
      e->to_node_ptr()->set_level(next_level);
    }
    _insert_frontier(e->to_node_ptr());
  }
}

// Procedure: _forward_propagate_rc_timing
// Update the elmore timing of a rctree that is attached to a rctree root pin.
void_t Timer::_forward_propagate_rc_timing(int_t level) {
 
  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;
  
  // Iterate the node in the nodelist.
  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    _forward_propagate_rc_timing(i.node_ptr()); 
  }
}

// Procedure: _forward_propagate_rc_timing
// Update the elmore timing of a rctree. The procedure takes a node pointer, checking if 
// it is a root of the rctree. If yes, then it performs rc timing update.
void_t Timer::_forward_propagate_rc_timing(node_pt node_ptr) {
  
  if(node_ptr == nullptr) return;

  if(!node_ptr->is_rctree_root()) return;
  net_pt net_ptr = node_ptr->pin_ptr()->net_ptr(); 

  if(net_ptr == nullptr || net_ptr->rctree_ptr() == nullptr) return;
  net_ptr->rctree_ptr()->update_rc_timing();
}

// Procedure: _forward_propagate_delay
// Update the delay of the fanin edges to a given level of the pipeline. The procedure first 
// obtains the nodelist from the given pipeline level and then perform "_forward_propagate_delay" on
// every node in the nodelist.
void_t Timer::_forward_propagate_delay(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    _forward_propagate_delay(i.node_ptr());
  }
}

// Procedure: _forward_propagate_delay
// Update the delay of the fanin edges to a given node pointer "to_node_ptr".
// The delay propagation consists of two cases 1) propagate the delay from a RC connection 
// and 2) propagate the delay through a circuit cell element. 
void_t Timer::_forward_propagate_delay(node_pt to_node_ptr) {
  
  if(to_node_ptr == nullptr) {
    return;
  }

  rctree_pt rctree_ptr;
  timing_arc_pt timing_arc_ptr;
  float_t CL, si;

  // Scal the fanin of the node "to_node_ptr" and update the delay.
  for(auto& e : to_node_ptr->fanin()) {

    switch(e->edge_type()) {

      // Case 1: RC tree propagation.
      case RCTREE_EDGE_TYPE:

        rctree_ptr = e->net_ptr()->rctree_ptr();
        if(rctree_ptr != nullptr) {                      
          rctree_node_pt to_rctree_node_ptr = to_node_ptr->pin_ptr()->rctree_node_ptr();

          EL_RF_RF_ITER(el, irf, orf) {
            e->set_delay(el, irf, orf, to_rctree_node_ptr->delay(el, orf));
          }
        }
      break;

      // Case 2: Combinational propagation.
      case COMBINATIONAL_EDGE_TYPE:
        
        EL_RF_RF_ITER(el, irf, orf) {
          timing_arc_ptr = e->timing_arc_ptr(el);

          // load capacitance at node to_node_ptr
          CL = to_node_ptr->pin_ptr()->load(el, orf);
          si = e->from_node_ptr()->slew(el, irf);

          if(!timing_arc_ptr->is_transition_defined(irf, orf)) continue;

          e->set_delay(el, irf, orf, timing_arc_ptr->delay(irf, orf, si, CL));
        }
      break;

      // Case 3: Constraint propagation.
      case CONSTRAINT_EDGE_TYPE:
      break;

      // Case 4: Assertion failure.
      default:
        CHECK(false);
      break;
    }
  }
}

// Procedure: _forward_propagate_at
// Update the arrival time of all nodes to a given level in the pipeline. The procedure obtains
// the nodelist in a given level to the pipeline and then perform "_forward_propagate_at" on each node
// in the nodelist.
void_t Timer::_forward_propagate_at(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  for(auto& node_ptr : *nodelist_ptr) {
    _forward_propagate_at(node_ptr);
  }
}

// Procedure: _forward_propagate_at
// Update the arrival time of a given node pointer "to_node_ptr" via fanin propagation.
// The arrival time propagation consists of two cases 1) propagate the arrival time from a 
// RC connection and 2) propagate the arrival time through a circuit cell element. 
void_t Timer::_forward_propagate_at(node_pt to_node_ptr) {
  
  if(to_node_ptr == nullptr) {
    return;
  }

  // No fanin.
  if(to_node_ptr->num_fanins() == 0) {
    if(to_node_ptr->is_primary_input()) {
      EL_RF_ITER(el, rf) {
        to_node_ptr->set_at(el, rf, to_node_ptr->pin_ptr()->primary_input_ptr()->at(el, rf));
        if(_clock_tree_root_node_ptr() == to_node_ptr) {
          to_node_ptr->set_is_at_clocked(el, rf, true);
        }
      }
    }
    return;
  }
  
  // Remove the arrival time on nodes.
  to_node_ptr->remove_at();

  timing_arc_pt timing_arc_ptr;
  
  // Non-pi nodes - update the arrival time through the value from the fanin nodes.
  // Scan the fanin of the node "to_node_ptr" and update the arrival time.
  for(auto& e : to_node_ptr->fanin()) {

    switch(e->edge_type()) {
      // Case 1: RC tree propagation.
      case RCTREE_EDGE_TYPE:
        EL_RF_ITER(el, rf) {
          to_node_ptr->relax_at(el, rf, rf, e);
        }
      break;
      // Case 2: Combinational propagation.
      case COMBINATIONAL_EDGE_TYPE:
        EL_RF_RF_ITER(el, irf, orf) {
          timing_arc_ptr = e->timing_arc_ptr(el);
          
          if(!timing_arc_ptr->is_transition_defined(irf, orf)) continue;

          to_node_ptr->relax_at(el, irf, orf, e);
        }
      break;
      // Case 3: Constraint.
      case CONSTRAINT_EDGE_TYPE:
      break;

      // Case 4: Assertion failure.
      default:
        LOG(ERROR) << "Failed to propagate AT (EdgeType exception)";
        return;
      break;
    }
  }                 
}

// Procedure: _backward_propagate_fanin
// Propagate the level and insert all fanin cone of the nodelist to a given level. This procedure
// is called before the propagation of required arrival time, which constructs the nodes that need
// to update the rat value.
void_t Timer::_backward_propagate_fanin(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    _backward_propagate_fanin(i.node_ptr());
  }
}

// Procedure: _backward_propagate_fanin
// Propagate the level and insert all fanin cone of the node. This procedure is called before the 
// propagation of required arrival time, which constructs the nodes that need to update the rat 
// value. All the levels should be legal now after the forward propagation.
void_t Timer::_backward_propagate_fanin(node_pt to_node_ptr) {
  
  if(to_node_ptr == nullptr) {
    return;
  }

  for(auto& e : to_node_ptr->fanin()) {
    _insert_frontier(e->from_node_ptr());
  }
}

// Procedure: _backward_propagate_rat
// Update the required arrival time of a given node list "nodelist_ptr" via fanout propagation.
// The procedure iterate the node in the nodelist and call "_backward_propagate_rat" on it.
void_t Timer::_backward_propagate_rat(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;
  for(auto& node_ptr : *nodelist_ptr) {
    _backward_propagate_rat(node_ptr);
  }
}

// Procedure: _backward_propagate_rat
// Update the required arrival time of a given node pointer "to_node_ptr" via fanout propagation.
// The arrival time propagation consists of two cases 1) propagate the arrival time from a RC
// connection and 2) propagate the required arrival time through a circuit cell element. 
void_t Timer::_backward_propagate_rat(node_pt from_node_ptr) {
 
  if(from_node_ptr == nullptr) {
    return;
  }

  // No fanout - obtain rat from 1) primary output or 2) hold/setup test at FF.
  if(from_node_ptr->num_fanouts() == 0) {

    // The node is constrained by a test, which could be either primary output RAT
    // or the setup/hold from a FF where we need to compute the RAT with CPPR.
    if(from_node_ptr->is_constrained()) {
      test_pt test_ptr = from_node_ptr->pin_ptr()->test_ptr();
      EL_RF_ITER(el, rf) {
        // EARLY split slack adjustment for RAT.
        if(el == EARLY) {
          from_node_ptr->set_rat(el, rf, from_node_ptr->at(el, rf) - test_ptr->slack(el, rf));
        }
        // LATE mode slack adjustment for RAT.
        else {
          from_node_ptr->set_rat(el, rf, from_node_ptr->at(el, rf) + test_ptr->slack(el, rf));
        }
      }
    }
    return;
  }
  
  // Remove the required arrival time on nodes.
  from_node_ptr->remove_rat();

  timing_arc_pt timing_arc_ptr;
  float_t rat;
  
  // Have fanout - Scan the fanout of the node "from_node_ptr" and update the rat.
  for(auto& e : from_node_ptr->fanout()) {
    
    switch(e->edge_type()) {
      // Case 1: RC propagation.
      case RCTREE_EDGE_TYPE:
        EL_RF_ITER(el, rf) {
          from_node_ptr->relax_rat(el, rf, e->to_node_ptr()->rat(el, rf) - e->delay(el, rf, rf));
        }
      break;
      // Case 2: Combinational propagation.
      case COMBINATIONAL_EDGE_TYPE:
        EL_RF_RF_ITER(el, irf, orf) {
          timing_arc_ptr = e->timing_arc_ptr(el);

          if(!timing_arc_ptr->is_transition_defined(irf, orf)) continue;

          rat = e->to_node_ptr()->rat(el, orf) - e->delay(el, irf, orf); 
          from_node_ptr->relax_rat(el, irf, rat);
        }
      break;
      // Case 3: Constraint propagation.
      case CONSTRAINT_EDGE_TYPE:

        CHECK(from_node_ptr->is_clock_sink());

        EL_RF_RF_ITER(el, ck_rf, d_rf) {
          timing_arc_ptr = e->timing_arc_ptr(el);
          
          if(!timing_arc_ptr->is_transition_defined(ck_rf, d_rf)) continue;

          if(el == EARLY) {
            rat = from_node_ptr->at(LATE, ck_rf) + e->to_node_ptr()->slack(EARLY, d_rf);
            from_node_ptr->relax_rat(LATE, ck_rf, rat);
          }
          else {
            rat = from_node_ptr->at(EARLY, ck_rf) - e->to_node_ptr()->slack(LATE, d_rf);
            from_node_ptr->relax_rat(EARLY, ck_rf, rat);
          }
        }
      break;  
      default:
        LOG(ERROR) << "Failed to propagate RAT (EdgeType exception)";
        return;
      break;
    }
  }                                                         // End of fanin iterator.
}

// Procedure: _forward_propagate_slew
// Update the slew of a given level. The procedure obtains the nodelist from the pipeline to
// the given level and then perform "_forward_propagate_slew" to each node in the nodelist.
void_t Timer::_forward_propagate_slew(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    _forward_propagate_slew(i.node_ptr());
  }
}

// Procedure: _forward_propagate_slew
// Update the slew of a given node pointer "to_node_ptr" via fanin propagation.
// The slew propagation consists of two cases 1) propagate the slew from a RC connection 
// and 2) propagate the slew through a circuit cell element. 
void_t Timer::_forward_propagate_slew(node_pt to_node_ptr) {

  if(to_node_ptr == nullptr) return;

  // No fanin - obtain the slew from primary input.
  if(to_node_ptr->num_fanins() == 0) {
    if(to_node_ptr->is_primary_input()) {
      EL_RF_ITER(el, rf) {
        to_node_ptr->set_slew(el, rf, to_node_ptr->pin_ptr()->primary_input_ptr()->slew(el, rf));
      }
    }
    return;
  }
  
  // Remove the slew on the nodes.
  to_node_ptr->remove_slew();

  rctree_pt rctree_ptr;
  timing_arc_pt timing_arc_ptr;
  float_t CL;
  float_t so;

  // Scan the fanin of the node "to_node_ptr" and update the slew.
  for(auto& e : to_node_ptr->fanin()) {

    switch(e->edge_type()) {
      // Case 1: RC propagation.
      case RCTREE_EDGE_TYPE:
        rctree_ptr = e->net_ptr()->rctree_ptr();
        if(rctree_ptr == nullptr) {                               // Undefined rctree. 
          EL_RF_ITER(el, rf) {
            to_node_ptr->relax_slew(el, rf, e->from_node_ptr()->slew(el, rf));
          }
        }
        else {                                                 // Propagate the slew.
          EL_RF_ITER(el, rf) {
            float_t rctree_si = e->from_node_ptr()->slew(el, rf);
            float_t rctree_so = rctree_ptr->slew(to_node_ptr->pin_ptr()->rctree_node_ptr(), el, rf, rctree_si);
            to_node_ptr->relax_slew(el, rf, rctree_so);
          }
        }
      break;
      // Case 2: Combinational propagation.
      case COMBINATIONAL_EDGE_TYPE:
        EL_RF_RF_ITER(el, irf, orf) {

          timing_arc_ptr = e->timing_arc_ptr(el);
          
          if(!timing_arc_ptr->is_transition_defined(irf, orf)) continue;

          CL = to_node_ptr->pin_ptr()->load(el, orf);
          so = timing_arc_ptr->slew(irf, orf, e->from_node_ptr()->slew(el, irf), CL);
          to_node_ptr->relax_slew(el, orf, so);
        }
      break;
      // Case 3: Constraint.
      case CONSTRAINT_EDGE_TYPE:
      break;
      // Case 4: Assertion failure.
      default:
        LOG(ERROR) << "Failed to propagate slew (EdgeType exception)";
        return;
      break;
    }
  }         
}

// Procedure: _forward_propagate_jump
// The procedure constructs the jump to a given node. The jump exists only when the given
// node is a head of the jump, where we have to traverse through the fanin cone of the given
// node to obtain a tree and then build a jump from every leave to the given root node.
void_t Timer::_forward_propagate_jump(int_t level) {

  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(level);

  if(nodelist_ptr == nullptr) return;

  for(auto& node_ptr : *(nodelist_ptr)) {
    _forward_propagate_jump(node_ptr);
  }
}

// Procedure: _forward_propagate_jump
// The procedure constructs the jump to a given node. The jump exists only when the given
// node is a head of the jump, where we have to traverse through the fanin cone of the given
// node to obtain a tree and then build a jump from every leave to the given root node.
void_t Timer::_forward_propagate_jump(node_pt node_ptr) {

  if(node_ptr == nullptr) return;

  // Remove all the jumps that are previously attached to this node.
  _remove_jump(node_ptr);
  
  // Return if the give node is not a head.
  if(/*node_ptr->is_in_clock_tree() || */!node_ptr->is_jump_head()) return;

  //printf("jump head found: %s\n", node_ptr->pin_ptr()->name().c_str());
  
  // Construct the fanin tree and then connect jumps from all leaves to the root.
  float_t d[2][2] = {
    {OT_DEFAULT_EDGE_DELAY, OT_DEFAULT_EDGE_DELAY},
    {OT_DEFAULT_EDGE_DELAY, OT_DEFAULT_EDGE_DELAY}
  };
  
  // Backward induce the jump edge to the node_ptr.
  _induce_jump(node_ptr, node_ptr, 0, d);
}

// Procedure: _induce_jump
// The procedure constructs the jump to a given node. The jump exists only when the given
// node is a head of the jump, where we have to traverse through the fanin cone of the given
// node to obtain a tree and then build a jump from every leave to the given root node.
void_t Timer::_induce_jump(node_pt v, node_pt root, int_t negs, float_t dv[][2]) {
  
  // Already reach the jump tail. Notice that we shouldn't consider the "loop-like" jump.
  if(v->is_jump_tail()) {
    if(v != root) {
      _insert_jump(v, root, negs, dv);     
    }
    return;
  }

  float_t du[2][2];
  int vrf;
  
  // Recursively traverse the fanin of the nodes.
  for(auto& e : v->fanin()) {

    if(e->edge_type() == CONSTRAINT_EDGE_TYPE /*|| e->from_node_ptr()->is_in_clock_tree()*/) continue;

    switch(e->timing_sense()) {
      
      case NEGATIVE_UNATE:
        EL_RF_ITER(el, rf) {
          vrf = rf ^ (negs & 1);
          du[el][rf] = dv[el][rf] + e->delay(el, !vrf, vrf);
        }
        _induce_jump(e->from_node_ptr(), root, negs + 1, du);
      break;
      
      case POSITIVE_UNATE:
        EL_RF_ITER(el, rf) {
          vrf = rf ^ (negs & 1);
          du[el][rf] = dv[el][rf] + e->delay(el, vrf, vrf);
        }
        _induce_jump(e->from_node_ptr(), root, negs, du);
      break;

      default:
        LOG(ERROR) << "Failed to induce jump (EdgeType exception)";
        return;
      break;
    }  
  }
}

// Procedure: _update_pipelined_tests
// The procedure propagate the arrival time and slew for a constrained timing arc ending at the
// D pin/node of a sequential element (i.e., flip-flop). The required arrival time is defined
// with respect to the setup/hold timing constraint. In order to keep track of the most critical
// post-CPPR slack path, the required arrival time will be offseted by the cppr credit.
void_t Timer::_update_pipelined_tests() {
  
  test_ptr_vt test_ptrs;
  test_ptrs.reserve(num_tests());
  
  // Obtain those tests which are pipelined by the optimization transform.
  for(const auto& t : *(circuit_ptr()->testset_ptr())) {
    if(t->is_in_pipeline()) {
      test_ptrs.push_back(t);
    }
  }

  #pragma omp parallel for schedule(dynamic, 1)
  for(unsigned_t i=0; i<test_ptrs.size(); ++i) {
    EL_RF_ITER(el, rf) {
      _forward_propagate_test(test_ptrs[i]); 
    }
  } // Implicit synchronization barrier. ----------------------------------------------------------
}

// Procedure: _forward_propagate_test
// The procedure propagate the arrival time and slew for a constrained timing arc ending at the
// D pin/node of a sequential element (i.e., flip-flop). The required arrival time is defined
// with respect to the setup/hold timing constraint. In order to keep track of the most critical
// post-CPPR slack path, the required arrival time will be offseted by the cppr credit.
void_t Timer::_forward_propagate_test(int_t l) {
  
  nodelist_pt nodelist_ptr = pipeline_ptr()->nodelist_ptr(l);

  if(nodelist_ptr == nullptr) return;

  for(NodelistIter i(nodelist_ptr); i(); ++i) {
    if(i.node_ptr()->is_constrained()) {
      #pragma omp task
      { _forward_propagate_test(i.node_ptr()->pin_ptr()->test_ptr()); }
    }
  }
}

// Procedure: _forward_propagate_test
// The procedure propagate the arrival time and slew for a constrained timing arc ending at the
// D pin/node of a sequential element (i.e., flip-flop). The required arrival time is defined
// with respect to the setup/hold timing constraint. In order to keep track of the most critical
// post-CPPR slack path, the required arrival time will be offseted by the cppr credit.
void_t Timer::_forward_propagate_test(test_pt test_ptr) {
  
  if(test_ptr == nullptr) return;
  
  // Update the pre-CPPR rat of the test.
  test_ptr->update_rat(circuit_ptr()->clock_tree_ptr());

  // Update the pre-CPPR slack of the test. 
  EL_RF_ITER(el, rf) {
    test_ptr->endpoint_ptr(el, rf)->update_slack();
  }

  // Obtain the worst post cppr slack for the testing D pin in el/rf status and then adjust the
  // required arrival time appropriately. Notice that we only care about the non-positive post
  // cppr worst slack. That is, if the pre CPPR slack is already positive we won't do anything.
  if(test_ptr->is_sequential_constraint()) {
    EL_RF_ITER(el, rf) {
      if(test_ptr->endpoint_ptr(el, rf)->slack() <= cutoff_slack(el, rf)) {
        if(test_ptr->constrained_node_ptr()->is_at_clocked(el, rf)) {
          CPPR cppr(test_ptr->endpoint_ptr(el, rf), nullptr, circuit_ptr()->clock_tree_ptr(), 1);
          test_ptr->endpoint_ptr(el, rf)->set_slack(cppr.report_worst_slack(1));
        }
      }
    }
  }
  
  // Update the heap position for each endpoint of the test.
  #pragma omp critical
  {
    EL_RF_ITER(el, rf) {
      if(test_ptr->endpoint_ptr(el, rf)->slack() > cutoff_slack(el, rf)) {
        endpoint_minheap_ptr()->remove(test_ptr->endpoint_ptr(el, rf));
      }
      else {
        endpoint_minheap_ptr()->update(test_ptr->endpoint_ptr(el, rf));
      }
    }
  }
}

// Procedure: _get_fanout_test_ptrs
// Obtain the tests that are residing in the fanout cone of a given pin. If the given pin is nullptr,
// we need to have all tests involved.
void_t Timer::_get_fanout_test_ptrs(pin_pt through_pin_ptr, test_ptr_vrt test_ptrs) {
  
  test_ptrs.clear();
  
  // The given pin pointer is a nullptr pointer.
  if(through_pin_ptr == nullptr) {
    for(const auto& t : circuit_ptr()->testset()) {
      test_ptrs.push_back(t);
    }
    return;
  }

  // Perform the BFS to obtain all tests covered in the fanout cone of the pin.
  node_pt u, v;
  bool_vt visited(num_nodes(), false); 
  queue < node_pt > que;

  u = through_pin_ptr->node_ptr();
  que.push(u);
  visited[u->idx()] = true;

  while(!que.empty()) {

    u = que.front();
    que.pop();

    if(u->is_constrained()) {
      test_ptrs.push_back(u->pin_ptr()->test_ptr());
    }

    for(auto& e : u->fanout()) {
      v = e->to_node_ptr();
      if(e->edge_type() == CONSTRAINT_EDGE_TYPE) continue;
      if(!visited[v->idx()]) {
        que.push(v);
        visited[v->idx()] = true;
      }
    }
  }
}

// Procedure: _get_endpoint_ptrs
// Extract the endpoints from a given vector of tests. A endpoint is defined with respect to a
// given test with timing split (early or late) and timing transition (rise or fall) specified
// explicitly. That is, a test can contribute to up to four endpoints.
void_t Timer::_get_endpoint_ptrs(test_ptr_vrt test_ptrs, endpoint_ptr_vrt endpoint_ptrs) {
  
  // Storage initialization.
  endpoint_ptrs.clear();
  endpoint_ptrs.reserve(test_ptrs.size() << 2);

  // Extract the endpoint pointers from the tests.
  for(unsigned_t i=0; i<test_ptrs.size(); ++i) {
    EL_RF_ITER(el, rf) {
      endpoint_ptrs.push_back(test_ptrs[i]->endpoint_ptr(el, rf)); 
    }
  }
}

// Procedure: _update_endpoint_slacks
// Update the slack at each endpoint. The procedure copies the slack from the node to the test
// point if the given through pin is a nullptr pointer. Otherwise, the we need to reperform the CPPR
// analysis by specifying the through pin as it might affect the true CPPR slack.
void_t Timer::_update_endpoint_slacks(pin_pt through_pin_ptr, endpoint_ptr_vrt endpoint_ptrs){
  
  // The through  pin is not specified where we can directly copy the node slack to the
  // endpoint slack.
  if(through_pin_ptr == nullptr) return;

  // The through pin is specified where we need to recompute the true worst cppr slack with respect 
  // to a given pin_ptr.
  #pragma omp parallel for schedule(dynamic, 1)
  for(unsigned_t i=0; i<endpoint_ptrs.size(); ++i) {
    if(endpoint_ptrs[i]->slack() > cutoff_slack(endpoint_ptrs[i]->el(), endpoint_ptrs[i]->rf()) || 
       through_pin_ptr->test_ptr()) continue;
    CPPR cppr(endpoint_ptrs[i], through_pin_ptr, circuit_ptr()->clock_tree_ptr(), 1);
    endpoint_ptrs[i]->set_slack(cppr.report_worst_slack(1));
  } // Implicit synchronization barrier. ----------------------------------------------------------
}

// Function: slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current design,
// i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and respectively 
// specify the desired transition. Options -early and -late are mutually exclusive, and 
// respectively specify the desired mode. By default the -early -rise options are selected. 
float_t Timer::slew(string_crt pin_name, int el, int rf) {
  return slew(circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Function: slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current design,
// i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and respectively 
// specify the desired transition. Options -early and -late are mutually exclusive, and 
// respectively specify the desired mode. By default the -early -rise options are selected. 
float_t Timer::slew(pin_pt pin_ptr, int el, int rf) {
  
  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Failed to get slew (nullptr exception)";
    return Utility::undefined_slew(el);
  }

  update_timing();

  return pin_ptr->node_ptr()->slew(el, rf);
}

// Function: at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
float_t Timer::at(string_crt pin_name, int el, int rf) {
  return at(circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Function: at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
float_t Timer::at(pin_pt pin_ptr, int el, int rf) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Failed to get arrival time (nullptr exception)";
    return Utility::undefined_at(el);
  }

  update_timing();
  return pin_ptr->node_ptr()->at(el, rf);
}

// Procedure: report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_at(int argc, char **argv) {
  _report_at(environment_ptr()->rpt_fptr(), argc, argv);
}

// Procedure: report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_at(string_crt pin_name, int el, int rf) {
  _report_at(environment_ptr()->rpt_fptr(), pin_name, el, rf);
}

// Procedure: report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_at(pin_pt pin_ptr, int el, int rf) {
  _report_at(environment_ptr()->rpt_fptr(), pin_ptr, el, rf);
}

// Procedure: _report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_at(file_pt fptr, int argc, char **argv) {

  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    ++i;
  }
  _report_at(fptr, pin_name, el, rf);
}

// Procedure: _report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_at(file_pt fptr, string_crt pin_name, int el, int rf) {
  _report_at(fptr, circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Procedure: _report_at
// Report the arrival time in picoseconds at a given pin name, where the pin will be in the
// current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_at(file_pt fptr, pin_pt pin_ptr, int el, int rf) {
  float_t numeric;
  __tau2015_numeric_guard(numeric = at(pin_ptr, el, rf));
  fprintf(fptr, "%.6f\n", numeric);
}

// Function: rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
float_t Timer::rat(string_crt pin_name, int el, int rf) {
  return rat(circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Function: rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
float_t Timer::rat(pin_pt pin_ptr, int el, int rf) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Failed to get required arrival time (nullptr exception)";
    return Utility::undefined_rat(el);
  }

  update_timing();
  return pin_ptr->node_ptr()->rat(el, rf);
}

// Procedure: report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_rat(int argc, char **argv) {
  _report_rat(environment_ptr()->rpt_fptr(), argc, argv);
}

// Procedure: report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_rat(string_crt pin_name, int el, int rf) {
  _report_rat(environment_ptr()->rpt_fptr(), pin_name, el, rf);
}

// Procedure: report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::report_rat(pin_pt pin_ptr, int el, int rf) {
  _report_rat(environment_ptr()->rpt_fptr(), pin_ptr, el, rf);
}

// Procedure: _report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_rat(file_pt fptr, int argc, char **argv) {

  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    ++i;
  }

  // By default the pin name should be specified.
  _report_rat(fptr, pin_name, el, rf);
}

// Procedure: _report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_rat(file_pt fptr, string_crt pin_name, int el, int rf) {
  _report_rat(fptr, circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Procedure: _report_rat
// Report the required arrival time in picoseconds at a given pin name, where the pin will be 
// in the current design, i.e., no internal spef nodes. Options -rise and -fall are mutually 
// exclusive, and respectively specify the desired transition. Options -early and -late are 
// mutually exclusive, and respectively specify the desired mode. By default the -early -rise
// options are selected. 
void_t Timer::_report_rat(file_pt fptr, pin_pt pin_ptr, int el, int rf) {
  float_t numeric;
  __tau2015_numeric_guard(numeric = rat(pin_ptr, el, rf));
  fprintf(fptr, "%.6f\n", numeric);
}

// Function: slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
float_t Timer::slack(string_crt pin_name, int el, int rf) {
  return slack(circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Function: slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
float_t Timer::slack(pin_pt pin_ptr, int el, int rf) {

  if(pin_ptr == nullptr) {
    LOG(ERROR) << "Failed to get slack (nullptr exception)";
    return Utility::undefined_slack(el);
  }

  update_timing();
  return pin_ptr->node_ptr()->slack(el, rf);
}

// Function: report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slack(int argc, char **argv) {
  _report_slack(environment_ptr()->rpt_fptr(), argc, argv);
}

// Function: report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slack(string_crt pin_name, int el, int rf) {
  _report_slack(environment_ptr()->rpt_fptr(), pin_name, el, rf);
}

// Function: report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slack(pin_pt pin_ptr, int el, int rf) {
  _report_slack(environment_ptr()->rpt_fptr(), pin_ptr, el, rf);
}

// Function: _report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slack(file_pt fptr, int argc, char **argv) {

  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    ++i;
  }

  // By default the pin name should be specified.
  _report_slack(fptr, pin_name, el, rf);
}

// Procedure: _report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slack(file_pt fptr, string_crt pin_name, int el, int rf) {
  _report_slack(fptr, circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Procedure: _report_slack
// Report the slack in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slack(file_pt fptr, pin_pt pin_ptr, int el, int rf) {
  float_t numeric;
  __tau2015_numeric_guard(numeric = slack(pin_ptr, el, rf));
  fprintf(fptr, "%.6f\n", numeric);
}

// Procedure: report_all_slacks
// The procedure print the slack at every pin in the design for early and late split. The slack
// at every pin for each timing split is considered the worst value between rise transition and
// fall transition.
void_t Timer::report_all_slacks() {

  float_t slacks[2];

  for(const auto& u : nodeset()) {
    slacks[EARLY] = min(u->slack(EARLY, RISE), u->slack(EARLY, FALL));
    slacks[LATE ] = min(u->slack(LATE , RISE), u->slack(LATE , FALL));
    fprintf(environment_ptr()->rpt_fptr(), "%s %.4f %.4f\n", u->pin_ptr()->name().c_str(), slacks[EARLY], slacks[LATE]);
    fprintf(environment_ptr()->rpt_fptr(), "-> AT  (%.4f %.4f %.4f %.4f)\n", u->pin_ptr()->node_ptr()->at(EARLY, RISE),
                                                                        u->pin_ptr()->node_ptr()->at(EARLY, FALL),
                                                                        u->pin_ptr()->node_ptr()->at(LATE, RISE),
                                                                        u->pin_ptr()->node_ptr()->at(LATE, FALL));
    fprintf(environment_ptr()->rpt_fptr(), "-> SLEW(%.4f %.4f %.4f %.4f)\n", u->pin_ptr()->node_ptr()->slew(EARLY, RISE),
                                                                          u->pin_ptr()->node_ptr()->slew(EARLY, FALL),
                                                                          u->pin_ptr()->node_ptr()->slew(LATE, RISE),
                                                                          u->pin_ptr()->node_ptr()->slew(LATE, FALL));
    fprintf(environment_ptr()->rpt_fptr(), "-> RAT (%.4f %.4f %.4f %.4f)\n", u->pin_ptr()->node_ptr()->rat(EARLY, RISE),
                                                                             u->pin_ptr()->node_ptr()->rat(EARLY, FALL),
                                                                             u->pin_ptr()->node_ptr()->rat(LATE, RISE),
                                                                             u->pin_ptr()->node_ptr()->rat(LATE, FALL));
    fprintf(environment_ptr()->rpt_fptr(), "-> LOAD(%.4f %.4f %.4f %.4f)\n", u->pin_ptr()->load(EARLY, RISE),
                                                                             u->pin_ptr()->load(EARLY, FALL),
                                                                             u->pin_ptr()->load(LATE, RISE),
                                                                             u->pin_ptr()->load(LATE, FALL));
    fprintf(environment_ptr()->rpt_fptr(), "\n");
  }
}

// Function: report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slew(int argc, char **argv) {
  _report_slew(environment_ptr()->rpt_fptr(), argc, argv);
}

// Function: report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slew(string_crt pin_name, int el, int rf) {
  _report_slew(environment_ptr()->rpt_fptr(), pin_name, el, rf);
}

// Function: report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::report_slew(pin_pt pin_ptr, int el, int rf) {
  _report_slew(environment_ptr()->rpt_fptr(), pin_ptr, el, rf);
}

// Function: _report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slew(file_pt fptr, int argc, char **argv) {

  int el = EARLY;             
  int rf = RISE;  
  int i = 0;
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-fall") == 0) rf = FALL;
    else if(strcmp(argv[i], "-late") == 0) el = LATE;
    ++i;
  }

  // By default the pin name should be specified.
  _report_slew(fptr, pin_name, el, rf);
}

// Procedure: _report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slew(file_pt fptr, string_crt pin_name, int el, int rf) {
  _report_slew(fptr, circuit_ptr()->pin_ptr(pin_name), el, rf);
}

// Procedure: _report_slew
// Report the slew in picoseconds at a given pin name, where the pin will be in the current 
// design, i.e., no internal spef nodes. Options -rise and -fall are mutually exclusive, and 
// respectively specify the desired transition. Options -early and -late are mutually 
// exclusive, and respectively specify the desired mode. By default the -early -rise options 
// are selected. 
void_t Timer::_report_slew(file_pt fptr, pin_pt pin_ptr, int el, int rf) {
  float_t numeric;
  __tau2015_numeric_guard(numeric = slew(pin_ptr, el, rf));
  fprintf(fptr, "%.6f\n", numeric);
}

// Function: tns
// Report the total negative slack among the design. The procedure sum up the values across all
// possible transitions and timing splits.
float_t Timer::tns() {

  // Update the timing.
  update_timing();

  // Update the tns.
  _update_tns();

  return _tns[EARLY][RISE] + _tns[EARLY][FALL] + _tns[LATE][RISE] + _tns[LATE][FALL];
}

// Function: tns
// Report the total negative slack at a given transition and timing split.
float_t Timer::tns(int el, int rf) {

  // Update the timing.
  update_timing();

  // Update the tns.
  _update_tns();

  return _tns[el][rf];
}

// Procedure: report_tns
// Report the total negative slack.
void_t Timer::report_tns() {
  _report_tns(environment_ptr()->rpt_fptr());
}

// Procedure: _report_tns
// Report the total negative slack.
void_t Timer::_report_tns(file_pt fp) {
  fprintf(fp, "%.5e\n", tns());
}

// Function: wns
// Report the worst negative slack among the design. The procedure check the wns across all
// possible transitions and timing splits.
float_t Timer::wns() {

  // Update the timing.
  update_timing();

  // Update the wns.
  _update_wns();

  return min(min(_wns[EARLY][RISE], _wns[EARLY][FALL]), min(_wns[LATE][RISE], _wns[LATE][FALL]));
}

// Function: wns
// Report the worst negative slack at a given transition and timing split.
float_t Timer::wns(int el, int rf) {

  // Update the timing.
  update_timing();

  // Update the wns.
  _update_wns();

  return _wns[el][rf];
}

// Procedure: report_wns
// Report the worst negative slack.
void_t Timer::report_wns() {
  _report_wns(environment_ptr()->rpt_fptr());
}

// Procedure: _report_wns
// Report the worst negative slack.
void_t Timer::_report_wns(file_pt fp) {
  fprintf(fp, "%.5e\n", wns());
}

// Procedure: get_worst_endpoints
// Report the top K critical endpoints that pass through a given pin, where the pin will be or
// not be in the current design. If the pin is not in the current design the paths are searched
// across the design.
void_t Timer::get_worst_endpoints(string_crt pin_name, size_t K, endpoint_ptr_vrt endpoint_ptrs) {
  
  // Obtain the through pin pointer. The through pin pointer could be a nullptr pointer when the 
  // input pin_name is not explicitly specified.
  pin_pt through_pin_ptr = circuit_ptr()->pin_ptr(pin_name);

  get_worst_endpoints(through_pin_ptr, K, endpoint_ptrs);
}

// Procedure: get_worst_endpoints
// Report the top K critical endpoints that pass through a given pin, where the pin will be or
// not be in the current design. If the pin is not in the current design the paths are searched
// across the design.
void_t Timer::get_worst_endpoints(pin_pt through_pin_ptr, size_t K, endpoint_ptr_vrt endpoint_ptrs) {
  
  // Update the timing. It is necessary to call the procedure update_timing so as to update all
  // timing information, including the delay, arrival time, and worst post-CPPR slack at each
  // timing pins.
  update_timing();
  
  // Obtain those tests that are covered by the fanout of the given pin. Based on these tests,
  // we extract endpoints from the test set and update the slack on each endpoint. Once we 
  // have the slack of each endpoint we sort them in non-desreasing order and get the top K
  // critical endpoints.

  auto CompareEndpoint = [] (endpoint_pt i, endpoint_pt j) -> bool_t {
    return i->slack() < j->slack();
  };

  if(through_pin_ptr) {
    test_ptr_vt test_ptrs;
    _get_fanout_test_ptrs(through_pin_ptr, test_ptrs);
    _get_endpoint_ptrs(test_ptrs, endpoint_ptrs);
    _update_endpoint_slacks(through_pin_ptr, endpoint_ptrs);
    sort(endpoint_ptrs.begin(), endpoint_ptrs.end(), CompareEndpoint);
    while(endpoint_ptrs.size() && (endpoint_ptrs.back()->slack() > OT_FLT_ZERO || endpoint_ptrs.size() > K)) {
      endpoint_ptrs.pop_back();
    }
  }
  else {
    endpoint_minheap_ptr()->top(K, endpoint_ptrs);
  }
}

// Procedure: get_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be or not be in the current design. The path is from the primary input to 
// primary output. For the set of paths, we only print out the paths with non-positive post-CPPR 
// slack. If the K is greater than the total number of paths through the pin, only output the 
// paths that pass through the given pin.
void_t Timer::get_worst_paths(string_crt through_pin_name, size_t K, path_cache_rt cache) {

  // Obtain the through pin pointer. The through pin pointer could be a nullptr pointer when the 
  // input pin_name is not explicitly specified.
  pin_pt through_pin_ptr = circuit_ptr()->pin_ptr(through_pin_name);

  get_worst_paths(through_pin_ptr, K, cache);
}

// Procedure: get_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be or not be in the current design. The path is from the primary input to 
// primary output. For the set of paths, we only print out the paths with non-positive post-CPPR 
// slack. If the K is greater than the total number of paths through the pin, only output the 
// paths that pass through the given pin.
void_t Timer::get_worst_paths(pin_pt through_pin_ptr, size_t K, path_cache_rt cache) {

  // Data declaration.
  endpoint_ptr_vt endpoint_ptrs;
  path_max_heap_vt heaps(OT_MAX_NUM_THREADS);

  // Clear the path cache.
  cache.clear();

  // Get the top K critical endpoints. The function calls will perform the update timing.
  get_worst_endpoints(through_pin_ptr, K, endpoint_ptrs);

  // Now we have the top K (or less than K) endpoints in which we have to generate the K 
  // shortest paths for each of them and store them into the path matrix.
  #pragma omp parallel num_threads(min((int_t)endpoint_ptrs.size(), omp_get_num_threads()))
  { 
    int_t tid = omp_get_thread_num();
    int_t num = endpoint_ptrs.size() / omp_get_num_threads();
    int_t beg = num * tid;
    int_t end = (tid == omp_get_num_threads()) ? endpoint_ptrs.size() : beg + num;

    for(int_t i=beg; i<end; ++i) {
      // Prune the endpoint.
      if(heaps[tid].size() == K && endpoint_ptrs[i]->slack() >= heaps[tid].top()->slack()) {
        break;
      }

      CPPR cppr(endpoint_ptrs[i], through_pin_ptr, circuit_ptr()->clock_tree_ptr(), K);
      cppr.update_worst_paths(K, heaps[tid]);
    }

    #pragma omp critical 
    {
      heaps[tid].emplace_path_ptrs(cache);
    }
  } // Implicit synchronization barrier. ----------------------------------------------------------

  cache.maintain_k_min(K);
}

// Procedure: report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::report_worst_paths(string_crt pin_name, size_t K) {
  _report_worst_paths(environment_ptr()->rpt_fptr(), pin_name, K);
}

// Procedure: report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::report_worst_paths(pin_pt pin_ptr, size_t K) {
  _report_worst_paths(environment_ptr()->rpt_fptr(), pin_ptr, K);
}

// Procedure: report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::report_worst_paths(int argc, char** argv) {
  _report_worst_paths(environment_ptr()->rpt_fptr(), argc, argv);
}

// Procedure: _report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::_report_worst_paths(file_pt fp, string_crt pin_name, size_t K) {

  // Obtain the through pin pointer. The through pin pointer could be a nullptr pointer when the 
  // input pin_name is not explicitly specified.
  pin_pt through_pin_ptr = circuit_ptr()->pin_ptr(pin_name);

  _report_worst_paths(fp, through_pin_ptr, K);
}
      
// Procedure: report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::_report_worst_paths(file_pt fp, int argc, char **argv) {
  int num_paths = 1;                                                   
  int i = 0; 
  string_t pin_name;
  while(i < argc) {
    if(strcmp(argv[i], "-pin") == 0) pin_name = argv[++i];
    else if(strcmp(argv[i], "-numPaths") == 0) num_paths = atoi(argv[++i]);
    ++i;
  }
  _report_worst_paths(fp, pin_name, num_paths);
}

// Procedure: _report_worst_paths
// Report the top K critical post-CPPR slack in picoseconds (ps) that pass through a given pin,
// where the pin will be in the current design. The path is from the primary input to primary 
// output. For the set of paths, we only print out the paths with non-positive post-CPPR slack.
// If the K is greater than the total number of paths through the pin, only output the paths that
// pass through the given pin.
void_t Timer::_report_worst_paths(file_pt fp, pin_pt through_pin_ptr, size_t K) {

  // Storage declaration.
  path_cache_t cache;

  // Get the worst K paths and store them into the cahce.
  get_worst_paths(through_pin_ptr, K, cache);

  // Print out the path trace for the top K critical paths. Here we only care about the non-positive
  // slack paths. That is we only print out the path with negative slack.
  int_t num_paths = cache.size();
  fprintf(fp, "report_worst_paths %d\n", num_paths);

  for(int_t k=0; k<num_paths; k++) {
    path_rt p = *(cache.path_ptrs()[k]);
    switch(p.path_type()) {
      case RAT_PATH_TYPE:
        fprintf(fp, "Path %d: RAT %.3f %lu %c\n", k+1, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      case SETUP_PATH_TYPE:
        fprintf(fp, "Path %d: Setup %.3f %lu %c\n", k+1, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      case HOLD_PATH_TYPE:
        fprintf(fp, "Path %d: Hold %.3f %lu %c\n", k+1, p.slack(), p.size(), (p.el() ? 'L' : 'E'));
      break;
      default:
        CHECK(false);
      break;
    }
  
    for(PathRIter i(p); i(); ++i) {
      fprintf(fp, "%s %c\n", i.pin_ptr()->name().c_str(), i.rf() ? 'F' : 'R');
    }
  }
}

// Procedure: report_timer
// Report the timer information. This procedure prints out the statistics of the timer, including
// the number of pins, the number of nets, the number of nodes, the number of gates, and so on.
// For timing report, please see the procedure report_timing.
void_t Timer::report_timer() {
  _report_timer(environment_ptr()->rpt_fptr());
}

// Procedure: _report_timer
// Report the timer information. This procedure prints out the statistics of the timer, including
// the number of pins, the number of nets, the number of nodes, the number of gates, and so on.
// For timing report, please see the procedure report_timing.
void_t Timer::_report_timer(file_pt fptr) {
  fprintf(fptr, "Timer                   : %s\n", name().c_str());
  fprintf(fptr, "Num of pins             : %lu\n", num_pins());
  fprintf(fptr, "Num of nets             : %lu\n", num_nets());
  fprintf(fptr, "Num of tests            : %lu\n", circuit_ptr()->num_tests());
  fprintf(fptr, "Num of gates            : %lu\n", num_gates());
  fprintf(fptr, "Num of nodes            : %lu\n", num_nodes());
  fprintf(fptr, "Num of node indices     : %lu\n", nodeset().num_indices());
  fprintf(fptr, "Num of edges            : %lu\n", num_edges());
  fprintf(fptr, "Num of jumps            : %lu\n", jumpset().size());
  fprintf(fptr, "Num of primary inputs   : %lu\n", num_primary_inputs());
  fprintf(fptr, "Num of primary outputs  : %lu\n", num_primary_outputs());
  fprintf(fptr, "Num of clock tree nodes : %lu\n", circuit_ptr()->clock_tree_ptr()->size());
  fprintf(fptr, "Num of clock tree levels: %d\n", circuit_ptr()->clock_tree_ptr()->level());
}

// Procedure: report_max_num_threads
// Report the maximum number of threads that OpenTimer can invoke.
void_t Timer::report_max_num_threads() {
  _report_max_num_threads(environment_ptr()->rpt_fptr());
}

// Procedure: _report_max_num_threads
// Report the maximum number of threads that OpenTimer can invoke.
void_t Timer::_report_max_num_threads(file_pt fptr) {
  fprintf(fptr, "%lu\n", max_num_threads());
}

// Procedure: exec_tau2015
// The procedure initializes the timer for TAU 2015 contest.
void_t Timer::exec_tau2015(int argc, char *argv[]) {
  environment_ptr()->parse_environment_tau2015(argc, argv);
  init_timer();
  exec_ops(environment_ptr()->ops_fpath());
}

// Procedure: exec_tau2016
// The procedure initializes the timer for TAU 2015 contest.
void_t Timer::exec_tau2016(int argc, char *argv[]) {
  environment_ptr()->parse_environment_tau2016(argc, argv);
  init_timer();
}

// Procedure: exec_iccad2015
// The procedure initializes the timer for ICCAD 2015 contest.
void_t Timer::exec_iccad2015(int argc, char *argv[]) {
  environment_ptr()->parse_environment_iccad2015(argc, argv);
  init_timer();
  exec_ops(environment_ptr()->ops_fpath());
}

// Procedure: init_timer
// The procedure initializes the timer using the given 1) verilog file, 2) two liberty files of
// early and late splits, 3) spef file, and 4) initial timing file. The initialization of the
// timer is performed in a parallel manner since parsing these files are mostly independent to
// each other. This procedure is the bottom-most internal call of all callers "init_timer" from users.
void_t Timer::init_timer() {

  // Direct the fptr.
  //environment_ptr()->rpt_fptr() = __wfptr(environment_ptr()->log_fpath().c_str());

  // [BEG INFO]
  LOG(INFO) << "Initializing timer ...";
  // [END INFO]

  // Initialization.  
  // Parallel region where each independent piece for the initialization of the timer is 
  // performed in a parallel fashion.
  verilog_pt verilog_ptr = new verilog_t();
  spef_pt spef_ptr = new spef_t();

  #pragma omp parallel
  {
    // Parallel task group 1: Read the file and initialize the circuit.
    // - read spef file
    // - read celllib file
    // - read verilog file
    // - initialize the circuit connection from verilog file.
    // - initialize the io timing.
    #pragma omp sections 
    {
      // Section task: parse the spef file
      #pragma omp section 
      { 
        _parse_spef(environment_ptr()->spef_fpath(), spef_ptr); 
      }
    
      // Section task: prase celllib, parse verilog, and initialize the circuit.
      #pragma omp section
      {
        // Child task: parse the early celllib.
        #pragma omp task
        { 
          _read_celllib(environment_ptr()->early_celllib_fpath(), celllib_ptr(EARLY)); 
        }

        // Child task: parse the late celllib.
        #pragma omp task
        { 
          _read_celllib(environment_ptr()->late_celllib_fpath(), celllib_ptr(LATE)); 
        }
      
        // Child task: parse the verilog
        #pragma omp task
        { 
          _parse_verilog(environment_ptr()->verilog_fpath(), verilog_ptr); 
        }
      
        // Section task: initialize the circuit from verilog
        #pragma omp taskwait

        _init_circuit_from_verilog(verilog_ptr);
        _init_io_timing(environment_ptr()->timing_fpath());
      }
    } // Implicit synchronization barrier after sections. -----------------------------------------

    // Parallel task group 2: Timing graph preprocessing.
    // - update the RC value of the initial circuit from spef file, 
    // - levelize the level indices of nodes.
    // - initialize the clock tree and build the cppr credit table.
    #pragma omp single
    {
      _update_spef(spef_ptr);

      #pragma omp task
      { _update_clock_tree(); }

      #pragma omp task
      { _update_all_node_levels(); }

    } // Implicit sychronization barrier after single directive. ----------------------------------

    // Parallel task group 4: Read the physical Layout (optional).
    // - read the design exchangeable file (DEF)
    // - read the library exchangeable file (LEF)
    #pragma omp single
    {
      if(__is_file_available(environment_ptr()->lef_fpath()) && __is_file_available(environment_ptr()->def_fpath())) {
        #pragma omp task
        { 
          _read_lef(environment_ptr()->lef_fpath()); 
        }
        #pragma omp task
        { 
          _read_def(environment_ptr()->def_fpath()); 
        }
      }
    } // Implicit synchronization barrier after single directive. ---------------------------------
  }

  delete verilog_ptr;
  delete spef_ptr;
  
  // [BEG INFO]
  LOG(INFO) << "Successfully initialized the timer";
  // [END INFO]
}

// Procedure: exec_ops
// The main procedure to handle the ops file. For the contest, we process the 
// ops file one line at a time, e.g., we don't change the given order of operations. 
void_t Timer::exec_ops(string_crt ops_fpath) {
  
  // [BEG INFO]
  LOG(INFO) << "Executing ops " + Utility::realpath(ops_fpath);
  // [END INFO]

  size_t max_file_line_size = __max_fileline_size(ops_fpath.c_str()) + 1024;
  file_pt ops_fptr = fopen(ops_fpath.c_str(), "r");
  
  if(ops_fptr == nullptr) {
    LOG(ERROR) << "Failed to open " + ops_fpath;
    return;
  }

  int el, rf;
  int num_paths;
  float_t numeric;
  char_cpt ops_delimiters = " \n\0";
  char_pt keyword, pin_name, gate_name, cell_name, net_name, spef_name;
  char_pt line = (char_pt)malloc(sizeof(char_t)*(max_file_line_size));
  
  // Step 3: interactive loop.
  while(fgets(line, max_file_line_size, ops_fptr)) {

    keyword = strtok(line, ops_delimiters);

    if(keyword == nullptr) continue;

    // Report the arrival time.
    if(strcmp(keyword, "report_at") == 0) {
      el = EARLY;                                                      // Default split selection.
      rf = RISE;  
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters);
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;              // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;              // Assign the split.
      }
      __tau2015_numeric_guard(numeric = at(pin_name, el, rf));
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report the required arrival time. 
    else if(strcmp(keyword, "report_rat") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters);
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
      }
      __tau2015_numeric_guard(numeric = rat(pin_name, el, rf));
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report the slack.
    else if(strcmp(keyword, "report_slack") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
      }
      __tau2015_numeric_guard(numeric = slack(pin_name, el, rf));
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report the slew
    else if(strcmp(keyword, "report_slew") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
      }
      __tau2015_numeric_guard(numeric = slew(pin_name, el, rf));
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report the worst paths.
    else if(strcmp(keyword, "report_worst_paths") == 0) {
      num_paths = 1;                                                    // Default path count.
      pin_name = nullptr;                                                  // Default pin name.
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-numPaths") == 0) num_paths = atoi(strtok(nullptr, ops_delimiters)); 
      }
      if(pin_name == nullptr) report_worst_paths("", num_paths);               
      else report_worst_paths(pin_name, num_paths);
    }
    // Report the tns.
    else if(strcmp(keyword, "report_tns") == 0) {
      __tau2015_numeric_guard(numeric = tns());
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report the wns.
    else if(strcmp(keyword, "report_wns") == 0) {
      __tau2015_numeric_guard(numeric = wns());
      fprintf(environment_ptr()->rpt_fptr(), "%.6f\n", numeric);
    }
    // Report all slacks.
    else if(strcmp(keyword, "report_all_slacks") == 0) {
      report_all_slacks();
    }
    // Set the arrival time
    else if(strcmp(keyword, "set_at") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
        else numeric = atof(keyword);
      }
      set_at(pin_name, el, rf, numeric);
    }
    // Set the slew
    else if(strcmp(keyword, "set_slew") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
        else numeric = atof(keyword);
      }
      set_slew(pin_name, el, rf, numeric);
    }
    // Set the rat
    else if(strcmp(keyword, "set_rat") == 0) {
      el = EARLY;                                                       // Default split selection.
      rf = RISE;                                                        // Default trans selection.
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else if(strcmp(keyword, "-fall") == 0) rf = FALL;               // Assign the trans.
        else if(strcmp(keyword, "-late") == 0) el = LATE;               // Assign the split.
        else numeric = atof(keyword);
      }
      set_rat(pin_name, el, rf, numeric);
    }
    // Set the load
    else if(strcmp(keyword, "set_load") == 0) {
      pin_name = nullptr;
      while((keyword = strtok(nullptr, ops_delimiters))) {
        if(strcmp(keyword, "-pin") == 0) pin_name = strtok(nullptr, ops_delimiters); 
        else numeric = atof(keyword);
      }
      EL_RF_ITER(el, rf) {
        set_load(pin_name, el, rf, numeric);
      }
    }
    // Gate-level circuit modification: insert a new gate
    else if(strcmp(keyword, "insert_gate") == 0){
      gate_name = strtok(nullptr, ops_delimiters);
      cell_name = strtok(nullptr, ops_delimiters);
      insert_gate(gate_name, cell_name);
    }
    // Gate-level circuit modification: gate sizing.
    else if(strcmp(keyword, "repower_gate") == 0) {
      gate_name = strtok(nullptr, ops_delimiters);
      cell_name = strtok(nullptr, ops_delimiters);
      repower_gate(gate_name, cell_name);
    }
    // Gate-level circuit modification: remove a gate
    else if(strcmp(keyword, "remove_gate") == 0) {
      gate_name = strtok(nullptr, ops_delimiters);
      remove_gate(gate_name);
    }
    // Net-level circuit modification: insert a net.
    else if(strcmp(keyword, "insert_net") == 0) {
      net_name = strtok(nullptr, ops_delimiters);
      insert_net(net_name);
    }
    // Net-level circuit modification: read rc info from a spef file.
    else if(strcmp(keyword, "read_spef") == 0) {
      spef_name = strtok(nullptr, ops_delimiters);
      read_spef(environment_ptr()->tau2015_wrapper_fdir() + spef_name);
    }
    // Net-level circuit modification: remove a net.
    else if(strcmp(keyword, "remove_net") == 0) {
      net_name = strtok(nullptr, ops_delimiters);
      remove_net(net_name);
    }
    // Pin-level circuit modification: connect a pin to a net.
    else if(strcmp(keyword, "connect_pin") == 0) {
      pin_name = strtok(nullptr, ops_delimiters);
      net_name = strtok(nullptr, ops_delimiters);
      connect_pin(pin_name, net_name);
    }
    // Pin-level circuit modification: disconnect a pin from a net.
    else if(strcmp(keyword, "disconnect_pin") == 0) {
      pin_name = strtok(nullptr, ops_delimiters);
      disconnect_pin(pin_name);
    } 
    else {
      LOG(WARNING) << "Unexpected op keyword " + string_t(keyword);
    }
  }
  
  free(line);
  fclose(ops_fptr);

  // [BEG INFO]
  LOG(INFO) << "Successfully executed the ops file";
  // [END INFO]
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------




