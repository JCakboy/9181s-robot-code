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

void releaseTray() {
  liftMotor->move(127);
  pros::delay(630);
  liftMotor->move(-110);
  pros::delay(720);
  liftMotor->move(0);
  pros::delay(405);
}

void skillsReleaseTray() {
  liftMotor->move(127);
  pros::delay(650);
  liftMotor->move(-100);
  pros::delay(715);
  liftMotor->move(0);
  pros::delay(750);
}

void autonomousBlueTall() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  releaseTray();
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
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(48 + (666 - tiltMotor->get_position()) * 0.213); // Simple P controller
  pid->powerDrive(10, 10);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(666, 45); // Gets rid of the jittering
  pros::delay(500);

  // Move back and release
  pid->velocityMove(-10, 40);
  tiltMotor->move_absolute(0, 60);
}

void autonomousRedTall() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  releaseTray();
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
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(48 + (666 - tiltMotor->get_position()) * 0.213); // Simple P controller
  pid->powerDrive(10, 10);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(666, 45); // Gets rid of the jittering
  pros::delay(500);

  // Move back and release
  pid->velocityMove(-10, 40);
  tiltMotor->move_absolute(0, 60);
}

void autonomousBlueFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  // Release the tray
  releaseTray();

  // Intake the first line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(14.3, 120);
  pid->velocityMove(31, 67);
  pid->velocityMove(10.3, 45);
  pros::delay(220);
  pid->move(-9.7);
  pid->setAbsoluteDesiredHeading(60);
  pid->velocityMove(8.2, 90);
  pid->setAbsoluteDesiredHeading(0);

  pid->move(-43);
  pid->pivot(-90);
  int steps = 35;
  pid->setAbsoluteDesiredHeading(-90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(-127);
    pros::delay(20);
  }

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  pid->move(12.8);

  intakeMotorRight->move(-48);
  intakeMotorLeft->move(-48);
  pros::delay(435);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Score the stack
  while (tiltMotor->get_position() < 700)
    tiltMotor->move(48 + (730 - tiltMotor->get_position()) * 0.22); // Simple P controller
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(730, 48); // Gets rid of the jittering
  pros::delay(800);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-10, 40, false);
  tiltMotor->move_absolute(0, 60);
}

void autonomousRedFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  // Release the tray
  releaseTray();

  // Intake the first line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(14.3, 120);
  pid->velocityMove(31, 67);
  pid->velocityMove(10.3, 45);
  pros::delay(220);
  pid->move(-9.7);
  pid->setAbsoluteDesiredHeading(-60);
  pid->velocityMove(8.2, 90);
  pid->setAbsoluteDesiredHeading(0);

  pid->move(-43.5);
  pid->pivot(90);
  int steps = 38;
  pid->setAbsoluteDesiredHeading(90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(120);
    pros::delay(20);
  }

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  pid->move(14.5);

  intakeMotorRight->move(-48);
  intakeMotorLeft->move(-48);
  pros::delay(435);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Score the stack
  while (tiltMotor->get_position() < 700)
    tiltMotor->move(48 + (730 - tiltMotor->get_position()) * 0.22); // Simple P controller
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(730, 48); // Gets rid of the jittering
  pros::delay(800);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-10, 40, false);
  tiltMotor->move_absolute(0, 60);
}

void autonnomousSkills() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = true;
  bool absoluteMove = true;

  // Release the tray
  skillsReleaseTray();

  liftMotor->move_absolute(490, 105);
  // Get in position to put the first cube in the alliance tower
  // intakeMotorRight->move(127);
  // intakeMotorLeft->move(127);
  pid->velocityMove(13, 70);
  // intakeMotorRight->move(-70);
  // intakeMotorLeft->move(-70);
  // pros::delay(580);
  // intakeMotorRight->move(0);
  // intakeMotorLeft->move(0);
  // Lift the intake
  pid->velocityMove(6, 38);
  // Score the cube
  intakeMotorRight->move(-100);
  intakeMotorLeft->move(-100);
  pros::delay(900);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Back up and get in position to intake the first line of cubes
  liftMotor->move_absolute(0, 100);
  pid->move(-13.8);
  pid->pivot(45);

  // Reset
  int steps = 40;
  pid->setAbsoluteDesiredHeading(45);
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(-50);
    pros::delay(20);
  }
  gyro->tarePosition();
  pid->tareDesiredHeading();

  // Intake the first line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(65, 70);
  pid->velocityMove(55, 60);

  // Turn to face the scoring zone
  intakeMotorRight->move(120);
  intakeMotorLeft->move(120);
  pid->pivot(-90);
  // Strafe align for scoring
  steps = 78;
  pid->setAbsoluteDesiredHeading(-90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(90);
    pros::delay(20);
  }
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  // Move into position to score
  pid->move(9.1); // Was 10.7

  // Score the first stack
  while (tiltMotor->get_position() < 700)
    tiltMotor->move(42 + (730 - tiltMotor->get_position()) * 0.1925); // Simple P controller
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(730, 48); // Gets rid of the jittering
  pros::delay(1400);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-15, 55, false);
  tiltMotor->move_absolute(0, 60);

  // Turn and reset to get in position to score the next tower
  pid->pivot(-90);
  steps = 40;
  pid->tareDesiredHeading();
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(-50);
    pros::delay(20);
  }
  gyro->tarePosition();
  pid->tareDesiredHeading();

  // Get the center tower cube
  intakeMotorRight->move(115);
  intakeMotorLeft->move(115);
  pid->move(46);
  pros::delay(500);
  // Set the cube in the intake
  intakeMotorRight->move(-70);
  intakeMotorLeft->move(-70);
  pros::delay(500);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Score the center tower
  liftMotor->move_absolute(490, 100);
  pid->velocityMove(-6, 40);
  pros::delay(500);
  pid->velocityMove(8, 40);
  intakeMotorRight->move(-100);
  intakeMotorLeft->move(-100);
  pros::delay(950);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Move into position and face the tall tower
  liftMotor->move_absolute(0, 80);
  pid->move(-31);
  pid->pivot(-88.8);
  pros::delay(100);
  pid->tareDesiredHeading();

  // Get the tower cube
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(14, 65);
  pros::delay(700);
  // Set the cube in the intake
  intakeMotorRight->move(-70);
  intakeMotorLeft->move(-70);
  pros::delay(600);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Score the tower
  liftMotor->move_absolute(614, 100);
  pid->velocityMove(-5, 40);
  pros::delay(800);
  pid->velocityMove(9.5, 40);
  intakeMotorRight->move(-115);
  intakeMotorLeft->move(-115);
  pros::delay(1000);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  // Get in position to intake the second row of cubes
  pid->move(-3.8);
  pid->pivot(90);
  liftMotor->move_absolute(0, 100);
  // Reset
  steps = 80;
  pid->tareDesiredHeading();
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(-50);
    pros::delay(20);
  }
  gyro->tarePosition();
  pid->tareDesiredHeading();

  // Intake the second line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(65, 70);
  pid->velocityMove(54, 50);

  pid->pivot(90);
  pid->move(35);
  pid->pivot(-90);

  // Strafe align for scoring
  steps = 88;
  pid->setAbsoluteDesiredHeading(0);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(85);
    pros::delay(20);
  }

  // Move into position to score
  pid->move(10.6);

  intakeMotorRight->move(-40);
  intakeMotorLeft->move(-40);
  pros::delay(350);
  intakeMotorRight->move(-13);
  intakeMotorLeft->move(-13);
  // Score the stack
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(42 + (666 - tiltMotor->get_position()) * 0.2075); // Simple P controller
  pid->powerDrive(10, 10);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  tiltMotor->move_absolute(666, 48); // Gets rid of the jittering
  pros::delay(1500);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-18, 55, false);
  tiltMotor->move_absolute(0, 60);
return;
  // Strafe
  steps = 55;
  pid->setAbsoluteDesiredHeading(-90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(85);
    pros::delay(20);
  }
  pros::delay(200);
  pid->pivot(170);
  pros::delay(100);

  // Get the tower cube
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->move(13);
  pros::delay(500);
  // Set the cube in the intake
  intakeMotorRight->move(-70);
  intakeMotorLeft->move(-70);
  pros::delay(600);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pid->velocityMove(-5, 40);

  // Score the tower
  liftMotor->move_absolute(614, 100);
  pros::delay(1500);
  pid->velocityMove(8, 40);
  intakeMotorRight->move(-127);
  intakeMotorLeft->move(-127);
  pros::delay(1000);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

}

void autonomousDrvSkills() {
  struct Temp {
    static void call(void * param) {
      opcontrol();
    }
  };

  competitionTimer->opcontrolClearTimer();

  while (controllerMain->get_analog(STICK_LEFT_X) == 0 && controllerMain->get_analog(STICK_LEFT_Y) == 0 && controllerMain->get_analog(STICK_RIGHT_X) == 0 && controllerMain->get_analog(STICK_RIGHT_Y) == 0 && !(controllerMain->get_digital(BUTTON_L1) || controllerMain->get_digital(BUTTON_L2) || controllerMain->get_digital(BUTTON_R1) || controllerMain->get_digital(BUTTON_R2)))
    pros::delay(1);

  pros::Task dsopcontrol (Temp::call, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Driver Skills");
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
  competitionTimer->opcontrolWaitUntil(60000);
  controllerMain->rumble(".");
  dsopcontrol.suspend();

  ports::frontLeftDrive->move(0);
  ports::backLeftDrive->move(0);
  ports::frontRightDrive->move(0);
  ports::backRightDrive->move(0);
  ports::intakeMotorRight->move(0);
  ports::intakeMotorLeft->move(0);
  ports::tiltMotor->move(0);

  pros::delay(5000);
}

void autonomousOther(int selectedAutonomous) {
  releaseTray();
  pros::delay(700);
  if (selectedAutonomous != 0) {
    pid->powerDrive(-100, -100);
    pros::delay(650);
    pid->powerDrive(100, 100);
    pros::delay(650);
    pid->powerDrive(0, 0);
    pros::delay(750);
  }
}

void autonomous() {
  // Sets the status on the LCD
	LCD::setStatus("Autonomous " + LCD::getAutonomousName());

	// Start the autonomous timer
	competitionTimer->autonomousStartTimer();

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
