# Copyright.
echo "#******************************************************************************"
echo "#*                                                                            *"
echo "#* Copyright (c) 2015, Tsung-Wei Huang and Martin D. F. Wong,                 *"
echo "#* University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *"
echo "#*                                                                            *"
echo "#* All Rights Reserved.                                                       *"
echo "#*                                                                            *"
echo "#* This program is free software. You can redistribute and/or modify          *"
echo "#* it in accordance with the temms of the accompanying license agreement.     *"
echo "#* See LICENSE in the top-level directory for details.                        *"
echo "#*                                                                            *"
echo "#******************************************************************************/"
echo ""

# Predefined variables.
echo "# Predefine variables."
echo "GLOG_HOME=3rd-party/glog-0.3.3"
echo "GLOG_INC=\$(GLOG_HOME)/src"
echo "GLOG_LIB=\$(GLOG_HOME)/libglog.la"
echo ""

# Automake options.
echo "# Automake options."
echo "AUTOMAKE_OPTIONS = foreign"
echo ""

# Make sure that when we re-make ./configure, we get the macros we need.
echo "# Make sure that when we re-make ./configure, we get the macros we need."
echo "ACLOCAL_AMFLAGS = -I m4 --install"
echo ""

# Automake directories.
echo "# Automake directories."
echo "SUBDIRS = \$(GLOG_HOME) ."
echo ""

# Package-related substitution variables.
echo "# Package-related substitution variables."
echo "CXX = @CXX@"
echo "CXXFLAGS = @CXXFLAGS@"
echo "LIBS = @LIBS@"
echo "DEFS = @DEFS@"
echo ""

# Initialize variables here so we can use += operator everywhere else.
echo "# Initialize variables here so we can use += operator everywhere else."
echo "lib_LTLIBRARIES ="
echo "noinst_LTLIBRARIES ="
echo "sbin_PROGRAMS ="
echo "bin_PROGRAMS ="
echo "noinst_PROGRAMS ="
echo "pkglibexec_PROGRAMS ="
echo "pkginclude_HEADERS ="
echo "nobase_pkginclude_HEADERS ="
echo "dist_bin_SCRIPTS ="
echo "dist_pkglibexec_SCRIPTS ="
echo "nobase_dist_pkgdata_DATA ="
echo "nodist_sbin_SCRIPTS ="
echo "check_PROGRAMS ="
echo "dist_check_SCRIPTS ="
echo "check_SCRIPTS ="
echo "BUILT_SOURCES ="
echo "CLEANFILES ="
echo "EXTRA_DIST ="
echo "PHONY_TARGETS ="
echo "LDADD ="
echo ""

# OpenTimer CXX flag
echo "# OpenTimer CXX flag"
echo "OT_CXXFLAGS = \$(CXXFLAGS)"
echo "OT_CXXFLAGS += \$(OPENMP_CXXFLAGS)"
echo "OT_CXXFLAGS += -Wall"
echo "OT_CXXFLAGS += -std=c++14"
echo ""

# OpenTimer include flag
for f in src/*
do
  echo "OT_CXXFLAGS += -I$f"
done
echo "OT_CXXFLAGS += -I3rd-party/LEF"
echo "OT_CXXFLAGS += -I3rd-party/DEF"
echo ""

# OpenTimer 3rd-party glog include flag
echo "# OpenTimer 3rd-party glog include flag"
echo "OT_CXXFLAGS += -I\$(GLOG_INC)"
echo ""

# Program: bin/OpenTimer
echo "# Program: bin/OpenTimer"
echo "bin_PROGRAMS += bin/OpenTimer"
echo "bin_OpenTimer_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "bin_OpenTimer_LDADD = lib/libOpenTimer.la"
echo "bin_OpenTimer_LDADD += \$(GLOG_LIB)"
echo "bin_OpenTimer_SOURCES = main/ot_main.cpp main/ot_main.h"
echo ""

# Program: bin/OpenTimer-agent
echo "# Program: bin/OpenTimer-agent"
echo "bin_PROGRAMS += bin/OpenTimer-agent"
echo "bin_OpenTimer_agent_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "bin_OpenTimer_agent_LDADD = lib/libOpenTimer.la"
echo "bin_OpenTimer_agent_LDADD += \$(GLOG_LIB)"
echo "bin_OpenTimer_agent_SOURCES = main/ot_dist_agent.cpp main/ot_dist_agent.h"
echo ""

# Program: bin/OpenTimer-master
echo "# Program: bin/OpenTimer-master"
echo "bin_PROGRAMS += bin/OpenTimer-master"
echo "bin_OpenTimer_master_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "bin_OpenTimer_master_LDADD = lib/libOpenTimer.la"
echo "bin_OpenTimer_master_LDADD += \$(GLOG_LIB)"
echo "bin_OpenTimer_master_SOURCES = main/ot_dist_master.cpp main/ot_dist_master.h"
echo ""

# Program: bin/OpenTimer-tau15
echo "# Program: bin/OpenTimer-tau15"
echo "noinst_PROGRAMS += bin/OpenTimer-tau15"
echo "bin_OpenTimer_tau15_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "bin_OpenTimer_tau15_LDADD = lib/libOpenTimer.la"
echo "bin_OpenTimer_tau15_LDADD += \$(GLOG_LIB)"
echo "bin_OpenTimer_tau15_SOURCES = main/ot_tau15.cpp main/ot_tau15.h"
echo ""

# Program: bin/OpenTimer-tau16
echo "# Program: bin/OpenTimer-tau16"
echo "noinst_PROGRAMS += bin/OpenTimer-tau16"
echo "bin_OpenTimer_tau16_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "bin_OpenTimer_tau16_LDADD = lib/libOpenTimer.la"
echo "bin_OpenTimer_tau16_LDADD += \$(GLOG_LIB)"
echo "bin_OpenTimer_tau16_SOURCES = main/ot_tau16.cpp main/ot_tau16.h"
echo ""

# Program: example/simple/simple
echo "# Program: example/simple/simple"
echo "noinst_PROGRAMS += example/simple/simple"
echo "example_simple_simple_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "example_simple_simple_LDADD = lib/libOpenTimer.la"
echo "example_simple_simple_LDADD += \$(GLOG_LIB)"
echo "example_simple_simple_SOURCES = example/simple/simple.cpp"
echo ""

# Program: example/c17/c17
echo "# Program: example/c17/c17"
echo "noinst_PROGRAMS += example/c17/c17"
echo "example_c17_c17_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "example_c17_c17_LDADD = lib/libOpenTimer.la"
echo "example_c17_c17_LDADD += \$(GLOG_LIB)"
echo "example_c17_c17_SOURCES = example/c17/c17.cpp"
echo ""

# Program: example/s27/s27
echo "# Program: example/s27/s27"
echo "noinst_PROGRAMS += example/s27/s27"
echo "example_s27_s27_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "example_s27_s27_LDADD = lib/libOpenTimer.la"
echo "example_s27_s27_LDADD += \$(GLOG_LIB)"
echo "example_s27_s27_SOURCES = example/s27/s27.cpp"
echo ""

# Library definition: lib/libOpenTimer.la
echo "# Library definition: lib/libOpenTimer.la"
echo "lib_LTLIBRARIES += lib/libOpenTimer.la"
echo "lib_libOpenTimer_la_CXXFLAGS = \$(OT_CXXFLAGS)"
echo "lib_libOpenTimer_la_LIBADD = lib/libLEF.la lib/libDEF.la"
echo "lib_libOpenTimer_la_LIBADD += \$(GLOG_LIB)"
echo "lib_libOpenTimer_la_SOURCES ="
for f in `find src -name *.cpp`
do
  echo "lib_libOpenTimer_la_SOURCES += $f"
done
echo ""

# Convenience 3rd-party library: LEF
echo "# Convenience library: lib/libLEF.la"
echo "noinst_LTLIBRARIES += lib/libLEF.la"
echo "lib_libLEF_la_CXXFLAGS = \$(CXXFLAGS)"
echo "lib_libLEF_la_CXXFLAGS += -Wno-reorder"
echo "lib_libLEF_la_SOURCES ="
for f in `find 3rd-party/LEF -name *.cpp`
do
  echo "lib_libLEF_la_SOURCES += $f"
done
echo ""

# Convenience 3rd-party library: DEF
echo "# Convenience library: lib/libDEF.la"
echo "noinst_LTLIBRARIES += lib/libDEF.la"
echo "lib_libDEF_la_CXXFLAGS = \$(CXXFLAGS)"
echo "lib_libDEF_la_CXXFLAGS += -Wno-reorder"
echo "lib_libDEF_la_SOURCES ="
for f in `find 3rd-party/DEF -name *.cpp`
do
  echo "lib_libDEF_la_SOURCES += $f"
done
echo ""

# OpenTimer package include.
echo "# OpenTimer Package include"
for f in `find src -name *.h -o -name *.hpp`
do
  echo "pkginclude_HEADERS += $f"
done
echo ""

# 3rd-party package hierarchical include: LEF
echo "# 3rd-party package hierarchical include: LEF"
for f in `find 3rd-party/LEF -name *.hpp -o -name *.h`
do
  echo "nobase_pkginclude_HEADERS += $f"
done
echo ""

# 3rd-party package hierarchical include: DEF
echo "# 3rd-party package hierarchical include: DEF"
for f in `find 3rd-party/DEF -name *.hpp -o -name *.h`
do
  echo "nobase_pkginclude_HEADERS += $f"
done
echo ""

# Extra distribution.
echo "# Add files to the distribution list"
echo "EXTRA_DIST += 3rd-party/LEF/LICENSE.PDF"
echo "EXTRA_DIST += 3rd-party/LEF/LICENSE.TXT"
echo "EXTRA_DIST += 3rd-party/DEF/LICENSE.PDF"
echo "EXTRA_DIST += 3rd-party/DEF/LICENSE.TXT"
echo "EXTRA_DIST += LICENSE"
echo "EXTRA_DIST += golden"
echo "EXTRA_DIST += example"
echo "EXTRA_DIST += Makefile.sh"
echo ""

# Regression target.
echo "# Regression TAU 2015"
echo "regression: all-am"
echo "	@cd golden/tau2015/ && ./regression.sh ../../bin/OpenTimer-tau15"
echo ""

# Project-specific targets
echo "# Project-specific targets"
echo "echo_CXX:"
echo "	@echo \$(CXX)"
echo ""

echo "echo_CPPFLAGS:"
echo "	@echo \$(CPPFLAGS)"
echo ""

echo "echo_CXXFLAGS:"
echo "	@echo \$(CXXFLAGS)"
echo ""

echo "echo_LIBS:"
echo "	@echo \$(LIBS)"
echo ""

echo "echo_DEFS:"
echo "	@echo \$(DEFS)"
echo ""

echo "echo_OT_CXXFLAGS:"
echo "	@echo \$(OT_CXXFLAGS)"
echo ""

echo "echo_bin_PROGRAMS:"
echo "	@echo \$(bin_PROGRAMS)"
echo ""

echo "echo_sbin_PROGRAMS:"
echo "	@echo \$(sbin_PROGRAMS)"
echo ""

echo "echo_check_PROGRAMS:"
echo "	@echo \$(check_PROGRAMS)"
echo ""

echo "echo_check_SCRIPTS:"
echo "	@echo \$(check_SCRIPTS)"
echo ""

echo "echo_lib_LTLIBRARIES:"
echo "	@echo \$(lib_LTLIBRARIES)"
echo ""

echo "echo_prefix:"
echo "	@echo \$(prefix)"
echo ""

echo "echo_builddir:"
echo "	@echo \$(builddir)"
echo ""

echo "echo_abs_builddir:"
echo "	@echo \$(abs_builddir)"
echo ""

echo "echo_top_builddir:"
echo "	@echo \$(top_builddir)"
echo ""

echo "echo_abs_top_builddir:"
echo "	@echo \$(abs_top_builddir)"
echo ""

echo "echo_top_build_prefix:"
echo "	@echo \$(top_build_prefix)"
echo ""

echo "echo_srcdir:"
echo "	@echo \$(srcdir)"
echo ""

echo "echo_abs_srcdir:"
echo "	@echo \$(abs_srcdir)"
echo ""

echo "echo_top_srcdir:"
echo "	@echo \$(top_srcdir)"
echo ""

echo "echo_abs_top_srcdir:"
echo "	@echo \$(abs_top_srcdir)"
echo ""

echo "echo_bindir:"
echo "	@echo \$(bindir)"
echo ""

echo "echo_sbindir:"
echo "	@echo \$(sbindir)"
echo ""

echo "echo_docdir:"
echo "	@echo \$(docdir)"
echo ""

echo "echo_exec_prefix:"
echo "	@echo \$(exec_prefix)"
echo ""

echo "echo_libdir:"
echo "	@echo \$(libdir)"
echo ""

echo "echo_pkglibdir:"
echo "	@echo \$(pkglibdir)"
echo ""

echo "echo_includedir:"
echo "	@echo \$(includedir)"
echo ""

echo "echo_pkgincludedir:"
echo "	@echo \$(pkgincludedir)"
echo ""

echo "echo_libexecdir:"
echo "	@echo \$(libexecdir)"
echo ""

echo "echo_datadir:"
echo "	@echo \$(datadir)"
echo ""

echo "echo_openmp_cflags:"
echo "	@echo \$(OPENMP_CFLAGS)"
echo ""

echo "echo_openmp_cxxflags:"
echo "	@echo \$(OPENMP_CXXFLAGS)"
echo ""

