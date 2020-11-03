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
	LCD::setStatus("Calibrating inertial sensor...");
	LCD::setText(2, "Do not touch the robot");

	ports::imu->reset();

	pros::delay(2250);
	LCD::setText(2, "");

	// Brake the intake motors
	ports::intakeMotorLeft->set_brake_mode(BRAKE_BRAKE);
	ports::intakeMotorRight->set_brake_mode(BRAKE_BRAKE);

	// Brake the drive motors
	ports::frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::frontRightDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backRightDrive->set_brake_mode(BRAKE_BRAKE);

	// Set the PID configuration
	ports::pid->setVelocityGyro(ports::gyro);
	ports::pid->setPowerLimits(120, 30);
	ports::pid->setMovePosPID(0.30, 0.000, 0.45);
	ports::pid->setMoveVelPID(3.775, 0.000, 1.65);
	ports::pid->setPivotPID(1.09, 0.0175, 6.32);

	ports::pid->setStrafePosPID(39, 0.775, 0.000, 0.05);
	ports::pid->setStrafeVelPID(7, 0.000, 0);
	ports::pid->setForwardAcceleration(1.12, 3, 50);
	ports::pid->setBackwardAcceleration(1.12, 3, 50);

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
