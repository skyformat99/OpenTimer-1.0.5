/******************************************************************************
 *                                                                            *
 * Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#ifndef OC_HEADERDEF_HPP_
#define OC_HEADERDEF_HPP_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <sstream>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <cstring>
#include <memory>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <functional>
#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <float.h>
#include <cassert>
#include <getopt.h>

// Top header declaration.
#include "oc_classdef.hpp"
#include "oc_macrodef.hpp"
#include "oc_types.hpp"
#include "oc_singleton.hpp"

// Independent template library.
#include "oc_arraylist.hpp"
#include "oc_list.hpp"
#include "oc_allocator.hpp"
#include "oc_logger.hpp"
#include "oc_mapreduce.hpp"

// Policy file
#include "oc_policy.hpp"


#endif
