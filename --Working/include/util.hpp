#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "main.h"
#include <utility>
#include "time.h"

/*
 * A utility namespace to perform common robot functions
 *
 */

namespace util {

	// Returns the lesser of the two values
	double min(double a, double b);

	// Returns the greater of the two values
	double max(double a, double b);

	// Returns the absolute of the given value
	double abs(double a);

	// Converts unsigned to signed integer
	int sign(unsigned int n);

	// Converts the given degrees to radians
	double degrees_to_radians(double degrees);

	// Analogous to inch_to_meter()
	double inch_to_metre(double value);

	// Converts the given amount of inches to metres
	double inch_to_meter(double value);

	// Scales the amount by cos(45°) * 100
	double tankScaleJoystick(double amt);

	// Returns whether a given value has an absolute of equal to or less x
	bool withinX(int x, int a);

	// Returns ±x, whichever is closer, if the value has an absolute greater than x. Otherwise, it returns the given value
	int limitX(int x, int a);

	// Returns whether the value has an absolute of equal to or less than 127
	bool within127(int a);

	// Returns ±127, whichever is closer, if the value has an absolute greater than 127. Otherwise, it returns the given value
	int limit127(int a);

	// Adds or subtracts 1 from the given value, whicher gets it closer to 0
	int step0(int a);

	// Ensures an amount of digits of an integer in string format, adding 0s for padding
	std::string ensureDigits(int amount, int i);

	// Returns a timestamp in the format: YYYY-MM-DD hh:mm:ss
	std::string timestamp();

	// Splits a string at the first occurance of regex
	std::pair<std::string, std::string> separateFirst(std::string s, std::string regex);

}

#endif
