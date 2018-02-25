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

#ifndef OC_MACRODEF_HPP_
#define OC_MACRODEF_HPP_

// Code generator - class accessor.
//
// OC_CODEGEN_ACCESSOR(obj)
// ->
// constexpr const auto& obj() const { return _obj; }  // immutable accessor.
// inline auto& obj() { return _obj; }  // mutable accessor.
//
#define OC_CODEGEN_ACCESSOR(object) \
  inline const auto& object() const { return this->_##object; } \
  inline auto& object() { return this->_##object; }


// Code generator - class accessor for unique_ptr object.
//
// OC_CODEGEN_ACCESSOR_UPTR(obj)
// ->
// constexpr const auto obj() const { return _obj##_uptr.get(); }  // immutable accessor.
// inline auto obj() { return _obj##_uptr.get(); }  // mutable accessor.
//
#define OC_CODEGEN_ACCESSOR_UPTR(object) \
  inline const auto object() const { return this->_##object##_uptr.get(); } \
  inline auto object() { return this->_##object##_uptr.get(); }


// Code generator - class accessor for shared_ptr object.
//
// OC_CODEGEN_ACCESSOR_SPTR(obj)
// ->
// constexpr const auto obj() const { return _obj##_sptr.get(); }  // immutable accessor.
// inline auto obj() { return _obj##_sptr.get(); }  // mutable accessor.
//
#define OC_CODEGEN_ACCESSOR_SPTR(object) \
  inline const auto object() const { return this->_##object##_sptr.get(); } \
  inline auto object() { return this->_##object##_sptr.get(); }


// Code generator - class mutator.
//
// OC_CODEGEN_MUTATOR(obj)
// ->
// inline void set_obj(const decltype(_obj)& val) { _obj = val; }
//
#define OC_CODEGEN_MUTATOR(object) \
  inline void set_##object(const decltype(_##object)& val) { this->_##object = val; }


// Code generator - class mutator for unique_ptr.
//
// OC_CODEGEN_MUTATOR_UPTR(obj)
// ->
// inline void make_obj(...) { _obj_uptr = make_unique<decltype(_obj_uptr)>(...); }
//
#define OC_CODEGEN_MUTATOR_UPTR(object) \
  template<typename... ArgsT> \
  inline void make_##object(ArgsT&&...args) { \
    this->_##object##_uptr = ::std::make_unique<typename decltype(_##object##_uptr)::element_type>(\
      ::std::forward<ArgsT>(args)...\
    ); \
  }

// Code generator - class mutator for shared_ptr.
//
// OC_CODEGEN_MUTATOR_UPTR(obj)
// ->
// inline void make_obj(...) { _obj_sptr = make_unique<decltype(_obj_sptr)>(...); }
//
#define OC_CODEGEN_MUTATOR_SPTR(object) \
  template<typename... ArgsT> \
  inline void make_##object(ArgsT&&...args) { \
    this->_##object##_sptr = ::std::make_shared<typename decltype(_##object##_sptr)::element_type>(\
      ::std::forward<ArgsT>(args)...\
    ); \
  }


// Code generator - class object size query.
// 
// OC_CODEGEN_SIZE_QUERY(foo, thing) 
// ->
// constexpr size_t num_things() const { return foo().size() }
//
#define OC_CODEGEN_SIZE_QUERY(container, item) \
  inline size_t num_##item##s() const { return this->container().size(); }


// Code generator - disable class copy
//
// OC_CODEGEN_DISABLE_COPY
//
// object(const object&) = delete;
// object& operator = (const object&) = delete; 
//
#define OC_CODEGEN_DISABLE_COPY(object) \
  object(const object&) = delete;\
  object& operator = (const object&) = delete;

// Code generator - disable class move
//
// OC_CODEGEN_DISABLE_MOVE
//
// object(object&&) = delete;
// object& operator = (object&&) = delete; 
//
#define OC_CODEGEN_DISABLE_MOVE(object) \
  object(object&&) = delete;\
  object& operator = (object&&) = delete;

namespace OpenCluster {

constexpr size_t MAX_NUM_WRITE_IOVECS {8};
constexpr size_t MAX_NUM_READ_IOVECS  {8};

};  // End of namespace OpenCluster. --------------------------------------------------------------

#endif



