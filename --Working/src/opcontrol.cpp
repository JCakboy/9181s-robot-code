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

	// Start the operator control timer
	competitionTimer->opcontrolStartTimer();

	// Time to set when the claw is unclamping
	int clawUnclamping = 0;

	while (true) {
		// Drives the robot with the main controller
		drive(controllerMain);

		// Maps the right analog stick to the lift motor
		liftMotor->move(controllerMain->get_analog(STICK_RIGHT_Y));

		// Maps the right trigger buttons to intake and outtake the cubes
		if (controllerMain->get_digital(BUTTON_R1))
			intakeMotor->move(127);
		else if (controllerMain->get_digital(BUTTON_R2))
			intakeMotor->move(-127);
		else
			intakeMotor->move(0);

		// Maps the claw motor to the left triggers
		if (controllerMain->get_digital_new_press(BUTTON_L1))
			clawMotor->move(127);
		else if (controllerMain->get_digital_new_press(BUTTON_L2)) {
			clawMotor->move(-127);
			clawUnclamping = util::sign(pros::millis()) + 750;
		} else if (clawUnclamping != 0 && util::sign(pros::millis()) > clawUnclamping) {
			clawUnclamping = 0;
			clawMotor->move(0);
		}

		// If A is pressed, tilt the stack to be upright
		if (controllerMain->get_digital(BUTTON_A))
			tiltMotor->move_absolute(145, 80);
		else if (tiltMotor->get_position() > 4)
			tiltMotor->move_absolute(0, 60);
		else tiltMotor->move(0);

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// If the up button is pressed, run autonomous
		if (controllerMain->get_digital_new_press(BUTTON_UP)) autonomous();
		// If down is pressed, reset the tilt motor
		if (controllerMain->get_digital_new_press(BUTTON_DOWN))
			tiltMotor->tare_position();

		// Update the LCD screen
		LCD::updateScreen();

		// Run every 20 ms
		pros::delay(20);
	}
}
