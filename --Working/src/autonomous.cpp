#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

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

// Powers the intake at a given power
void powerIntake(int power) {
  intakeMotorRight->move(power);
  intakeMotorLeft->move(power);
}

void cycle(int power) {
  flywheel->move(power);
  powerIntake(power);
  indexer->move(power);
}

void flipout() {
  indexer->move(127);
  pros::delay(400);
  indexer->move(0);
}

void waitForUltrasonic(double maxtime, double threshold = 150, bool n = false) { // threshold in cm
  int c = 0;
  while ((c * 0.05) < maxtime) {
    bool condition = intakeUltrasonic->get_value() < threshold;
    if (n)
      condition = !condition;
    if (condition)
      break;
    pros::delay(20);
    c += 1;
  }
}

void autonomousBlueRight() {

}

void autonomousRedRight() {

}

void autonomousBlueLeft() {

}

void autonomousRedLeft() {

}

void autonnomousSkills() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  // Release the hood
  flipout();
  pros::delay(500);

  // Drive and turn to collect the ball
  pid->velocityMove(2.2, 53);
  pros::delay(250);
  pid->pivot(-35);
  powerIntake(127);
  indexer->move(69);
  pid->move(75.8);
  pros::delay(250);
  indexer->move(-30);

  // Turn and score the ball
  pid->pivotAbsolute(89.7);
  cycle(0);
  pid->move(40.85, 15, true);
  
  // Score the ball
  flywheel->move(127);
  pros::delay(150);
  cycle(127);
  pros::delay(950);
  indexer->move(0);
  pros::delay(300);
  flywheel->move(0);
  // First tower done

  // Back up and align with the next ball
  cycle(-90);
  indexer->move(-127);
  pid->move(-30.6);
  cycle(0);
  pid->pivotAbsolute(27);
  
  // Intake and score the next ball
  powerIntake(127);
  flywheel->move(127);
  indexer->move(30);
  pid->move(77.8, 21, true);

  // Score the corner 
  cycle(127);
  pros::delay(1150);
  cycle(0);
  // Second tower done

  // Back up and align with the next ball
  cycle(-127);

  // // Reset routine
  // int steps = 10;
  // for (int i = 0; i < steps; i++) {
  //   pid->driveStraight(40);
  //   pros::delay(20);
  // }
  // pid->move(-14.8, 15, true);
  // pid->pivotAbsolute(-90);
  // steps = 35;
  // pid->tareDesiredHeading();
  // for (int i = 0; i < steps; i++) {
  //   pid->driveStraight(-60);
  //   pros::delay(20);
  // }
  // pid->tareDesiredHeading();

  pid->move(-31.5);
  cycle(-60);
  pid->pivotAbsolute(-100);

  // Collect the ball
  powerIntake(127);
  pid->move(56.2, 12,true);

  // Turn and score the ball
  pid->pivotAbsolute(0);
  indexer->move(30);
  flywheel->move(127);
  pid->move(38.2, 15, true);

  // Score the ball
  cycle(127);
  pros::delay(900);
  indexer->move(0);
  pros::delay(250);
  cycle(0);
  // Third tower done

  // Back up and align with the next ball
  cycle(-90);
  pid->move(-24.38);
  cycle(0);
  pid->pivotAbsolute(-90);

  // Collect the ball
  powerIntake(127);
  pid->move(51, 15, true);
  pros::delay(400);

  // Turn and score the ball
  pid->pivot(56.5);
  indexer->move(30);
  flywheel->move(127);
  pid->move(32.5, 15, true);

  // Score the ball
  cycle(127);
  pros::delay(640);
  indexer->move(0);
  pros::delay(400);
  flywheel->move(0);
  // Fourth tower done

  // Back up and align with the next ball
  cycle(-90);
  pid->move(-20);
  pid->pivotAbsolute(-209.3);

  // Collect the ball
  powerIntake(127);
  pid->move(56.5, 20, true);
  pros::delay(600);

  // Turn and score the ball
  pid->pivotAbsolute(-89);
  indexer->move(30);
  flywheel->move(127);
  pid->move(37.5, 16.5, true);

  // Score the ball
  cycle(127);
  pros::delay(650);
  indexer->move(0);
  pros::delay(300);
  flywheel->move(0);
  // Fifth tower done

 // Back up and align with the next ball
  cycle(-90);
  indexer->move(-127);
  pid->move(-32.2);
  cycle(0);
  pid->pivotAbsolute(-151);
  
  // Intake and score the next ball
  powerIntake(110);
  indexer->move(127);
  pid->move(74.5, 19, true);

  // Score the corner ball
  cycle(127);
  powerIntake(-127);
  pros::delay(700);
  flywheel->move(0);
  cycle(0);
  powerIntake(-127);
  // Sixth tower done

  // Reset routine
  int steps = 10;
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(40);
    pros::delay(20);
  }
  pid->move(-14.8, 15, true);
  pid->pivotAbsolute(-270);
  steps = 35;
  pid->tareDesiredHeading();
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(-60);
    pros::delay(20);
  }
  pid->tareDesiredHeading();

  // Collect the next ball
  powerIntake(127);
  pid->move(73.5, 18, true);
  pid->pivot(90);
  cycle(0);
  flywheel->move(127);
  pid->velocityMove(9, 80);
  pros::delay(400);

  // Score the ball
  cycle(127);
  pros::delay(1150);
  cycle(0);
  // Seventh tower done

  // Center tower
  pid->move(-12);
  pid->pivot(172.5);
  cycle(40);
  powerIntake(127);
  pid->velocityMove(49.8, 80, 15, true);
  cycle(0);

  pid->velocityMove(-3.5,40);
  pid->pivot(29);
  pid->velocityMove(4,55);
  pid->powerDrive(50,60);
  cycle(127);
  pros::delay(1500);
  cycle(0);
}

// Driver skills with autonomous-controlled timing (meant to simulate a real match controller)
void autonomousDrvSkills() {
  struct Temp {
    static void call(void * param) {
      opcontrol();
    }
  };
  // Clear the timer
  competitionTimer->opcontrolClearTimer();

  // Wait for the driver to begin
  while (controllerMain->get_analog(STICK_LEFT_X) == 0 && controllerMain->get_analog(STICK_LEFT_Y) == 0 && controllerMain->get_analog(STICK_RIGHT_X) == 0 && controllerMain->get_analog(STICK_RIGHT_Y) == 0 && !(controllerMain->get_digital(BUTTON_L1) || controllerMain->get_digital(BUTTON_L2) || controllerMain->get_digital(BUTTON_R1) || controllerMain->get_digital(BUTTON_R2)))
    pros::delay(1);

  // Allow movement and start timing
  pros::Task dsopcontrol (Temp::call, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Driver Skills");

  // Use vibrations to tell how much time is left
  competitionTimer->opcontrolWaitUntil(30000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(50000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(55000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(56000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(57000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(58000);
  controllerMain->rumble(".");
  competitionTimer->opcontrolWaitUntil(59000);
  controllerMain->rumble(".");

  // When one minute is up, prevent the controls
  competitionTimer->opcontrolWaitUntil(60000);
  controllerMain->rumble(".");
  dsopcontrol.suspend();

  // Stop all motors
  if (ports::port1 != NULL) ports::port1->move(0);
  if (ports::port2 != NULL) ports::port2->move(0);
  if (ports::port3 != NULL) ports::port3->move(0);
  if (ports::port4 != NULL) ports::port4->move(0);
  if (ports::port5 != NULL) ports::port5->move(0);
  if (ports::port6 != NULL) ports::port6->move(0);
  if (ports::port7 != NULL) ports::port7->move(0);
  if (ports::port8 != NULL) ports::port8->move(0);
  if (ports::port9 != NULL) ports::port9->move(0);
  if (ports::port10 != NULL) ports::port10->move(0);
  if (ports::port11 != NULL) ports::port11->move(0);
  if (ports::port12 != NULL) ports::port12->move(0);
  if (ports::port13 != NULL) ports::port13->move(0);
  if (ports::port14 != NULL) ports::port14->move(0);
  if (ports::port15 != NULL) ports::port15->move(0);
  if (ports::port16 != NULL) ports::port16->move(0);
  if (ports::port17 != NULL) ports::port17->move(0);
  if (ports::port18 != NULL) ports::port18->move(0);
  if (ports::port19 != NULL) ports::port19->move(0);
  if (ports::port20 != NULL) ports::port20->move(0);
  if (ports::port21 != NULL) ports::port21->move(0);

  // Wait for 5 seconds before giving control back
  pros::delay(5000);
}

void autonomousOther(int selectedAutonomous) {
  flywheel->move(100);
  waitForUltrasonic(2, 150, true);
  flywheel->move(0);
  if (selectedAutonomous == 0)
    flipout();
  // If an invalid autonomous is selected, run a one point autonomous
  if (selectedAutonomous == -1) {
    pid->powerDrive(127, 127);
    pros::delay(1000);
    pid->powerDrive(0, 0);
  } else if (selectedAutonomous == 7) {
      pid->move(-12);
  pid->pivot(172.5);
  cycle(40);
  powerIntake(127);
  pid->velocityMove(49.8, 80, 15, true);
  cycle(0);

  pid->velocityMove(-3.5,40);
  pid->pivot(28);
  pid->velocityMove(4,55);
  cycle(127);
  pros::delay(1500);
  cycle(0);
  }

}

// Entry point for autonomous
void autonomous() {
  // Sets the status on the LCD
	LCD::setStatus("Autonomous " + LCD::getAutonomousName());

	// Start the autonomous timer
	competitionTimer->autonomousStartTimer();

  // Based on the selected autonomous, run
  if (selectedAutonomous == 1)
    autonomousBlueRight();
  else if (selectedAutonomous == 2)
    autonomousRedRight();
  else if (selectedAutonomous == 3)
    autonomousBlueLeft();
  else if (selectedAutonomous == 4)
    autonomousRedLeft();
  else if (selectedAutonomous == 5)
    autonnomousSkills();
  else if (selectedAutonomous == 6)
    autonomousDrvSkills();
  else
    autonomousOther(selectedAutonomous);

  // Stop the autonomous timer
	competitionTimer->autonomousEndTimer();

  // Log the message to the message holder and set it to the screen
  messageHolder->appendLine("Autonomous took " + std::to_string(competitionTimer->autonomousTime()) + " ms");
  LCD::setText(6, "Auto took " + std::to_string(competitionTimer->autonomousTime()) + " ms");
}
