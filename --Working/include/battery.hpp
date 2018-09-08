#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

/*
 * A class that is an alias for pros::battery
 *
 * https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-battery
 */

struct Battery {
	double get_capacity();

	double get_current();

	double get_temperature();

	double get_voltage();
};

#endif
