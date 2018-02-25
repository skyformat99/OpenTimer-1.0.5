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

#ifndef OC_SINGLETON_HPP_
#define OC_SINGLETON_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>

namespace OpenCluster {

// Class: Singleton
// Guarantees that only a single instance of an object will exist throughout the
// lifetime of the program.
template <class Derived>
class Singleton {

  public:

    inline static Derived* get();                           // Return the pointer.

  protected:
    
    Singleton();                                            // Disable constructor.
    ~Singleton();                                           // Disable destructor.
    static Derived* _ptr;                                   // Pointer to the object.

  private:
    
    Singleton(const Singleton&);                            // Disable constructor.
    Singleton & operator = (const Singleton&);              // Disable copy.
    Singleton & operator = (Singleton&&);                   // Disable move.
};

// Initialize the singleton object.
template <class Derived>
Derived* Singleton<Derived>::_ptr = nullptr;

// Constructor
template <class Derived>
Singleton<Derived>::Singleton() {
}

// Constructor
template <class Derived>
Singleton<Derived>::~Singleton() {
  delete _ptr;
}

// Function: get
template <class Derived>
inline Derived* Singleton<Derived>::get() {
  return (_ptr == nullptr) ? _ptr = new Derived() : _ptr;
}

};  // End of OpenCluster namespace. --------------------------------------------------------------

#endif


