#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

bool visionFinished = false;
double visionLastError = 0;

// Drives the robot based on the given controller
void drive(pros::Controller * controller) {
	int movePower = controller->get_analog(STICK_LEFT_Y);
	int turnPower = controller->get_analog(STICK_LEFT_X);

	// If vision align is requested, take over the turn power
	if (controller->get_digital(BUTTON_L2) && flagVision != NULL) {
		// PID values
		double kp = 0.32;
		double kd = 0.42;

		// Prepare variables for decision
		int sigID = LCD::isAutonomousBlue() ? 1 : 2;
		int focus = LCD::isAutonomousBlue() ? 150 : 150;
		pros::vision_object_s_t sig = flagVision->get_by_sig(0, sigID);
		int middle = util::sign(sig.x_middle_coord);
		double error = middle - focus;
		// If a signature is detected, lock to it. Otherwise, give control back over to the driver
		if (middle > -2000) {
			if (util::abs(error) > 2)
				turnPower = error * kp + (12 * util::abs(error) / error) + (kd * (error - visionLastError));
			else {
				turnPower = 0;
				if (!visionFinished) {
					visionFinished = true;
					controller->rumble(".");
				}
			}
			visionLastError = error;
		}
	} else {
		visionFinished = false;
		visionLastError = 0;
	}
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

		// Maps the right joystick to the lift, implementing liftLock
		if (controllerMain->get_analog(STICK_RIGHT_Y) == 0 && liftLock)
      liftMotor->move_absolute(285, 100);
    else {
      if (liftLock) liftLock = false;
      liftMotor->move(controllerMain->get_analog(STICK_RIGHT_Y));
    }

		// Lock the lift if A is pressed
		if (controllerMain->get_digital(BUTTON_A))
			liftLock = true;

		// Maps the intake motor to the right triggers
		intakeMotor->move(util::limit127((double) controllerMain->get_digital(BUTTON_R1) * 2 * 127 - controllerMain->get_digital(BUTTON_R2) * 90));

		// Maps the index motor forward to the high left trigger and backward to B
		indexMotor->move(util::limit127((double) controllerMain->get_digital(BUTTON_L1) * 2 * 127 - controllerMain->get_digital(BUTTON_B) * 127));

		// Toggles the flywheel power when X is pressed
		if (controllerMain->get_digital_new_press(BUTTON_X)) {
			flywheelRunning = !flywheelRunning;
			flywheelMotor->move(flywheelRunning * 127);
		}
		// If down is pressed, reset the lift
		if (controllerMain->get_digital_new_press(BUTTON_DOWN))
			liftMotor->tare_position();

		// DEBUG - Trigger autonomous through the controller up button
		if (controllerMain->get_digital_new_press(BUTTON_UP))
			autonomous();

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// Prints debug information to the LCD
		LCD::printDebugInformation();

		LCD::updateScreen();
		pros::delay(20);
	}
}
