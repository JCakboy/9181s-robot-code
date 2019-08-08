#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

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
		intakeMotor->move((double) controllerMain->get_digital(BUTTON_L1) * 2 * 127 - controllerMain->get_digital(BUTTON_L2) * 90);

		// If A is pressed, tilt the stack to be upright
		if (controllerMain->get_digital(BUTTON_A))
			tiltMotor->move_absolute(145, 80);
		else if (tiltMotor->get_position() > 4)
			tiltMotor->move_absolute(0, 60);
		else tiltMotor->move(0);

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		LCD::setText(2, std::to_string(frontLeftDrive == NULL));
		LCD::setText(3, std::to_string(backLeftDrive == NULL));
		LCD::setText(4, std::to_string(frontRightDrive == NULL));
		LCD::setText(5, std::to_string(backRightDrive == NULL));

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// If down is pressed, reset the tilt motor
		if (controllerMain->get_digital_new_press(BUTTON_DOWN))
			tiltMotor->tare_position();

		LCD::updateScreen();
		pros::delay(20);
	}
}
