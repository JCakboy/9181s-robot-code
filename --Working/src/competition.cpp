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
  ports::drive = new DriveFunction(ports::driveControl);
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
  drive->move(1350);

  LCD::setStatus("Auto Step 2");
  takeBall(true);

  LCD::setStatus("Auto Step 3");
  drive->move(-1100);

  LCD::setStatus("Auto Step 4");
  turn90(clockwise);

  LCD::setStatus("Auto Step 5");
  drive->move(400);
  shoot();
  drive->move(1200);


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

  Profiles::run();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  LCD::setStatus("Disabled");
}
