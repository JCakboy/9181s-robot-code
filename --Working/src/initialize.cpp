#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Initialize the LLEMU
	LCD::initializeLLEMU(ports::controllerMain, ports::controllerPartner);

	// Initializes the gyro with calibration
	LCD::setStatus("Calibrating gyroscope...");
	LCD::setText(2, "***DO NOT MOVE THE ROBOT***");
	pros::delay(1500);
	LCD::setText(2, "");

	// Use the second gyro only
	ports::gyro->useFirstGyro(false);
	ports::gyro->useSecondGyro(true);

	// Brake the tilt motor
	ports::tiltMotor->set_brake_mode(BRAKE_BRAKE);

	// Start gyroscope tracking
	ports::gyroTask = new pros::Task(gyroTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Gyro");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
