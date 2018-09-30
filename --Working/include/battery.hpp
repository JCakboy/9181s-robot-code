#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

/*
 * A class that is an alias for pros::battery and pros::Controller 's battery
 *
 * https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-battery
 */

class Battery {
	public:
		virtual double capacity() = 0;

};

class BrainBattery : public Battery {
	public:
		double capacity() override;

		double current();

		double temperature();

		double voltage();

};

class ControllerBattery : public Battery {
	private:
		pros::Controller * controller;

	public:
		ControllerBattery (pros::controller_id_e_t controller);

		ControllerBattery (pros::Controller & controller);

		double capacity() override;

		double level();

};

#endif
