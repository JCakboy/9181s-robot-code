#include "main.h"
#include <cmath>
#include <climits>
#include <ctime>
#include <utility>
#include "definitions.hpp"
#include "util.hpp"

namespace util {
	/*
	 * All of these methods perform simple tasks that are utilized throughout the project
	 *
	 * For documentation, see the header file
	 */

	double min(double a, double b) {
		return ((a < b) ? a : b);
	}

	double max(double a, double b) {
		return ((a > b) ? a : b);
	}

	double abs(double a) {
		return ((a >= 0) ? a : -a);
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

	double degrees_to_radians(double degrees) {
		return degrees * PI / 180.0;
	}

	double inch_to_meter(double value) {
		return inch_to_metre(value);
	}

	double inch_to_metre(double value) {
		return value / 39.37;
	}

	double tankScaleJoystick(double amt) {
		return amt / std::cos(degrees_to_radians(45));
	}



	bool withinX(double x, double a) {
		return limitX(x, a) == a;
	}

	int limitX(double x, double a) {
		x = std::abs(x);
		return util::max(util::min(a, x), -x);
	}

	bool within127(double a) {
		return limit127(a) == a;
	}

	int limit127(double a) {
		return limitX(127, a);
	}

	int step0(double a) {
		if (a > 0)
			a = a - 1;
		else if (a < 0)
			a = a + 1;
		return a;
	}

	std::string ensureDigits(int amount, int i) {
		std::string s = std::to_string(i);
		while (s.length() < amount) s = "0" + s;
		return s;
	}

	std::string timestamp() {
		int millis = sign(pros::millis());
		return ensureDigits(2, std::floor(millis / 60000)) + ":" + ensureDigits(2, (millis / 1000) % 60);
	}

	std::pair<std::string, std::string> separateFirst(std::string s, std::string regex) {
		if (s.find(regex) == std::string::npos)
			return std::pair(s, std::string());
		std::string first = s.substr(0, s.find(regex));
		s.erase(0, s.find(regex) + 1);
		return std::pair(first, s);
	}

}
