#include "main.h"
#include "sys/stat.h"
#include <iostream>
#include <cstdio>

const std::string LOG_PATH = "/usd/logs/";

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

FILE * out = stdout;

bool fileExists(std::string name) {
  if (FILE *file = fopen(name.c_str(), "r")) {
      fclose(file);
      return true;
  } else {
      return false;
	}
}

std::string ensure3Digits(int i) {
	std::string s = std::to_string(i);
	while (s.length() < 3) s = "0" + s;
	return s;
}

FILE * nextLogFile() {
	int i = 0;
	while (fileExists(LOGS_PATH + ensure3Digits(i) + ".log")) i++;
	std::string filename = LOGS_PATH + ensure3Digits(i) + ".log";
	fputs(filename.c_str(), out);
	return fopen(filename.c_str(), "w");
}

void opcontrol() {

	FILE * file = nextLogFile();

	fputs("opened file\n", out);

	if (file == NULL)
		pros::lcd::set_text(3, "null");
	else {
		fputs("test to test file\n", file);
		pros::lcd::set_text(3, "written");
	}

	fclose(file);

	while (true) {
		pros::lcd::set_text(0, "waiting");
		pros::delay(20);
	}
}
