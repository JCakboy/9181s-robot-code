#include "main.h"
#include <cmath>
#include "definitions.hpp"
#include "emath.hpp"

namespace emath {
	double min(double a, double b) {
		return ((a < b) ? a : b);
	}

	double max(double a, double b) {
		return ((a > b) ? a : b);
	}

	double degrees_to_radians(double radians) {
		return radians * PI / 180.0;
	}

	double tankScaleJoystick(double amt) {
		return amt / std::cos(degrees_to_radians(45));
	}

	int limit127(int a) {
		return emath::max(emath::min(a, 127), -127);
	}
}
