#include "main.h"
#include <cstdio>
#include <iostream>

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
void my_task_fn(void* param) {
	while (true) {
		pros::lcd::set_text(0, "waiting in task");
		std::string s;
		getline(std::cin, s);
		std::cout << "Out: " << s << "\n";
		pros::delay(750);
	}
}

void opcontrol() {
	pros::Task my_task (my_task_fn, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "My Task");
	while (true) {
		pros::lcd::set_text(0, "waiting in op");
		pros::delay(1000);
	}
}
