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

void autonomousBlueTall() {

}

void autonomousRedTall() {

}

void autonomousBlueFlat() {
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
  pid->pivotAbsolute(-132);
  pid->velocityMove(14.4, 50);

  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);

  tiltMotor->move_absolute(403, 30);
  intakeMotorRight->move(-25);
  intakeMotorLeft->move(-25);
  pros::delay(2000);

  intakeMotorRight->move(-60);
  intakeMotorLeft->move(-60);
  tiltMotor->move_absolute(0, 30);
  pros::delay(2500);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pid->move(-10);

  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
}

void autonomousRedFlat() {
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
  pid->velocityMove(12.5, 50);

  frontRightDrive->set_brake_mode(BRAKE_HOLD);
  frontLeftDrive->set_brake_mode(BRAKE_HOLD);
  backRightDrive->set_brake_mode(BRAKE_HOLD);
  backLeftDrive->set_brake_mode(BRAKE_HOLD);

  tiltMotor->move_absolute(403, 30);
  intakeMotorRight->move(-25);
  intakeMotorLeft->move(-25);
  pros::delay(2000);

  intakeMotorRight->move(-60);
  intakeMotorLeft->move(-60);
  tiltMotor->move_absolute(0, 30);
  pros::delay(2500);

  intakeMotorRight->move(0);
  intakeMotorLeft->move(0);
  pid->move(-10);

  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
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
