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

#ifndef OT_TIMER_H_
#define OT_TIMER_H_

#include "ot_circuit.h"
#include "ot_node.h"
#include "ot_edge.h"
#include "ot_celllib.h"
#include "ot_techlib.h"
#include "ot_verilog.h"
#include "ot_iterator.h"
#include "ot_environment.h"
#include "ot_cppr.h"
#include "ot_dump.h"
#include "ot_spef.h"
#include "ot_pipeline.h"
#include "ot_path.h"
#include "ot_functor.h"
#include "ot_debug_graph.h"
#include "ot_log.h"
#include "ot_shell.h"

namespace OpenTimer {

// Class: Timer
class Timer {

  public:

    Timer();                                                            // Constructor.
    ~Timer();                                                           // Destructor.

    inline void_t set_cutoff_slack(int, int, float_ct);                 // Set the cutoff slack.

    net_pt net_ptr(string_crt) const;                                   // Query the net pointer.
    net_pt insert_net(string_crt);                                      // Create a new net.

    cell_pt cell_ptr(int, string_crt) const;                            // Query the cell pointer.

    pin_pt pin_ptr(string_crt) const;                                   // Query the pin pointer.

    primary_input_pt primary_input_ptr(string_crt) const;               // Query the PI port pointer.
    primary_input_pt insert_primary_input(string_crt);                  // Create a new PI port.

    primary_output_pt primary_output_ptr(string_crt) const;             // Query the PO port pointer.
    primary_output_pt insert_primary_output(string_crt);                // Create a new PO port.
    
    gate_pt gate_ptr(string_crt) const;                                 // Query the gate pointer.
    gate_pt insert_gate(string_crt, string_crt);                        // Create a new gate.
    
    void_t exec_tau2015(int argc, char *argv[]);                        // Execute the timer.
    void_t exec_tau2016(int argc, char *argv[]);                        // Execute the timer.
    void_t exec_iccad2015(int argc, char *argv[]);                      // Execute the timer.
    void_t exec_ops(string_crt);                                        // Execute the ops.
    void_t init_timer();                                                // Initialize the timer.
    void_t report_timer();                                              // Report the timer.
    void_t report_max_num_threads();                                    // Report the thread limit.
    void_t report_tns();                                                // Report the tns.
    void_t report_wns();                                                // Report the wns.
    void_t report_worst_paths(string_crt, size_t);                      // Report the worst paths.
    void_t report_worst_paths(pin_pt, size_t);                          // Report the worst paths.
    void_t report_worst_paths(int, char**);                             // Report the worst paths.
    void_t report_at(int, char**);                                      // Report the AT.
    void_t report_at(string_crt, int, int);                             // Report the AT.
    void_t report_at(pin_pt, int, int);                                 // Report the AT.
    void_t report_rat(int, char**);                                     // Report the RAT.
    void_t report_rat(string_crt, int, int);                            // Report the RAT.
    void_t report_rat(pin_pt, int, int);                                // Report the RAT.
    void_t report_slack(int, char**);                                   // Report the slack.
    void_t report_slack(string_crt, int, int);                          // Report the slack.
    void_t report_slack(pin_pt, int, int);                              // Report the slack.
    void_t report_slew(int, char**);                                    // Report the slew.
    void_t report_slew(string_crt, int, int);                           // Report the slew.
    void_t report_slew(pin_pt, int, int);                               // Report the slew.
    void_t report_all_slacks();                                         // Report all slacks.
    void_t get_worst_paths(string_crt, size_t, path_cache_rt);          // Report the worst paths.
    void_t get_worst_paths(pin_pt, size_t, path_cache_rt);              // Report the worst paths.
    void_t get_worst_endpoints(string_crt, size_t, endpoint_ptr_vrt);   // Get the worst endpoints.
    void_t get_worst_endpoints(pin_pt, size_t, endpoint_ptr_vrt);       // Get the worst endpoints.
    void_t read_spef(string_crt);                                       // Read a spef file.
    void_t repower_gate(string_crt, string_crt);                        // Resize an existing gate.
    void_t remove_net(string_crt);                                      // Remove an existing net.
    void_t remove_net(net_pt);                                          // Remove an existing net.
    void_t remove_gate(string_crt);                                     // Remove an existing gate.
    void_t remove_gate(gate_pt);                                        // Remove an existing gate.
    void_t disconnect_pin(string_crt);                                  // Disconnect the pin from its net.
    void_t disconnect_pin(pin_pt);                                      // Disconnect the pin from its net.
    void_t connect_pin(string_crt, string_crt);                         // Connect the pin to the net.
    void_t connect_pin(string_crt, net_pt);                             // Connect the pin to the net.
    void_t connect_pin(pin_pt, net_pt);                                 // Connect the pin to the net.
    void_t splice_driver(string_crt, string_crt, string_crt);           // Insert an input driver.
    void_t update_timing(bool_t is_incremental = true);                 // Update the timing.
    void_t set_num_threads(int_ct);                                     // Set the number of threads.
    void_t set_rpt_fpath(string_crt);                                   // Set the report file path.
    void_t set_slew(int, char**);                                       // Assert the slew on a pin.
    void_t set_slew(string_crt, int, int, float_ct);                    // Assert the slew on a pin.
    void_t set_slew(pin_pt, int, int, float_ct);                        // Assert the slew on a pin.
    void_t set_at(int, char**);                                         // Assert the at on a pin.
    void_t set_at(string_crt, int, int, float_ct);                      // Assert the at on a pin.
    void_t set_at(pin_pt, int, int, float_ct);                          // Assert the at on a pin.
    void_t set_load(int, char**);                                       // Assert the load on a pin.
    void_t set_load(string_crt, int, int, float_ct);                    // Assert the load on a pin.
    void_t set_load(primary_output_pt, int, int, float_ct);             // Assert the load on a pin.
    void_t set_rat(int, char**);                                        // Assert the rat on a pin.
    void_t set_rat(string_crt, int, int, float_ct);                     // Assert the rat on a pin.
    void_t set_rat(pin_pt, int, int, float_ct);                         // Assert the rat on a pin.
    void_t set_sdc_fpath(char_cpt);                                     // Set the sdc file path.
    void_t set_spef_fpath(char_cpt);                                    // Set the spef file path.
    void_t set_verilog_fpath(char_cpt);                                 // Set the verilog file path.
    void_t set_lef_fpath(char_cpt);                                     // Set the lef file path.
    void_t set_def_fpath(char_cpt);                                     // Set the def file path.
    void_t set_timing_fpath(char_cpt);                                  // Set the timing file path.
    void_t set_early_celllib_fpath(char_cpt);                           // Set the early lib file path.
    void_t set_late_celllib_fpath(char_cpt);                            // Set the late lib file path.
    void_t write_early_celllib(char_cpt);                               // Write the early celllib.
    void_t write_late_celllib(char_cpt);                                // Write the late celllib.
    
    float_t slew(string_crt, int, int);                                 // Report the slew.
    float_t slew(pin_pt, int, int);                                     // Report the slew. 
    float_t at(string_crt, int, int);                                   // Report the at.
    float_t at(pin_pt, int, int);                                       // Report the at.
    float_t rat(string_crt, int, int);                                  // Report the rat.
    float_t rat(pin_pt, int, int);                                      // Report the rat.
    float_t slack(string_crt, int, int);                                // Report the slack.
    float_t slack(pin_pt, int, int);                                    // Report the slack.
    float_t tns(int, int);                                              // Report the tns.
    float_t tns();                                                      // Report the tns.
    float_t wns(int, int);                                              // Report the wns.
    float_t wns();                                                      // Report the wns.

    file_pt log_fptr() const;                                           // Query the log file pointer.

    inline float_t cutoff_slack(int, int) const;                        // Query the cutoff slack.

    inline string_crt name() const;                                     // Query the timer name.
    inline celllib_pt celllib_ptr(int) const;                           // Query the celllib pointer.
    inline techlib_pt techlib_ptr() const;                              // Query the techlib pointer.
    inline circuit_pt circuit_ptr() const;                              // Query the circuit pointer.
    inline environment_pt environment_ptr() const;                      // Query the environment pointer.
    
    size_t num_pins() const;                                            // Query the pin count.
    size_t num_nets() const;                                            // Query the net count.
    size_t num_nodes() const;                                           // Query the node count.
    size_t num_edges() const;                                           // Query the edge count.
    size_t num_tests() const;                                           // Query the test count.
    size_t num_jumps() const;                                           // Query the jump count.
    size_t num_gates() const;                                           // Query the gate count.
    size_t num_primary_inputs() const;                                  // Query the number of primary inputs.
    size_t num_primary_outputs() const;                                 // Query the number of primary outputs.
    size_t num_threads() const;                                         // Query the number of threads.

    inline size_t max_num_threads() const;                              // Query the max number of threads.

    inline nodeset_rt nodeset() const;                                  // Query the nodeset reference.
    inline nodeset_pt nodeset_ptr() const;                              // Query the nodeset pointer.
    inline jumpset_rt jumpset() const;                                  // Query the jumpset reference.
    inline jumpset_pt jumpset_ptr() const;                              // Query the jumpset pointer.
    inline edgelist_pt edgelist_ptr() const;                            // Query the edgelist pointer.
    inline pipeline_rt pipeline() const;                                // Query the pipeline reference.
    inline pipeline_pt pipeline_ptr() const;                            // Query the pipeline pointer.
    inline endpoint_minheap_pt endpoint_minheap_ptr() const;            // Query the endpoint min heap.
    inline endpoint_minheap_rt endpoint_minheap() const;                // Query the endpoint min heap.
    
    static void_t init_logging(char_cpt, bool_ct);                      // Initialize the logging.
    static void_t run();                                                // Run interactive timer.

  private:

    string_t _name;                                                     // Timer name.

    environment_upt _environment_uptr;                                  // Environment.

    float_t _tns[2][2];                                                 // Total negative slack.
    float_t _wns[2][2];                                                 // Worst negative slack.
    float_t _cutoff_slack[2][2];                                        // Cutoff slack.

    bool_t _is_tns_updated[2][2];                                       // tns update flag.
    bool_t _is_wns_updated[2][2];                                       // wns update flag.
     
    celllib_upt _celllib_uptr[2];                                       // Early/Late celllib pointer.
    techlib_upt _techlib_uptr;                                          // Technoligy library pointer.
    circuit_upt _circuit_uptr;                                          // Circuit object pointer.
    pipeline_upt _pipeline_uptr;                                        // Pipeline pointer.
     
    nodeset_upt _nodeset_uptr;
    jumpset_upt _jumpset_uptr;                                          // Jumpset.
    edgelist_pt _edgelist_ptr;                                          // Edgelist.

    endpoint_minheap_upt _endpoint_minheap_uptr;                        // Endpoint min heap.

    node_pt _insert_node(pin_pt);                                       // Insert a node.
    node_pt _clock_tree_root_node_ptr() const;                          // Return the clock tree root node.

    edge_pt _insert_edge(string_crt, string_crt, net_pt);               // Insert an edge (net arc).
    edge_pt _insert_edge(node_pt, node_pt, net_pt);                     // Insert an edge (net arc).
    edge_pt _insert_edge(pin_pt, pin_pt, net_pt);                       // Insert an edge (net arc).
    edge_pt _insert_edge(string_crt, string_crt, timing_arc_pt[]);      // Insert an edge (cell arc).
    edge_pt _insert_edge(pin_pt, pin_pt, timing_arc_pt[]);              // Insert an edge (cell arc).
    edge_pt _insert_edge(node_pt, node_pt, timing_arc_pt[]);            // Insert an edge (cell arc).
    edge_pt _insert_edge(string_crt, string_crt, test_pt);              // Insert an edge (cell arc).
    edge_pt _insert_edge(pin_pt, pin_pt, test_pt);                      // Insert an edge (cell arc);
    edge_pt _insert_edge(node_pt, node_pt, test_pt);                    // Insert an edge (cell arc);
    edge_pt _find_edge_ptr(string_crt, string_crt);                     // Find an edge.
    edge_pt _find_edge_ptr(pin_pt, pin_pt);                             // Find an edge.
    edge_pt _find_edge_ptr(node_pt, node_pt);                           // Find an edge.
    
    jump_pt _insert_jump(pin_pt, pin_pt, int_t, float_t [][2]);         // Insert a jump.
    jump_pt _insert_jump(node_pt, node_pt, int_t, float_t [][2]);       // Insert a jump.

    void_t _parse_verilog(string_crt, verilog_pt);                      // Parse the verilog.
    void_t _parse_spef(string_crt, spef_pt);                            // Parse the spef file.
    void_t _read_celllib(string_crt, celllib_pt);                       // Read the library.
    void_t _read_def(string_crt);                                       // Read a def file.
    void_t _read_lef(string_crt);                                       // Read a lef file.
    void_t _write_celllib(string_crt, celllib_pt);                      // Write the celllib.
    void_t _update_tns();                                               // Update the tns.
    void_t _update_tns(int, int);                                       // Update the tns.
    void_t _update_wns();                                               // Update the wns.
    void_t _update_wns(int, int);                                       // Update the wns.
    void_t _update_spef(spef_pt);                                       // Update the spef object.
    void_t _update_forward_propagate_pipeline();                        // Update the forward pipeline.
    void_t _update_pipelined_tests();                                   // Update the pipelined tests.
    void_t _update_endpoint_slacks(pin_pt, endpoint_ptr_vrt);           // Update the endpoint slacks.
    void_t _update_clock_tree();                                        // Update the clock tree.
    void_t _update_node_level(node_pt, int_t) const;                    // Update the node level.
    void_t _update_all_node_levels() const;                             // Update all node levels.
    void_t _get_fanout_test_ptrs(pin_pt, test_ptr_vrt);                 // Update the pin fanout tests.
    void_t _get_endpoint_ptrs(test_ptr_vrt, endpoint_ptr_vrt);          // Update the endpoints.
    void_t _induce_jump(node_pt, node_pt, int_t, float_t dv[][2]);      // Induce the jump
    void_t _report_worst_paths(file_pt, string_crt, size_t);            // Report the worst paths.
    void_t _report_worst_paths(file_pt, pin_pt, size_t);                // Report the worst paths.
    void_t _report_worst_paths(file_pt, int, char**);                   // Report the worst paths.
    void_t _report_tns(file_pt);                                        // Report the TNS.
    void_t _report_wns(file_pt);                                        // Report the WNS.
    void_t _report_at(file_pt, int, char**);                            // Report the AT.
    void_t _report_at(file_pt, string_crt, int, int);                   // Report the AT.
    void_t _report_at(file_pt, pin_pt, int, int);                       // Report the AT.
    void_t _report_rat(file_pt, int, char**);                           // Report the RAT.
    void_t _report_rat(file_pt, string_crt, int, int);                  // Report the RAT.
    void_t _report_rat(file_pt, pin_pt, int, int);                      // Report the RAT.
    void_t _report_slack(file_pt, int, char**);                         // Report the slack.
    void_t _report_slack(file_pt, string_crt, int, int);                // Report the slack.
    void_t _report_slack(file_pt, pin_pt, int, int);                    // Report the slack.
    void_t _report_slew(file_pt, int, char**);                          // Report the slew.
    void_t _report_slew(file_pt, string_crt, int, int);                 // Report the slew.
    void_t _report_slew(file_pt, pin_pt, int, int);                     // Report the slew.
    void_t _report_timer(file_pt);                                      // Report the timer.
    void_t _report_max_num_threads(file_pt);                            // Report the thread limit.
    void_t _init_circuit_from_verilog(verilog_pt);                      // Initialize circuit connection.
    void_t _init_io_timing(string_crt);                                 // Initialize the io timing.
    void_t _init_full_timing_update();                                  // Initialize the full timing update.
    void_t _clear_edgelist();                                           // Clear the edgelist.
    void_t _remap_cellpin(int, pin_pt, cellpin_pt);                     // Remap the cellpin.
    void_t _remove_node(node_pt);                                       // Remove the node. 
    void_t _remove_edge(edge_pt);                                       // Remove the edge.
    void_t _remove_jump(node_pt);                                       // Remove the jump.
    void_t _remove_jump(jump_pt);                                       // Remove the jump.
    void_t _insert_frontier(node_pt) const;                             // Insert a frontier node.
    void_t _remove_frontier(node_pt) const;                             // Remove a frontier node.
    void_t _remove_all_frontiers();                                     // Clear the pipeline.
    void_t _enable_lazy_evaluation();                                   // Enable the lazy evaluation.
    void_t _forward_propagate_fanout(node_pt);                          // Forward propagate fanout.
    void_t _forward_propagate_fanout(int_t);                            // Forward propagate fanout.
    void_t _forward_propagate_at(node_pt);                              // Forward propagate at.
    void_t _forward_propagate_at(int_t);                                // Forward propagate at.
    void_t _forward_propagate_slew(node_pt);                            // Forward propagate slew.
    void_t _forward_propagate_slew(int_t);                              // Forward propagate slew.
    void_t _forward_propagate_delay(node_pt);                           // Forward propagate delay.
    void_t _forward_propagate_delay(int_t);                             // Forward propagate delay.
    void_t _forward_propagate_rc_timing(int_t);                         // Forward propagate the rc delay.
    void_t _forward_propagate_rc_timing(node_pt);                       // Forward propagate the rc delay.
    void_t _forward_propagate_jump(int_t);                              // Forward propagate the jump.
    void_t _forward_propagate_jump(node_pt);                            // Forward propagate the jump.
    void_t _forward_propagate_test(int_t);                              // Forward propagate the test.
    void_t _forward_propagate_test(test_pt);                            // Forward propagate the endpoint.
    void_t _backward_propagate_fanin(node_pt);                          // Backward propagate the level.
    void_t _backward_propagate_fanin(int_t);                            // Backward propagate the level.
    void_t _backward_propagate_rat(node_pt);                            // Backward propagate the rat.
    void_t _backward_propagate_rat(int_t);                              // Backward propagate the rat.
};

// Function: name
// Return the timer's name
inline string_crt Timer::name() const {
  return _name;
}

// Procedure: set_cutoff_slack
inline void_t Timer::set_cutoff_slack(int el, int rf, float_ct slack) {
  _cutoff_slack[el][rf] = slack;
}

// Function: max_num_threads
inline size_t Timer::max_num_threads() const {
  return OT_MAX_NUM_THREADS;
}

// Function: environment_ptr
// Return the pointer to the environment object.
inline environment_pt Timer::environment_ptr() const {
  return _environment_uptr.get();
}

// Function: celllib_ptr
// Return the pointer to the celllib.
inline celllib_pt Timer::celllib_ptr(int el) const {
  return _celllib_uptr[el].get();
}

// Function: techlib_ptr
// Return the pointer to the techlib.
inline techlib_pt Timer::techlib_ptr() const {
  return _techlib_uptr.get();
}

// Function: circuit_ptr
// Return the circuit pointer.
inline circuit_pt Timer::circuit_ptr() const {
  return _circuit_uptr.get();
}

// Function: nodeset_ptr
inline nodeset_pt Timer::nodeset_ptr() const {
  return _nodeset_uptr.get();
}

// Function: nodeset
inline nodeset_rt Timer::nodeset() const {
  return *nodeset_ptr();
}

// Function: jumpset_ptr
inline jumpset_pt Timer::jumpset_ptr() const {
  return _jumpset_uptr.get();
}

// Function: jumpset
inline jumpset_rt Timer::jumpset() const {
  return *jumpset_ptr();
}

// Function: edgelist_ptr
// Return the pointer to the edgelist.
inline edgelist_pt Timer::edgelist_ptr() const {
  return _edgelist_ptr;
}

// Function: pipeline_ptr
// Return the pointer to the pipeline.
inline pipeline_pt Timer::pipeline_ptr() const {
  return _pipeline_uptr.get();
}

// Function: pipeline
// Return the reference to the pipeline.
inline pipeline_rt Timer::pipeline() const {
  return *(pipeline_ptr());
}

// Function: endpoint_minheap_ptr
inline endpoint_minheap_pt Timer::endpoint_minheap_ptr() const {
  return _endpoint_minheap_uptr.get();
}

// Function: endpoint_minheap
inline endpoint_minheap_rt Timer::endpoint_minheap() const {
  return *endpoint_minheap_ptr();
}

// Function: cutoff_slack
// Return the cutoff slack.
inline float_t Timer::cutoff_slack(int el, int rf) const {
  return _cutoff_slack[el][rf];
}

};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif


