#include "main.h"
#include <cmath>

Unused::Unused() {}
Unused::Unused(int i) {}

void ports::init() {
  ports::controllerMain = new pros::Controller(CONTROLLER_MAIN);
  ports::controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  ports::brainBattery = new BrainBattery();
  ports::controllerMainBattery = new ControllerBattery(*(ports::controllerMain));
  // ports::controllerPartnerBattery = new ControllerBattery(*(ports::controllerPartner));

  ports::port1 = new Unused();
  ports::port2 = new Unused();
  ports::port3 = new Unused();
  ports::port4 = new Unused();
  ports::port5 = new Unused();
  ports::port6 = new Unused();
  ports::port7 = new Unused();
  ports::port8 = new Unused();
  ports::port9 = new Unused();
  ports::port10 = new Unused();
  ports::backLeftDrive = new pros::Motor(11, GEARSET_200, FWD, ENCODER_DEGREES);
  ports::frontLeftDrive = new pros::Motor(12, GEARSET_200, FWD, ENCODER_DEGREES);
  ports::intakeMotor = new pros::Motor(13, GEARSET_200, REV, ENCODER_DEGREES);
  ports::frontLauncherMotor = new pros::Motor(14, GEARSET_200, REV, ENCODER_DEGREES);
  ports::backLauncherMotor = new pros::Motor(15, GEARSET_200, FWD, ENCODER_DEGREES);
  ports::port16 = new Unused();
  ports::port17 = new Unused();
  ports::liftMotor = new pros::Motor(18, GEARSET_200, FWD, ENCODER_DEGREES);
  ports::frontRightDrive = new pros::Motor(19, GEARSET_200, REV, ENCODER_DEGREES);
  ports::backRightDrive = new pros::Motor(20, GEARSET_200, REV, ENCODER_DEGREES);
  ports::port21 = new Unused();

  ports::driveLock = new pros::Mutex();
  ports::launcherLock = new pros::Mutex();
  ports::intakeLock = new pros::Mutex();
  ports::liftLock = new pros::Mutex();

  ports::driveControl = new DriveControl(*ports::driveLock, *ports::frontLeftDrive, *ports::backLeftDrive, *ports::frontRightDrive, *ports::backRightDrive);
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
 void drive(int degrees)
 {
   driveControl->moveRelative(0, degrees, 10, true, true);
 }

 void takeBall(bool stopMotor)
 {
   intakeMotor->move(127);
   pros::delay(800);
   if(stopMotor)
   {
     intakeMotor->move(0);
   }
 }

 void shoot()
 {
   frontLauncherMotor->move(127);
   backLauncherMotor->move(127);
   pros::delay(4000);
   intakeMotor->move(127);
   pros::delay(500);
   frontLauncherMotor->move(0);
   backLauncherMotor->move(0);
   intakeMotor->move(0);
 }

 void turn90(bool isClockwise)
 {
   int amount = 400;
   int power = (isClockwise) ? 100 : -100;
   frontRightDrive->tare_position();
   frontLeftDrive->tare_position();
   backRightDrive->tare_position();
   backLeftDrive->tare_position();

   while(std::abs(frontRightDrive->get_position()) < amount)
   {
     frontRightDrive->move(-power);
     frontLeftDrive->move(power);
     backRightDrive->move(-power);
     backLeftDrive->move(power);
   }
 }

int selectedAutonomous = 0;
void autonomous()
{

  driveControl->clearPID();

  bool clockwise = (selectedAutonomous == -1) ? false : true;

  LCD::setStatus("Auto Step 1");
  drive(1350);
  LCD::setStatus("Auto Step 2");
  takeBall(true);
  LCD::setStatus("Auto Step 3");
  drive(-1100);
  LCD::setStatus("Auto Step 4");
  turn90(clockwise);
  LCD::setStatus("Auto Step 5");
  drive(400);
  shoot();
  drive(1200);


  /*pros::lcd::set_text(1, "step 6");
  turn90(true);
  pros::lcd::set_text(1, "step 6.5");
  drive(1830);
/*  pros::lcd::set_text(1, "step 7");
  turn90(false);
  pros::lcd::set_text(1, "step 8");
  drive(1000);*/
  LCD::setStatus("Auto Complete");

  /*
  pros::lcd::set_text(1, "step 1");
  drive(100, -75);
  pros::lcd::set_text(1, "step 2");
  shoot();
  pros::lcd::set_text(1, "step 3");
  drive(587, -100);
  pros::lcd::set_text(1, "step 4");
  turn90(false);
  pros::lcd::set_text(1, "step 5");
  drive(1000, 127);
  pros::lcd::set_text(1, "auto done");
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

  int currentFlywheelVelocity = 0;

  bool controllerDC = false;

	while (true) {

    if(controllerMain->get_digital(BUTTON_X))
    {
      autonomous();
      LCD::setStatus("Returning to Operator Control");
    }
    driveControl->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true);

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

    //if (overheat != "") pros::lcd::set_text(5, overheat + " ovrht");
    //if (hiheat != "") pros::lcd::set_text(6, hiheat + " hiheat");

		pros::delay(20);
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
