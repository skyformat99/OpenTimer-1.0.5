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

#ifndef OC_TYPES_HPP_
#define OC_TYPES_HPP_

namespace OpenCluster {

// is_unique_ptr: base
template<typename T>
struct is_unique_ptr {
  static const bool value = false;
};

// is_unique_ptr: recursiion
template <>
template <typename U>
struct is_unique_ptr <::std::unique_ptr<U>> {
  static const bool value = true;
};

// is_shared_ptr: base
template<typename T>
struct is_shared_ptr {
  static const bool value = false;
};

// is_shared_ptr: recursiion
template <>
template <typename U>
struct is_shared_ptr <::std::shared_ptr<U>> {
  static const bool value = true;
};

// is_weak_ptr: base
template<typename T>
struct is_weak_ptr {
  static const bool value = false;
};

// is_weak_ptr: recursiion
template <>
template <typename U>
struct is_weak_ptr <::std::weak_ptr<U>> {
  static const bool value = true;
};

// is_hour: base
template <typename T>
struct is_hour {
  static const bool value = false;
};

// is_hour: recursion
template <>
struct is_hour< ::std::chrono::hours > {
  static const bool value = true;
}; 

// is_minute: base
template <typename T>
struct is_minute {
  static const bool value = false;
};

// is_minute: recursion
template <>
struct is_minute < ::std::chrono::minutes > {
  static const bool value = true;
};

// is_second: base
template <typename T>
struct is_second {
  static const bool value = false;
};

// is_second: recursion
template <>
struct is_second < ::std::chrono::seconds > {
  static const bool value = true;
}; 

// is_millisecond: base
template <typename T>
struct is_millisecond {
  static const bool value = false;
};

// is_millisecond: recursion
template <>
struct is_millisecond < ::std::chrono::milliseconds > {
  static const bool value = true;
};

// is_microsecond: base
template <typename T>
struct is_microsecond {
  static const bool value = false;
};

// is_microsecond: recursion
template <>
struct is_microsecond < ::std::chrono::microseconds > {
  static const bool value = true;
};

// is_nanosecond: base
template <typename T>
struct is_nanosecond {
  static const bool value = false;
};

// is_nanosecond: recursion
template <>
struct is_nanosecond < ::std::chrono::nanoseconds > {
  static const bool value = true;
};


/*// Type extraction.
// Extract the type of a templated class. For instance, consider a class as follow:
// 
// template <int, short, float, double, char> class C;
//
// extract_type<C, 0>::type => int
// extract_type<C, 1>::type => short
// extract_type<C, 2>::type => float
// extract_type<C, 3>::type => double
// extract_type<C, 4>::type => char
//

// extract_type: recursive definition.
template <template <typename...> typename C, size_t idx, typename T, typename... RestT> 
struct extract_type;

// extract_type: base
template <template <typename...> typename C, typename T, typename... RestT>
struct extract_type< C<RestT...>, 0, RestT... > {
  using type = T;
};

// extract_type: recursive definition.
template <template <typename...> typename C, size_t idx, typename T, typename... RestT>
struct extract_type : public extract_type< C<RestT...>, idx-1, RestT... > {
};  */


};  // End of namespace OpenCluster. --------------------------------------------------------------


#endif



