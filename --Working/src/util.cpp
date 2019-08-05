#include "main.h"

namespace util {

  // Limits the given number to ±127
  int limit127(int a) {
    return (a > 127 ? 127 : (a < -127 ? -127 : a));
  }

  // Limits the given number to ±127
  double limit127(double a) {
    return (a > 127 ? 127 : (a < -127 ? -127 : a));
  }

  // Returns the absolute value of a
  int abs(int a) {
    return (a > 0 ? a : -a);
  }

  // Returns the absolute value of a
  double abs(double a) {
    return (a > 0 ? a : -a);
  }

  // Converts an unsigned integer to a signed integer
  int sign(unsigned int n) {
    if (n > INT_MAX) {
      if (n <= UINT_MAX + INT_MIN) {
        return 0;
      }
      return static_cast<int>(n + INT_MIN) - (UINT_MAX + INT_MIN + 1);
    } else {
      return static_cast<int>(n);
    }
  }

}
