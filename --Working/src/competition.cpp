#include "main.h"
#include <cmath>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  if (false)
	  Logger::init("/usd/logs/" + util::timestamp() + ".txt");
  Logger::init("/usd/logs/test.txt");


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
 *//*
void autonomous()
{
  	pros::Controller master(pros::E_CONTROLLER_MASTER);
  	pros::Motor frontRightDrive(19, GEARSET_200, true, ENCODER_DEGREES);
  	pros::Motor frontLeftDrive(12, GEARSET_200, false, ENCODER_DEGREES);
  	pros::Motor backRightDrive(20, GEARSET_200, true, ENCODER_DEGREES);
  	pros::Motor backLeftDrive(11, GEARSET_200, false, ENCODER_DEGREES);
  	pros::Motor lift(18, GEARSET_200, false, ENCODER_DEGREES);
  	pros::Motor intakeMotor(13, GEARSET_200, true, ENCODER_DEGREES);
  	pros::Motor frontLauncherMotor(14, GEARSET_200, true, ENCODER_DEGREES);
    pros::Motor backLauncherMotor(15, GEARSET_200, false, ENCODER_DEGREES);

    shoot();
    drive(687, -100);
    turn90(false);
    drive(1000, 127);
}

void drive(int position, int velocity)
{
  frontRightDrive.moveRelative(position, veloctity);
  frontLeftDrive.moveRelative(position, velocity);
  backRightDrive.moveRelative(position, veloctiy);
  backLeftDrive.moveRelative(position, velocity);

  frontRightDrive.set_brake_mode(BRAKE_BRAKE);
  frontLeftDrive.set_brake_mode(BRAKE_BRAKE);
  backRightDrive.set_brake_mode(BRAKE_BRAKE);
  backLeftDrive.set_brake_mode(BRAKE_BRAKE);
  pros:delay(150);

  frontRightDrive.set_brake_mode(BRAKE_COAST);
  frontLeftDrive.set_brake_mode(BRAKE_COAST);
  backRightDrive.set_brake_mode(BRAKE_COAST);
  backLeftDrive.set_brake_mode(BRAKE_COAST);
}

void shoot()
{
  frontLauncherMotor.move(127);
  backLauncherMotor.move(127);
  pros::delay(1000);
  intakeMotor.move(127);
  pros::delay(100);
  frontLauncherMotor.move(0);
  backLauncherMotor.move(0);
  intakeMotor.move(0);
}

void turn90(bool isClockwise)
{
  int power = (isClockwise) ? 100 : -100;
  frontRightDrive.move(-power);
  frontLeftDrive.move(power);
  backRightDrive.move(-power);
  backLeftDrive.move(power);
}
*/
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
	pros::Motor frontRightDrive(19, GEARSET_200, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor frontLeftDrive(12, GEARSET_200, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor backRightDrive(20, GEARSET_200, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor backLeftDrive(11, GEARSET_200, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor lift(18, GEARSET_200, false, MOTOR_ENCODER_ROTATIONS);
	pros::Motor intakeMotor(13, GEARSET_200, true, MOTOR_ENCODER_ROTATIONS);
	pros::Motor frontLauncherMotor(14, GEARSET_200, true, MOTOR_ENCODER_ROTATIONS);
  pros::Motor backLauncherMotor(15, GEARSET_200, false, MOTOR_ENCODER_ROTATIONS);

	pros::Mutex driveMutex;

	DriveControl dc (driveMutex, frontLeftDrive, backLeftDrive, frontRightDrive, backRightDrive);

  Logger::log(LOG_ERROR, "test!");

  lift.set_brake_mode(BRAKE_BRAKE);

  dc.setPID(20, .5, .1, .2, 50);

  int currentFlywheelVelocity = 0;

	while (true) {

    if(master.get_digital(BUTTON_X))
    {
      autonomous();
    }

    dc.run(master.get_analog(STICK_LEFT_Y), master.get_analog(STICK_LEFT_X), false, false, true);


		if (master.get_digital(BUTTON_R1))
      intakeMotor.move(127);
    else
      intakeMotor.move(0);

    if (master.get_digital(BUTTON_R2)) {
      frontLauncherMotor.move(-127);
      backLauncherMotor.move(-127);
    } else if (master.get_digital(BUTTON_L2)) {
      frontLauncherMotor.move(127);
      backLauncherMotor.move(127);
    } else if (master.get_digital(BUTTON_L1)) {
      frontLauncherMotor.move(100);
      backLauncherMotor.move(100);
    } else {
      frontLauncherMotor.move(0);
      backLauncherMotor.move(0);
    }


/* pid dynamic
    if (master.get_digital(BUTTON_B)) {
      if (master.get_digital(BUTTON_UP))
        dc.getPID().modifykd(0.01);
      if (master.get_digital(BUTTON_DOWN))
        dc.getPID().modifykd(-0.01);
      if (master.get_digital(BUTTON_LEFT))
        dc.getPID().modifykd(0.1);
      if (master.get_digital(BUTTON_RIGHT))
        dc.getPID().modifykd(-0.1);
    }

    if (master.get_digital(BUTTON_X)) {
      if (master.get_digital(BUTTON_UP))
        dc.getPID().modifykp(0.01);
      if (master.get_digital(BUTTON_DOWN))
        dc.getPID().modifykp(-0.01);
      if (master.get_digital(BUTTON_LEFT))
        dc.getPID().modifykp(0.1);
      if (master.get_digital(BUTTON_RIGHT))
        dc.getPID().modifykp(-0.1);
    }

    if (master.get_digital(BUTTON_Y)) {
      if (master.get_digital(BUTTON_UP))
        dc.getPID().modifyki(0.01);
      if (master.get_digital(BUTTON_DOWN))
        dc.getPID().modifyki(-0.01);
      if (master.get_digital(BUTTON_LEFT))
        dc.getPID().modifyki(0.1);
      if (master.get_digital(BUTTON_RIGHT))
        dc.getPID().modifyki(-0.1);
    }

    if (master.get_digital(BUTTON_A)) {
      pros::lcd::set_text(4, "running pid");
      dc.moveRelative(0, 1000, 10, true, true);
    }
*/

    // flywheel dynamic
    //if (master.get_digital(BUTTON_UP)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity + 1);
    //if (master.get_digital(BUTTON_DOWN)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity - 1);

    pros::lcd::set_text(1, "kp = " + std::to_string(dc.getPID().getkp()));
    pros::lcd::set_text(2, "ki = " + std::to_string(dc.getPID().getki()));
    pros::lcd::set_text(3, "kd = " + std::to_string(dc.getPID().getkd()));

    std::string hiheat = "";
    std::string overheat = "";

    if (backLeftDrive.get_temperature() > 55) overheat += "bldrive ";
    else if (backLeftDrive.get_temperature() > 45) hiheat += "bldrive ";

    if (backRightDrive.get_temperature() > 55) overheat += "brdrive ";
    else if (backLeftDrive.get_temperature() > 45) hiheat += "brdrive ";

    if (frontLeftDrive.get_temperature() > 55) overheat += "fldrive ";
    else if (frontLeftDrive.get_temperature() > 45) hiheat += "fldrive ";

    if (frontRightDrive.get_temperature() > 55) overheat += "frdrive ";
    else if (frontRightDrive.get_temperature() > 45) hiheat += "frdrive ";

    if (lift.get_temperature() > 55) overheat += "lift ";
    else if (lift.get_temperature() > 45) hiheat += "lift ";

    if (intakeMotor.get_temperature() > 55) overheat += "intake ";
    else if (intakeMotor.get_temperature() > 45) hiheat += "intake ";

    if (frontLauncherMotor.get_temperature() > 55) overheat += "frontfw ";
    else if (frontLauncherMotor.get_temperature() > 45) hiheat += "frontfw ";

    if (backLauncherMotor.get_temperature() > 55) overheat += "backfw ";
    else if (backLauncherMotor.get_temperature() > 45) hiheat += "backfw ";

    if (overheat != "") pros::lcd::set_text(5, overheat + " ovrht");
    //if (hiheat != "") pros::lcd::set_text(6, hiheat + " hiheat");


		lift.move(master.get_analog(ANALOG_RIGHT_Y));

		pros::delay(20);
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}
