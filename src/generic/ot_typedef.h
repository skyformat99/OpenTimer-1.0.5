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

#ifndef OT_TYPEDEF_H_
#define OT_TYPEDEF_H_

#include "ot_unitdef.h"
#include "ot_headerdef.h"
#include "ot_enumdef.h"
#include "ot_classdef.h"
#include "ot_dictionary.h"
#include "ot_list.h"
#include "ot_orderedset.h"
#include "ot_geometry.h"
#include "ot_allocator.h"

namespace OpenTimer {

// Typedef definition field.
// suffix 't': generic type.
// suffix 'e': enum type.
// prefix 'p': ptr.
// prefix 'up': unique ptr.
// prefix 'c': const.
// prefix 'r': reference.
// prefix 'v': vector.
// prefix 's': static.
// prefix 'm': matrix type.
// prefix 'dict': dict type.

// Typedef of integer variable.
typedef int          int_t;                                 // Integer type.
typedef int*         int_pt;                                // Integer ptr type.
typedef int&         int_rt;                                // Integer reference type.
typedef const int    int_ct;                                // Constant integer type.
typedef const int*   int_cpt;                               // Constant integer ptr type.
typedef const int&   int_crt;                               // Constant integer reference type.
typedef vector < int > int_vt;                              // Int 1D vector type.
typedef vector < int >* int_vpt;                            // Int 1D vector ptr type.
typedef vector < int >& int_vrt;                            // Int 1D vector reference type.
typedef vector < vector<int> > int_mt;                      // Int 2D vector type.
typedef vector < vector<int> >* int_mpt;                    // Int 2D vector ptr type.
typedef vector < vector<int> >& int_mrt;                    // Int 2D vector reference type.
typedef struct IntPair int_pair_t;                          // Int pair type.
typedef struct IntPair& int_pair_rt;                        // Int pair reference type.
typedef struct IntPair* int_pair_pt;                        // Int pair ptr type.
typedef const struct IntPair int_pair_ct;                   // Constant int pair type.
typedef const struct IntPair* int_pair_cpt;                 // Constant int pair ptr type.
typedef const struct IntPair& int_pair_crt;                 // Constant int pair reference type.
typedef vector < int_pair_t > int_pair_vt;                  // Int pair vector type.
typedef set < int_t > intset_t;                             // Int set type.
typedef set < int_t >* intset_pt;                           // Int set poitner type.
typedef set < int_t >& intset_rt;                           // Int set reference type.
typedef intset_t::iterator intset_iter_t;                   // Int set iterator.
typedef queue < int_t > int_que_t;                          // Int queue type.

// Typedef of unsigned variable.
typedef unsigned  unsigned_t;                               // Unsigned type.
typedef unsigned* unsigned_pt;                              // Unsigned ptr type.
typedef unsigned& unsigned_rt;                              // Unsigned reference type.
typedef const unsigned  unsigned_ct;                        // Constant unsigned type.
typedef const unsigned* unsigned_cpt;                       // Constant unsigned ptr type.
typedef const unsigned& unsigned_crt;                       // Constant unsigned reference type.

// Typedef of floating variable.
typedef float  float_t;                                     // Numeric type.
typedef float* float_pt;                                    // Numeric ptr type.
typedef float& float_rt;                                    // Numeric reference type.
typedef const float  float_ct;                              // Constant numeric type.
typedef const float* float_cpt;                             // Constant numeric ptr type.
typedef const float& float_crt;                             // Constant numeric reference type.
typedef vector < float > float_vt;                          // Numeric vector type.
typedef vector < float >* float_vpt;                        // Numeric vector ptr type.
typedef vector < float >& float_vrt;                        // Numeric vector reference type.
typedef const vector < float >& float_cvrt;                 // Constant numeric vector ref type.
typedef vector < vector<float> > float_mt;                  // Numeric matrix type.
typedef vector < vector<float> >* float_mpt;                // Numeric matrix type.
typedef vector < vector<float> >& float_mrt;                // Numeric matrix reference type.
typedef const vector < vector<float> >& float_cmrt;         // Constant numeric matrix ref type.
typedef pair <float_t, float_t> float_pair_t;               // Float pair type.
typedef pair <float_t, float_t>* float_pair_pt;             // Float pair ptr type.
typedef pair <float_t, float_t>& float_pair_rt;             // Float pair ref type.

// Typedef of char variable.
typedef char              char_t;                           // Char type.
typedef char*             char_pt;                          // Char ptr type.
typedef char&             char_rt;                          // Char reference type.
typedef char*&            char_prt;                         // Char reference ptr type.
typedef const char        char_ct;                          // Constant char type.
typedef const char*       char_cpt;                         // Constant char ptr type.
typedef const char&       char_crt;                         // Constant char reference type.
typedef vector < char_t > char_vt;                          // Char 1D vector type.
typedef vector < char_t >* char_vpt;                        // Char 1D vector type.
typedef vector < char_t >& char_vrt;                        // Char 1D reference vector type.

// Typedef of void variable.
typedef void              void_t;                           // Void type.
typedef void*             void_pt;                          // Void ptr type.
typedef const void        void_ct;                          // Constant har void.
typedef const void*       void_cpt;                         // Constant void ptr type.

// Typedef of boolean variable.
typedef bool          bool_t;                               // Bool type.
typedef bool*         bool_pt;                              // Bool ptr type.
typedef bool&         bool_rt;                              // Bool reference type.
typedef const bool    bool_ct;                              // Constant bool type.
typedef const bool*   bool_cpt;                             // Constant bool ptr type.
typedef const bool&   bool_crt;                             // Constant bool reference type.
typedef vector < bool > bool_vt;                            // Bool vector type.
typedef vector < bool >* bool_vpt;                          // Bool vector type.
typedef vector < bool >& bool_vrt;                          // Bool vector type.
typedef vector < vector<bool> > bool_mt;                    // Bool matrix type.
typedef vector < vector<bool> >* bool_mpt;                  // Bool matrix type.
typedef vector < vector<bool> >& bool_mrt;                  // Bool matrix type.

// Typedef of a point variable.
typedef Point <float_t> point_t;                            // Point type.
typedef Point <float_t>* point_pt;                          // Point ptr type.
typedef Point <float_t>& point_rt;                          // Point reference type.
typedef const Point <float_t> point_ct;                     // Constant point type.
typedef const Point <float_t>& point_crt;                   // Constant point reference type.
typedef Point <float_t>::value_t point_value_t;             // Point value type.
typedef const Point <float_t>::value_t point_value_ct;      // Point value type.
typedef const Point <float_t>::value_t* point_value_pt;     // Point value ptr type.
typedef const Point <float_t>::value_t& point_value_rt;     // Point value reference type.

// Typedef of a rectangle variable.
typedef Rectangle <point_value_t> rectangle_t;              // Rectangle type.
typedef Rectangle <point_value_t>* rectangle_pt;            // Rectangle ptr type.
typedef Rectangle <point_value_t>& rectangle_rt;            // Rectangle reference type.
typedef const Rectangle <point_value_t> rectangle_ct;       // Constant point type.
typedef const Rectangle <point_value_t>& rectangle_crt;     // Constant point reference type.
typedef vector <rectangle_t> rectangle_vt;                  // Rectangle vector type.
typedef vector <rectangle_t>* rectangle_vpt;                // Rectangle vector ptr type.
typedef vector <rectangle_t>& rectangle_vrt;                // Rectangle vector reference type.    

// Typedef of the rc-tree variable.
typedef RCTree rctree_t;                                    // RCTree type.
typedef RCTree* rctree_pt;                                  // RCTree point type.
typedef RCTree& rctree_rt;                                  // RCTree reference type.
typedef const RCTree rctree_ct;                             // Constant RCTree type.
typedef const RCTree* rctree_cpt;                           // Constant point RCTree type.
typedef const RCTree& rctree_crt;                           // Constant reference RCTree type.
typedef unique_ptr<rctree_t> rctree_upt;                    // RCTree unique ptr type.

// Typedef of the rc-tree node variable.
typedef RCTreeNode rctree_node_t;                           // RCTreeNode type.
typedef RCTreeNode* rctree_node_pt;                         // RCTreeNode point type.
typedef RCTreeNode& rctree_node_rt;                         // RCTreeNode reference type.
typedef const RCTreeNode rctree_node_ct;                    // Constant RCTreeNode type.
typedef const RCTreeNode* rctree_node_cpt;                  // Constant point RCTreeNode type.
typedef const RCTreeNode& rctree_node_crt;                  // Constant reference RCTreeNode type.

// Typedef of the rc-tree edge variable.
typedef RCTreeEdge rctree_edge_t;                           // RCTreeEdge type.
typedef RCTreeEdge* rctree_edge_pt;                         // RCTreeEdge point type.
typedef RCTreeEdge& rctree_edge_rt;                         // RCTreeEdge reference type.
typedef const RCTreeEdge rctree_edge_ct;                    // Constant RCTreeEdge type.
typedef const RCTreeEdge* rctree_edge_cpt;                  // Constant point RCTreeEdge type.
typedef const RCTreeEdge& rctree_edge_crt;                  // Constant reference RCTreeEdge type.

// Typedef of the file reader.
typedef FileReader freader_t;                               // FileReader type.
typedef FileReader* freader_pt;                             // FileReader point type.
typedef FileReader& freader_rt;                             // FileReader reference type.
typedef const FileReader freader_ct;                        // Constant FileReader type.
typedef const FileReader* freader_cpt;                      // Constant point FileReader type.
typedef const FileReader& freader_crt;                      // Constant reference FileReader type.

// Typedef of string variable.
typedef string  string_t;                                   // String type.
typedef string* string_pt;                                  // String ptr type.
typedef string& string_rt;                                  // String reference type.
typedef const string    string_ct;                          // Constant string type.
typedef const string*   string_cpt;                         // Constant string ptr type.
typedef const string&   string_crt;                         // Constant string reference type.
typedef vector < string > string_vt;                        // String vector type.
typedef vector < string >* string_vpt;                      // String vector ptr type.
typedef vector < string >& string_vrt;                      // String vector reference type.
typedef string_vt::iterator string_vec_iter_t;              // String vector iterator type.

// Stringstream
typedef stringstream stringstream_t;                        // String stream type.
typedef stringstream* stringstream_pt;                      // String stream ptr type.
typedef stringstream& stringstream_rt;                      // String stream ref type.

// Typedef of cell variable.
typedef Cell        cell_t;                                 // Cell type.
typedef Cell*       cell_pt;                                // Cell ptr type.
typedef Cell&       cell_rt;                                // Cell reference type.
typedef const Cell  cell_ct;                                // Constant cell type.
typedef const Cell* cell_cpt;                               // Constant cell ptr type.
typedef const Cell& cell_crt;                               // Constant cell reference type.

// Typedef of macropin variable.
typedef Macropin  macropin_t;                               // Macropin type.
typedef Macropin* macropin_pt;                              // Macropin ptr type.
typedef Macropin& macropin_rt;                              // Macropin reference type.
typedef const Macropin  macropin_ct;                        // Constant macropin type.
typedef const Macropin* macropin_cpt;                       // Constant macropin ptr type.
typedef const Macropin& macropin_crt;                       // Constant macropin reference type.

// Typedef of cellpin variable.
typedef Cellpin  cellpin_t;                                 // Cellpin type.
typedef Cellpin* cellpin_pt;                                // Cellpin ptr type.
typedef Cellpin& cellpin_rt;                                // Cellpin reference type.
typedef const Cellpin  cellpin_ct;                          // Constant cellpin type.
typedef const Cellpin* cellpin_cpt;                         // Constant cellpin ptr type.
typedef const Cellpin& cellpin_crt;                         // Constant cellpin reference type.

// Typedef of techlib variable.
typedef TechLib        techlib_t;                           // Celllib type.
typedef TechLib*       techlib_pt;                          // Celllib ptr type.
typedef TechLib&       techlib_rt;                          // Celllib reference type.
typedef const TechLib  techlib_ct;                          // Constant techlib type.
typedef const TechLib* techlib_cpt;                         // Constant techlib ptr type.
typedef const TechLib& techlib_crt;                         // Constant techlib reference type.
typedef unique_ptr < techlib_t > techlib_upt;               // Techlib uptr type.

// Typedef of celllib variable.
typedef CellLib        celllib_t;                           // Celllib type.
typedef CellLib*       celllib_pt;                          // Celllib ptr type.
typedef CellLib&       celllib_rt;                          // Celllib reference type.
typedef const CellLib  celllib_ct;                          // Constant celllib type.
typedef const CellLib* celllib_cpt;                         // Constant celllib ptr type.
typedef const CellLib& celllib_crt;                         // Constant celllib reference type.
typedef unique_ptr < celllib_t > celllib_upt;               // Celllib uptr type.

// Typedef of gate variable.
typedef Gate        gate_t;                                 // Gate type.
typedef Gate*       gate_pt;                                // Gate ptr type.
typedef Gate&       gate_rt;                                // Gate reference type.
typedef const Gate  gate_ct;                                // Constant gate type.
typedef const Gate* gate_cpt;                               // Constant gate ptr type.
typedef const Gate& gate_crt;                               // Constant gate reference type.

// Typedef of primary input port variable.
typedef PrimaryInput        primary_input_t;                // PrimaryInput type.
typedef PrimaryInput*       primary_input_pt;               // PrimaryInput ptr type.
typedef PrimaryInput&       primary_input_rt;               // PrimaryInput reference type.
typedef const PrimaryInput  primary_input_ct;               // Constant primary input port type.
typedef const PrimaryInput* primary_input_cpt;              // Constant primary input port ptr type.
typedef const PrimaryInput& primary_input_crt;              // Constant gate reference type.

// Typedef of primary output port variable.
typedef PrimaryOutput        primary_output_t;              // PrimaryOutput type.
typedef PrimaryOutput*       primary_output_pt;             // PrimaryOutput ptr type.
typedef PrimaryOutput&       primary_output_rt;             // PrimaryOutput reference type.
typedef const PrimaryOutput  primary_output_ct;             // Constant primary output port type.
typedef const PrimaryOutput* primary_output_cpt;            // Constant primary output port ptr type.
typedef const PrimaryOutput& primary_output_crt;            // Constant gate reference type.

// Typedef of net variable.
typedef Net        net_t;                                   // Net type.
typedef Net*       net_pt;                                  // Net ptr type.
typedef Net&       net_rt;                                  // Net reference type.
typedef const Net  net_ct;                                  // Constant net type.
typedef const Net* net_cpt;                                 // Constant net ptr type.
typedef const Net& net_crt;                                 // Constant net reference type.

// Typedef of pin variable.
typedef Pin           pin_t;                                // Pin type.
typedef Pin*          pin_pt;                               // Pin ptr type.
typedef Pin&          pin_rt;                               // Pin reference type.
typedef const Pin     pin_ct;                               // Constant pin type.
typedef const Pin*    pin_cpt;                              // Constant pin ptr type.
typedef const Pin&    pin_crt;                              // Constant pin reference type.

// Typedef of node variable.
typedef Node           node_t;                              // Node type.
typedef Node*          node_pt;                             // Node ptr type.
typedef Node&          node_rt;                             // Node reference type.
typedef const Node     node_ct;                             // Constant node type.
typedef const Node*    node_cpt;                            // Constant node ptr type.
typedef const Node&    node_crt;                            // Constant node reference type.
typedef vector < Node* > node_ptr_vt;                       // Node ptr vector type.
typedef vector < Node* >& node_ptr_vrt;                     // Node ptr vector reference type.
typedef vector < Node* >* node_ptr_vpt;                     // Node ptr vector ptr type.
typedef vector < Node* >::iterator node_ptrs_iter_t;        // Node ptr vector iterator type.
typedef OrderedSet < Node > nodeset_t;                      // Nodeset type.
typedef OrderedSet < Node >* nodeset_pt;                    // Nodeset ptr type.
typedef OrderedSet < Node >& nodeset_rt;                    // Nodeset ref type.
typedef const OrderedSet < Node >& nodeset_crt;             // Constant nodeset ref type.
typedef unique_ptr < nodeset_t > nodeset_upt;               // Nodeset uptr type.

// Typedef of explorer.
typedef struct Explorer explorer_t;                         // Explorer type.
typedef struct Explorer* explorer_pt;                       // Explorer type ptr type.
typedef struct Explorer& explorer_rt;                       // Explorer type reference type.
typedef const struct Explorer& explorer_crt;                // Constant explorer reference type.
typedef vector < explorer_t > explorer_vt;                  // Explorer vector type.
typedef vector < explorer_t >* explorer_vpt;                // Explorer vector ptr type.
typedef vector < explorer_t >& explorer_vrt;                // Explorer vector reference type.
typedef vector < vector < explorer_t > > explorer_mt;       // Explorer matrix type.
typedef vector < vector < explorer_t > >* explorer_mpt;     // Explorer matrix type.
typedef vector < vector < explorer_t > >& explorer_mrt;     // Explorer matrix type.
typedef deque < explorer_t > explorer_qt;                   // Explorer deque type.
typedef deque < explorer_t >* explorer_qpt;                 // Explorer deque ptr type.
typedef deque < explorer_t >& explorer_qrt;                 // Explorer deque reference type.

// Typedef of edge variable.
typedef Edge           edge_t;                              // Edge type.
typedef Edge*          edge_pt;                             // Edge ptr type.
typedef Edge&          edge_rt;                             // Edge reference type.
typedef const Edge     edge_ct;                             // Constant edge type.
typedef const Edge*    edge_cpt;                            // Constant edge ptr type.
typedef const Edge&    edge_crt;                            // Constant edge reference type.
typedef vector < Edge* > edge_ptr_vt;                       // Edge vector type.

// Typedef of jump variable.
typedef Jump jump_t;                                        // Jump type.
typedef Jump* jump_pt;                                      // Jump ptr type.
typedef Jump& jump_rt;                                      // Jump reference type.
typedef const Jump jump_ct;                                 // Constant jump type.
typedef const Jump* jump_cpt;                               // Constant jump ptr type.
typedef const Jump& jump_crt;                               // Constant jump reference type.
typedef vector < Jump* > jump_ptr_vt;                       // Jump vector type.
typedef OrderedSet < Jump > jumpset_t;                      // Jumpset type.
typedef OrderedSet < Jump >* jumpset_pt;                    // Jumpset ptr type.
typedef OrderedSet < Jump >& jumpset_rt;                    // Jumpset reference type.
typedef const OrderedSet < Jump >& jumpset_crt;             // Constant jumpset ref type.
typedef unique_ptr < jumpset_t > jumpset_upt;               // Jumpset uptr type.

// Typedef of circuit_uit variable.
typedef Circuit           circuit_t;                        // Circuit type.
typedef Circuit*          circuit_pt;                       // Circuit ptr type.
typedef Circuit&          circuit_rt;                       // Circuit reference type.
typedef const Circuit     circuit_ct;                       // Constant circuit_uit type.
typedef const Circuit*    circuit_cpt;                      // Constant circuit_uit ptr type.
typedef const Circuit&    circuit_crt;                      // Constant circuit_uit reference type.
typedef unique_ptr < circuit_t > circuit_upt;               // Circuit uptr type.

// Typedef of the clock variable.
typedef ClockTree clock_tree_t;                             // Clock type.
typedef ClockTree* clock_tree_pt;                           // Clock point type.
typedef ClockTree& clock_tree_rt;                           // Clock reference type.
typedef const ClockTree clock_tree_ct;                      // Constant Clock type.
typedef const ClockTree* clock_tree_cpt;                    // Constant point Clock type.
typedef const ClockTree& clock_tree_crt;                    // Constant reference Clock type.
typedef unique_ptr <clock_tree_t> clock_tree_upt;           // Clock tree uptr type.

// Typedef of timing arc variable.
typedef TimingArc        timing_arc_t;                      // Timing-arc type.
typedef TimingArc*       timing_arc_pt;                     // Timing-arc ptr type.
typedef TimingArc&       timing_arc_rt;                     // Timing-arc reference type.
typedef const TimingArc  timing_arc_ct;                     // Constant timing-arc type.
typedef const TimingArc* timing_arc_cpt;                    // Constant timing-arc ptr type.
typedef const TimingArc& timing_arc_crt;                    // Constant timing-arc reference type.
typedef vector < TimingArc* > timing_arcs_vt;               // Timing-arc ptr vector type.
typedef vector < TimingArc* >& timing_arcs_vrt;             // Timing-arc ptr vector reference type.
typedef vector < TimingArc* >* timing_arcs_vpt;             // Timing-arc ptr vector ptr type.
typedef timing_arcs_vt::iterator timing_arcs_iter_t;        // Iterator.

// Typedef of timing variable.
typedef Timing        timing_t;                             // Timing type.
typedef Timing*       timing_pt;                            // Timing ptr type.
typedef Timing&       timing_rt;                            // Timing reference type.
typedef const Timing  timing_ct;                            // Constant timing type.
typedef const Timing* timing_cpt;                           // Constant timing ptr type.
typedef const Timing& timing_crt;                           // Constant timing reference type.
typedef unique_ptr < timing_t > timing_upt;                 // Timing unique ptr type.
typedef vector < Timing* > timing_ptr_vt;                   // Timing ptr vector type.
typedef vector < Timing* >& timing_ptr_vrt;                 // Timing ptr vector reference type.
typedef vector < Timing* >* timing_ptr_vpt;                 // Timing ptr vector ptr type.
typedef timing_ptr_vt::iterator timing_ptrs_iter_t;         // Iterator.
typedef OrderedSet < Timing > timingset_t;                  // Timingset type.
typedef OrderedSet < Timing >& timingset_rt;                // Timingset reference type.
typedef OrderedSet < Timing >* timingset_pt;                // Timingset ptr type.
typedef unique_ptr < timingset_t > timingset_upt;           // Timingset unique pointer.

// Typedef of the test variable.
typedef Test test_t;                                        // Test type.
typedef Test* test_pt;                                      // Test point type.
typedef Test& test_rt;                                      // Test reference type.
typedef const Test test_ct;                                 // Constant onstraint type.
typedef const Test* test_cpt;                               // Constant endptr type.
typedef const Test& test_crt;                               // Constant test reference type.
typedef vector < Test* > test_ptr_vt;                       // Test vector type.
typedef vector < Test* >* test_ptr_vpt;                     // Test vector ptr type.
typedef vector < Test* >& test_ptr_vrt;                     // Test vector reference type.
typedef OrderedSet < Test > testset_t;                      // Testset type.
typedef OrderedSet < Test >& testset_rt;                    // Testset reference type.
typedef OrderedSet < Test >* testset_pt;                    // Testset ptr type.
typedef List < Test* > testlist_t;                          // Test ptr list type.
typedef List < Test* >* testlist_pt;                        // Test ptr list type.
typedef ListNode < Test* >* testlist_iter_t;                // Test ptr list edge ptr type.
typedef unique_ptr < testset_t > testset_upt;               // Testset uptr type.

// Typedef of the endpoint variable.
typedef Endpoint endpoint_t;                                // Endpoint type.
typedef Endpoint* endpoint_pt;                              // Endpoint point type.
typedef Endpoint& endpoint_rt;                              // Endpoint reference type.
typedef const Endpoint endpoint_ct;                         // Constant endpoint type.
typedef const Endpoint* endpoint_cpt;                       // Constant endpoint type.
typedef const Endpoint& endpoint_crt;                       // Constant endpoint reference type.
typedef vector < endpoint_t > endpoint_vt;                  // Endpoint vector type.
typedef vector < endpoint_t >* endpoint_vpt;                // Endpoint vector ptr type.
typedef vector < endpoint_t >& endpoint_vrt;                // Endpoint vector reference type.
typedef vector < endpoint_pt > endpoint_ptr_vt;             // Endpoint ptr vector type.
typedef vector < endpoint_pt >* endpoint_ptr_vpt;           // Endpoint ptr vector ptr type.
typedef vector < endpoint_pt >& endpoint_ptr_vrt;           // Endpoint ptr vector reference type.

// Typedef of endpoint min heap variable.
typedef EndpointMinHeap endpoint_minheap_t;                 // Endpoint type.
typedef EndpointMinHeap* endpoint_minheap_pt;               // Endpoint min heap ptr type.
typedef EndpointMinHeap& endpoint_minheap_rt;               // Endpoint min heap reference type.
typedef unique_ptr < endpoint_minheap_t > endpoint_minheap_upt;

// Typedef of parasitics variable.
typedef Spef spef_t;                                        // Spef type.
typedef Spef* spef_pt;                                      // Spef ptr type.
typedef Spef& spef_rt;                                      // Spef reference type.
typedef const Spef spef_ct;                                 // Constant spef type.
typedef const Spef* spef_cpt;                               // Constant spef ptr type.
typedef const Spef& spef_crt;                               // Constant spef reference type.

// Typedef of parasitics net variable.
typedef SpefNet spefnet_t;                                  // SpefNet type.
typedef SpefNet* spefnet_pt;                                // SpefNet ptr type.
typedef SpefNet& spefnet_rt;                                // SpefNet reference type.
typedef const SpefNet spefnet_ct;                           // Constant spef net type.
typedef const SpefNet* spefnet_cpt;                         // Constant spef net ptr type.
typedef const SpefNet& spefnet_crt;                         // Constant spef net reference type.
typedef vector < SpefNet* > spefnet_ptr_vt;                 // Spef pin ptr vector type.
typedef vector < SpefNet* >& spefnet_ptr_vrt;               // Spef pin ptr vector reference type.
typedef vector < SpefNet* >* spefnet_ptr_vpt;               // Spef pin ptr vector ptr type.
typedef spefnet_ptr_vt::iterator spefnet_ptr_vec_iter_t;    // Iterator.

// Typedef of parasitics pin variable.
typedef SpefPin spefpin_t;                                  // SpefPin type.
typedef SpefPin* spefpin_pt;                                // SpefPin ptr type.
typedef SpefPin& spefpin_rt;                                // SpefPin reference type.
typedef const SpefPin spefpin_ct;                           // Constant spef pin type.
typedef const SpefPin* spefpin_cpt;                         // Constant spef pin ptr type.
typedef const SpefPin& spefpin_crt;                         // Constant spef pin reference type.
typedef vector < SpefPin* > spefpin_ptr_vt;                 // Spef pin ptr vector type.
typedef vector < SpefPin* >* spefpin_ptr_vpt;               // Spef pin ptr vector ptr type.
typedef spefpin_ptr_vt::iterator spefpin_ptr_vec_iter_t;    // Iterator.

// Typedef of parasitics cap variable.
typedef SpefCap spefcap_t;                                  // SpefCap type.
typedef SpefCap* spefcap_pt;                                // SpefCap ptr type.
typedef SpefCap& spefcap_rt;                                // SpefCap reference type.
typedef const SpefCap spefcap_ct;                           // Constant spef cap type.
typedef const SpefCap* spefcap_cpt;                         // Constant spef cap ptr type.
typedef const SpefCap& spefcap_crt;                         // Constant spef cap reference type.
typedef vector < SpefCap* > spefcap_ptr_vt;                 // Spef cap ptr vector type.
typedef vector < SpefCap* >* spefcap_ptr_vpt;               // Spef cap ptr vector ptr type.
typedef spefcap_ptr_vt::iterator spefcap_ptr_vec_iter_t;    // Iterator.

// Typedef of parasitics res variable.
typedef SpefRes spefres_t;                                  // SpefRes type.
typedef SpefRes* spefres_pt;                                // SpefRes ptr type.
typedef SpefRes& spefres_rt;                                // SpefRes reference type.
typedef const SpefRes spefres_ct;                           // Constant spef res type.
typedef const SpefRes* spefres_cpt;                         // Constant spef res ptr type.
typedef const SpefRes& spefres_crt;                         // Constant spef res reference type.
typedef vector < SpefRes* > spefres_ptr_vt;                 // Spef res ptr vector type.
typedef vector < SpefRes* >* spefres_ptr_vpt;               // Spef res ptr vector ptr type.
typedef spefres_ptr_vt::iterator spefres_ptr_vec_iter_t;    // Iterator type.

// Typedef of path finder variable.
typedef CPPR  cppr_t;                                       // CPPR type.
typedef CPPR* cppr_pt;                                      // CPPR ptr type.
typedef CPPR& cppr_rt;                                      // CPPR reference type.
typedef const CPPR  cppr_ct;                                // Constant path finder type.
typedef const CPPR* cppr_cpt;                               // Constant path finder ptr type.
typedef const CPPR& cppr_crt;                               // Constant path finder reference type.

// Typedef of path prefix variable.
typedef Prefix prefix_t;                                    // Prefix type.
typedef Prefix* prefix_pt;                                  // Prefix ptr type.
typedef Prefix& prefix_rt;                                  // Prefix reference type.
typedef const Prefix* prefix_cpt;                           // const prefix ptr type.
typedef const Prefix& prefix_crt;                           // Const prefix reference type.
typedef vector <Prefix*> prefix_ptr_vt;                     // Prefix vector type.
typedef vector <Prefix*>* prefix_ptr_vpt;                   // Prefix vector ptr type.
typedef vector <Prefix*>& prefix_ptr_vrt;                   // Prefix vector reference type.

// Typedef of path.
typedef Path path_t;                                        // Path type.
typedef Path* path_pt;                                      // Path ptr type.
typedef Path& path_rt;                                      // Path reference type.
typedef const Path& path_crt;                               // Path constant reference type.
typedef vector < path_t > path_vt;                          // Path vector type.
typedef vector < path_t >* path_vpt;                        // Path vector ptr type.
typedef vector < path_t >& path_vrt;                        // Path vector reference type.
typedef vector < path_pt > path_ptr_vt;                     // Path ptr vector type.
typedef vector < path_pt >* path_ptr_vpt;                   // Path ptr vector ptr type.
typedef vector < path_pt >& path_ptr_vrt;                   // Path ptr vector reference type.
typedef const vector < path_pt >& path_ptr_cvrt;            // Path ptr vector reference type.
typedef explorer_vt::iterator path_iter_t;                  // Path iterator type.
typedef explorer_vt::reverse_iterator path_riter_t;         // Path reversed iterator type.
typedef vector < path_vt > path_mt;                         // Path matrix type.
typedef vector < path_vt >* path_mpt;                       // Path matrix ptr type.
typedef vector < path_vt >& path_mrt;                       // Path matrix reference type.
typedef const vector < path_vt > path_cmt;                  // Constant path matrix type.
typedef const vector < path_vt >* path_cmpt;                // Constant path matrix ptr type.
typedef const vector < path_vt >& path_cmrt;                // Constant path matrix reference type.

// Typedef of path cache.
typedef PathCache path_cache_t;                             // Path cache type.
typedef path_cache_t* path_cache_pt;                        // Path cache ptr type.
typedef path_cache_t& path_cache_rt;                        // Path cache reference type.
typedef const PathCache* path_cache_cpt;                    // Const path cache ptr type.
typedef const PathCache& path_cache_crt;                    // Const path cache reference type.
typedef vector < path_cache_t > path_cache_vt;              // Path cache vector type.
typedef vector < path_cache_t >* path_cache_vpt;            // Path cache vector ptr type.
typedef vector < path_cache_t >& path_cache_vrt;            // Path cache vector reference type.

// Typedef of path heap variable.
typedef PrefixTree prefix_tree_t;                           // Path tree type.
typedef PrefixTree* prefix_tree_pt;                         // Path tree ptr type.
typedef PrefixTree& prefix_tree_rt;                         // Path tree reference type.
typedef const PrefixTree prefix_tree_ct;                    // const prefix tree type.
typedef const PrefixTree* prefix_tree_cpt;                  // Const prefix tree ptr type.
typedef const PrefixTree& prefix_tree_crt;                  // Const prefix tree reference type.

// Typedef of path heap variable.
typedef PrefixHeap prefix_heap_t;                           // Path heap type.
typedef PrefixHeap* prefix_heap_pt;                         // Path heap ptr type.
typedef PrefixHeap& prefix_heap_rt;                         // Path heap reference type.
typedef PathMaxHeap path_max_heap_t;                        // Path heap type.
typedef vector<PathMaxHeap> path_max_heap_vt;               // Path heap vector type.
typedef PathMaxHeap* path_max_heap_pt;                      // Path heap ptr type.
typedef PathMaxHeap& path_max_heap_rt;                      // Path heap reference type.

// Typedef of a layer geometry variable.
typedef LayerGeometry        layer_geometry_t;              // Layer geometry type.
typedef LayerGeometry*       layer_geometry_pt;             // Layer geometry ptr type.
typedef LayerGeometry&       layer_geometry_rt;             // Layer geometry reference type.
typedef const LayerGeometry  layer_geometry_ct;             // Constant layer geometry type.
typedef const LayerGeometry* layer_geometry_cpt;            // Constant layer geometry ptr type.
typedef const LayerGeometry& layer_geometry_crt;            // Constant layer geometry reference type.

// Typedef of a layer variable.
typedef Layer        layer_t;                               // layer type.
typedef Layer*       layer_pt;                              // layer ptr type.
typedef Layer&       layer_rt;                              // layer reference type.
typedef const Layer  layer_ct;                              // Constant layer type.
typedef const Layer* layer_cpt;                             // Constant layer ptr type.
typedef const Layer& layer_crt;                             // Constant layer reference type.

// Typedef of a macro variable.
typedef Macro        macro_t;                               // macro type.
typedef Macro*       macro_pt;                              // macro ptr type.
typedef Macro&       macro_rt;                              // macro reference type.
typedef const Macro  macro_ct;                              // Constant macro type.
typedef const Macro* macro_cpt;                             // Constant macro ptr type.
typedef const Macro& macro_crt;                             // Constant macro reference type.

// Typedef of a site variable.
typedef Site        site_t;                                 // site type.
typedef Site*       site_pt;                                // site ptr type.
typedef Site&       site_rt;                                // site reference type.
typedef const Site  site_ct;                                // Constant site type.
typedef const Site* site_cpt;                               // Constant site ptr type.
typedef const Site& site_crt;                               // Constant site reference type.

// Typedef of a timer variable.
typedef Timer        timer_t;                               // timer type.
typedef Timer*       timer_pt;                              // timer ptr type.
typedef Timer&       timer_rt;                              // timer reference type.
typedef const Timer  timer_ct;                              // Constant timer type.
typedef const Timer* timer_cpt;                             // Constant timer ptr type.
typedef const Timer& timer_crt;                             // Constant timer reference type.
typedef unique_ptr < timer_t > timer_upt;                   // Timer unique ptr type.

// Typedef of a environment variable.
typedef Environment environment_t;                          // Environment type.
typedef Environment* environment_pt;                        // Environment ptr type.
typedef Environment& environment_rt;                        // Environment reference type.
typedef unique_ptr < environment_t > environment_upt;       // Environment uptr type.

// Typedef of verilog.
typedef Verilog verilog_t;                                  // Verilog type.
typedef Verilog* verilog_pt;                                // Verilog ptr type.
typedef Verilog& verilog_rt;                                // Verilog reference type.
typedef const Verilog verilog_ct;                           // Constant verilog type.
typedef const Verilog* verilog_cpt;                         // Constant verilog ptr type.

// Typedef of pipeline
typedef Pipeline pipeline_t;                                // Pipeline type.
typedef Pipeline* pipeline_pt;                              // Pipeline ptr type.
typedef Pipeline& pipeline_rt;                              // Pipeline reference type.
typedef const Pipeline pipeline_ct;                         // Constant pipeline type.
typedef const Pipeline& pipeline_crt;                       // Constant pipeline reference type.
typedef const Pipeline* pipeline_cpt;                       // Constant pipeline ptr type.
typedef unique_ptr < pipeline_t > pipeline_upt;             // Pipeline uptr type.

// Typedef of verilog module.
typedef VerilogModule verilog_module_t;                     // Verilog module type.
typedef VerilogModule* verilog_module_pt;                   // Verilog module ptr type.
typedef VerilogModule& verilog_module_rt;                   // Verilog module reference type.
typedef const VerilogModule verilog_module_ct;              // Constant verilog module type.
typedef const VerilogModule* verilog_module_cpt;            // Constant verilog module ptr type.

// Typedef of verilog wire.
typedef VerilogWire verilog_wire_t;                         // Verilog wire type.
typedef VerilogWire* verilog_wire_pt;                       // Verilog wire ptr type.
typedef VerilogWire& verilog_wire_rt;                       // Verilog wire reference type.
typedef const VerilogWire verilog_wire_ct;                  // Constant verilog wire type.
typedef const VerilogWire* verilog_wire_cpt;                // Constant verilog wire ptr type.
typedef vector < VerilogWire* > verilog_wire_ptr_vt;        // Verilog wire ptr vector type.
typedef vector < VerilogWire* >* verilog_wire_ptr_vpt;      // Verilog wire ptr vector ptr type.
typedef verilog_wire_ptr_vt::iterator verilog_wire_ptr_vec_iter_t;

// Typedef of verilog input.
typedef VerilogInput verilog_input_t;                       // Verilog input type.
typedef VerilogInput* verilog_input_pt;                     // Verilog input ptr type.
typedef VerilogInput& verilog_input_rt;                     // Verilog input reference type.
typedef const VerilogInput verilog_input_ct;                // Constant verilog input type.
typedef const VerilogInput* verilog_input_cpt;              // Constant verilog input ptr type.
typedef vector < VerilogInput* > verilog_input_ptr_vt;      // Verilog input ptr vector type.
typedef vector < VerilogInput* >* verilog_input_ptr_vpt;    // Verilog input ptr vector ptr type.
typedef verilog_input_ptr_vt::iterator verilog_input_ptr_vec_iter_t;

// Typedef of verilog output.
typedef VerilogOutput verilog_output_t;                     // Verilog output type.
typedef VerilogOutput* verilog_output_pt;                   // Verilog output ptr type.
typedef VerilogOutput& verilog_output_rt;                   // Verilog output reference type.
typedef const VerilogOutput verilog_output_ct;              // Constant verilog output type.
typedef const VerilogOutput* verilog_output_cpt;            // Constant verilog output ptr type.
typedef vector < VerilogOutput* > verilog_output_ptr_vt;    // Verilog wire ptr vector type.
typedef vector < VerilogOutput* >* verilog_output_ptr_vpt;  // Verilog wire ptr vector ptr type.
typedef verilog_output_ptr_vt::iterator verilog_output_ptr_vec_iter_t;

// Typedef of verilog gate.
typedef VerilogGate verilog_gate_t;                         // Verilog gate type.
typedef VerilogGate* verilog_gate_pt;                       // Verilog gate ptr type.
typedef VerilogGate& verilog_gate_rt;                       // Verilog gate reference type.
typedef const VerilogGate verilog_gate_ct;                  // Constant verilog gate type.
typedef const VerilogGate* verilog_gate_cpt;                // Constant verilog gate ptr type.
typedef vector < VerilogGate* > verilog_gate_ptr_vt;    
typedef vector < VerilogGate* >* verilog_gate_ptr_vpt;
typedef verilog_gate_ptr_vt::iterator verilog_gate_ptr_vec_iter_t;

// Typedef of timing look-up table.
typedef LUTTemplate lut_template_t;                         // Timing lut template type.
typedef LUTTemplate* lut_template_pt;                       // Timing lut template ptr type.
typedef LUTTemplate& lut_template_rt;                       // Timing lut template reference type.
typedef unique_ptr < lut_template_t > lut_template_upt;     // Timing lut template uptr type.
typedef TimingLUT timing_lut_t;                             // Timing lut type.
typedef TimingLUT* timing_lut_pt;                           // Timing lut ptr type.
typedef TimingLUT& timing_lut_rt;                           // Timing lut reference type.
typedef unique_ptr < timing_lut_t > timing_lut_upt;         // Timing lut uptr type.

// Typedef of list type.
typedef List < Pin* > pinlist_t;                            // Pinlist type.
typedef List < Pin* >* pinlist_pt;                          // Pinlist ptr type.
typedef List < Pin* >& pinlist_rt;                          // Pinlist ref type.
typedef ListNode < Pin* >* pinlist_iter_t;                  // Pin ptr list node ptr type.
typedef List < Node* > nodelist_t;                          // Node ptr list type.
typedef List < Node* >& nodelist_rt;                        // Node ptr list reference type.
typedef List < Node* >* nodelist_pt;                        // Node ptr list ptr type.
typedef vector < List <Node*>* > node_bucketlist_t;         // Node bucket list vector type.
typedef vector < List <Node*>* >& node_bucketlist_rt;       // Node bucket list vector reference type.
typedef vector < List <Node*>* >* node_bucketlist_pt;       // Node bucket list vector ptr type.
typedef ListNode < Node* >* nodelist_iter_t;                // Node ptr list node ptr type.
typedef List < RCTreeNode* > rctree_nodelist_t;             // RCTree node ptr list type.
typedef List < RCTreeNode* >* rctree_nodelist_pt;           // RCTree node ptr list type.
typedef ListNode < RCTreeNode* >* rctree_nodelist_iter_t;   // RCTree node ptr list node ptr type.
typedef List < Edge* > edgelist_t;                          // Edge ptr list type.
typedef List < Edge* >* edgelist_pt;                        // Edge ptr list ptr type.
typedef List < Edge* >& edgelist_rt;                        // Edge ptr list ref type.
typedef ListNode < Edge* >* edgelist_iter_t;                // Edge ptr list edge ptr type.
typedef List < Jump* > jumplist_t;                          // Jump ptr list type.
typedef List < Jump* >* jumplist_pt;                        // jump ptr list type.
typedef List < Jump* >& jumplist_rt;                        // jump ptr list type.
typedef ListNode < Jump* >* jumplist_iter_t;                // jump ptr list edge ptr type.
typedef List < RCTreeEdge* > rctree_edgelist_t;             // RC-tree edgelist type.
typedef List < RCTreeEdge* >* rctree_edgelist_pt;           // RC-tree edgelist ptr type.
typedef List < RCTreeEdge* >& rctree_edgelist_rt;           // RC-tree edgelist ref type.
typedef ListNode < RCTreeEdge* >* rctree_edgelist_iter_t;   // RC-tree edgelist node ptr type.

// Typedef of dict type.
typedef Dictionary < string_t, Pin > pin_dict_t;                            // Pin dict type.
typedef Dictionary < string_t, Pin >* pin_dict_pt;                          // Pin dict ptr type.
typedef Dictionary < string_t, Pin >& pin_dict_rt;                          // Pin dict ref type.
typedef Dictionary < string_t, Net > net_dict_t;                            // Net dict type.
typedef Dictionary < string_t, Net >* net_dict_pt;                          // Net dict ptr type.
typedef Dictionary < string_t, Net >& net_dict_rt;                          // Net dict ref type.
typedef Dictionary < string_t, Site > site_dict_t;                          // Site dict type.
typedef Dictionary < string_t, Site >* site_dict_pt;                        // Site dict ptr type.
typedef Dictionary < string_t, Cell > cell_dict_t;                          // Cell dict type.
typedef Dictionary < string_t, Cell >* cell_dict_pt;                        // Cell dict ptr type.
typedef Dictionary < string_t, Node > node_dict_t;                          // Node dict type.
typedef Dictionary < string_t, Node >* node_dict_pt;                        // Node dict ptr type.
typedef Dictionary < string_t, Edge > edge_dict_t;                          // Edge dict type.
typedef Dictionary < string_t, Edge >* edge_dict_pt;                        // Edge dict ptr type.
typedef Dictionary < string_t, Macro > macro_dict_t;                        // Macro dict type.
typedef Dictionary < string_t, Macro >* macro_dict_pt;                      // Macro dict ptr type.
typedef Dictionary < string_t, Layer > layer_dict_t;                        // Layer dict type.
typedef Dictionary < string_t, Layer >* layer_dict_pt;                      // Layer dict ptr type.
typedef Dictionary < string_t, Cellpin > cellpin_dict_t;                    // Cellpin dict type.
typedef Dictionary < string_t, Cellpin >* cellpin_dict_pt;                  // Cellpin dict ptr type.
typedef Dictionary < string_t, Macropin > macropin_dict_t;                  // Macropin dict type.
typedef Dictionary < string_t, Macropin >* macropin_dict_pt;                // Macropin dict ptr type.
typedef Dictionary < string_t, Gate > gate_dict_t;                          // Gate dict type.
typedef Dictionary < string_t, Gate >* gate_dict_pt;                        // Gate dict ptr type.
typedef Dictionary < string_t, TimingArc > timing_arc_dict_t;               // Timing arc dict type.
typedef Dictionary < string_t, TimingArc >* timing_arc_dict_pt;             // Timing arc dict ptr type.
typedef Dictionary < string_t, TimingArc >& timing_arc_dict_rt;             // Timing arc dictionary reference type.
typedef Dictionary < string_t, LUTTemplate > lut_template_dict_t;           // LUT template dict type.
typedef Dictionary < string_t, LUTTemplate >* lut_template_dict_pt;         // LUT template dict ptr type.
typedef Dictionary < string_t, RCTreeNode > rctree_node_dict_t;             // LUT template dict type.
typedef Dictionary < string_t, RCTreeNode >* rctree_node_dict_pt;           // LUT template dict ptr type.
typedef Dictionary < string_t, RCTreeNode >& rctree_node_dict_rt;           // LUT template dict ref type.
typedef Dictionary < string_t, PrimaryInput > primary_input_dict_t;         // Primary input port dict type.
typedef Dictionary < string_t, PrimaryInput >* primary_input_dict_pt;       // Primary input port dict ptr type.
typedef Dictionary < string_t, PrimaryInput >& primary_input_dict_rt;       // Primary input port dict ref type.
typedef Dictionary < string_t, PrimaryOutput > primary_output_dict_t;       // Primary input port dict type.
typedef Dictionary < string_t, PrimaryOutput >* primary_output_dict_pt;     // Primary input port dict ptr type.
typedef Dictionary < string_t, PrimaryOutput >& primary_output_dict_rt;     // Primary input port dict ref type.
typedef Dictionary < string_t, LayerGeometry > layer_geometry_dict_t;       // Layer geometry dict type.
typedef Dictionary < string_t, LayerGeometry >* layer_geometry_dict_pt;     // Layer geometry dict ptr type.
typedef Dictionary < string_t, LayerGeometry >& layer_geometry_dict_rt;     // Layer geometry dict ptr type.
typedef Dictionary < string_t, VerilogModule > verilog_module_dict_t;       // Verilog module dict type.
typedef Dictionary < string_t, VerilogModule >* verilog_module_dict_pt;     // Verilog module dict ptr type.
typedef Dictionary < string_t, VerilogGate > verilog_gate_dict_t;           // Verilog gate dict type.
typedef Dictionary < string_t, VerilogGate >* verilog_gate_dict_pt;         // Verilog gate dict ptr type.

// Typedef of dict uptr type.
typedef unique_ptr < site_dict_t > site_dict_upt;                           // Site dict uptr type.
typedef unique_ptr < layer_dict_t > layer_dict_upt;                         // Layer dict uptr type.
typedef unique_ptr < macro_dict_t > macro_dict_upt;                         // Macro dict uptr type.
typedef unique_ptr < pin_dict_t > pin_dict_upt;                             // Pin dict uptr type.
typedef unique_ptr < net_dict_t > net_dict_upt;                             // Net dict uptr type.
typedef unique_ptr < cell_dict_t > cell_dict_upt;                           // Cell dict uptr type.
typedef unique_ptr < cellpin_dict_t > cellpin_dict_upt;                     // Cellpin dict uptr type.
typedef unique_ptr < macropin_dict_t > macropin_dict_upt;                   // Macropin dict uptr type.
typedef unique_ptr < gate_dict_t > gate_dict_upt;                           // Gate dict uptr type.
typedef unique_ptr < timing_arc_dict_t > timing_arc_dict_upt;               // Timing arc dict uptr type.
typedef unique_ptr < lut_template_dict_t > lut_template_dict_upt;           // LUT template dict uptr type.
typedef unique_ptr < rctree_node_dict_t > rctree_node_dict_upt;             // RC tree node dict uptr type.
typedef unique_ptr < primary_input_dict_t > primary_input_dict_upt;         // PI dict uptr type.
typedef unique_ptr < primary_output_dict_t > primary_output_dict_upt;       // PO dict uptr type.
typedef unique_ptr < layer_geometry_dict_t > layer_geometry_dict_upt;       // Layer geometry dict uptr type.
typedef unique_ptr < verilog_module_dict_t > verilog_module_dict_upt;       // Verilog module dict uptr type.
typedef unique_ptr < verilog_gate_dict_t > verilog_gate_dict_upt;           // Verilog gate dict uptr type.
typedef unique_ptr < lut_template_dict_t > lut_template_dict_upt;           // LUT template dict uptr type.

// Typedef of dict iter type.
typedef pin_dict_t::iter_t pin_dict_iter_t;                                 // Pin dict iter type.
typedef net_dict_t::iter_t net_dict_iter_t;                                 // Net dict iter type.
typedef cell_dict_t::iter_t cell_dict_iter_t;                               // Cell dict iter type.
typedef lut_template_dict_t::iter_t lut_template_dict_iter_t;               // LUT template dict iter type.
typedef node_dict_t::iter_t node_dict_iter_t;                               // Node dict iter type.
typedef edge_dict_t::iter_t edge_dict_iter_t;                               // Edge dict iter type.
typedef cellpin_dict_t::iter_t cellpin_dict_iter_t;                         // Cellpin dict iter type.
typedef gate_dict_t::iter_t gate_dict_iter_t;                               // Gate dict iter type.
typedef timing_arc_dict_t::iter_t timing_arc_dict_iter_t;                   // Timing arc dict iter type.
typedef rctree_node_dict_t::iter_t rctree_node_dict_iter_t;                 // RC tree node dict iter type.
typedef verilog_module_dict_t::iter_t verilog_module_dict_iter_t;           // Verilog module dict iter type.
typedef verilog_gate_dict_t::iter_t verilog_gate_dict_iter_t;               // Verilog gate dict iter type.
typedef primary_input_dict_t::iter_t primary_input_dict_iter_t;             // Primary input port dict iter type.
typedef primary_output_dict_t::iter_t primary_output_dict_iter_t;           // Primary output port dict iter type.

// Typedef of map type.
typedef map < string_t, string_t > string_to_string_map_t;                  // String to string map type.
typedef map < string_t, string_t >* string_to_string_map_pt;                // String to string map ptr type.
typedef map < string_t, int_t > string_to_int_map_t;                        // String to int map type.

// Typedef of map iterator type.
typedef string_to_string_map_t::iterator string_to_string_iter_t;
typedef string_to_int_map_t::iterator string_to_int_iter_t;

// Typedef of iterator
typedef CircuitGateIterator circuit_gate_iter_t;
typedef CircuitNetIterator circuit_net_iter_t;
typedef CircuitPinIterator circuit_pin_iter_t;
typedef CircuitPrimaryInputIterator circuit_primary_input_iter_t;
typedef CircuitPrimaryOutputIterator circuit_primary_output_iter_t;
typedef CellpinIterator cellpin_iter_t;
typedef CellTimingArcIterator cell_timing_arc_iter_t;
typedef LibraryCellIterator library_cell_iter_t;
typedef NetPinlistIterator net_pinlist_iter_t;
typedef VerilogModuleIterator verilog_module_iter_t;
typedef VerilogGateCellpinToNetMappingIterator verilog_gate_cellpin_to_net_mapping_iter_t;
typedef TimerNodeIterator timer_node_iter_t;
typedef TimerEdgeIterator timer_edge_iter_t;

// Typedef of io stream type.
typedef ifstream ifstream_t;                                // Input file stream type.
typedef ifstream& ifstream_rt;                              // Input file stream type.
typedef ofstream ofstream_t;                                // Output file stream type.
typedef ofstream& ofstream_rt;                              // Output file stream type.
typedef istream istream_t;                                  // Input stream type.
typedef istream& istream_rt;                                // Input stream reference type.
typedef FILE* file_pt;                                      // File ptr type.

// Typedef of enum type.
typedef CapacitanceUnitBase capacitance_unit_base_e;        // Capacitance unit base enum.
typedef CurrentUnitBase current_unit_base_e;                // Current unit base enum.
typedef LibraryDelayModel library_delay_model_e;            // Library delay model enum.
typedef LibraryTechnology library_technology_e;             // Library technology enum.
typedef LookupTableVariable lut_variable_e;                 // Lut variable enum.
typedef NodeTimingType node_timing_type_e;                  // Node timing enum.
typedef PinDirection pin_direction_e;                       // Pin direction enum.
typedef PowerUnitBase power_unit_base_e;                    // Power unit base enum.
typedef ResistanceUnitBase resistance_unit_base_e;          // Resistance unit base enum.
typedef TimingSense timing_sense_e;                         // Timing sense enum.
typedef TimingType timing_type_e;                           // Timing arc type enum.
typedef TimeUnitBase time_unit_base_e;                      // Timing unit base enum.
typedef VoltageUnitBase voltage_unit_base_e;                // Voltage unit base enum.
typedef EdgeType edge_type_e;                               // Edge type enum.
typedef LibraryType library_type_e;                         // Library type enum.
typedef PathType path_type_e;                               // Path type enum.
typedef SiteClass site_class_e;                             // Site class enum.
typedef MacroClass macro_class_e;                           // Macro class enum.
typedef LayerDirection layer_direction_e;                   // Layer direction enum.
typedef LayerType layer_type_e;                             // Layer type enum.
typedef Mobility mobility_e;                                // Mobility enum.
typedef Orientation orientation_e;                          // Orientation enum.
typedef PinUse pin_use_e;                                   // Macropin use type enum.
typedef GateSource gate_source_e;                           // Gate source type enum.
typedef NextStateType nextstate_type_e;                     // Next state type enum.

// Typedef of constant enum type.
typedef const CapacitanceUnitBase capacitance_unit_base_ce; // Const capacitance unit base enum.
typedef const CurrentUnitBase current_unit_base_ce;         // Const current unit base enum.
typedef const LibraryDelayModel library_delay_model_ce;     // Const library delay model enum.
typedef const LibraryTechnology library_technology_ce;      // Const library technology enum.
typedef const LookupTableVariable lut_variable_ce;          // Const lut variable enum.  
typedef const NodeTimingType node_timing_type_ce;           // Const node timing type enum.
typedef const PinDirection pin_direction_ce;                // Const pin direction enum.
typedef const PowerUnitBase power_unit_base_ce;             // Const power unit base enum.
typedef const ResistanceUnitBase resistance_unit_base_ce;   // Const resistance unit base enum.
typedef const TimingSense timing_sense_ce;                  // Const timing sense enum.
typedef const TimingType timing_type_ce;                    // Const timing arc type enum.
typedef const TimeUnitBase time_unit_base_ce;               // Const timing unit base enum.
typedef const VoltageUnitBase voltage_unit_base_ce;         // Const voltage unit base enum.
typedef const EdgeType edge_type_ce;                        // Const edge type enum.
typedef const PathType path_type_ce;                        // Const path type enum.
typedef const SiteClass site_class_ce;                      // Const site class enum.
typedef const MacroClass macro_class_ce;                    // Const macro class enum.
typedef const LayerDirection layer_direction_ce;            // Const layer direction enum.
typedef const LayerType layer_type_ce;                      // Const layer type enum.
typedef const Mobility mobility_ce;                         // Const mobility enum.
typedef const Orientation orientation_ce;                   // Const orientation enum.
typedef const PinUse pin_use_ce;                            // Macropin use type enum.
typedef const GateSource gate_source_ce;                    // Gate source type enum.
typedef const NextStateType nextstate_type_ce;              // Const nextstate type enum.

// Typedef of unit measurement.
typedef Unit < float_t, time_unit_base_e        > time_unit_t;         // Time unit. 
typedef Unit < float_t, power_unit_base_e       > power_unit_t;        // Power unit. 
typedef Unit < float_t, resistance_unit_base_e  > resistance_unit_t;   // Time unit. 
typedef Unit < float_t, current_unit_base_e     > current_unit_t;      // Time unit. 
typedef Unit < float_t, capacitance_unit_base_e > capacitance_unit_t;  // Time unit. 
typedef Unit < float_t, voltage_unit_base_e     > voltage_unit_t;      // Time unit. 

//-------------------------------------------------------------------------------------------------

// Abstractor
using abstractor_t = Abstractor;                            // Abstractor type.
using abstractor_pt = abstractor_t*;                        // Abstractor ptr type.
using abstractor_rt = abstractor_t&;                        // Abstractor ref type.
using abstractor_ct = const abstractor_t;                   // Constant abstractor type.
using abstractor_cpt = const abstractor_t*;                 // Constant abstractor ref type.
using abstractor_crt = const abstractor_t&;                 // Constant abstractor ref type.

// AbsNode
using abs_node_t = AbsNode;                                 // Abs node type.
using abs_node_pt = abs_node_t*;                            // Abs node ptr type.
using abs_node_rt = abs_node_t&;                            // Abs node ref type.
using abs_nodeset_t = OrderedSet < abs_node_t >;            // Abs nodeset type.
using abs_nodeset_pt = abs_nodeset_t*;                      // Abs nodeset ptr type.
using abs_nodeset_rt = abs_nodeset_t&;                      // Abs nodeset ref type.
using abs_nodeset_crt = const abs_nodeset_t&;               // Constant abs nodeset ref type.
using abs_nodeset_upt = unique_ptr < abs_nodeset_t >;       // Abs nodeset uptr type.

// AbsEdge
using abs_edge_t = AbsEdge;                                 // Abs edge type.
using abs_edge_pt = abs_edge_t*;                            // Abs edge ptr type.
using abs_edge_rt = abs_edge_t&;                            // Abs edge ref type.
using abs_edgeset_t = OrderedSet < abs_edge_t >;            // Abs edgeset type.
using abs_edgeset_pt = abs_edgeset_t*;                      // Abs edgeset ptr type.
using abs_edgeset_rt = abs_edgeset_t&;                      // Abs edgeset ref type.
using abs_edgeset_crt = const abs_edgeset_t&;               // Constant abs edgeset ref type.
using abs_edgeset_upt = unique_ptr < abs_edgeset_t >;       // Abs edgeset uptr type.

// Abs edgelist.
using abs_edgelist_t = List < abs_edge_pt >;                // Abs edgelist type.
using abs_edgelist_pt = abs_edgelist_t*;                    // Abs edgelist ptr type.
using abs_edgelist_rt = abs_edgelist_t&;                    // Abs edgelist ref type.
using abs_edgelist_iter_t = ListNode < abs_edge_pt >*;      // Abs edgelist iter type.

// Allocator.
using abs_node_allocator_t = Allocator < abs_node_t >;                 // Abs node allocator.
using abs_edge_allocator_t = Allocator < abs_edge_t >;                 // Abs edge allocator.

// Shell related field.
using shell_reactor_t = function < void_t(int, char**, timer_pt) >;
using shell_reactor_dict_t = unordered_map < string_t, shell_reactor_t >;
using shell_reactor_dict_pt = shell_reactor_dict_t*;
using shell_reactor_dict_rt = shell_reactor_dict_t&;
using shell_reactor_dict_upt = unique_ptr < shell_reactor_dict_t >;


};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif

