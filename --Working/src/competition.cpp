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

    ports::port1 = new Unused(1);
    ports::port2 = new Unused(2);
    ports::port3 = new Unused(3);
    ports::port4 = new Unused(4);
    ports::port5 = new Unused(5);
    ports::port6 = new Unused(6);
    ports::port7 = new Unused(7);
    ports::port8 = new Unused(8);
    ports::port9 = new Unused(9);
    ports::port10 = new Unused(10);
    ports::frontLeftDrive = new pros::Motor(11, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::frontRightDrive = new pros::Motor(12, GEARSET_200, REV, ENCODER_DEGREES);
    ports::intake = new pros::Motor(13, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::port14 = new Unused(14);
    ports::port15 = new Unused(15);
    ports::port16 = new Unused(16);
    ports::puncherVariable = new pros::Motor(17, GEARSET_200, REV, ENCODER_DEGREES);
    ports::puncherMotor = new pros::Motor(18, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::backRightDrive = new pros::Motor(19, GEARSET_200, REV, ENCODER_DEGREES);
    ports::backLeftDrive = new pros::Motor(20, GEARSET_200, FWD, ENCODER_DEGREES);
    ports::port21 = new Unused(21);

    ports::driveLock = new pros::Mutex();
    ports::launcherLock = new pros::Mutex();
    ports::intakeLock = new pros::Mutex();
    ports::liftLock = new pros::Mutex();

    ports::driveControl = new DriveControl(*ports::driveLock, *ports::frontLeftDrive, *ports::frontRightDrive);
    // ports::driveControl = new DriveControl(*ports::driveLock, *ports::frontLeftDrive, *ports::backLeftDrive, *ports::frontRightDrive, *ports::backRightDrive);
    ports::drive = new DriveFunction(ports::driveControl);

    ports::puncher = new Puncher(launcherLock, ports::puncherMotor);

    driveControl->setPID(20, 0.54, 0.000000, 0.000000, false, 127, 10000, 200, MOTOR_MOVE_RELATIVE_THRESHOLD, 20, 50);
    drive->setGearRatio(1, 1, 4);
    drive->setTurnValues(501, 50);
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
  Logger::initializeDefaultLoggers();

  Logger::log(LOG_INFO, "#########################");
  Logger::log(LOG_INFO, "#     PROGRAM START     #");
  Logger::log(LOG_INFO, "#########################");
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
  // No autonomous
  puncher->prime();
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

  bool controllerDC = false;

  puncherVariable->set_brake_mode(BRAKE_BRAKE);

	while (true) {

    drive->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true);
    puncher->run();

    puncherVariable->move(controllerMain->get_analog(STICK_RIGHT_Y) / 127.0 * 50);

    if (controllerMain->get_digital(BUTTON_R1))
      intake->move(127);
    else if (controllerMain->get_digital(BUTTON_R2))
      intake->move(-127);
    else
      intake->move(0);

    if (controllerMain->get_digital_new_press(BUTTON_L1)) {
      Logger::log(LOG_INFO, "High routine");
      puncherVariable->move(-75);
      puncher->move(360);
      pros::delay(525);
      puncherVariable->move(0);
    } else if (controllerMain->get_digital_new_press(BUTTON_L2)) {
      Logger::log(LOG_INFO, "Mid routine");
      puncherVariable->move(75);
      puncher->move(360);
      pros::delay(525);
      puncherVariable->move(0);
    }

    if (controllerMain->get_digital_new_press(BUTTON_A))
      puncher->togglePrime();

    if (controllerMain->get_digital_new_press(BUTTON_X))
      puncher->prime();

    if (controllerMain->get_digital_new_press(BUTTON_B))
      puncher->unprime();

    if (!controllerMain->is_connected() && !controllerDC) {
      LCD::setStatus("Operator Controller Disconnected");
      controllerDC = true;
    } else if (controllerMain->is_connected() && controllerDC) {
      LCD::setStatus("Operator Controller Reconnected");
      controllerDC = false;
    }
/*
    LCD::setText(3, "Left front: " + std::to_string((frontLeftDrive->get_position())));
    LCD::setText(4, "Left back: " + std::to_string((backLeftDrive->get_position())));
    LCD::setText(5, "Right front: " + std::to_string((frontRightDrive->get_position())));
    LCD::setText(6, "Right back: " + std::to_string((backRightDrive->get_position())));
    LCD::setText(2, "Puncher: P" + std::to_string((puncher->primed)) + " B" + std::to_string(puncherMotor->get_brake_mode() == BRAKE_BRAKE));
*/    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

    if (selectedAutonomous == 1) {
      drive->pivot(90);
      selectedAutonomous = 0;
    } else if (selectedAutonomous == 2) {
      drive->move(36);
      selectedAutonomous = 0;
    } else if (selectedAutonomous == 3) {
      drive->pivot(180);
      selectedAutonomous = 0;
    }

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
