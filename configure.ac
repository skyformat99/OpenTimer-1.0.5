#******************************************************************************
#*                                                                            *
#* Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,                 *
#* University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
#*                                                                            *
#* All Rights Reserved.                                                       *
#*                                                                            *
#* This program is free software. You can redistribute and/or modify          *
#* it in accordance with the temms of the accompanying license agreement.     *
#* See LICENSE in the top-level directory for details.                        *
#*                                                                            *
#******************************************************************************/

# Force autoconf to be at least this version number:
#AC_PREREQ([2.68])

# AC_INIT([package], [version], [bug-report], [tarname], [url])
AC_INIT([OpenTimer], [1.0.5], [twh760812@gmail.com],
        [OpenTimer], [<http://web.engr.illinois.edu/~thuang19>])

# Safety check - list a source file that wouldn't be in other directories:
#AC_CONFIG_SRCDIR([src/hello.c])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable "automake" to simplify creating makefiles:
AM_INIT_AUTOMAKE([subdir-objects tar-ustar])

# Enable "libtool" to simplify creating makefiles:
LT_PREREQ([2.2])
LT_INIT
LT_LANG([C++])


# Specify where the Makefile should be produced. 
# These are the same locations as your Makefile.in's, but named as Makefile only.
# We need to do this because both Autoconf and Automake read this file and produce 
# Makefiles from this list.
AC_CONFIG_FILES([Makefile])
#AC_CONFIG_HEADERS([inc/config.h])

# Configure 3rd-party packages.
AC_CONFIG_SUBDIRS([3rd-party/glog-0.3.3])

# Language configuration.
AC_LANG(C++)

# Program configuration.
AC_PROG_CXX
AC_PROG_YACC
AC_PROG_AWK
AC_PROG_LIBTOOL
#AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_LN_S
#AM_PROG_AR

# Library, header check, so we can easily #include them:
AC_HEADER_STDC
AC_CHECK_TYPE(uint8_t, ac_cv_have_uint8_t=1, ac_cv_have_uint8_t=0)
AC_CHECK_TYPE(uint16_t, ac_cv_have_uint16_t=1, ac_cv_have_uint16_t=0)
AC_CHECK_TYPE(uint32_t, ac_cv_have_uint32_t=1, ac_cv_have_uint32_t=0)
AC_CHECK_TYPE(uint64_t, ac_cv_have_uint32_t=1, ac_cv_have_uint32_t=0)

# Generic compiler characteristics.
AC_OPENMP

# Do final output.
AC_OUTPUT
LT_OUTPUT


