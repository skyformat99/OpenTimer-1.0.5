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

#include "ot_site.h"

namespace OpenTimer {

// Constructor.
Site::Site(string_crt key):
  _name(key),
  _site_class(UNDEFINED_SITE_CLASS),
  _is_x_symmetry(OT_DEFAULT_SITE_IS_X_SYMMETRY),
  _is_y_symmetry(OT_DEFAULT_SITE_IS_Y_SYMMETRY),
  _is_r90_symmetry(OT_DEFAULT_SITE_IS_R90_SYMMETRY)
{

}

// Destructor.
Site::~Site() {

}

};
