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

// Blue front/ autonomous (represented with a tall stack of cubes)
void autonomousBlueTall() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  // releaseTray();
  pros::delay(750);

  // Drive slowly to knock the first stack
  intakeMotorRight->move(108);
  intakeMotorLeft->move(108);
  pid->velocityMove(30.5, 50);

  // Woit and try and intake any fallen cubes
  pros::delay(1000);
  pid->velocityMove(20.5, 50);
  pros::delay(300);

  // Move back to score
  intakeMotorRight->move(50);
  intakeMotorLeft->move(50);
  pid->move(-27);

  // Turn to intake the solo cube
  double pivotAmount = 90;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Intake the solo cube
  intakeMotorRight->move(108);
  intakeMotorLeft->move(108);
  pid->velocityMove(13, 50);

  // Turn to score
  pivotAmount = 30;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Move to score
  intakeMotorRight->move(50);
  intakeMotorLeft->move(50);
  pid->move(17);

  // Score the stack
  intakeMotorRight->move(-10);
  intakeMotorLeft->move(-10);
  // while (tiltMotor->get_position() < 630)
  //   tiltMotor->move(48 + (666 - tiltMotor->get_position()) * 0.213); // Simple P controller
  pid->powerDrive(10, 10);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  // tiltMotor->move_absolute(666, 45); // Gets rid of the jittering
  pros::delay(500);

  // Move back and release
  pid->velocityMove(-10, 40);
  // tiltMotor->move_absolute(0, 60);
}

void autonomousRedTall() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  // releaseTray();
  pros::delay(750);

  // Drive slowly to knock the first stack
  intakeMotorRight->move(108);
  intakeMotorLeft->move(108);
  pid->velocityMove(30.5, 50);

  // Woit and try and intake any fallen cubes
  pros::delay(1000);
  pid->velocityMove(20.5, 50);
  pros::delay(300);

  // Move back to score
  intakeMotorRight->move(50);
  intakeMotorLeft->move(50);
  pid->move(-27);

  // Turn to intake the solo cube
  double pivotAmount = -90;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Intake the solo cube
  intakeMotorRight->move(108);
  intakeMotorLeft->move(108);
  pid->velocityMove(13, 50);

  // Turn to score
  pivotAmount = -30;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Move to score
  intakeMotorRight->move(50);
  intakeMotorLeft->move(50);
  pid->move(17);

  // Score the stack
  intakeMotorRight->move(-10);
  intakeMotorLeft->move(-10);
  // while (tiltMotor->get_position() < 630)
  //   tiltMotor->move(48 + (666 - tiltMotor->get_position()) * 0.213); // Simple P controller
  pid->powerDrive(10, 10);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  // tiltMotor->move_absolute(666, 45); // Gets rid of the jittering
  pros::delay(500);

  // Move back and release
  pid->velocityMove(-10, 40);
  // tiltMotor->move_absolute(0, 60);
}

void autonomousBlueFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  ports::frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  ports::frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  ports::backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  ports::backRightDrive->set_brake_mode(BRAKE_BRAKE);

  // Release the tray
  // releaseTray();
  pros::delay(250);

  // Intake the first line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(56.2, 60);
  pros::delay(300);

  pid->move(-48);
  pid->pivot(-90);
  int steps = 70;
  pid->setAbsoluteDesiredHeading(-90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(-80);
    pros::delay(20);
  }
  pid->move(6.8);

  // Prepare to stack
  powerIntake(-40);
  pros::delay(500);
  powerIntake(0);

  // Score the stack
  // while (tiltMotor->get_position() < 630)
  //   tiltMotor->move(48 + (730 - tiltMotor->get_position()) * 0.22); // Simple P controller
  powerIntake(0);
  // tiltMotor->move_absolute(730, 48); // Gets rid of the jittering
  pros::delay(800);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-10, 40, false);
  // tiltMotor->move_absolute(0, 60);

  pid->move(-15);
}

void autonomousRedFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  ports::frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  ports::frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  ports::backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  ports::backRightDrive->set_brake_mode(BRAKE_BRAKE);

  // Release the tray
  // releaseTray();

  // Intake the first line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(57, 60);
  pros::delay(300);

  pid->move(-51);
  pid->pivot(90);
  int steps = 70;
  pid->setAbsoluteDesiredHeading(90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(80);
    pros::delay(20);
  }
  pid->move(8.5);

  powerIntake(-48);
  pros::delay(435);
  powerIntake(0);

  // Score the stack
  // while (tiltMotor->get_position() < 630)
  //   tiltMotor->move(48 + (730 - tiltMotor->get_position()) * 0.22); // Simple P controller
  powerIntake(0);
  // tiltMotor->move_absolute(730, 48); // Gets rid of the jittering
  pros::delay(800);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-10, 40, false);
  // tiltMotor->move_absolute(0, 60);
}

void autonnomousSkills() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  // Release the hood
  flipout();
  pros::delay(500);

  // Drive and turn to collect the ball
  pid->velocityMove(2, 50);
  pros::delay(400);
  pid->pivot(-35);
  powerIntake(127);
  indexer->move(69);
  pid->move(75.5);
  pros::delay(250);

  // Turn and score the ball
  pid->pivotAbsolute(89.7);
  powerIntake(0);
  indexer->move(40);
  pid->move(40.6, 15, true);
  
  // Score the ball
  indexer->move(-80);
  pros::delay(300);
  waitForUltrasonic(1.7, 150, true);
  cycle(127);
  pros::delay(950);
  indexer->move(0);
  pros::delay(300);
  flywheel->move(0);
  // First tower done

  // Back up and align with the next ball
  cycle(-90);
  indexer->move(-127);
  pid->move(-31.8);
  cycle(0);
  pid->pivotAbsolute(27.2);
  
  // Intake and score the next ball
  powerIntake(127);
  indexer->move(40);
  pid->move(78.8, 15, true);

  // Score the corner ball
  indexer->move(-80);
  pros::delay(300);
  waitForUltrasonic(1.7, 150, true);  
  cycle(127);
  pros::delay(1150);
  flywheel->move(0);
  // pros::delay(650);
  cycle(0);
  // Second tower done

  // Back up and align with the next ball
  cycle(-127);
  pid->move(-31.5);
  cycle(-60);
  pid->pivotAbsolute(-100);

  // Collect the ball
  powerIntake(127);
  pid->move(56.2);

  // Turn and score the ball
  pid->pivotAbsolute(0);
  indexer->move(60.6);
  pid->move(38.2, 15, true);

  // Score the ball
  indexer->move(-80);
  waitForUltrasonic(2, 150, true);
  cycle(127);
  pros::delay(900);
  indexer->move(0);
  pros::delay(250);
  cycle(0);
  // Third tower done

  // Back up and align with the next ball
  cycle(-90);
  pid->move(-24);
  cycle(0);
  pid->pivotAbsolute(-90);

  // Collect the ball
  powerIntake(127);
  pid->move(51);

  // Turn and score the ball
  pid->pivot(56);
  indexer->move(60);
  pid->move(33.6, 15, true);

  // Score the ball
  cycle(127);
  pros::delay(850);
  indexer->move(0);
  pros::delay(250);
  flywheel->move(0);
  // Fourth tower done

  // Back up and align with the next ball
  cycle(-127);
  pid->move(-20);
  pid->pivotAbsolute(-210);

  // Collect the ball
  powerIntake(127);
  pid->move(58);

  // Turn and score the ball
  pid->pivotAbsolute(-90);
  indexer->move(60);
  pid->move(38, 15, true);

  // Score the ball
  cycle(127);
  pros::delay(900);
  indexer->move(0);
  pros::delay(250);
  flywheel->move(0);
  // Fifth tower done

  return;
  cycle(-127);
  pid->move(-20);
  pid->pivot(-150);
  powerIntake(127);
  indexer->move(60);

  pid->move(56);

  pid->pivotAbsolute(0);

  // Poke three times
  pid->velocityMove(14.2, 50);
  pros::delay(500);
  pid->velocityMove(-14.4, 50);
  pros::delay(500);

  pid->velocityMove(15., 50);
  pros::delay(500);
  pid->velocityMove(-14.6, 50);
  pros::delay(500);

  pid->velocityMove(15., 50);
  pros::delay(500);
  pid->velocityMove(-1.5, 50);
  pros::delay(500);

  return;
  indexer->move(100);
  flywheel->move(-10);
  pid->velocityMove(80.1, 48);
  pros::delay(500);
  pid->pivot(56.4);
  powerIntake(127);
  indexer->move(0);
  flywheel->move(0);

  // Poke three times
  pid->velocityMove(14.2, 50);
  pros::delay(500);
  pid->velocityMove(-14.4, 50);
  pros::delay(500);

  pid->velocityMove(15., 50);
  pros::delay(500);
  pid->velocityMove(-14.6, 50);
  pros::delay(500);

  pid->velocityMove(15., 50);
  pros::delay(500);
  pid->velocityMove(-1.5, 50);
  pros::delay(500);

  // Turn to score the balls
  pid->pivot(27.5);

  // Score the ball
  flywheel->move(127);
  powerIntake(127);
  indexer->move(127);
  pros::delay(1150);
  powerIntake(0);
  indexer->move(0);
  flywheel->move(0);

  // Back up and line up with balls
  pid->move(-10);
  pid->pivot(-59);
  powerIntake(127);
  indexer->move(80);
  pid->velocityMove(109.5, 50);
  powerIntake(0);
  pros::delay(300);

  // Score the balls
  flywheel->move(127);
  pros::delay(250);
  powerIntake(127);
  indexer->move(127);

  pros::delay(600);

  indexer->move(0);

  pros::delay(350);

  flywheel->move(0);

  pros::delay(350);

  powerIntake(0);
  
  pid->move(-10);
  powerIntake(-0);
  indexer->move(-0);
  flywheel->move(0);
  return;
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
    autonomousBlueTall();
  else if (selectedAutonomous == 2)
    autonomousRedTall();
  else if (selectedAutonomous == 3)
    autonomousBlueFlat();
  else if (selectedAutonomous == 4)
    autonomousRedFlat();
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
