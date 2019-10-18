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

	// Set the PID configuration
	ports::pid->setVelocityGyro(ports::gyro);
	ports::pid->setPowerLimits(120, 20);
	ports::pid->setMovePID(0.271, 0.225, 0.335, 0.135);
	ports::pid->setPivotPID(977, 0.13, 0.43);
	ports::pid->setForwardAcceleration(1.2, 5, 50);
	ports::pid->setBackwardAcceleration(1.14, 3, 50);

	ports::pid->setNoStopDebug(false);
	ports::pid->setLoggingDebug(false);

	// Start gyroscope tracking
	ports::gyroTask = new pros::Task(gyroTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Gyro");
	// Start message debugging if the debugger is attached
	ports::mhTask = new pros::Task(mhTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Message Handler");

	LCD::setStatus("Initialization complete");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	LCD::setStatus("Disabled");
}

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
