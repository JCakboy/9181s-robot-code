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
  PID * frontLeftPID = new PID(20, 0.43000, 0.00000, 3.30000, true, 127, 19, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 12, 12);
  PID * frontRightPID = new PID(20, 0.43000, 0.00000, 3.30000, true, 127, 16, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 12, 12);
  PID * backLeftPID = new PID(20, 0.43000, 0.00000, 3.30000, true, 127, 19, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 12, 12);
  PID * backRightPID = new PID(20, 0.43000, 0.00000, 3.30000, true, 127, 16, 10000, 200, true, MOTOR_MOVE_RELATIVE_THRESHOLD, 12, 12);
  // Set the PID values
  driveControl->setPID(frontLeftPID, backLeftPID, frontRightPID, backRightPID);
  // Sets the gear ratio of drive
  drive->setGearRatio(1, 1, 4);
  // Sets the turn values of drive
  drive->setTurnValues(852, 54);
  // Sets the vision alignment system
  puncher->setAligner(true, driveControl, flagVision, false);
  // Limit the current of the variable puncher motor to reduce clicking
  // Torque is directly proportional to current, so with it limited, the motor can only output a limited torque, reducing the liklihood for forced gear slipping
  puncherVariable->set_current_limit(800);
  // Limit torque on drive motors to ensure straight driving
  backLeftDrive->set_current_limit(2500);
  frontLeftDrive->set_current_limit(2500);
  backRightDrive->set_current_limit(2500);
  frontRightDrive->set_current_limit(2500);
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

void highRoutine() {
  Logger::log(LOG_INFO, "Puncher High Routine");
  // Snap the puncher to the position (with limited torque)
  puncherVariable->move(puncher->primed() ? -127 : -127);
  pros::delay(200);
  // Punch
  puncher->move(360);
  // Wait until the punch is complete
  int millis = util::sign(pros::millis());
  while (std::lround(puncher->motor->get_position()) % 360 < 350 && util::sign(pros::millis()) - millis < 900)
    pros::delay(1);
  // Wait before allowing drive movement
  pros::delay(30);
  puncherVariable->move(0);
}

void midRoutine() {
  Logger::log(LOG_INFO, "Puncher Mid Routine");
  // Snap the puncher to the position (with limited torque)
  puncherVariable->move(puncher->primed() ? 127 : 127);
  pros::delay(200);
  // Punch
  puncher->move(360);
  // Wait until the punch is complete
  int millis = util::sign(pros::millis());
  while (std::lround(puncher->motor->get_position()) % 360 < 350 && util::sign(pros::millis()) - millis < 900)
    pros::delay(1);
  // Wait before allowing drive movement
  pros::delay(30);
  puncherVariable->move(0);
}


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

  //if (selectedAutonomous == 1) { // Red flags
    // Reset the arm
    arm->move(-127);
    // // Drive forward and intake the ball
    // intake->move(127);
    // pros::delay(50);
    // drive->move(46);
    // arm->tare_position();
    // intake->move(0);
    // pros::delay(450);
    // arm->move_absolute(365, 75);
    // // Reset and prime the puncher
    // drive->move(-51);
    // puncher->prime();
    // drive->move(11);
    // // Turn to face the flags
    // drive->pivot(-88);
    // drive->move(-5);
    // // Shoot the balls
    // arm->move_absolute(365, 100);
    highRoutine();
    intake->move(127);
    pros::delay(750);
    midRoutine();
    puncher->unprime();
    arm->tare_position();
    // Drive forward and toggle the low flag
    // drive->pivot(-7);
    intake->move(90);
    drive->move(52);
    // Get in position for next routine
    drive->move(-27);
    intake->move(0);
    drive->pivot(90);
    drive->move(-12);
    // Drive forward and toggle the cap
    intake->move(-120);
    drive->move(35);
    // Attempt the middle flag
    drive->pivot(-45);
    midRoutine();
    pros::delay(400);
    drive->pivot(-10);
    // Ram into the low flag
    drive->move(37);
    drive->pivot(-30);
  // } else if (selectedAutonomous == 2) { // Red far
  //   // Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(90);
  //   pros::delay(50);
  //   drive->move(45);
  //   arm->move(0);
  //   arm->tare_position();
  //   intake->move(0);
  //   pros::delay(250);
  //   drive->move(-3);
  //   // Turn and shoot the balls
  //   arm->move_absolute(296, 100);
  //   drive->pivot(-80);
  //   highRoutine();
  //   intake->move(127);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   intake->move(0);
  //   // Tip the cap
  //   arm->move_absolute(0, 100);
  //   drive->pivot(-22);
  //   drive->move(-11);
  //   arm->move_absolute(240, 100);
  //   pros::delay(600);
  //   arm->move_absolute(0, 100);
  //   // Park
  //   drive->pivot(-70);
  //   drive->move(7);
  //   drive->pivot(90);
  //   intake->move(127);
  //   drive->move(50);
  // } else if (selectedAutonomous == 3) { // Blue flags
  //   // Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(90);
  //   pros::delay(50);
  //   drive->move(46);
  //   arm->tare_position();
  //   intake->move(0);
  //   pros::delay(450);
  //   arm->move_absolute(365, 75);
  //   // Reset and prime the puncher
  //   drive->move(-53);
  //   puncher->prime();
  //   drive->move(11);
  //   // Turn to face the flags
  //   drive->pivot(88);
  //   drive->move(-3);
  //   // Shoot the balls
  //   arm->move_absolute(365, 100);
  //   highRoutine();
  //   intake->move(127);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   // Drive forward and toggle the low flag
  //   drive->pivot(5);
  //   intake->move(90);
  //   drive->move(52);
  //   // Get in position for next routine
  //   drive->move(-27);
  //   intake->move(0);
  //   drive->pivot(-90);
  //   drive->move(-12);
  //   // Drive forward and toggle the cap
  //   intake->move(-80);
  //   drive->move(35);
  //   // Attempt the middle flag
  //   drive->pivot(45);
  //   midRoutine();
  //   pros::delay(400);
  //   drive->pivot(10);
  //   // Ram into the low flag
  //   drive->move(37);
  // } else if (selectedAutonomous == 4) { // Blue far
  //   // Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(90);
  //   pros::delay(50);
  //   drive->move(45);
  //   arm->move(0);
  //   arm->tare_position();
  //   intake->move(0);
  //   pros::delay(250);
  //   drive->move(-3);
  //   // Turn and shoot the balls
  //   arm->move_absolute(296, 100);
  //   drive->pivot(80);
  //   highRoutine();
  //   intake->move(127);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   intake->move(0);
  //   // Tip the cap
  //   arm->move_absolute(0, 100);
  //   drive->pivot(22);
  //   drive->move(-12);
  //   arm->move_absolute(240, 100);
  //   pros::delay(600);
  //   arm->move_absolute(0, 100);
  //   // Park
  //   drive->pivot(70);
  //   drive->move(6);
  //   drive->pivot(-90);
  //   intake->move(127);
  //   drive->move(50);
  // } else if (selectedAutonomous == 5) { // Skills
  //   // Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(100);
  //   pros::delay(50);
  //   drive->move(46);
  //   arm->tare_position();
  //   intake->move(0);
  //   pros::delay(450);
  //   arm->move_absolute(365, 75);
  //   // Reset and prime the puncher
  //   drive->move(-51);
  //   puncher->prime();
  //   drive->move(11);
  //   // Turn to face the flags
  //   drive->pivot(-88);
  //   drive->move(-5);
  //   // Shoot the balls
  //   arm->move_absolute(365, 100);
  //   highRoutine();
  //   intake->move(127);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   // Drive forward and toggle the low flag
  //   // drive->pivot(-7);
  //   intake->move(90);
  //   drive->move(52);
  //   // Get in position for next routine
  //   drive->move(-27);
  //   intake->move(0);
  //   drive->pivot(90);
  //   drive->move(-12);
  //   // Drive forward and toggle the cap
  //   intake->move(-120);
  //   drive->move(35);
  //   // Attempt the middle flag
  //   drive->pivot(-45);
  //   midRoutine();
  //   pros::delay(600);
  //   puncher->unprime();
  //   puncherMotor->move(0);
  //   // Drive back and park
  //   drive->pivot(45);
  //   drive->move(-41);
  //   drive->move(11);
  //   drive->pivot(90);
  //   intake->move(60);
  //   arm->move_absolute(600, 100);
  //   drive->move(47);
  //   drive->pivot(-90);
  //   drive->move(-18);
  //   arm->move_absolute(0, 50);
  //   drive->move(115);
  //   /*
  //     Red Far Autonomous
  //   */
  //   /*// Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(100);
  //   pros::delay(50);
  //   drive->move(45);
  //   arm->move(0);
  //   arm->tare_position();
  //   intake->move(0);
  //   pros::delay(250);
  //   drive->move(-3);
  //   // Turn and shoot the balls
  //   arm->move_absolute(296, 100);
  //   drive->pivot(-80);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   intake->move(0);
  //   // Tip the cap
  //   arm->move_absolute(0, 100);
  //   drive->pivot(-22);
  //   drive->move(-11);
  //   arm->move_absolute(240, 100);
  //   pros::delay(600);
  //   arm->move_absolute(0, 100);
  //   // Drive to reset
  //   drive->pivot(-70);
  //   intake->move(60);
  //   drive->move(58);
  //   drive->move(-10);
  //   // Get in position for next routine
  //   drive->pivot(90);
  //   arm->move_absolute(365, 100);
  //   drive->move(49);
  //   intake->move(0);
  //   drive->pivot(90);
  //   drive->move(-12);*/
  //   /*
  //     Red Flag Autonomous
  //   *//*
  //   // Reset the arm
  //   arm->move(-127);
  //   // Drive forward and intake the ball
  //   intake->move(90);
  //   pros::delay(50);
  //   drive->move(46);
  //   intake->move(0);
  //   pros::delay(450);
  //   arm->move_absolute(365, 75);
  //   // Reset and prime the puncher
  //   drive->move(-53);
  //   puncher->prime();
  //   drive->move(11);
  //   // Turn to face the flags
  //   drive->pivot(-88);
  //   drive->move(-3);
  //   // Shoot the balls
  //   arm->move_absolute(365, 100);
  //   highRoutine();
  //   intake->move(127);
  //   pros::delay(750);
  //   midRoutine();
  //   puncher->unprime();
  //   // Drive forward and toggle the low flag
  //   // drive->pivot(-7);
  //   intake->move(90);
  //   drive->move(52);
  //   // Get in position for next routine
  //   drive->move(-27);
  //   intake->move(0);
  //   drive->pivot(90);
  //   drive->move(-12);
  //   // Drive forward and toggle the cap
  //   intake->move(-80);
  //   drive->move(35);
  //   // Attempt the middle flag
  //   drive->pivot(-45);
  //   midRoutine();
  //   pros::delay(400);
  //   // Drive back and park
  //   drive->pivot(45);
  //   drive->move(-40);
  //   drive->move(11);
  //   drive->pivot(90);
  //   intake->move(60);
  //   drive->move(50);
  //   drive->pivot(-90);
  //   drive->move(-12);
  //   drive->move(100);
  //   */
  // } else if (selectedAutonomous == 0)
  //   Logger::log(LOG_WARNING, "No autonomous was selected but was called");
  // else
  //   Logger::log(LOG_ERROR, "Selected autonomous (" + std::to_string(selectedAutonomous) + ") is not valid! No autonomous will run");

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

  // Flag to set when arm lock was requested
  bool armLock = false;

  // Flags to set when the L1 and L2 buttons are pressed
  bool l1Pressed = false;
  bool l2Pressed = false;

  // Cache to store the puncher priming
  bool primeCacheSet = false;
  bool primeCache = false;

  // Puncher should hold its angle
  puncherVariable->set_brake_mode(BRAKE_BRAKE);
  // Brake the arm
  arm->set_brake_mode(BRAKE_HOLD);

  start:

  // The operator control loop, code here is executed every 20ms when runOperatorControlLoop is set
  while (runOperatorControlLoop) {

    // Runs the debugger and executes any required methods
    Debugger::run();
    // Updates the LCD on every cycle
    LCD::updateScreen();

    // Run driving code, this function handles all of the math to do with it. Should never be changed. For motor changes, go to ports::init()
    /* Standard driver code*/ // drive->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true, 1.0, 1.0);
    // Check for puncher adjustment or outtake and if so, lower the sensitivity
    // If B is pressed, align with the flags
    if (controllerMain->get_digital(BUTTON_B))
      puncher->align(false);
    else if (l1Pressed || l2Pressed)
      drive->runStrafe(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), controllerMain->get_digital(BUTTON_A), true, true, sensitivity * adjustingSensitivity, sensitivity * adjustingSensitivity);
    else if (controllerMain->get_digital(BUTTON_R2))
      drive->runStrafe(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), controllerMain->get_digital(BUTTON_A), true, true, sensitivity * (adjustingSensitivity + 0), sensitivity * (adjustingSensitivity + 0));
    else
      drive->runStrafe(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), controllerMain->get_digital(BUTTON_A), true, true, sensitivity, sensitivity);

    // Run puncher code. Should never be changed, unless a puncher is no longer desired
    puncher->run();

    // If the robot is punching, reset the arm to a horizontal position, otherwise, map the right analog stick to the scoring arm
    if ((controllerMain->get_analog(ANALOG_RIGHT_Y) == 0 && armLock) || l1Pressed || l2Pressed || puncher->primed())
      arm->move_absolute(296, 100);
    else {
      if (armLock) armLock = false;
      arm->move(controllerMain->get_analog(ANALOG_RIGHT_Y));
    }

    // Intake code, R1 intakes balls, R2 outtakes balls and flips ground caps
    if (controllerMain->get_digital(BUTTON_R1))
      intake->move(127);
    else if (controllerMain->get_digital(BUTTON_R2))
      intake->move(-95);
    else
      intake->move(0);

    // Code for mid flag puncher routine, two parts
    if (!l1Pressed && controllerMain->get_digital(BUTTON_L1)) {
      // When the button is held, prime the puncher and lower the drive sensitivity for easier adjustment
      l1Pressed = true;
      if (!primeCacheSet) {
        primeCache = puncher->primed();
        primeCacheSet = true;
      }
      puncher->prime();
    } else if (l1Pressed && !controllerMain->get_digital(BUTTON_L1)) {
      // When the button is released, punch the puncher and return the drive sensitivity to normal
      drive->stop();
      midRoutine();
      l1Pressed = false;
      // If the puncher was previously primed, keep it as is. Otherwise, return it to its neutral position
      if (controllerMain->get_digital(BUTTON_L2))
        puncher->prime();
      else if (primeCacheSet) {
        primeCacheSet = false;
        if (primeCache)
          puncher->prime();
        else
          puncher->unprime();
      } else
        puncher->unprime();
    }

    // Code for mid flag puncher routine, two parts
    if (!l2Pressed && controllerMain->get_digital(BUTTON_L2)) {
      // When the button is held, prime the puncher and lower the drive sensitivity for easier adjustment
      l2Pressed = true;
      if (!primeCacheSet) {
        primeCache = puncher->primed();
        primeCacheSet = true;
      }
      puncher->prime();
    } else if (l2Pressed && !controllerMain->get_digital(BUTTON_L2)) {
      // When the button is released, punch the puncher and return the drive sensitivity to normal
      drive->stop();
      highRoutine();
      l2Pressed = false;
      // If the puncher was previously primed, keep it as is. Otherwise, return it to its neutral position
      if (controllerMain->get_digital(BUTTON_L1))
        puncher->prime();
      else if (primeCacheSet) {
        primeCacheSet = false;
        if (primeCache)
          puncher->prime();
        else
          puncher->unprime();
      } else
        puncher->unprime();
    }

    // If X is pressed, toggle the prime status of the puncher
    if (controllerMain->get_digital_new_press(BUTTON_X))
      puncher->togglePrime();

    // Lock the arm if Y is pressed
    if (controllerMain->get_digital_new_press(BUTTON_Y))
      armLock = true;

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
    // if (controllerMain->get_digital_new_press(BUTTON_LEFT)) drive->move(40);
    // if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) drive->move(10);
    // Resets the arm motor if the down button is pressed
    if (controllerMain->get_digital_new_press(BUTTON_DOWN)) arm->tare_position();
    // Calls autonomous. TO BE REMOVED BEFORE TOURNAMENT
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
    // if (controllerMain->get_digital_new_press(BUTTON_LEFT)) drive->move(40);
    // if (controllerMain->get_digital_new_press(BUTTON_RIGHT)) drive->move(10);
    // Resets the arm motor if the down button is pressed
    if (controllerMain->get_digital_new_press(BUTTON_DOWN)) arm->tare_position();

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
