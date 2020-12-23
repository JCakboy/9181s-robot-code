#include "main.h"
#include <cmath>
#include <climits>
#include <ctime>
#include <utility>

// Dump ports namespace for ease of use
using namespace ports;

// Drives the robot based on the given controller
void drive(pros::Controller * controller) {
	// Set the forward and backward movement
	int movePower = controller->get_analog(STICK_LEFT_Y);
	// Treat the drive left/right as turning
	int turnPower = controller->get_analog(STICK_LEFT_X);
	// Treat the secondary left/right as strafing
	int strafePower = controller->get_analog(STICK_RIGHT_X);

	// Calculate the powers for each motor
	int frontLeftPower = movePower + turnPower + strafePower;
	int backLeftPower = movePower + turnPower - strafePower;
	int frontRightPower = movePower - turnPower - strafePower;
	int backRightPower = movePower - turnPower + strafePower;

	// Assign motor powers
	frontLeftDrive->move(frontLeftPower);
	backLeftDrive->move(backLeftPower);
	frontRightDrive->move(frontRightPower);
	backRightDrive->move(backRightPower);
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

	while (true) {
		// Drives the robot with the main controller
		drive(controllerMain);

		// Maps the right trigger buttons to intake and outtake the balls
		int intakeSpeed = 0;
		if (controllerMain->get_digital(BUTTON_R1))
			intakeSpeed = 127;
		else if (controllerMain->get_digital(BUTTON_R2))
			intakeSpeed = -80;
		intakeMotorLeft->move(intakeMotorLeft->get_efficiency() < 25 && intakeSpeed > 0 ? 127 : intakeSpeed);
		intakeMotorRight->move(intakeMotorRight->get_efficiency() < 25 && intakeSpeed > 0 ? 127 : intakeSpeed);

		bool outtake = controllerMain->get_digital(BUTTON_A);
		indexer->move(outtake ? -127 : controllerMain->get_digital(BUTTON_L1) * 127);
		flywheel->move(outtake ? -127 : controllerMain->get_digital(BUTTON_L2) * 127);

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
		if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
		if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// If the up button is pressed, run autonomous
		if (controllerMain->get_digital_new_press(BUTTON_UP)) autonomous();

		// Update the LCD screen
		LCD::updateScreen();

		// Run every 20 ms
		pros::delay(20);
	}
}
