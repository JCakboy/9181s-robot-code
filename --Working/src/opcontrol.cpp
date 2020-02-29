#include "main.h"
#include <cmath>
#include <climits>
#include <ctime>
#include <utility>

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

// Flag for strafing
bool strafing = false;

// Drives the robot based on the given controller
void drive(pros::Controller * controller) {
	// Set the forward and backward movement
	int movePower = controller->get_analog(STICK_LEFT_Y);
	// If L2 is pressed, treat the left/write as strafing. Otherwise, turning
	if (!controller->get_digital(BUTTON_L2)) {
		int turnPower = controller->get_analog(STICK_LEFT_X);

		int leftPower = movePower + turnPower;
		int rightPower = movePower - turnPower;

		frontLeftDrive->move(leftPower);
		backLeftDrive->move(leftPower);
		frontRightDrive->move(rightPower);
		backRightDrive->move(rightPower);
		strafing = false;
	} else {
		int strafePower = controller->get_analog(STICK_LEFT_X);
		if (!strafing) {
			pid->tareDesiredHeading();
			strafing = true;
		}
		pid->strafeStraight(strafePower, movePower);
	}
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

	// Flag for locking the lift
	int liftLock = 0;

	// Start the operator control timer
	competitionTimer->opcontrolStartTimer();

	// Target for tray to be perpendicular to the ground
	const int traytarget = 780;
	int trayMoveStage = 0;

	while (true) {
		// Drives the robot with the main controller
		drive(controllerMain);

		// Maps the right joystick to the lift, implementing liftLock
		if (controllerMain->get_analog(STICK_RIGHT_Y) == 0 && liftLock > 1)
			liftMotor->move_absolute(614, 100);
		else if (controllerMain->get_analog(STICK_RIGHT_Y) == 0 && liftLock == 1)
			liftMotor->move_absolute(490, 100);
		else {
			if (liftLock) liftLock = 0;
			liftMotor->move(controllerMain->get_analog(STICK_RIGHT_Y));
		}

		// Lock the lift if X is pressed
		if (controllerMain->get_digital_new_press(BUTTON_X))
			liftLock++;

		// Maps the right trigger buttons to intake and outtake the cubes
		int intakeSpeed = 0;
		if (controllerMain->get_digital(BUTTON_R1))
			intakeSpeed = 127;
		else if (controllerMain->get_digital(BUTTON_R2))
			intakeSpeed = -80;
		intakeMotorLeft->move(intakeMotorLeft->get_efficiency() < 25 && intakeSpeed > 0 ? 127 : intakeSpeed);
		intakeMotorRight->move(intakeMotorRight->get_efficiency() < 25 && intakeSpeed > 0 ? 127 : intakeSpeed);

		// If the left triggers are pressed, tilt the stack to be upright
		if (controllerMain->get_digital(BUTTON_L1) && trayMoveStage == 0) {
			frontLeftDrive->set_brake_mode(BRAKE_HOLD);
			frontRightDrive->set_brake_mode(BRAKE_HOLD);
			backLeftDrive->set_brake_mode(BRAKE_HOLD);
			backRightDrive->set_brake_mode(BRAKE_HOLD);
		}

		if (controllerMain->get_digital(BUTTON_L1) && tiltMotor->get_position() < (traytarget * .535) && trayMoveStage <= 1) {
			tiltMotor->move(127); // Simple P controller
			trayMoveStage = 1;
		} else if (controllerMain->get_digital(BUTTON_L1) && tiltMotor->get_position() < (traytarget * .368) && trayMoveStage <= 2) {
			tiltMotor->move(55 + (traytarget - tiltMotor->get_position()) * 0.125); // Simple P controller
			trayMoveStage = 2;
		} else if (controllerMain->get_digital(BUTTON_L1) && tiltMotor->get_position() < (traytarget - 15) && trayMoveStage <= 3) {
			tiltMotor->move(55 + (traytarget - tiltMotor->get_position()) * 0.059); // Simple P controller
			trayMoveStage = 3;
		} else if (controllerMain->get_digital(BUTTON_L1) && trayMoveStage <= 4) {
			tiltMotor->move_absolute(traytarget, 36); // Gets rid of the jittering
			trayMoveStage = 4;
		}

		// Otherwise, lower the tray
		else if (tiltMotor->get_position() > 5) {
			tiltMotor->move_absolute(0, 50);
			trayMoveStage = 0;
			frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
			frontRightDrive->set_brake_mode(BRAKE_BRAKE);
			backLeftDrive->set_brake_mode(BRAKE_BRAKE);
			backRightDrive->set_brake_mode(BRAKE_BRAKE);
		} else tiltMotor->move(0);

		// if (controllerMain->get_digital(BUTTON_X)) {
		// 	pid->setNoStopDebug(true);
		// 	pid->setControllerXStop(true);
		// 	pid->setLoggingDebug(true);
		// 	pid->move(10);
		// }
		//
		// if (controllerMain->get_digital(BUTTON_B)) {
		// 	pid->setNoStopDebug(true);
		// 	pid->setControllerXStop(true);
		// 	pid->setLoggingDebug(true);
		// 	pid->move(-10);
		// }
		//
		// if (controllerMain->get_digital(BUTTON_Y)) {
		// 	pid->setControllerXStop(true);
		// 	pid->setLoggingDebug(true);
		// 	pid->pivot(-180);
		// }
		//
		// if (controllerMain->get_digital(BUTTON_A)) {
		// 	pid->setControllerXStop(true);
		// 	pid->setLoggingDebug(true);
		// 	pid->pivot(-90);
		// }

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// If the up button is pressed, run autonomous
		if (controllerMain->get_digital_new_press(BUTTON_UP)) autonomous();
		// If down is pressed, run the reset routine
		if (controllerMain->get_digital_new_press(BUTTON_DOWN)) {
			tiltMotor->move(-40);
			liftMotor->move(-40);
			pros::delay(2000);
			tiltMotor->tare_position();
			liftMotor->tare_position();
			liftLock = false;
		}

		// Update the LCD screen
		LCD::updateScreen();

		// Run every 20 ms
		pros::delay(20);
	}
}
