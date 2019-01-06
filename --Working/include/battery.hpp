#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

/*
 * A placeholder class should represent a general battery
 * A virtual class, should have its capacity() method overwritten
 *
 */
class Battery {
	public:
		// Should return the capacity of a battery as a percentage
		virtual double capacity() = 0;

};

/*
 * A class meant to interface a brain battery
 *
 * For documentation, see: https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-battery
 */

class BrainBattery : public Battery {
	public:
		// Returns the charge of the battery as a percentage
		double capacity() override;

		// Returns the current of the battery in mA
		double current();

		// Returns the temperature of the battery in ºC
		double temperature();

		// Returns the voltage of the battery in mV
		double voltage();

};

/*
 * A class meant to interface a controller battery
 *
 * For documentation, see: https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-controller
 */

class ControllerBattery : public Battery {
	private:
		// The stored controller pointer
		pros::Controller * controller;

	public:
		// Creates the controller battery object, given a controller id
		ControllerBattery (pros::controller_id_e_t controller);

		// Creates the controller battery object, given a controller pointer
		ControllerBattery (pros::Controller * controller);

		// Returns the charge of the battery as a percentage
		double capacity() override;

		// Returns the voltage of the battery in mV
		double level();

};

#endif
