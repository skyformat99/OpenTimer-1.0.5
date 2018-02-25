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

#ifndef OC_POLICY_HPP_
#define OC_POLICY_HPP_

namespace OpenCluster {

struct StreamBufferPolicy {

  static_assert(
    sizeof(unsigned char)==1,
    "unsigned char is not correctly supported."
  );

  constexpr static size_t DATA_BLOCK_SIZE {1024};
  constexpr static size_t MAX_NUM_WIOVECS {8};
  constexpr static size_t MAX_NUM_RIOVECS {8};
};



};  // End of namespace OpenCluster. --------------------------------------------------------------


#endif
