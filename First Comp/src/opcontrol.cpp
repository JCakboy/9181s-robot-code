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
	pros::Motor leftDrive(1);
	pros::Motor rightDrive(2);
	while (true) {
		int left = min(127, max(-127, master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X)));
		int right = min(127, max(-127, master.get_analog(ANALOG_LEFT_X) - master.get_analog(ANALOG_LEFT_Y)));

		left_mtr = left;
		right_mtr = right;
		pros::delay(20);
	}
}
