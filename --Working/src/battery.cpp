#include "main.h"
#include "battery.hpp"

// All of BrainBattery's functions pass directly to the PROS API
double BrainBattery::capacity() {
	return pros::battery::get_capacity();
}

double BrainBattery::current() {
	return pros::battery::get_current();
}

double BrainBattery::temperature() {
	return pros::battery::get_temperature();
}

double BrainBattery::voltage() {
	return pros::battery::get_voltage();
}

ControllerBattery::ControllerBattery (pros::controller_id_e_t controller) {
	// Create and store the contorller
	ControllerBattery::controller = new pros::Controller(controller);
}

ControllerBattery::ControllerBattery (pros::Controller * controller) {
	// Store the contorller
	ControllerBattery::controller = controller;
}

double ControllerBattery::capacity() {
	// If the controller is connected, return the capacity. Otherwise, return -1
	if (ControllerBattery::controller->is_connected())
		return ControllerBattery::controller->get_battery_capacity();
	return -1;
}

double ControllerBattery::level() {
	// If the controller is connected, return the voltage. Otherwise, return -1
	if (ControllerBattery::controller->is_connected())
		return ControllerBattery::controller->get_battery_level();
	return -1;
}
