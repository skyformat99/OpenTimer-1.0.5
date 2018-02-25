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

#ifndef OC_MAPREDUCE_HPP_
#define OC_MAPREDUCE_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <type_traits>

namespace OpenCluster {


namespace Map {

};  // End of namespace Map. ----------------------------------------------------------------------




namespace Reduce {

// Function: reduce (base definition)
template <typename OperatorT, typename T>
constexpr T reduce(const T& v) {
  return v;
}

// Function: reduce (recursive definition)
template <typename OperatorT, typename T, typename... ArgsT>
constexpr T reduce(const T& v, ArgsT&&... args) {
  return OperatorT()(v, reduce<OperatorT>(::std::forward<ArgsT>(args)...));
}

};  // End of namespace Reduce. -------------------------------------------------------------------




namespace Operator {

// Operator group: Bitwise
struct Bitwise {
  
  struct OR {
    template <typename T>
    constexpr T operator()(const T& lhs, const T& rhs) const {
      static_assert(::std::is_integral<T>::value, "Fail to compile bitwise OR (integer required)");
      return lhs | rhs;
    }
  };

  struct AND {
    template <typename T>
    constexpr T operator()(const T& lhs, const T& rhs) const {
      static_assert(::std::is_integral<T>::value, "Fail to compile bitwise AND (integer required)");
      return lhs & rhs;
    }
  };

  struct XOR {
    template <typename T>
    constexpr T operator()(const T& lhs, const T& rhs) const {
      static_assert(::std::is_integral<T>::value, "Fail to compile bitwise XOR (integer required)");
      return lhs ^ rhs;
    }
  };
};


};  // End of namespace Operator. -----------------------------------------------------------------




};  // End of namespace OpenCluster. ---------------------------------------------------------------------


#endif
