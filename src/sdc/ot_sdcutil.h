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

#ifndef OT_SDCUTIL_H_
#define OT_SDCUTIL_H_

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <limits.h>
#include <float.h>
#include "ot_utility.h"

namespace OpenTimer {

// Procedure: free_sdc_objects
static inline void free_sdc_objects(std::vector<char*>& objects) {
  for(int i=0; i<(int)objects.size(); ++i) {
    free(objects[i]);
  }
}

// Procedure: free_sdc_object
static inline void free_sdc_object(char* object) {
  if(object) free(object);
}


};


#endif
