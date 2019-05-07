#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "main.h"
#include <cmath>
#include <climits>

/*
 * A utility function to hold general common robot calculations
 */
namespace util {

  // Limits the given number to ±127
  int limit127(int a);

  // Limits the given number to ±127
  double limit127(double a);

  // Returns the absolute value of a
  int abs(int a);

  // Returns the absolute value of a
  double abs(double a);

  // Converts an unsigned integer to a signed integer
  signed int sign(unsigned int a);

}

#endif
