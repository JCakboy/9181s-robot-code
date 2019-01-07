#include "main.h"
#include <cmath>

Unused::Unused() {}
Unused::Unused(int i) {}

namespace ports {
  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MAIN);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Batteries
  BrainBattery * brainBattery;
  ControllerBattery * controllerMainBattery;
  ControllerBattery * controllerPartnerBattery;

  // Ports
  Unused * port1 = new Unused(1);
  Unused * port2 = new Unused(2);
  Unused * port3 = new Unused(3);
  Unused * port4 = new Unused(4);
  Unused * port5 = new Unused(5);
  Unused * port6 = new Unused(6);
  Unused * port7 = new Unused(7);
  Unused * port8 = new Unused(8);
  Unused * port9 = new Unused(9);
  Unused * port10 = new Unused(10);
  pros::Motor * port11 = new pros::Motor(11, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port12 = new pros::Motor(12, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port13 = new pros::Motor(13, GEARSET_200, FWD, ENCODER_DEGREES);
  Unused * port14 = new Unused(14);
  Unused * port15 = new Unused(15);
  Unused * port16 = new Unused(16);
  pros::Motor * port17 = new pros::Motor(17, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port18 = new pros::Motor(18, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port19 = new pros::Motor(19, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port20 = new pros::Motor(20, GEARSET_200, FWD, ENCODER_DEGREES);
  Unused * port21 = new Unused(21);

  // Mapping
  pros::Motor * frontLeftDrive = ports::port11;
  pros::Motor * frontRightDrive = ports::port12;
  pros::Motor * intake = ports::port13;
  pros::Motor * puncherVariable = ports::port17;
  pros::Motor * puncherMotor = ports::port18;
  pros::Motor * backRightDrive = ports::port19;
  pros::Motor * backLeftDrive = ports::port20;

  // Mutexes
  pros::Mutex * driveLock = new pros::Mutex();
  pros::Mutex * launcherLock = new pros::Mutex();
  pros::Mutex * intakeLock = new pros::Mutex();
  pros::Mutex * liftLock = new pros::Mutex();

  // Driving
  DriveControl * driveControl = new DriveControl(*ports::driveLock, *ports::frontLeftDrive, *ports::backLeftDrive, *ports::frontRightDrive, *ports::backRightDrive);
  DriveFunction * drive = new DriveFunction(ports::driveControl);

  // Puncher
  Puncher * puncher = new Puncher(launcherLock, ports::puncherMotor);

  void init() {
    // Set the PID values
    driveControl->setPID(20, 0.54, 0.000000, 0.000000, false, 127, 10000, 200, MOTOR_MOVE_RELATIVE_THRESHOLD, 20, 50);
    // Sets the gear ratio of drive
    drive->setGearRatio(1, 1, 4);
    // Sets the turn values of drive
    drive->setTurnValues(501, 50);
  }
}

// This file uses many of the fields above. Dump it into the global namespace for ease of programming
using namespace ports;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Initialize the ports
  ports::init();
  // Initialize the LCD
  LCD::initialize();
  // Initialize all the loggers that log to the microSD card
  Logger::initializeDefaultLoggers();
  // Initialize the USB debugger
  Debugger::start();

  // Simple header to signal the start of a new program in a log file
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

// The currently selected autonomous, has external linkage (meaning its global throughout the project)
int selectedAutonomous = 0;
// Whether the autonomous was completed successfully. Used to determine whether the autonomous routine takes too long to complete
bool autonomousComplete = true;
void autonomous() {
  autonomousComplete = false;
  // Log the start of autonomous to signify in a log file the location of the log
  Logger::log(LOG_INFO, "--- Autonomous ---");
  LCD::setStatus("Autonomous");
  // No autonomous
  puncher->prime();
  autonomousComplete = true;
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

 // Whether the operator control loop should run, has external linkage
bool runOperatorControlLoop = true;
void opcontrol() {
  // Log the start of operator control to signify in a log file the location of the log
  Logger::log(LOG_INFO, "--- Operator Control Task ---");
  LCD::setStatus("Operator Control");

  if (!autonomousComplete) {
    Logger::log(LOG_WARNING, "Autonomous was not completed successfully!");
    autonomousComplete = true;
  }

  // Flag to set when the main controller has disconnected
  bool controllerDC = false;

  // Puncher should hold its angle
  puncherVariable->set_brake_mode(BRAKE_BRAKE);

  start:

  // The operator control loop, code here is executed every 20ms when runOperatorControlLoop is set
	while (runOperatorControlLoop) {

    // Run driving code, this function handles all of the math to do with it. Should never be changed. For motor changes, go to ports::init()
    drive->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true);
    // Run puncher code. Should never be changed, unless a puncher is no longer desired
    puncher->run();

    // Map the right analog stick to the puncherVariable motor to change the puncher angle
    puncherVariable->move(controllerMain->get_analog(STICK_RIGHT_Y) / 127.0 * 50);

    // Intake code, R1 intakes balls, R2 outtakes balls and flips ground caps
    if (controllerMain->get_digital(BUTTON_R1))
      intake->move(127);
    else if (controllerMain->get_digital(BUTTON_R2))
      intake->move(-127);
    else
      intake->move(0);

    // Runs the high and mid flag routine when L1 and L2 is pressed, respectively
    if (controllerMain->get_digital_new_press(BUTTON_L1)) {
      Logger::log(LOG_INFO, "High routine");
      puncherVariable->move(puncher->primed() ? -127 : -75);
      puncher->move(360);
      pros::delay(525);
      puncherVariable->move(0);
    } else if (controllerMain->get_digital_new_press(BUTTON_L2)) {
      Logger::log(LOG_INFO, "Mid routine");
      puncherVariable->move(puncher->primed() ? 127 : 75);
      puncher->move(360);
      pros::delay(525);
      puncherVariable->move(0);
    }

    // Buttons A, X and B deal with puncher priming. See Puncher for documentation
    if (controllerMain->get_digital_new_press(BUTTON_A))
      puncher->togglePrime();

    if (controllerMain->get_digital_new_press(BUTTON_X))
      puncher->prime();

    if (controllerMain->get_digital_new_press(BUTTON_B))
      puncher->unprime();

    // Runs simple checks on whether the main controller is disconnected, utilizing controllerDC
    if (!controllerMain->is_connected() && !controllerDC) {
      LCD::setStatus("Operator Controller Disconnected");
      Logger::log(LOG_ERROR, "Operator Controller has been disconnected!");
      controllerDC = true;
    } else if (controllerMain->is_connected() && controllerDC) {
      LCD::setStatus("Operator Controller Reconnected");
      Logger::log(LOG_INFO, "Operator Controller has been reconnected!");
      controllerDC = false;
    }

    /* Debugging code: displays drive and puncher motor values to the LCD screen
     * DEPRECATED: use USB debugging to query the motor positions
    LCD::setText(3, "Left front: " + std::to_string((frontLeftDrive->get_position())));
    LCD::setText(4, "Left back: " + std::to_string((backLeftDrive->get_position())));
    LCD::setText(5, "Right front: " + std::to_string((frontRightDrive->get_position())));
    LCD::setText(6, "Right back: " + std::to_string((backRightDrive->get_position())));
    LCD::setText(2, "Puncher: P" + std::to_string((puncher->primed)) + " B" + std::to_string(puncherMotor->get_brake_mode() == BRAKE_BRAKE));
    */

    // Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

    // Run every 20ms
		pros::delay(20);
	}

  // Code loop when the operator control loop is paused. Runs
  while (!runOperatorControlLoop) {

    // Runs simple checks on whether the main controller is disconnected, utilizing controllerDC
    if (!controllerMain->is_connected() && !controllerDC) {
      Logger::log(LOG_ERROR, "Operator Controller has been disconnected!");
      controllerDC = true;
    } else if (controllerMain->is_connected() && controllerDC) {
      Logger::log(LOG_INFO, "Operator Controller has been reconnected!");
      controllerDC = false;
    }

    // Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

    // Run every 20ms
    pros::delay(20);
  }

  // Jump back to the operator control loop when re-enabled
  goto start;
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // Log the start of disablement to signify in a log file the location of the log
  Logger::log(LOG_INFO, "--- Disabled ---");
  LCD::setStatus("Disabled");
  if (!autonomousComplete) {
    Logger::log(LOG_WARNING, "Autonomous was not completed successfully!");
    autonomousComplete = true;
  }
}
