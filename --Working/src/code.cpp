#include "main.h"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  if (SD_INSERTED)
	  Logger::init("/usd/logs/" + util::timestamp() + ".txt");


	pros::lcd::initialize();
	pros::lcd::set_text(1, "init complete");
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


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

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
  pros::lcd::set_text(1, "op started");

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor frontRightDrive(1, MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor frontLeftDrive(2, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor backRightDrive(3, MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor backLeftDrive(4, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor lift(5, MOTOR_GEARSET_18, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor intakeMotor(6, MOTOR_GEARSET_18, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor launcherMotor(10, MOTOR_GEARSET_36, false, MOTOR_ENCODER_ROTATIONS);

	pros::Mutex driveMutex;

	DriveControl dc (driveMutex, frontLeftDrive, backLeftDrive, frontRightDrive, backRightDrive);

Logger::log(LOG_INFO, "test!");

	int launcherAngle = 30;
	int rpm = 200 * 21;
	double distanceToFlag = 0;
	double flagHeight = 0;
	//double initialVelocity =

	while (true) {
		dc.run(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_LEFT_X), false, false, true );

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
			launcherMotor.move_relative(1,127);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			intakeMotor.move(127);
		else intakeMotor.move(0);

		lift.move(master.get_analog(ANALOG_RIGHT_Y) * 3 / 4);

		pros::delay(20);
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}
