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

#ifndef OT_SINGLETON_H_
#define OT_SINGLETON_H_

namespace OpenTimer {

// Class: Singleton
// Guarantees that only a single instance of an object will exist throughout the
// lifetime of the program.
template <class Derived>
class Singleton
{
  public:

    inline static Derived* get();                           // Return the pointer.

  protected:
    
    Singleton();                                            // Disable constructor.
    ~Singleton();                                           // Disable destructor.
    static std::unique_ptr<Derived> _uptr;                  // Unique pointer to the object.

  private:
    
    Singleton(const Singleton&);                            // Disable constructor.
    Singleton & operator = (const Singleton&);              // Disable copy.
};

// Initialize the singleton object.
template <class Derived>
std::unique_ptr<Derived> Singleton<Derived>::_uptr = nullptr;

// Constructor
template <class Derived>
Singleton<Derived>::Singleton() {
}

// Constructor
template <class Derived>
Singleton<Derived>::~Singleton() {
}

// Function: get
template <class Derived>
inline Derived* Singleton<Derived>::get() {
  if(_uptr != nullptr) return _uptr.get();
  std::unique_ptr<Derived> tmp(new Derived());
  _uptr = std::move(tmp);
  return _uptr.get();
}


};  // End of OpenTimer namespace. ----------------------------------------------------------------

#endif


