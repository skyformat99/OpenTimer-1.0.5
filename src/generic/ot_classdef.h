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

#ifndef OT_CLASSDEF_H_
#define OT_CLASSDEF_H_

namespace OpenTimer {

class Abstractor;
class AbsNode;
class AbsEdge;
class Cell;
class Cellpin;
class CellpinIterator;
class CellTimingArcIterator;
class CellLib;
class Circuit;
class CircuitGateIterator;
class CircuitNetIterator;
class CircuitPinIterator;
class CircuitPrimaryInputIterator;
class CircuitPrimaryOutputIterator;
class ClockTree;
class ComparePath;
class CompareEndpoint;
class DataChannel;
class DEFReader;
class Environment;
class Edge;
class Endpoint;
class EndpointMinHeap;
class EndpointMinHeapKV;
class Event;
class FileReader;
class Gate;
class Jump;
class Layer;
class LayerGeometry;
class LEFReader;
class LUTTemplate;
class LibraryCellIterator;
class Macro;
class Macropin;
class MurMurHash32;
class Net;
class NetPinlistIterator;
class Node;
class CPPR;
class CPPRStorage;
class Prefix;
class PrefixHeap;
class PrefixTree;
class Path;
class PathCache;
class PathMaxHeap; 
class Pin;
class Pipeline;
class PrimaryInput;
class PrimaryOutput;
class RCTree;
class RCTreeEdge;
class RCTreeNode;
class Reactor;
class Site;
class Spef;
class SpefPin;
class SpefNet;
class SpefRes;
class SpefCap;
class SuffixTree;
class String;
class Test;
class TechLib;
class Timer;
class TimerNodeIterator;
class TimerEdgeIterator;
class Timing;
class TimingArc;
class TimingArcIterator;
class TimingLUT;
class Utility;
class Verilog;
class VerilogGate;
class VerilogWire;
class VerilogInput;
class VerilogOutput;
class VerilogGateCellpinToNetMappingIterator;
class VerilogModule;
class VerilogModuleIterator;
class VerilogModuleInputIterator;
class VerilogModuleGateIterator;
class VerilogModuleOutputIterator;
class VerilogModuleWireIterator;
class xxHash32;
class xxHash64;

template <class T> class Allocator;
template <class T> class Array;
template <class T> class Data; 
template <class T> class ListNode;
template <class M, class B> class Unit; 
template <class K, class V, class Hash, class KeyEqual, class Allocator> class Dictionary;
template <class T> class Point;
template <class T> class Rectangle;

};  // End of OpenTimer namespace. ----------------------------------------------------------------


#endif
