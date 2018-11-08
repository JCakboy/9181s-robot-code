#include "main.h"
#include "battery.hpp"

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
	ControllerBattery::controller = new pros::Controller(controller);
}

ControllerBattery::ControllerBattery (pros::Controller & controller) {
	ControllerBattery::controller = &controller;
}

double ControllerBattery::capacity() {
	if (ControllerBattery::controller->is_connected())
		return ControllerBattery::controller->get_battery_capacity();
	return -1;
}

double ControllerBattery::level() {
	if (ControllerBattery::controller->is_connected())
		return ControllerBattery::controller->get_battery_level();
	return -1;
}
