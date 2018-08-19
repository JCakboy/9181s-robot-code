#ifndef _EMATH_HPP_
#define _EMATH_HPP_

/*
 * An extended math namespace to perform common robot math functions
 *
 */

namespace emath {

	// Returns the lesser of the two values
	double min(double a, double b);

	// Returns the greater of the two values
	double max(double a, double b);

	// Converts the given degrees to radians
	double degrees_to_radians(double degrees);

	// Scales the amount by cos(45°) * 100
	double tankScaleJoystick(double amt);

	// Returns whether the values has an absolute of equal to or less than 127
	bool within127(int a);

	// Returns ±127, whichever is closer, if the value has an absolute values greater than 127. Otherwise, it returns the given value
	int limit127(int a);

	// Adds or subtracts 1 from the given value, whicher gets it closer to 0
	int step0(int a);

}

#endif
