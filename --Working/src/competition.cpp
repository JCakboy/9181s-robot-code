#include "main.h"
#include <cmath>

Unused::Unused() {}
Unused::Unused(int i) {}

namespace ports {
  void init() {
    ports::controllerMain = new pros::Controller(CONTROLLER_MAIN);
    ports::controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

    //ports::brainBattery = new BrainBattery();
    //ports::controllerMainBattery = new ControllerBattery(*(ports::controllerMain));
    // ports::controllerPartnerBattery = new ControllerBattery(*(ports::controllerPartner));

    ports::frontLeftDrive = new pros::Motor(1, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::port2 = new Unused(2);
    ports::port3 = new Unused(3);
    ports::port4 = new Unused(4);
    ports::port5 = new Unused(5);
    ports::port6 = new Unused(6);
    ports::port7 = new Unused(7);
    ports::port8 = new Unused(8);
    ports::port9 = new Unused(9);
    ports::frontRightDrive = new pros::Motor(10, GEARSET_200, REV, ENCODER_DEGREES);
    ports::backLeftDrive = new pros::Motor(11, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::intakeMotor = new pros::Motor(12, GEARSET_200, REV, ENCODER_DEGREES);
    ports::port13 = new Unused(13);
    ports::port14 = new Unused(14);
    ports::port15 = new Unused(15);
    ports::port16 = new Unused(16);
    ports::frontLauncherMotor = new pros::Motor(17, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::backLauncherMotor = new pros::Motor(18, GEARSET_200, REV, ENCODER_DEGREES);
    ports::liftMotor = new pros::Motor(19, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::backRightDrive = new pros::Motor(20, GEARSET_200, REV, ENCODER_DEGREES);
    ports::port21 = new Unused();

    ports::driveLock = new pros::Mutex();
    ports::launcherLock = new pros::Mutex();
    ports::intakeLock = new pros::Mutex();
    ports::liftLock = new pros::Mutex();

    ports::driveControl = new DriveControl(*ports::driveLock, *ports::frontLeftDrive, *ports::backLeftDrive, *ports::frontRightDrive, *ports::backRightDrive);
    ports::drive = new DriveFunction(ports::driveControl);
  }
}

using namespace ports;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  ports::init();
  LCD::initialize();
  //Profiles::registerAll();

  if (false)
	  Logger::init("/usd/logs/" + util::timestamp() + ".txt");
  Logger::init("/usd/logs/test.txt");

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

int selectedAutonomous = 0;
void autonomous() {
  if (selectedAutonomous == 1 || selectedAutonomous == 3) {
    bool turnInvert = (selectedAutonomous == 3);

    LCD::setStatus("Auto Step 1");
    // Move forward
    drive->move(1100);

    LCD::setStatus("Auto Step 2");
    // Get the ball
    intakeMotor->move(127);
    drive->move(250);

    LCD::setStatus("Auto Step 3");
    // Turn to face other cap
    drive->move(-650);
    drive->pivot(turnInvert ? 70 : -70);

    LCD::setStatus("Auto Step 4");
    // Flip the cap
    intakeMotor->move(-127);
    drive->move(1000);
    intakeMotor->move(0);
    drive->move(-700);

    LCD::setStatus("Auto Step 5");
    // Get in position to platform
    drive->pivot(turnInvert ? -110 : 110);

    LCD::setStatus("Auto Step 7");
    // Get on platform
    drive->move(1700);
  }
  else if (selectedAutonomous == 0 || selectedAutonomous == 2) {/*
    LCD::setStatus("Auto Step 1");
    // Move forward
    drive->move(1350);
    intakeMotor->move(127);
    */
    bool turnInvert = (selectedAutonomous == 2);
    LCD::setStatus("Auto Step 3");
    // Start the flywheel
    frontLauncherMotor->move(127);
    backLauncherMotor->move(127);
    /*
    LCD::setStatus("Auto Step 4");
    // Move backward
    drive->move(-1350);
    intakeMotor->move(0);

    LCD::setStatus("Auto Step 5");
    // turn right
    drive->move(75);
    drive->pivot(90);

    LCD::setStatus("Auto Step 6");
    // Move to high flag position
    drive->move(200);
    */
    pros::delay(3000);
    intakeMotor->move(127);
    pros::delay(3000);

    LCD::setStatus("Auto Step 7");
    // Move to mid flag position
    drive->move(750);/*
    intakeMotor->move(127);
    pros::delay(1800);
    intakeMotor->move(0);
    */
    // Toggle low flag and reset
    drive->pivot(turnInvert ? -27 : 27);
    drive->move(610);

    LCD::setStatus("Auto Step 8");
    // Drive to platform
    drive->move(-2250);
    drive->pivot(turnInvert ? 100 : -130);
    drive->move(1600);
  }
  /*
  if (selectedAutonomous == 0) {
    driveControl->clearPID();
    int speed = 110;

    // Go forward, get the ball and return
    driveControl->runLeftMotors(speed);
    driveControl->runRightMotors(speed);
    while (frontRightDrive->get_position() < 1050) pros::delay(1);
    intakeMotor->move(speed);
    while (frontRightDrive->get_position() < 1250) pros::delay(1);
    driveControl->runLeftMotors(-speed);
    driveControl->runRightMotors(-speed);

    frontLauncherMotor->move(127);
    backLauncherMotor->move(127);

    while (frontRightDrive->get_position() > 300) pros::delay(1);
    intakeMotor->move(0);
    while (frontRightDrive->get_position() > -100) pros::delay(1);
    driveControl->runLeftMotors(speed);
    driveControl->runRightMotors(speed);
    pros::delay(100);


    for (const auto & motor : driveControl->rightMotors)
      motor.tare_position();
    for (const auto & motor : driveControl->leftMotors)
      motor.tare_position();

    intakeMotor->move(127);

    // Turn right
    driveControl->runLeftMotors(speed);
    driveControl->runRightMotors(-speed);
    while (frontRightDrive->get_position() > -250) pros::delay(1);

    for (const auto & motor : driveControl->rightMotors)
      motor.tare_position();
    for (const auto & motor : driveControl->leftMotors)
      motor.tare_position();

    // Move to high flag position
    driveControl->runLeftMotors(speed);
    driveControl->runRightMotors(speed);
    while (frontRightDrive->get_position() < 50) pros::delay(1);
    driveControl->runLeftMotors(0);
    driveControl->runRightMotors(0);
    pros::delay(400);

    // Move to mid flag position
    driveControl->runLeftMotors(speed);
    driveControl->runRightMotors(speed);
    while (frontRightDrive->get_position() < 270) pros::delay(1);
    driveControl->runLeftMotors(0);
    driveControl->runRightMotors(0);
    pros::delay(1000);
  } else if (selectedAutonomous == 1) {
    frontLauncherMotor->move(127);
    backLauncherMotor->move(127);
    pros::delay(2000);
    intakeMotor->move(127);
    pros::delay(3000);
    frontLauncherMotor->move(0);
    backLauncherMotor->move(0);
    driveControl->runLeftMotors(127);
    driveControl->runRightMotors(127);
    pros::delay(1500);
    driveControl->runLeftMotors(-127);
    driveControl->runRightMotors(-127);
    pros::delay(1750);
    driveControl->runLeftMotors(127);
    driveControl->runRightMotors(-127);
    pros::delay(250);
    driveControl->runLeftMotors(-127);
    driveControl->runRightMotors(-127);
    pros::delay(3000);
  } else if (selectedAutonomous == 2) {
    frontLauncherMotor->move(127);
    backLauncherMotor->move(127);
    pros::delay(2000);
    intakeMotor->move(127);
    pros::delay(3000);
    frontLauncherMotor->move(0);
    backLauncherMotor->move(0);
    driveControl->runLeftMotors(127);
    driveControl->runRightMotors(110);
    pros::delay(1250);

    for (const auto & motor : driveControl->rightMotors)
      motor.tare_position();
    for (const auto & motor : driveControl->leftMotors)
      motor.tare_position();

    driveControl->runLeftMotors(-127);
    driveControl->runRightMotors(-127);
    while (frontRightDrive->get_position() > -1500) pros::delay(1);

    for (const auto & motor : driveControl->rightMotors)
      motor.tare_position();
    for (const auto & motor : driveControl->leftMotors)
      motor.tare_position();

    driveControl->runLeftMotors(-127);
    driveControl->runRightMotors(127);

    while (frontRightDrive->get_position() < 150) pros::delay(1);

    driveControl->runLeftMotors(127);
    driveControl->runRightMotors(127);

    pros::delay(3000);
  }
  */
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
  LCD::setStatus("Operator Control");

  Logger::log(LOG_ERROR, "test!");

  liftMotor->set_brake_mode(BRAKE_BRAKE);

  bool controllerDC = false;
  int cycle = 0;

	while (true) {

    if(controllerMain->get_digital(BUTTON_X))
    {
      autonomous();
      LCD::setStatus("Returning to Operator Control");
    }

    drive->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true);

    if (controllerMain->get_digital(BUTTON_R2)) {
      frontLauncherMotor->move(-127);
      backLauncherMotor->move(-127);
      intakeMotor->move(-127);
    } else {
  		if (controllerMain->get_digital(BUTTON_R1))
        intakeMotor->move(127);
      else
        intakeMotor->move(0);

      if (controllerMain->get_digital(BUTTON_L2)) {
        frontLauncherMotor->move(127);
        backLauncherMotor->move(127);
      } else if (controllerMain->get_digital(BUTTON_L1)) {
        frontLauncherMotor->move(110);
        backLauncherMotor->move(110);
      } else {
        frontLauncherMotor->move(0);
        backLauncherMotor->move(0);
      }
    }
    liftMotor->move(controllerMain->get_analog(ANALOG_RIGHT_Y));

    if (!controllerMain->is_connected() && !controllerDC) {
      LCD::setStatus("Operator Controller Disconnected");
      controllerDC = true;
    } else if (controllerMain->is_connected() && controllerDC) {
      LCD::setStatus("Operator Controller Reconnected");
      controllerDC = false;
    }

    LCD::setText(3, "Left avg: " + std::to_string((frontLeftDrive->get_position() + backLeftDrive->get_position()) / 2));
    LCD::setText(4, "Right avg: " + std::to_string((frontRightDrive->get_position() + backRightDrive->get_position()) / 2));

    /* pid dynamic
    if (controllerMain->get_digital(BUTTON_B)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifykd(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifykd(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifykd(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifykd(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_X)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifykp(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifykp(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifykp(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifykp(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_Y)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifyki(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifyki(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifyki(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifyki(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_A)) {
      driveControl->moveRelative(0, 1000, 10, true, true);
    }

    pros::lcd::set_text(1, "kp = " + std::to_string(driveControl->getPID().getkp()));
    pros::lcd::set_text(2, "ki = " + std::to_string(driveControl->getPID().getki()));
    pros::lcd::set_text(3, "kd = " + std::to_string(driveControl->getPID().getkd()));
    */

    // flywheel dynamic
    //if (controllerMain->get_digital(BUTTON_UP)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity + 1);
    //if (controllerMain->get_digital(BUTTON_DOWN)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity - 1);

    /* Overheat lcd
    std::string hiheat = "";
    std::string overheat = "";

    if (backLeftDrive->get_temperature() > 55) overheat += "bldrive ";
    else if (backLeftDrive->get_temperature() > 45) hiheat += "bldrive ";

    if (backRightDrive->get_temperature() > 55) overheat += "brdrive ";
    else if (backLeftDrive->get_temperature() > 45) hiheat += "brdrive ";

    if (frontLeftDrive->get_temperature() > 55) overheat += "fldrive ";
    else if (frontLeftDrive->get_temperature() > 45) hiheat += "fldrive ";

    if (frontRightDrive->get_temperature() > 55) overheat += "frdrive ";
    else if (frontRightDrive->get_temperature() > 45) hiheat += "frdrive ";

    if (liftMotor->get_temperature() > 55) overheat += "lift ";
    else if (liftMotor->get_temperature() > 45) hiheat += "lift ";

    if (intakeMotor->get_temperature() > 55) overheat += "intake ";
    else if (intakeMotor->get_temperature() > 45) hiheat += "intake ";

    if (frontLauncherMotor->get_temperature() > 55) overheat += "frontfw ";
    else if (frontLauncherMotor->get_temperature() > 45) hiheat += "frontfw ";

    if (backLauncherMotor->get_temperature() > 55) overheat += "backfw ";
    else if (backLauncherMotor->get_temperature() > 45) hiheat += "backfw ";

    if (overheat != "") pros::lcd::set_text(5, overheat + " ovrht");
    if (hiheat != "") pros::lcd::set_text(6, hiheat + " hiheat");
    */
    if (controllerMain->get_digital(BUTTON_UP)) drive->turn(true, -90);
    if (controllerMain->get_digital(BUTTON_RIGHT)) drive->turn(true, 90);
    if (controllerMain->get_digital(BUTTON_DOWN)) drive->pivot(90);
    if (controllerMain->get_digital(BUTTON_LEFT)) drive->pivot(-90);
    if (controllerMain->get_digital(BUTTON_B)) drive->move(1000);
    if (controllerMain->get_digital(BUTTON_A)) drive->move(-1000);
    if (controllerMain->get_digital(BUTTON_X)) autonomous();

		pros::delay(20);

    if (cycle == 20) {
      cycle = 0;
      controllerMain->set_text(0, 0, ("Flywheel RPM: " + std::to_string(frontLauncherMotor->get_actual_velocity() * 21)).c_str());
    } else cycle++;

	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  LCD::setStatus("Disabled");
}
