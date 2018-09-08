#include "main.h"
#include "battery.hpp"

double Battery::get_capacity() {
	return pros::battery::get_capacity();
}

double Battery::get_current() {
	return pros::battery::get_current();
}

double Battery::get_temperature() {
	return pros::battery::get_temperature();
}

double Battery::get_voltage() {
	return pros::battery::get_voltage();
}
