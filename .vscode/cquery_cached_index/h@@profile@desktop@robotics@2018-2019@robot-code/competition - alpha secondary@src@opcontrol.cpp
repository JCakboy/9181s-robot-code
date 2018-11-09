#include "main.h"

#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor leftDrive(2, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor rightDrive(1, MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);

	pros::Motor leftLift(4, MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor rightLift(3, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);

	pros::Motor clawMotor(8, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);

	pros::Motor intakeMotor(9, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor launcherMotor(10, MOTOR_GEARSET_36, false, MOTOR_ENCODER_ROTATIONS);

	bool rotated = false;

	while (true) {
		int right = min(127, max(-127, master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X)));
		int left = min(127, max(-127, master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X)));

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
			launcherMotor.move_relative(1,127);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			intakeMotor.move(127);
		else intakeMotor.move(0);

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (rotated)
				clawMotor.move_absolute(0, 127);
			else
				clawMotor.move_absolute(180, 127);
			rotated = !rotated;
		}

		leftDrive.move(left);
		rightDrive.move(right);
		leftLift.move(master.get_analog(ANALOG_RIGHT_Y) * 3 / 4);
		rightLift.move(master.get_analog(ANALOG_RIGHT_Y) * 3 / 4);
		pros::delay(20);
	}
}
