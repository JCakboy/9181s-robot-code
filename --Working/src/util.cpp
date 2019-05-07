#include "main.h"

namespace util {

  int limit127(int a) {
    return (a > 127 ? 127 : (a < -127 ? -127 : a));
  }

  double limit127(double a) {
    return (a > 127 ? 127 : (a < -127 ? -127 : a));
  }

  int abs(int a) {
    return (a > 0 ? a : -a);
  }

  double abs(double a) {
    return (a > 0 ? a : -a);
  }

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
