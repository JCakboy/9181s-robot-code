#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Initialize the LCD
	LCD::initialize(ports::controllerMain, ports::controllerPartner);
	LCD::setControllerText("Initializing...");

	// Initializes the gyro with calibration
	LCD::setStatus("Calibrating gyroscope...");
	LCD::setText(2, "***DO NOT MOVE THE ROBOT***");
	ports::gyro1 = new pros::ADIGyro('A', -1);
	LCD::setText(2, "");

	LCD::setStatus("Preparing motors...");
	// Sets the lift to brake
	ports::liftMotor->set_brake_mode(BRAKE_BRAKE);

	// Sets the drive to brake
	ports::frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::frontRightDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backRightDrive->set_brake_mode(BRAKE_BRAKE);

	// Sets the PID values
	ports::pid = new PID();
	ports::pid->setMovePID(0.365, 0.55, 0.5, 0.5);
	ports::pid->setPivotPID(0.132, 0.285);

	// Brake the index and intake motor
	ports::indexMotor->set_brake_mode(BRAKE_BRAKE);
	ports::intakeMotor->set_brake_mode(BRAKE_BRAKE);

	// Update the LCD
	LCD::updateScreen(true);
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
