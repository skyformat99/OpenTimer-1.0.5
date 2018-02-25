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

#include "ot_iterator.h"

namespace OpenTimer {

// Constructor: CircuitGateIter
CircuitGateIter::CircuitGateIter(circuit_pt circuit_ptr) : _circuit_ptr(circuit_ptr) {
  
  CHECK(_circuit_ptr != nullptr);
  
  _i = _circuit_ptr->gate_dict_ptr()->begin();
}

// Destructor: ~CircuitGateIter
CircuitGateIter::~CircuitGateIter() {
}

// Function: gate_ptr
// Return the pointer to the gate ptr being iterated.
gate_pt CircuitGateIter::gate_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CircuitGateIter::operator ()() const { 
  return _i != _circuit_ptr->gate_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CircuitGateIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: CircuitNetIter
CircuitNetIter::CircuitNetIter(circuit_pt circuit_ptr) : _circuit_ptr(circuit_ptr) {
  
  CHECK(_circuit_ptr != nullptr);
  
  _i = _circuit_ptr->net_dict_ptr()->begin();
}

// Constructor: CircuitNetIter
CircuitNetIter::~CircuitNetIter() {
}

// Function: net_ptr
// Return the pointer to the net being iterated.
net_pt CircuitNetIter::net_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CircuitNetIter::operator ()() const { 
  return _i != _circuit_ptr->net_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CircuitNetIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: NetPinlistIter
NetPinlistIter::NetPinlistIter(net_pt net_ptr) : _net_ptr(net_ptr) {
  
  CHECK(_net_ptr != nullptr);
  
  _i = _net_ptr->pinlist_ptr()->head();
}

// Constructor: NetPinlistIter
NetPinlistIter::~NetPinlistIter() {
}

// Function: net_ptr
// Return the pointer to the net being iterated.
pin_pt NetPinlistIter::pin_ptr() const {
  return _i->item();
}

// Operator: ()
// Check if the current itr ends.
bool_t NetPinlistIter::operator ()() const { 
  return _i != nullptr;
}

// Operator: ++
// Increment the step.
void_t NetPinlistIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: CircuitPinIter
CircuitPinIter::CircuitPinIter(circuit_pt circuit_ptr) : _circuit_ptr(circuit_ptr) {
  
  CHECK(_circuit_ptr != nullptr);
  
  _i = _circuit_ptr->pin_dict_ptr()->begin();
}

// Constructor: CircuitPinIter
CircuitPinIter::~CircuitPinIter() {
}

// Function: pin_ptr
// Return the pointer to the pin being iterated.
pin_pt CircuitPinIter::pin_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CircuitPinIter::operator ()() const { 
  return _i != _circuit_ptr->pin_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CircuitPinIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: CircuitPrimaryInputIter
CircuitPrimaryInputIter::CircuitPrimaryInputIter(circuit_pt circuit_ptr) : _circuit_ptr(circuit_ptr) {
  
  CHECK(_circuit_ptr != nullptr);
  
  _i = _circuit_ptr->primary_input_dict_ptr()->begin();
}

// Constructor: CircuitPrimaryInputIter
CircuitPrimaryInputIter::~CircuitPrimaryInputIter() {
}

// Function: primary_input_ptr
// Return the pointer to the primary input port being iterated.
primary_input_pt CircuitPrimaryInputIter::primary_input_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CircuitPrimaryInputIter::operator ()() const { 
  return _i != _circuit_ptr->primary_input_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CircuitPrimaryInputIter::operator ++() {
  ++_i;
}

// Constructor: CircuitPrimaryOutputIter
CircuitPrimaryOutputIter::CircuitPrimaryOutputIter(circuit_pt circuit_ptr) : _circuit_ptr(circuit_ptr) {
  
  CHECK(_circuit_ptr != nullptr);
  
  _i = _circuit_ptr->primary_output_dict_ptr()->begin();
}

// Constructor: CircuitPrimaryOutputIter
CircuitPrimaryOutputIter::~CircuitPrimaryOutputIter() {
}

// Function: primary_output_ptr
// Return the pointer to the primary output port being iterated.
primary_output_pt CircuitPrimaryOutputIter::primary_output_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CircuitPrimaryOutputIter::operator ()() const { 
  return _i != _circuit_ptr->primary_output_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CircuitPrimaryOutputIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: LibraryCellIter
LibraryCellIter::LibraryCellIter(celllib_pt celllib_ptr) : _celllib_ptr(celllib_ptr) {
  
  CHECK(_celllib_ptr != nullptr);
  
  _i = _celllib_ptr->cell_dict_ptr()->begin();
}

// Constructor: LibraryCellIter
LibraryCellIter::~LibraryCellIter() {
}

// Function: cell_ptr
// Return the pointer to the timing arc being iterated.
cell_pt LibraryCellIter::cell_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t LibraryCellIter::operator ()() const { 
  return _i != _celllib_ptr->cell_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t LibraryCellIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: LUTTemplateDictIter
LUTTemplateDictIter::LUTTemplateDictIter(lut_template_dict_pt lut_template_dict_ptr) : 
  _lut_template_dict_ptr(lut_template_dict_ptr) {
  
  CHECK(_lut_template_dict_ptr != nullptr);
  
  _i = _lut_template_dict_ptr->begin();
}

// Constructor: LUTTemplateDictIter
LUTTemplateDictIter::~LUTTemplateDictIter() {
}

// Function: lut_template_ptr
// Return the pointer to the lut template being iterated.
lut_template_pt LUTTemplateDictIter::lut_template_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t LUTTemplateDictIter::operator ()() const { 
  return _i != _lut_template_dict_ptr->end();
}

// Operator: ++
// Increment the step.
void_t LUTTemplateDictIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: CellpinIter
CellpinIter::CellpinIter(cell_pt cell_ptr) : _cell_ptr(cell_ptr) {
  
  CHECK(_cell_ptr != nullptr);
  
  _i = _cell_ptr->cellpin_dict_ptr()->begin();
}

// Constructor: CellpinIter
CellpinIter::~CellpinIter() {
}

// Function: cellpin_ptr
// Return the pointer to the cellpin being iterated.
cellpin_pt CellpinIter::cellpin_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t CellpinIter::operator ()() const { 
  return _i != _cell_ptr->cellpin_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t CellpinIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: CellpinEarlyLateIter
CellpinEarlyLateIter::CellpinEarlyLateIter(cell_pt early_cell_ptr, cell_pt late_cell_ptr) {
  
  CHECK(early_cell_ptr != nullptr && late_cell_ptr != nullptr);
  CHECK(early_cell_ptr->name() == late_cell_ptr->name());
  
  _cell_ptr[EARLY] = early_cell_ptr;
  _cell_ptr[LATE ] = late_cell_ptr;
  _i[EARLY] = _cell_ptr[EARLY]->cellpin_dict_ptr()->begin();
  _i[LATE ] = _cell_ptr[LATE ]->cellpin_dict_ptr()->begin();
}

// Constructor: CellpinEarlyLateIter
CellpinEarlyLateIter::~CellpinEarlyLateIter() {
}

// Function: cellpin_ptr
// Return the pointer to the cellpin being iterated.
cellpin_pt CellpinEarlyLateIter::cellpin_ptr(int el) const {
  return _i[el]->second;
}

// Function: cellpin_name
// Return the cellpin name.
string_crt CellpinEarlyLateIter::cellpin_name() const {

  CHECK(cellpin_ptr(EARLY)->name() == cellpin_ptr(LATE)->name());

  return cellpin_ptr(EARLY)->name();
}

// Function: cellpin_direction
// Return the cellpin direction.
pin_direction_e CellpinEarlyLateIter::cellpin_direction() const {
  
  CHECK(cellpin_ptr(EARLY)->direction() == cellpin_ptr(LATE)->direction());

  return cellpin_ptr(EARLY)->direction();
}

// Operator: ()
// Check if the current itr ends.
bool_t CellpinEarlyLateIter::operator ()() const { 
  return (_i[EARLY] != _cell_ptr[EARLY]->cellpin_dict_ptr()->end() &&
          _i[LATE ] != _cell_ptr[LATE ]->cellpin_dict_ptr()->end());
}

// Operator: ++
// Increment the step.
void_t CellpinEarlyLateIter::operator ++() {
  ++_i[EARLY];
  ++_i[LATE];
}

//-------------------------------------------------------------------------------------------------

// Constructor: TimingArcIter
TimingArcIter::TimingArcIter(cellpin_pt early_cellpin_ptr, cellpin_pt late_cellpin_ptr) {

  CHECK(early_cellpin_ptr != nullptr && late_cellpin_ptr != nullptr);

  _cellpin_ptr[EARLY] = early_cellpin_ptr;
  _cellpin_ptr[LATE] = late_cellpin_ptr;
  _i[EARLY] = _cellpin_ptr[EARLY]->timing_arc_dict_ptr()->begin();
  _i[LATE] = _cellpin_ptr[LATE]->timing_arc_dict_ptr()->begin();

  CHECK(_cellpin_ptr[EARLY]->num_timing_arcs() == _cellpin_ptr[LATE]->num_timing_arcs());
}

// Constructor: TimingArcIter
TimingArcIter::~TimingArcIter() {
}

// Function: timing_arc_ptr
// Return the pointer to the cellpin being iterated.
timing_arc_pt TimingArcIter::timing_arc_ptr(int el) const {
  return _i[el]->second;
}

// Function: from_cellpin_name
string_crt TimingArcIter::from_cellpin_name() const {

  CHECK(timing_arc_ptr(EARLY)->from_cellpin_name() == timing_arc_ptr(LATE)->from_cellpin_name());

  return _i[EARLY]->second->from_cellpin_name();
}

// Function: to_cellpin_name
string_crt TimingArcIter::to_cellpin_name() const {

  
  CHECK(timing_arc_ptr(EARLY)->to_cellpin_name() == timing_arc_ptr(LATE)->to_cellpin_name());
  

  return _i[EARLY]->second->to_cellpin_name();
}

// Operator: ()
// Check if the current itr ends.
bool_t TimingArcIter::operator ()() const { 
  return _i[EARLY] != _cellpin_ptr[EARLY]->timing_arc_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t TimingArcIter::operator ++() {
  ++_i[EARLY];
  ++_i[LATE];
}

// Function: is_constraint
bool_t TimingArcIter::is_constraint() const {
  return timing_arc_ptr(EARLY)->is_constraint();
}

//-------------------------------------------------------------------------------------------------

// Constructor: VerilogModuleIter
VerilogModuleIter::VerilogModuleIter(verilog_pt verilog_ptr) : _verilog_ptr(verilog_ptr) {
  
  CHECK(_verilog_ptr != nullptr);
  
  _i = _verilog_ptr->module_dict_ptr()->begin();
}

// Constructor: VerilogModuleIter
VerilogModuleIter::~VerilogModuleIter() {
}

// Function: verilog_gate_pt
// Return the pointer to the timing arc being iterated.
verilog_module_pt VerilogModuleIter::module_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t VerilogModuleIter::operator ()() const { 
  return _i != _verilog_ptr->module_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t VerilogModuleIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor
VerilogModuleInputIter::VerilogModuleInputIter(verilog_module_pt verilog_module_ptr) 
: _verilog_module_ptr(verilog_module_ptr) {
  
  CHECK(_verilog_module_ptr != nullptr);
  
  _i = _verilog_module_ptr->inputs_ptr()->begin();
}

// Destructor.
VerilogModuleInputIter::~VerilogModuleInputIter() {
}

// Function: verilog_input
verilog_input_pt VerilogModuleInputIter::input_ptr() const {
  return *_i;
}

// Operator: ()
bool_t VerilogModuleInputIter::operator ()() const {
  return (_i != _verilog_module_ptr->inputs_ptr()->end());
}

// Operator: ++
void_t VerilogModuleInputIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor
VerilogModuleWireIter::VerilogModuleWireIter(verilog_module_pt verilog_module_ptr) 
: _verilog_module_ptr(verilog_module_ptr) {
  
  CHECK(_verilog_module_ptr != nullptr);
  
  _i = _verilog_module_ptr->wires_ptr()->begin();
}

// Destructor.
VerilogModuleWireIter::~VerilogModuleWireIter() {
}

// Function: verilog_wire
verilog_wire_pt VerilogModuleWireIter::wire_ptr() const {
  return *_i;
}

// Operator: ()
bool_t VerilogModuleWireIter::operator ()() const {
  return (_i != _verilog_module_ptr->wires_ptr()->end());
}

// Operator: ++
void_t VerilogModuleWireIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor
VerilogModuleOutputIter::VerilogModuleOutputIter(verilog_module_pt verilog_module_ptr) 
: _verilog_module_ptr(verilog_module_ptr) {
  
  CHECK(_verilog_module_ptr != nullptr);
  
  _i = _verilog_module_ptr->outputs_ptr()->begin();
}

// Destructor.
VerilogModuleOutputIter::~VerilogModuleOutputIter() {
}

// Function: verilog_input
verilog_output_pt VerilogModuleOutputIter::output_ptr() const {
  return *_i;
}

// Operator: ()
bool_t VerilogModuleOutputIter::operator ()() const {
  return (_i != _verilog_module_ptr->outputs_ptr()->end());
}

// Operator: ++
void_t VerilogModuleOutputIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor
VerilogModuleGateIter::VerilogModuleGateIter(verilog_module_pt verilog_module_ptr) 
: _verilog_module_ptr(verilog_module_ptr) {
  
  CHECK(_verilog_module_ptr != nullptr);
  
  _i = _verilog_module_ptr->gates_ptr()->begin();
}

// Destructor.
VerilogModuleGateIter::~VerilogModuleGateIter() {
}

// Function: verilog_input
verilog_gate_pt VerilogModuleGateIter::gate_ptr() const {
  return *_i;
}

// Operator: ()
bool_t VerilogModuleGateIter::operator ()() const {
  return (_i != _verilog_module_ptr->gates_ptr()->end());
}

// Operator: ++
void_t VerilogModuleGateIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: VerilogGateCellpinToNetMappingIter
VerilogGateCellpinToNetMappingIter::VerilogGateCellpinToNetMappingIter(verilog_gate_pt gate_ptr) : _gate_ptr(gate_ptr){
  
  CHECK(_gate_ptr != nullptr);
  CHECK(_gate_ptr->cellpin_to_net_ptr() != nullptr);
  
  _i = _gate_ptr->cellpin_to_net_ptr()->begin();
}

// Constructor: VerilogGateCellpinToNetMappingIter
VerilogGateCellpinToNetMappingIter::~VerilogGateCellpinToNetMappingIter() {
}

// Function: cellpin
// Return the cellpin name.
char_cpt VerilogGateCellpinToNetMappingIter::cellpin_name() const {
  return _i->first.c_str(); 
}

// Function: net
// Return the net name.
char_cpt VerilogGateCellpinToNetMappingIter::net_name() const {
  return _i->second.c_str(); 
}

// Operator: ()
// Check if the current itr ends.
bool_t VerilogGateCellpinToNetMappingIter::operator ()() const { 
  return _i != _gate_ptr->cellpin_to_net_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t VerilogGateCellpinToNetMappingIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: TimerEdgeIter
TimerEdgeIter::TimerEdgeIter(timer_pt timer_ptr) : _timer_ptr(timer_ptr) {
  
  CHECK(_timer_ptr != nullptr);
  
  _i = _timer_ptr->edgelist_ptr()->head();
}

// Constructor: TimerEdgeIter
TimerEdgeIter::~TimerEdgeIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
edge_pt TimerEdgeIter::edge_ptr() const {
  return _i->item();
}

// Operator: ()
// Check if the current itr ends.
bool_t TimerEdgeIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t TimerEdgeIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: NodelistIter
NodelistIter::NodelistIter(nodelist_pt nodelist_ptr) : _nodelist_ptr(nodelist_ptr) {
  
  CHECK(_nodelist_ptr != nullptr);
  
  _i = _nodelist_ptr->head();
}

// Destructor: ~NodelistIter
NodelistIter::~NodelistIter() {
}

// Function: node_ptr
// Return the pointer to the node that is being iterated.
node_pt NodelistIter::node_ptr() const {
  return _i->item();
}

// Operator: ()
bool_t NodelistIter::operator ()() const {
  return _i;
}

// Operator: ++
// Increment the step.
void_t NodelistIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: EdgelistIter
EdgelistIter::EdgelistIter(edgelist_pt edgelist_ptr) : _edgelist_ptr(edgelist_ptr) {
  
  CHECK(_edgelist_ptr != nullptr);
  
  _i = _edgelist_ptr->head();
}

// Destructor: ~EdgelistIter
EdgelistIter::~EdgelistIter() {
}

// Function: edge_ptr
// Return the pointer to the edge that is being iterated.
edge_pt EdgelistIter::edge_ptr() const {
  return _i->item();
}

// Function: from_node_ptr
// Return the from_node_ptr
node_pt EdgelistIter::from_node_ptr() const {
  return edge_ptr()->from_node_ptr();
}

// Function: to_node_ptr
// Return the to_node_ptr
node_pt EdgelistIter::to_node_ptr() const {
  return edge_ptr()->to_node_ptr();
}

// Operator: ()
bool_t EdgelistIter::operator ()() const {
  return _i;
}

// Operator: ++
// Increment the step.
void_t EdgelistIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: NodeFaninIter
NodeFaninIter::NodeFaninIter(node_pt node_ptr) : _node_ptr(node_ptr) {
  
  CHECK(_node_ptr != nullptr);
  
  _i = _node_ptr->fanin_ptr()->head();
}

// Constructor: NodeFaninIter
NodeFaninIter::~NodeFaninIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
edge_pt NodeFaninIter::edge_ptr() const {
  return _i->item();
}

// Function: edge_type
// Return the edgetype.
edge_type_e NodeFaninIter::edge_type() const {
  return edge_ptr()->edge_type();
}

// Function: from_node_ptr
// Return the pointer to the fanin node.
node_pt NodeFaninIter::from_node_ptr() const {
  return _i->item()->from_node_ptr();
}

// Operator: ()
// Check if the current itr ends.
bool_t NodeFaninIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t NodeFaninIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: NodeFanoutIter
NodeFanoutIter::NodeFanoutIter(node_pt node_ptr) : _node_ptr(node_ptr) {
  
  CHECK(_node_ptr != nullptr);
  
  _i = _node_ptr->fanout_ptr()->head();
}

// Constructor: NodeFanoutIter
NodeFanoutIter::~NodeFanoutIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
edge_pt NodeFanoutIter::edge_ptr() const {
  return _i->item();
}

// Function: edge_type
// Return the edgetype.
edge_type_e NodeFanoutIter::edge_type() const {
  return edge_ptr()->edge_type();
}

// Function: to_node_ptr
// Return the pointer to the to_node.
node_pt NodeFanoutIter::to_node_ptr() const {
  return _i->item()->to_node_ptr();
}

// Operator: ()
// Check if the current itr ends.
bool_t NodeFanoutIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t NodeFanoutIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: NodeJumpinIter
NodeJumpinIter::NodeJumpinIter(node_pt node_ptr) : _node_ptr(node_ptr) {
  
  CHECK(_node_ptr != nullptr);
  
  _i = _node_ptr->jumpin_ptr()->head();
}

// Constructor: NodeJumpinIter
NodeJumpinIter::~NodeJumpinIter() {
}

// Function: jump_ptr
// Return the pointer to the jump being iterated.
jump_pt NodeJumpinIter::jump_ptr() const {
  return _i->item();
}

// Function: from_node_ptr
// Return the pointer to the fanin node.
node_pt NodeJumpinIter::from_node_ptr() const {
  return _i->item()->from_node_ptr();
}

// Operator: ()
// Check if the current itr ends.
bool_t NodeJumpinIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t NodeJumpinIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: NodeJumpoutIter
NodeJumpoutIter::NodeJumpoutIter(node_pt node_ptr) : _node_ptr(node_ptr) {
  
  CHECK(_node_ptr != nullptr);
  
  _i = _node_ptr->jumpout_ptr()->head();
}

// Constructor: NodeJumpinIter
NodeJumpoutIter::~NodeJumpoutIter() {
}

// Function: jump_ptr
// Return the pointer to the jump being iterated.
jump_pt NodeJumpoutIter::jump_ptr() const {
  return _i->item();
}

// Function: to_node_ptr
// Return the pointer to the jumpout node.
node_pt NodeJumpoutIter::to_node_ptr() const {
  return _i->item()->from_node_ptr();
}

// Operator: ()
// Check if the current itr ends.
bool_t NodeJumpoutIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t NodeJumpoutIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: RCTreeNodeIter
RCTreeNodeIter::RCTreeNodeIter(rctree_pt rctree_ptr) : _rctree_ptr(rctree_ptr) {
  
  CHECK(_rctree_ptr != nullptr);
  
  _i = _rctree_ptr->rctree_node_dict_ptr()->begin();
}

// Constructor: RCTreeNodeIter
RCTreeNodeIter::~RCTreeNodeIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
rctree_node_pt RCTreeNodeIter::rctree_node_ptr() const {
  return _i->second;
}

// Operator: ()
// Check if the current itr ends.
bool_t RCTreeNodeIter::operator ()() const { 
  return _i != _rctree_ptr->rctree_node_dict_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t RCTreeNodeIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: RCTreeEdgeIter
RCTreeEdgeIter::RCTreeEdgeIter(rctree_pt rctree_ptr) : _rctree_ptr(rctree_ptr) {
  
  CHECK(_rctree_ptr != nullptr);
  
  _i = _rctree_ptr->rctree_edgelist_ptr()->head();
}

// Constructor: RCTreeEdgeIter
RCTreeEdgeIter::~RCTreeEdgeIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
rctree_edge_pt RCTreeEdgeIter::rctree_edge_ptr() const {
  return _i->item();
}

// Operator: ()
// Check if the current itr ends.
bool_t RCTreeEdgeIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t RCTreeEdgeIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: RCTreeNodeFaninIter
RCTreeNodeFaninIter::RCTreeNodeFaninIter(rctree_node_pt rctree_node_ptr) : _rctree_node_ptr(rctree_node_ptr) {
  
  CHECK(_rctree_node_ptr != nullptr);
  
  _i = _rctree_node_ptr->fanin_ptr()->head();
}

// Constructor: RCTreeNodeFaninIter
RCTreeNodeFaninIter::~RCTreeNodeFaninIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
rctree_edge_pt RCTreeNodeFaninIter::rctree_edge_ptr() const {
  return _i->item();
}

// Operator: ()
// Check if the current itr ends.
bool_t RCTreeNodeFaninIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t RCTreeNodeFaninIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: RCTreeNodeFanoutIter
RCTreeNodeFanoutIter::RCTreeNodeFanoutIter(rctree_node_pt rctree_node_ptr) : _rctree_node_ptr(rctree_node_ptr) {
  
  CHECK(_rctree_node_ptr != nullptr);
  
  _i = _rctree_node_ptr->fanout_ptr()->head();
}

// Constructor: RCTreeNodeFanoutIter
RCTreeNodeFanoutIter::~RCTreeNodeFanoutIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
rctree_edge_pt RCTreeNodeFanoutIter::rctree_edge_ptr() const {
  return _i->item();
}

// Operator: ()
// Check if the current itr ends.
bool_t RCTreeNodeFanoutIter::operator ()() const { 
  return _i;
}

// Operator: ++
// Increment the step.
void_t RCTreeNodeFanoutIter::operator ++() {
  _i = _i->next();
}

//-------------------------------------------------------------------------------------------------

// Constructor: SpefNetIter
SpefNetIter::SpefNetIter(spef_pt spef_ptr) : _spef_ptr(spef_ptr) {
  
  CHECK(spef_ptr != nullptr);
  
  _i = spef_ptr->spefnets_ptr()->begin();
}

// Constructor: SpefNetIter
SpefNetIter::~SpefNetIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
spefnet_pt SpefNetIter::spefnet_ptr() const {
  return *_i;
}

// Operator: ()
// Check if the current itr ends.
bool_t SpefNetIter::operator ()() const { 
  return _i != _spef_ptr->spefnets_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t SpefNetIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: SpefPinIter
SpefPinIter::SpefPinIter(spefnet_pt spefnet_ptr) : _spefnet_ptr(spefnet_ptr) {
  
  CHECK(spefnet_ptr != nullptr);
  
  _i = spefnet_ptr->spefpins_ptr()->begin();
}

// Constructor: SpefPinIter
SpefPinIter::~SpefPinIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
spefpin_pt SpefPinIter::spefpin_ptr() const {
  return *_i;
}

// Operator: ()
// Check if the current itr ends.
bool_t SpefPinIter::operator ()() const { 
  return _i != _spefnet_ptr->spefpins_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t SpefPinIter::operator ++() {
  ++_i;
}

// Constructor: SpefCapIter
SpefCapIter::SpefCapIter(spefnet_pt spefnet_ptr) : _spefnet_ptr(spefnet_ptr) {
  
  CHECK(spefnet_ptr != nullptr);
  
  _i = spefnet_ptr->spefcaps_ptr()->begin();
}

// Constructor: SpefCapIter
SpefCapIter::~SpefCapIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
spefcap_pt SpefCapIter::spefcap_ptr() const {
  return *_i;
}

// Operator: ()
// Check if the current itr ends.
bool_t SpefCapIter::operator ()() const { 
  return _i != _spefnet_ptr->spefcaps_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t SpefCapIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: SpefResIter
SpefResIter::SpefResIter(spefnet_pt spefnet_ptr) : _spefnet_ptr(spefnet_ptr) {
  
  CHECK(spefnet_ptr != nullptr);
  
  _i = spefnet_ptr->spefreses_ptr()->begin();
}

// Constructor: SpefResIter
SpefResIter::~SpefResIter() {
}

// Function: edge_ptr
// Return the pointer to the edge being iterated.
spefres_pt SpefResIter::spefres_ptr() const {
  return *_i;
}

// Operator: ()
// Check if the current itr ends.
bool_t SpefResIter::operator ()() const { 
  return _i != _spefnet_ptr->spefreses_ptr()->end();
}

// Operator: ++
// Increment the step.
void_t SpefResIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: PathIter
PathIter::PathIter(path_pt path_ptr) {

  
  CHECK(path_ptr != nullptr);
  

  _path_ptr = path_ptr;
  _i = _path_ptr->begin();
}

// Constructor: PathIter
PathIter::PathIter(path_rt path) {
  _path_ptr = &path;
  _i = _path_ptr->begin();
}

// Destructor: PathIter
PathIter::~PathIter() {

}

// Function: node_ptr
// Query the node pointer.
node_pt PathIter::node_ptr() const {
  return _i->second;
}

// Function: pin_ptr
// Query the pin pointer.
pin_pt PathIter::pin_ptr() const {
  return node_ptr()->pin_ptr();
}

// Function: rf
// Query the transition status.
int PathIter::rf() const {
  return _i->first;
}

// Operator: ()
bool_t PathIter::operator ()() const {
  return _i != _path_ptr->end();
}

// Operator: ++
void_t PathIter::operator ++() {
  ++_i;
}

//-------------------------------------------------------------------------------------------------

// Constructor: PathRIter
PathRIter::PathRIter(path_pt path_ptr) {

  CHECK(path_ptr != nullptr);

  _path_ptr = path_ptr;
  _i = _path_ptr->rbegin();
}

// Constructor: PathRIter
PathRIter::PathRIter(path_rt path) {
  _path_ptr = &path;
  _i = _path_ptr->rbegin();
}

// Destructor: PathRIter
PathRIter::~PathRIter() {

}

// Function: node_ptr
// Query the node pointer.
node_pt PathRIter::node_ptr() const {
  return _i->second;
}

// Function: pin_ptr
// Query the pin pointer.
pin_pt PathRIter::pin_ptr() const {
  return node_ptr()->pin_ptr();
}

// Function: rf
// Query the transition status.
int PathRIter::rf() const {
  return _i->first;
}

// Operator: ()
bool_t PathRIter::operator ()() const {
  return _i != _path_ptr->rend();
}

// Operator: ++
void_t PathRIter::operator ++() {
  ++_i;
}


};


