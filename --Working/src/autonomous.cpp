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
  // Outtake to throw the rollers forward
  intakeMotorLeft->move(-127);
  intakeMotorRight->move(-127);

  // Push the tray back and reset, just in case it didn't start there
  tiltMotor->move(-40);
  pros::delay(100);

  // Stop the rollers and reset the tray
  intakeMotorLeft->move(0);
  intakeMotorRight->move(0);
  tiltMotor->tare_position();
  tiltMotor->move(0);

  // Wait for the tray to complete flip out
  pros::delay(750);
  intakeMotorLeft->move(127);
  intakeMotorRight->move(127);
}

void driveReleaseTray(int drivePower) {
  // Outtake to throw the rollers forward
  pid->resetEncoders();
  intakeMotorLeft->move(-127);
  intakeMotorRight->move(-127);

  // Push the tray back and reset, just in case it didn't start there
  tiltMotor->move(-40);
  for (int i = 0; i < 5; i++) {
    pid->driveStraight(drivePower);
    pros::delay(20);
  }

  // Stop the rollers and reset the tray
  intakeMotorLeft->move(0);
  intakeMotorRight->move(0);
  tiltMotor->tare_position();
  tiltMotor->move(0);

  // Wait for the tray to complete flip out
  for (int i = 0; i < 40; i++) {
    pid->driveStraight(drivePower);
    pros::delay(20);
  }
  intakeMotorLeft->move(127);
  intakeMotorRight->move(127);
}

void autonomousBlueTall() {
  tiltMotor->move(100);
  pros::delay(500);
  tiltMotor->move(-100);
  pros::delay(500);
  tiltMotor->move(0);
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(43, 39);
  pros::delay(100);
  pid->move(-21);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pid->pivotAbsolute(132);

}

void autonomousRedTall() {
  tiltMotor->move(100);
  pros::delay(500);
  tiltMotor->move(-100);
  pros::delay(500);
  tiltMotor->move(0);
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(43, 38);
  pros::delay(500);
  pid->move(-21);
  pid->pivotAbsolute(-90);
  pid->move(21.5);

  pros::delay(500);

  frontRightDrive->move(73);
  frontLeftDrive->move(-73);
  backRightDrive->move(73);
  backLeftDrive->move(-73);

  pros::delay(430);

  frontRightDrive->move(0);
  frontLeftDrive->move(0);
  backRightDrive->move(0);
  backLeftDrive->move(0);

  pros::delay(250);

  pid->velocityMove(15, 127);

  pros::delay(250);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  frontRightDrive->move(-73);
  frontLeftDrive->move(73);
  backRightDrive->move(-73);
  backLeftDrive->move(73);

  pros::delay(125);

  pid->velocityMove(3.5, 60);

  pros::delay(500);

  // stack

  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);

  tiltMotor->move_absolute(403, 30);
  intakeMotorRight->move(-25);
  intakeMotorLeft->move(-25);
  pros::delay(2500);

  intakeMotorRight->move(-60);
  intakeMotorLeft->move(-60);
  tiltMotor->move_absolute(0, 30);
  pros::delay(2500);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pid->velocityMove(-22, 127);

  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
}

void autonomousBlueFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  releaseTray();

  // Intake the first line of cubes
  intakeMotorRight->move(110);
  intakeMotorLeft->move(110);
  pid->velocityMove(43, 78);
  pros::delay(100);

  intakeMotorRight->move(80);
  intakeMotorLeft->move(80);

   /* A major source of consistency, route was changed
   // Strafe to get into position for the next line of cubes
   pid->resetEncoders();
   while (frontRightDrive->get_position() > -1700) {
     pid->strafeStraight(120, -127);
   }

   pid->resetEncoders();
   while (frontRightDrive->get_position() > -408) {
     pid->strafeStraight(120, 0);
   }
  */

  // Get in position to turn to the next line of cubes
  double pivotAmount = 27;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  pid->move(-46.8, absoluteMove);

  pivotAmount = -27;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Intake the next line of cubes
  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(31, 78);
  pros::delay(300);

  // Move into scoring position
  pid->move(-8.65, absoluteMove);

  intakeMotorRight->move(80);
  intakeMotorLeft->move(80);

  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);

  // Pivot to face to the scoring zone
  ports::pid->setPowerLimits(100, 32);
  pivotAmount = -136.5;
  if (absoluteTurn)
    pid->pivot(pivotAmount, 7);
  else
    pid->pivotRelative(pivotAmount, 7);
  ports::pid->setPowerLimits(120, 32);

  // Let the robot settle
  pros::delay(100);
  intakeMotorRight->move(-15);
  intakeMotorLeft->move(-15);

  // Move into the scoring position
  pid->velocityMove(18.7, 75, absoluteMove);
  pid->velocityMove(-1.191, 45, absoluteMove);
  pid->powerDrive(0,0);
  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pros::delay(250);

  // Tilt the stack upright using a P controller
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(27 + (680 - tiltMotor->get_position()) * 0.3515);
  tiltMotor->move_absolute(723, 23); // Gets rid of the jittering
  pros::delay(1300);

  // Brake the motors
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-15, 35, false);
  tiltMotor->move_absolute(0, 60);
}

void autonomousRedFlat() {
  // Flags to set when driving, deciding whether to use absolute or relative gyro positions
  bool absoluteTurn = false;
  bool absoluteMove = false;

  // Release the tray
  releaseTray();

  // Intake the first line of cubes
  intakeMotorRight->move(100);
  intakeMotorLeft->move(100);
  pid->velocityMove(50, 85);
  pros::delay(100);

  // Slow the intake and drive back
  intakeMotorRight->move(70);
  intakeMotorLeft->move(70);
  pid->move(-37.5);

  // Get in position to turn to the next line of cubes
  double pivotAmount = -65;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  pid->move(-25, absoluteMove);

  pivotAmount = 65;
  if (absoluteTurn)
    pid->pivot(pivotAmount);
  else
    pid->pivotRelative(pivotAmount);

  // Intake the next line of cubes
  intakeMotorRight->move(110);
  intakeMotorLeft->move(110);
  pid->velocityMove(34, 85);
  pros::delay(300);

  // Move into scoring position
  pid->move(-18.7, absoluteMove);

  intakeMotorRight->move(80);
  intakeMotorLeft->move(80);

  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);

  // Pivot to face to the scoring zone
  // ports::pid->setPowerLimits(100, 32);
  pivotAmount = 135;
  if (absoluteTurn)
    pid->pivot(pivotAmount, 9);
  else
    pid->pivotRelative(pivotAmount, 9);
  // ports::pid->setPowerLimits(120, 32);

  // Let the robot settle
  pros::delay(100);
  // intakeMotorRight->move(-15);
  // intakeMotorLeft->move(-15);

  // Move forward into the scoring position
  intakeMotorRight->move(-20);
  intakeMotorLeft->move(-20);
  pid->velocityMove(16, 75, absoluteMove);
  pid->velocityMove(1.27, 35, absoluteMove);
  pid->powerDrive(0,0);
  pros::delay(250);
  intakeMotorRight->move(-28);
  intakeMotorLeft->move(-28);

  while (tiltMotor->get_position() < 705)
    tiltMotor->move(35 + (708 - tiltMotor->get_position()) * 0.22); // Simple P controller
  tiltMotor->move_absolute(708, 45); // Gets rid of the jittering
  pros::delay(1300);

  // Brake the motors
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-15, 35, false);
  tiltMotor->move_absolute(0, 60);
}

void autonnomousSkills() {

}

void autonomousDrvSkills() {
  struct Temp {
    static void call(void * param) {
      opcontrol();
    }
  };

  competitionTimer->opcontrolClearTimer();

  while (controllerMain->get_analog(STICK_LEFT_X) == 0 && controllerMain->get_analog(STICK_LEFT_Y) == 0 && !(controllerMain->get_digital(BUTTON_L1) || controllerMain->get_digital(BUTTON_L2) || controllerMain->get_digital(BUTTON_R1) || controllerMain->get_digital(BUTTON_R2)))
    pros::delay(1);

  pros::Task dsopcontrol (Temp::call, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Driver Skills");
  competitionTimer->opcontrolWaitUntil(60000);
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
  driveReleaseTray(50);
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
