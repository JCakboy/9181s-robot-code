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

	double degrees_to_radians(double degrees) {
		return degrees * PI / 180.0;
	}

	double tankScaleJoystick(double amt) {
		return amt / std::cos(degrees_to_radians(45));
	}

	bool within127(int a) {
		return limit127(a) == a;
	}

	int limit127(int a) {
		return emath::max(emath::min(a, 127), -127);
	}

	int step0(int a) {
		if (a > 0)
			a--;
		else if (a < 0)
			a++;
		return a;
	}

}
