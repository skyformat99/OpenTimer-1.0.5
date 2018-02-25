#******************************************************************************
#*                                                                            *
#* Copyright (c) 2015, Tsung-Wei Huang and Martin D. F. Wong,                 *
#* University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
#*                                                                            *
#* All Rights Reserved.                                                       *
#*                                                                            *
#* This program is free software. You can redistribute and/or modify          *
#* it in accordance with the temms of the accompanying license agreement.     *
#* See LICENSE in the top-level directory for details.                        *
#*                                                                            *
#******************************************************************************/

# Predefine variables.
GLOG_HOME=3rd-party/glog-0.3.3
GLOG_INC=$(GLOG_HOME)/src
GLOG_LIB=$(GLOG_HOME)/libglog.la

# Automake options.
AUTOMAKE_OPTIONS = foreign

# Make sure that when we re-make ./configure, we get the macros we need.
ACLOCAL_AMFLAGS = -I m4 --install

# Automake directories.
SUBDIRS = $(GLOG_HOME) .

# Package-related substitution variables.
CXX = @CXX@
CXXFLAGS = @CXXFLAGS@
LIBS = @LIBS@
DEFS = @DEFS@

# Initialize variables here so we can use += operator everywhere else.
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
sbin_PROGRAMS =
bin_PROGRAMS =
noinst_PROGRAMS =
pkglibexec_PROGRAMS =
pkginclude_HEADERS =
nobase_pkginclude_HEADERS =
dist_bin_SCRIPTS =
dist_pkglibexec_SCRIPTS =
nobase_dist_pkgdata_DATA =
nodist_sbin_SCRIPTS =
check_PROGRAMS =
dist_check_SCRIPTS =
check_SCRIPTS =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
PHONY_TARGETS =
LDADD =

# OpenTimer CXX flag
OT_CXXFLAGS = $(CXXFLAGS)
OT_CXXFLAGS += $(OPENMP_CXXFLAGS)
OT_CXXFLAGS += -Wall
OT_CXXFLAGS += -std=c++14

OT_CXXFLAGS += -Isrc/debug
OT_CXXFLAGS += -Isrc/dist
OT_CXXFLAGS += -Isrc/dump
OT_CXXFLAGS += -Isrc/event
OT_CXXFLAGS += -Isrc/generic
OT_CXXFLAGS += -Isrc/iterator
OT_CXXFLAGS += -Isrc/liberty
OT_CXXFLAGS += -Isrc/log
OT_CXXFLAGS += -Isrc/sdc
OT_CXXFLAGS += -Isrc/shell
OT_CXXFLAGS += -Isrc/spef
OT_CXXFLAGS += -Isrc/timer
OT_CXXFLAGS += -Isrc/verilog
OT_CXXFLAGS += -I3rd-party/LEF
OT_CXXFLAGS += -I3rd-party/DEF

# OpenTimer 3rd-party glog include flag
OT_CXXFLAGS += -I$(GLOG_INC)

# Program: bin/OpenTimer
bin_PROGRAMS += bin/OpenTimer
bin_OpenTimer_CXXFLAGS = $(OT_CXXFLAGS)
bin_OpenTimer_LDADD = lib/libOpenTimer.la
bin_OpenTimer_LDADD += $(GLOG_LIB)
bin_OpenTimer_SOURCES = main/ot_main.cpp main/ot_main.h

# Program: bin/OpenTimer-agent
bin_PROGRAMS += bin/OpenTimer-agent
bin_OpenTimer_agent_CXXFLAGS = $(OT_CXXFLAGS)
bin_OpenTimer_agent_LDADD = lib/libOpenTimer.la
bin_OpenTimer_agent_LDADD += $(GLOG_LIB)
bin_OpenTimer_agent_SOURCES = main/ot_dist_agent.cpp main/ot_dist_agent.h

# Program: bin/OpenTimer-master
bin_PROGRAMS += bin/OpenTimer-master
bin_OpenTimer_master_CXXFLAGS = $(OT_CXXFLAGS)
bin_OpenTimer_master_LDADD = lib/libOpenTimer.la
bin_OpenTimer_master_LDADD += $(GLOG_LIB)
bin_OpenTimer_master_SOURCES = main/ot_dist_master.cpp main/ot_dist_master.h

# Program: bin/OpenTimer-tau15
noinst_PROGRAMS += bin/OpenTimer-tau15
bin_OpenTimer_tau15_CXXFLAGS = $(OT_CXXFLAGS)
bin_OpenTimer_tau15_LDADD = lib/libOpenTimer.la
bin_OpenTimer_tau15_LDADD += $(GLOG_LIB)
bin_OpenTimer_tau15_SOURCES = main/ot_tau15.cpp main/ot_tau15.h

# Program: bin/OpenTimer-tau16
noinst_PROGRAMS += bin/OpenTimer-tau16
bin_OpenTimer_tau16_CXXFLAGS = $(OT_CXXFLAGS)
bin_OpenTimer_tau16_LDADD = lib/libOpenTimer.la
bin_OpenTimer_tau16_LDADD += $(GLOG_LIB)
bin_OpenTimer_tau16_SOURCES = main/ot_tau16.cpp main/ot_tau16.h

# Program: example/simple/simple
noinst_PROGRAMS += example/simple/simple
example_simple_simple_CXXFLAGS = $(OT_CXXFLAGS)
example_simple_simple_LDADD = lib/libOpenTimer.la
example_simple_simple_LDADD += $(GLOG_LIB)
example_simple_simple_SOURCES = example/simple/simple.cpp

# Program: example/c17/c17
noinst_PROGRAMS += example/c17/c17
example_c17_c17_CXXFLAGS = $(OT_CXXFLAGS)
example_c17_c17_LDADD = lib/libOpenTimer.la
example_c17_c17_LDADD += $(GLOG_LIB)
example_c17_c17_SOURCES = example/c17/c17.cpp

# Program: example/s27/s27
noinst_PROGRAMS += example/s27/s27
example_s27_s27_CXXFLAGS = $(OT_CXXFLAGS)
example_s27_s27_LDADD = lib/libOpenTimer.la
example_s27_s27_LDADD += $(GLOG_LIB)
example_s27_s27_SOURCES = example/s27/s27.cpp

# Library definition: lib/libOpenTimer.la
lib_LTLIBRARIES += lib/libOpenTimer.la
lib_libOpenTimer_la_CXXFLAGS = $(OT_CXXFLAGS)
lib_libOpenTimer_la_LIBADD = lib/libLEF.la lib/libDEF.la
lib_libOpenTimer_la_LIBADD += $(GLOG_LIB)
lib_libOpenTimer_la_SOURCES =
lib_libOpenTimer_la_SOURCES += src/debug/ot_debug_graph.cpp
lib_libOpenTimer_la_SOURCES += src/verilog/ot_verilog.cpp
lib_libOpenTimer_la_SOURCES += src/shell/ot_shell.cpp
lib_libOpenTimer_la_SOURCES += src/dump/ot_dump.cpp
lib_libOpenTimer_la_SOURCES += src/log/ot_log.cpp
lib_libOpenTimer_la_SOURCES += src/spef/ot_spef.cpp
lib_libOpenTimer_la_SOURCES += src/sdc/ot_sdc.cpp
lib_libOpenTimer_la_SOURCES += src/sdc/ot_sdcdef.cpp
lib_libOpenTimer_la_SOURCES += src/sdc/ot_sdcutil.cpp
lib_libOpenTimer_la_SOURCES += src/sdc/ot_sdcdump.cpp
lib_libOpenTimer_la_SOURCES += src/liberty/ot_celllib.cpp
lib_libOpenTimer_la_SOURCES += src/iterator/ot_iterator.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_functor.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_gate.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_pin.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_layer.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_techlib.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_site.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_timer.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_clock.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_environment.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_test.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_lefreader.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_pipeline.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_circuit.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_unitdef.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_macro.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_cppr.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_edge.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_defreader.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_net.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_io_port.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_path.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_rctree.cpp
lib_libOpenTimer_la_SOURCES += src/timer/ot_node.cpp
lib_libOpenTimer_la_SOURCES += src/generic/ot_utility.cpp
lib_libOpenTimer_la_SOURCES += src/generic/ot_dictionary.cpp
lib_libOpenTimer_la_SOURCES += src/generic/ot_freader.cpp
lib_libOpenTimer_la_SOURCES += src/dist/ot_master.cpp
lib_libOpenTimer_la_SOURCES += src/dist/ot_agent.cpp

# Convenience library: lib/libLEF.la
noinst_LTLIBRARIES += lib/libLEF.la
lib_libLEF_la_CXXFLAGS = $(CXXFLAGS)
lib_libLEF_la_CXXFLAGS += -Wno-reorder
lib_libLEF_la_SOURCES =
lib_libLEF_la_SOURCES += 3rd-party/LEF/lef.tab.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiUnits.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefrSettings.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lef_keywords.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefrData.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiMisc.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefwWriterCalls.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiTBExt.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiViaRule.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiVia.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiPropType.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiArray.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiEncryptInt.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiCrossTalk.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiDebug.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiProp.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiNonDefault.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiMacro.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/crypt.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefrReader.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefrCallbacks.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefiLayer.cpp
lib_libLEF_la_SOURCES += 3rd-party/LEF/lefwWriter.cpp

# Convenience library: lib/libDEF.la
noinst_LTLIBRARIES += lib/libDEF.la
lib_libDEF_la_CXXFLAGS = $(CXXFLAGS)
lib_libDEF_la_CXXFLAGS += -Wno-reorder
lib_libDEF_la_SOURCES =
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiBlockage.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defrData.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/def.tab.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiIOTiming.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defwWriterCalls.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiNonDefault.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiPath.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiFPC.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiScanchain.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defwWriter.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiTimingDisable.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiComponent.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiAssertion.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiProp.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiPinCap.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiRegion.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiMisc.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defrCallbacks.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiPropType.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiPartition.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiPinProp.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiFill.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiRowTrack.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiSlot.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiSite.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defrReader.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/def_keywords.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiVia.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defrSettings.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiDebug.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiUtil.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiGroup.cpp
lib_libDEF_la_SOURCES += 3rd-party/DEF/defiNet.cpp

# OpenTimer Package include
pkginclude_HEADERS += src/event/ot_event.h
pkginclude_HEADERS += src/event/ot_reactor.h
pkginclude_HEADERS += src/event/ot_data_channel.h
pkginclude_HEADERS += src/event/ot_event_handler.h
pkginclude_HEADERS += src/debug/ot_debug_graph.h
pkginclude_HEADERS += src/verilog/ot_verilog.h
pkginclude_HEADERS += src/shell/ot_shell.h
pkginclude_HEADERS += src/dump/ot_dump.h
pkginclude_HEADERS += src/log/ot_log.h
pkginclude_HEADERS += src/spef/ot_spef.h
pkginclude_HEADERS += src/sdc/ot_sdcutil.h
pkginclude_HEADERS += src/sdc/ot_sdcdef.h
pkginclude_HEADERS += src/sdc/ot_sdcdump.h
pkginclude_HEADERS += src/sdc/ot_sdc.h
pkginclude_HEADERS += src/liberty/ot_celllib.h
pkginclude_HEADERS += src/iterator/ot_iterator.h
pkginclude_HEADERS += src/timer/ot_functor.h
pkginclude_HEADERS += src/timer/ot_site.h
pkginclude_HEADERS += src/timer/ot_lefreader.h
pkginclude_HEADERS += src/timer/ot_pin.h
pkginclude_HEADERS += src/timer/ot_node.h
pkginclude_HEADERS += src/timer/ot_path.h
pkginclude_HEADERS += src/timer/ot_rctree.h
pkginclude_HEADERS += src/timer/ot_environment.h
pkginclude_HEADERS += src/timer/ot_circuit.h
pkginclude_HEADERS += src/timer/ot_unitdef.h
pkginclude_HEADERS += src/timer/ot_io_port.h
pkginclude_HEADERS += src/timer/ot_techlib.h
pkginclude_HEADERS += src/timer/ot_edge.h
pkginclude_HEADERS += src/timer/ot_net.h
pkginclude_HEADERS += src/timer/ot_layer.h
pkginclude_HEADERS += src/timer/ot_clock.h
pkginclude_HEADERS += src/timer/ot_gate.h
pkginclude_HEADERS += src/timer/ot_cppr.h
pkginclude_HEADERS += src/timer/ot_defreader.h
pkginclude_HEADERS += src/timer/ot_pipeline.h
pkginclude_HEADERS += src/timer/ot_macro.h
pkginclude_HEADERS += src/timer/ot_timer.h
pkginclude_HEADERS += src/timer/ot_test.h
pkginclude_HEADERS += src/generic/ot_list.h
pkginclude_HEADERS += src/generic/ot_dictionary.h
pkginclude_HEADERS += src/generic/ot_enumdef.h
pkginclude_HEADERS += src/generic/ot_utility.h
pkginclude_HEADERS += src/generic/ot_allocator.h
pkginclude_HEADERS += src/generic/ot_classdef.h
pkginclude_HEADERS += src/generic/ot_freader.h
pkginclude_HEADERS += src/generic/ot_headerdef.h
pkginclude_HEADERS += src/generic/ot_macrodef.h
pkginclude_HEADERS += src/generic/ot_geometry.h
pkginclude_HEADERS += src/generic/ot_typedef.h
pkginclude_HEADERS += src/generic/ot_singleton.h
pkginclude_HEADERS += src/generic/ot_orderedset.h
pkginclude_HEADERS += src/dist/oc_policy.hpp
pkginclude_HEADERS += src/dist/oc_exec.hpp
pkginclude_HEADERS += src/dist/oc_event.hpp
pkginclude_HEADERS += src/dist/ot_packet.h
pkginclude_HEADERS += src/dist/oc_socket.hpp
pkginclude_HEADERS += src/dist/oc_classdef.hpp
pkginclude_HEADERS += src/dist/oc_allocator.hpp
pkginclude_HEADERS += src/dist/oc_headerdef.hpp
pkginclude_HEADERS += src/dist/oc_types.hpp
pkginclude_HEADERS += src/dist/ot_master.h
pkginclude_HEADERS += src/dist/oc_reactor.hpp
pkginclude_HEADERS += src/dist/oc_fifo.hpp
pkginclude_HEADERS += src/dist/ot_agent.h
pkginclude_HEADERS += src/dist/oc_config.h
pkginclude_HEADERS += src/dist/oc_utility.hpp
pkginclude_HEADERS += src/dist/oc_logger.hpp
pkginclude_HEADERS += src/dist/oc_mapreduce.hpp
pkginclude_HEADERS += src/dist/oc_list.hpp
pkginclude_HEADERS += src/dist/oc_arraylist.hpp
pkginclude_HEADERS += src/dist/oc_demux.hpp
pkginclude_HEADERS += src/dist/oc_singleton.hpp
pkginclude_HEADERS += src/dist/oc_mempool.hpp
pkginclude_HEADERS += src/dist/oc_macrodef.hpp
pkginclude_HEADERS += src/dist/oc_iostream.hpp
pkginclude_HEADERS += src/dist/oc_ticker.hpp

# 3rd-party package hierarchical include: LEF
nobase_pkginclude_HEADERS += 3rd-party/LEF/lef.tab.h
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiVia.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefrData.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiUnits.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiMacro.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefwWriterCalls.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefrCallBacks.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/crypt.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiMisc.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefrSettings.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiEncryptInt.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiPropType.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiDefs.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiViaRule.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiDebug.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefrReader.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiArray.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiUtil.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiLayer.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiKRDefs.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefwWriter.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiCrossTalk.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiProp.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lex.h
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiNonDefault.hpp
nobase_pkginclude_HEADERS += 3rd-party/LEF/lefiUser.hpp

# 3rd-party package hierarchical include: DEF
nobase_pkginclude_HEADERS += 3rd-party/DEF/defwWriterCalls.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiPinProp.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiFPC.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiPath.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiVia.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiPropType.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiNet.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiBlockage.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiDefs.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/def.tab.h
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiIOTiming.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiSlot.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiScanchain.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiRowTrack.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiFill.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiSite.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiRegion.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiAssertion.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defrSettings.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiMisc.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defrData.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiProp.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiComponent.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiDebug.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiPartition.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiPinCap.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiUser.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defwWriter.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiTimingDisable.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/lex.h
nobase_pkginclude_HEADERS += 3rd-party/DEF/defrReader.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiGroup.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiNonDefault.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defrCallBacks.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiUtil.hpp
nobase_pkginclude_HEADERS += 3rd-party/DEF/defiKRDefs.hpp

# Add files to the distribution list
EXTRA_DIST += 3rd-party/LEF/LICENSE.PDF
EXTRA_DIST += 3rd-party/LEF/LICENSE.TXT
EXTRA_DIST += 3rd-party/DEF/LICENSE.PDF
EXTRA_DIST += 3rd-party/DEF/LICENSE.TXT
EXTRA_DIST += LICENSE
EXTRA_DIST += golden
EXTRA_DIST += example
EXTRA_DIST += Makefile.sh

# Regression TAU 2015
regression: all-am
	@cd golden/tau2015/ && ./regression.sh ../../bin/OpenTimer-tau15

# Project-specific targets
echo_CXX:
	@echo $(CXX)

echo_CPPFLAGS:
	@echo $(CPPFLAGS)

echo_CXXFLAGS:
	@echo $(CXXFLAGS)

echo_LIBS:
	@echo $(LIBS)

echo_DEFS:
	@echo $(DEFS)

echo_OT_CXXFLAGS:
	@echo $(OT_CXXFLAGS)

echo_bin_PROGRAMS:
	@echo $(bin_PROGRAMS)

echo_sbin_PROGRAMS:
	@echo $(sbin_PROGRAMS)

echo_check_PROGRAMS:
	@echo $(check_PROGRAMS)

echo_check_SCRIPTS:
	@echo $(check_SCRIPTS)

echo_lib_LTLIBRARIES:
	@echo $(lib_LTLIBRARIES)

echo_prefix:
	@echo $(prefix)

echo_builddir:
	@echo $(builddir)

echo_abs_builddir:
	@echo $(abs_builddir)

echo_top_builddir:
	@echo $(top_builddir)

echo_abs_top_builddir:
	@echo $(abs_top_builddir)

echo_top_build_prefix:
	@echo $(top_build_prefix)

echo_srcdir:
	@echo $(srcdir)

echo_abs_srcdir:
	@echo $(abs_srcdir)

echo_top_srcdir:
	@echo $(top_srcdir)

echo_abs_top_srcdir:
	@echo $(abs_top_srcdir)

echo_bindir:
	@echo $(bindir)

echo_sbindir:
	@echo $(sbindir)

echo_docdir:
	@echo $(docdir)

echo_exec_prefix:
	@echo $(exec_prefix)

echo_libdir:
	@echo $(libdir)

echo_pkglibdir:
	@echo $(pkglibdir)

echo_includedir:
	@echo $(includedir)

echo_pkgincludedir:
	@echo $(pkgincludedir)

echo_libexecdir:
	@echo $(libexecdir)

echo_datadir:
	@echo $(datadir)

echo_openmp_cflags:
	@echo $(OPENMP_CFLAGS)

echo_openmp_cxxflags:
	@echo $(OPENMP_CXXFLAGS)

