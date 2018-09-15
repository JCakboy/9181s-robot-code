#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

/*
 * A class that is an alias for pros::battery and pros::Controller 's battery
 *
 * https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-battery
 */

struct Battery {

	virtual double capacity();

};

struct BrainBattery : public Battery {

	double capacity();

	double current();

	double temperature();

	double voltage();

};

struct ControllerBattery : public Battery {
	private:
		pros::Controller * controller;

	public:
		ControllerBattery (pros::Controller & controller);

		double capacity();

		double level();

};

#endif
