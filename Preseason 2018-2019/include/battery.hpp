#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

struct Battery {
	double get_capacity();

	double get_current();

	double get_temperature();

	double get_voltage();
};

#endif
