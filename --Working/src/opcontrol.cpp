#include "main.h"
#include <cmath>
#include <climits>
#include <ctime>
#include <utility>
#include "definitions.hpp"
// Dump ports namespace for ease of use
using namespace ports;

int sign(unsigned int n) {
	if (n > INT_MAX) {
		if (n <= UINT_MAX + INT_MIN) {
			return 0;
		}
		return static_cast<int>(n + INT_MIN) - (UINT_MAX + INT_MIN + 1);
	} else {
		return static_cast<int>(n);
	}
}

// Drives the robot based on the given controller
void drive(pros::Controller * controller) {
	int movePower = controller->get_analog(STICK_LEFT_Y);
	int turnPower = controller->get_analog(STICK_LEFT_X);

	int leftPower = movePower + turnPower;
	int rightPower = movePower - turnPower;
	frontLeftDrive->move(leftPower);
	backLeftDrive->move(leftPower);
	frontRightDrive->move(rightPower);
	backRightDrive->move(rightPower);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	// Sets the status on the LCD
	LCD::setStatus("Operator Control");

	int counts = 0;
	int last = 0;
	while (true) {
		if (controllerMain->get_digital(BUTTON_A))
			pid->driveStraight(-100);
		else {
			frontLeftDrive->move(0);
			frontRightDrive->move(0);
			backLeftDrive->move(0);
			backRightDrive->move(0);
		}

		if (controllerMain->get_digital_new_press(BUTTON_UP))
			messageHolder->appendLine("Up");
		if (controllerMain->get_digital_new_press(BUTTON_DOWN))
			messageHolder->appendLine("Down");
		if (controllerMain->get_digital_new_press(BUTTON_LEFT))
			messageHolder->appendLine("Left");
		if (controllerMain->get_digital_new_press(BUTTON_RIGHT))
			messageHolder->appendLine("Right");

		if (controllerMain->get_digital(BUTTON_B))
			pid->pivot(-20);

		if (controllerMain->get_digital(BUTTON_X))
			pid->pivot(40);

		if (controllerMain->get_digital(BUTTON_Y))
			pid->pivot(-40);

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		LCD::setText(6, std::to_string(competitionTimer->opcontrolTime()));

		// LCD::setText(4, std::to_string(gyro->getValue()));

		LCD::updateScreen();
		counts++;
		pros::delay(1);
	}
}
