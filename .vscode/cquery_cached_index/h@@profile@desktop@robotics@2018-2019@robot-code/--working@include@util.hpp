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

	// Converts the given degrees to radians
	double degrees_to_radians(double degrees);

	// Analogous to inch_to_meter()
	double inch_to_metre(double value);

	// Converts the given amount of inches to metres
	double inch_to_meter(double value);

	// Scales the amount by cos(45°) * 100
	double tankScaleJoystick(double amt);

	// Returns whether the values has an absolute of equal to or less than 127
	bool within127(int a);

	// Returns ±127, whichever is closer, if the value has an absolute values greater than 127. Otherwise, it returns the given value
	int limit127(int a);

	// Adds or subtracts 1 from the given value, whicher gets it closer to 0
	int step0(int a);

	// Returns a timestamp in the format: YYYY-MM-DD hh:mm:ss
	std::string timestamp();

	// Splits a string at the first occurance of regex
	std::pair<std::string, std::string> separateFirst(std::string s, std::string regex);

	// Gets the name of the given logging level
	std::string getLoggingLevelName(logging_levels level);

}

#endif
