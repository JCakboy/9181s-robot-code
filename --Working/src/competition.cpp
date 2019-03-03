#include "main.h"
#include <cmath>

void ports::init() {
  /*
   * Include temporary port changes here
   * Be sure to delete the existing pointer to the port before reinitializing it
   */

  // If there are temporary port changes, driveControl may need to be reinitialized
  // driveControl.reinitialize(ports::frontLeftDrive, ports::backLeftDrive, ports::frontRightDrive, ports::backRightDrive);

  // If there are temporary port changes, puncher may need to be reinitialized
  // puncher.reinitialize(ports::puncherMotor);

  // Driving sensitivity
  ::sensitivity = 1.0;
  ::adjustingSensitivity = 0.45;

  // Individual PID values
  PID * frontLeftPID = new PID(20, 0.44000, 0.00000, -0.25000, true, 110, 11, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 7, 7);
  PID * frontRightPID = new PID(20, 0.40000, 0.00000, 7.50000, true, 110, 8.7, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 7, 7);
  PID * backLeftPID = new PID(20, 0.44000, 0.00000, -0.25000, true, 110, 11, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 7, 7);
  PID * backRightPID = new PID(20, 0.40000, 0.00000, 7.50000, true, 111, 8.7, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 7, 7);
  // Set the PID values
  driveControl->setPID(frontLeftPID, backLeftPID, frontRightPID, backRightPID);
  // Sets the gear ratio of drive
  drive->setGearRatio(1, 1, 4);
  // Sets the turn values of drive
  drive->setTurnValues(852, 54);
}

// This file uses many of the fields in the ports namespace. Dump it into the global namespace for ease of programming
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
  // Initialize the LCD of the brain and the controllers
  LCD::initialize(controllerMain, controllerPartner);
  // Initialize all the loggers that log to the microSD card
  Logger::initializeDefaultLoggers();
  // Initialize the USB debugger
  Debugger::start(driveControl, drive);

  // Simple header to signal the start of a new program in a log file
  Logger::log(LOG_INFO, "#####################################");
  Logger::log(LOG_INFO, "#                                   #");
  Logger::log(LOG_INFO, "#           PROGRAM START           #");
  Logger::log(LOG_INFO, "#                                   #");
  Logger::log(LOG_INFO, "#####################################");
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
  autonomousComplete = false;
  /*
  if (selectedAutonomous == 1) { // Red flags
  } else if (selectedAutonomous == 2) { // Red far
  } else if (selectedAutonomous == 3) { // Blue flags
  } else if (selectedAutonomous == 4) { // Blue far
  } else if (selectedAutonomous == 5) { // Skills
  } else */
  if (selectedAutonomous == 0)
    Logger::log(LOG_WARNING, "No autonomous was selected but was called");
  else
    Logger::log(LOG_ERROR, "Selected autonomous (" + std::to_string(selectedAutonomous) + ") is not valid! No autonomous will run");

  end:
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
void opcontrol() {
  // Log the start of operator control to signify in a log file the location of the log
  Logger::log(LOG_INFO, "---===( Operator Control )===---");
  LCD::setStatus("Operator Control");

  if (!autonomousComplete) {
    Logger::log(LOG_WARNING, "Autonomous was not completed successfully!");
    autonomousComplete = true;
  }

  // Flag to set when the main controller has disconnected
  bool controllerDC = false;

  start:

  // The operator control loop, code here is executed every 20ms when runOperatorControlLoop is set
  while (runOperatorControlLoop) {

    // Runs the debugger and executes any required methods
    Debugger::run();
    // Updates the LCD on every cycle
    LCD::updateScreen();

    // Run driving code, this function handles all of the math to do with it. Should never be changed. For motor changes, go to ports::init()
    drive->runH(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), true, true, 1.0, 1.0);

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

    // Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();
    // Calls autonomous
    if (controllerMain->get_digital_new_press(BUTTON_UP)) autonomous();

    // Run every 20ms
    pros::delay(20);
  }

  // Code loop when the operator control loop is paused. Runs
  while (!runOperatorControlLoop) {

    // Runs the debugger and executes any required methods
    Debugger::run();
    // Updates the LCD on every cycle
    LCD::updateScreen();

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
  Logger::log(LOG_INFO, "---===( Disabled )===---");
  LCD::setStatus("Disabled");
  if (!autonomousComplete) {
    Logger::log(LOG_WARNING, "Autonomous was not completed successfully!");
    autonomousComplete = true;
  }
  while (true) {
    // Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
    if (controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
    if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

    // Run every 20ms
    pros::delay(20);
  }
}
