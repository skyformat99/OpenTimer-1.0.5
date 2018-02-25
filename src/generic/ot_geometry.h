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

#ifndef OT_GEOMETRY_H_
#define OT_GEOMETRY_H_

#include "ot_macrodef.h"

namespace OpenTimer {

//-------------------------------------------------------------------------------------------------

// Class: Point
template<class T>
class Point {

  public:

    typedef T value_t;                                      // Value type.

    Point();                                                // Constructor.           
    Point(const T&, const T&);                              // Constructor.
    ~Point();                                               // Destructor.
    
    inline void set_x(const T);                             // Set the x coordinate.
    inline void set_y(const T);                             // Set the y coordinate.

    inline const T& x() const;                              // Query the x value.
    inline const T& y() const;                              // Query the y value.

  private:

    T _x;                                                   // x coordinate.
    T _y;                                                   // y coordinate.
};

// Constructor.
template <class T>
Point<T>::Point(): 
  _x(OT_UNDEFINED_COORDINATE),
  _y(OT_UNDEFINED_COORDINATE)
{
}

// Constructor.
template <class T>
Point<T>::Point(const T& x, const T& y):
  _x(x),
  _y(y)
{
}

// Destructor.
template <class T>
Point<T>::~Point() {
}

// Function: x
// Query the x value.
template <class T>
inline const T& Point<T>::x() const {
  return _x;
}

// Function: y
// Query the y value.
template <class T>
inline const T& Point<T>::y() const {
  return _y;
}

// Procedure: set_x
// Set the x coordinate value of the point.
template<class T>
inline void Point<T>::set_x(const T x) {
  _x = x;
}

// Procedure: set_y
// Set the y coordinate value of the point.
template<class T>
inline void Point<T>::set_y(const T y) {
  _y = y;
}

//-------------------------------------------------------------------------------------------------

// Class: Rectangle
template<class T>
class Rectangle {

  public:
    
    typedef T value_t;                                      // Value type.

    Rectangle();                                            // Constructor.
    Rectangle(const Point<T>&, const Point<T>&);            // Constructor.
    Rectangle(const T&, const T&, const T&, const T&);      // Constructor.
    
    inline void set_lower_left_point(const Point <T> &);    // Set the lower left point.
    inline void set_upper_right_point(const Point <T> &);   // Set the upper right point.
    inline void set_lower_left_point(const T, const T);     // Set the lower left point.
    inline void set_upper_right_point(const T, const T);    // Set the upper right point.

    inline const Point<T>& lower_left_point() const;        // Query the lower left point.
    inline const Point<T>& upper_right_point() const;       // Query the upper right point.

    inline T width() const;                                 // Query the width (x difference).
    inline T height() const;                                // Query the height (y difference).
    inline T area() const;                                  // Query the area.
    
  private:

    Point <T> _lower_left_point;                            // lower left point.
    Point <T> _upper_right_point;                           // upper right point.
};

// Constructor.
template <class T>
Rectangle<T>::Rectangle() {

}

// Constructor
template <class T>
Rectangle<T>::Rectangle(const Point<T>& llp, const Point<T>& urp):
  _lower_left_point(llp),
  _upper_right_point(urp)
{

}

// Constructor.
template <class T>
Rectangle<T>::Rectangle(const T& llx, const T& lly, const T& urx, const T& ury):
  _lower_left_point(llx, lly),
  _upper_right_point(urx, ury)
{

}

// Procedure: set_lower_left_point
// Set the lower-left coordinate.
template <class T>
inline void Rectangle<T>::set_lower_left_point(const Point <T> & llp) {
  _lower_left_point = llp;
}

// Procedure: set_lower_left_point
// Set the lower-left coordinate.
template <class T>
inline void Rectangle<T>::set_lower_left_point(const T x, const T y) {
  _lower_left_point.set_x(x);
  _lower_left_point.set_y(y);
}

// Procedure: set_upper_right_point
// Set the upper-right coordinate.
template <class T>
inline void Rectangle<T>::set_upper_right_point(const Point <T> & urp) {
  _upper_right_point = urp;
}

// Procedure: set_upper_right_point
// Set the upper-right coordinate.
template <class T>
inline void Rectangle<T>::set_upper_right_point(const T x, const T y) {
  _upper_right_point.set_x(x);
  _upper_right_point.set_y(y);
}

// Function: width
// Return the width.
template <class T>
inline T Rectangle<T>::width() const {
  return _upper_right_point.x() - _lower_left_point.x();
}

// Function: height
// Return the height.
template <class T>
inline T Rectangle<T>::height() const {
  return _upper_right_point.y() - _lower_left_point.y();
}

// Function: area
// Return the area.
template <class T>
inline T Rectangle<T>::area() const {
  return width() * height();
}

// Function: lower_left_point
// Return the lower-left point.
template <class T>
inline const Point<T>& Rectangle<T>::lower_left_point() const {
  return _lower_left_point;
}

// Function: upper_right_point
// Return the upper-right point.
template <class T>
inline const Point<T>& Rectangle<T>::upper_right_point() const {
  return _upper_right_point;
}

//-------------------------------------------------------------------------------------------------




};

#endif



