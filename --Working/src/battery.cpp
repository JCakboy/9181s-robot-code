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

ControllerBattery::ControllerBattery (pros::Controller & controller) {
	ControllerBattery::controller = &controller;
}

double ControllerBattery::capacity() {
	return ControllerBattery::controller->get_battery_capacity();
}

double ControllerBattery::level() {
	return ControllerBattery::controller->get_battery_level();
}
