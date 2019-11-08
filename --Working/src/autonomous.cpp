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
  intakeMotorLeft->move(127);
  intakeMotorRight->move(127);
  tiltMotor->move(-40);
  pros::delay(300);
  tiltMotor->tare_position();
  tiltMotor->move(0);
  intakeMotorLeft->move(-127);
  intakeMotorRight->move(-127);
  pros::delay(100);
  intakeMotorLeft->move(0);
  intakeMotorRight->move(0);
  pros::delay(600);
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
  releaseTray();

  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(43, 70);
  pros::delay(100);

  intakeMotorRight->move(70);
  intakeMotorLeft->move(70);

  pid->resetEncoders();
  while (frontRightDrive->get_position() > -1700) {
    pid->strafeStraight(120, -127);
  }

  pid->resetEncoders();
  while (frontRightDrive->get_position() > -408) {
    pid->strafeStraight(120, 0);
  }

  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);

  pid->velocityMove(39, 70);
  pros::delay(300);
  pid->pivot(-137.4);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);

  pid->move(48);
  pros::delay(250);

  // If the left triggers are pressed, tilt the stack to be upright
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(28 + (680 - tiltMotor->get_position()) * 0.2325); // Simple P controller
  tiltMotor->move_absolute(723, 23); // Gets rid of the jittering
  pros::delay(1875);

  tiltMotor->move_absolute(0, 60);
  pid->velocityMove(-15, 40);


  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
}

void autonomousRedFlat() {
  releaseTray();

  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);
  pid->velocityMove(43, 70);
  pros::delay(100);

  intakeMotorRight->move(80);
  intakeMotorLeft->move(80);

  pid->resetEncoders();
  while (frontLeftDrive->get_position() > -1500) {
    pid->strafeStraight(-120, -127);
  }

  pid->resetEncoders();
  while (frontLeftDrive->get_position() > -338) {
    pid->strafeStraight(-127, 0);
  }

  intakeMotorRight->move(127);
  intakeMotorLeft->move(127);

  pid->velocityMove(38.5, 70);
  pros::delay(300);
  pid->pivot(131.1);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);

  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);

  pid->move(51.9);
  pros::delay(250);

  // If the left triggers are pressed, tilt the stack to be upright
  while (tiltMotor->get_position() < 650)
    tiltMotor->move(28 + (680 - tiltMotor->get_position()) * 0.2325); // Simple P controller
  tiltMotor->move_absolute(723, 23); // Gets rid of the jittering
  pros::delay(1875);

  tiltMotor->move_absolute(0, 60);
  pid->velocityMove(-15, 40);


  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
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
  releaseTray();
  pros::delay(700);
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
	competitionTimer->autonomousStartTimer();

  // Log the message to the message holder
  messageHolder->appendLine("Autonomous took " + std::to_string(competitionTimer->autonomousTime()) + " ms");
}
