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

  // Release the tray
  // skillsReleaseTray();

  // liftMotor->move_absolute(490, 105);
  // Get in position to put the first cube in the alliance tower
  // powerIntake(127);
  pid->velocityMove(13, 70);
  // powerIntake(-70);
  // pros::delay(580);
  // powerIntake(0);
  // Lift the intake
  pid->velocityMove(6, 38);
  // Score the cube
  powerIntake(-100);
  pros::delay(880);
  powerIntake(0);

  // Back up and get in position to intake the first line of cubes
  // liftMotor->move_absolute(0, 100);
  pid->move(-12.35);
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
  powerIntake(127);
  pid->velocityMove(12, 38);

  pid->velocityMove(55, 68);
  pid->velocityMove(56.5, 55);

  // Turn to face the scoring zone
  powerIntake(120);
  pid->pivot(-90);
  // Strafe align for scoring
  steps = 110;
  pid->setAbsoluteDesiredHeading(-90);
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(90);
    pros::delay(20);
  }
  powerIntake(0);

  // Move into position to score
  pid->move(9.1, 12, true); // Was 10.7

  // Score the first stack
  // safeStack();
  powerIntake(0);
  pros::delay(1350);
  // tiltMotor->move_absolute(0, 60);
  powerIntake(-60);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-15.5, 60, false);

  // Turn and reset to get in position to score the next tower
  powerIntake(0);
  pid->pivot(-90);
  steps = 40;
  pid->tareDesiredHeading();
  for (int i = 0; i < steps; i++) {
    pid->driveStraight(-50);
    pros::delay(20);
  }
  // gyro->tarePosition();
  // pid->tareDesiredHeading();

  // Get the center tower cube
  powerIntake(115);
  pid->move(46);
  pros::delay(500);
  // Set the cube in the intake
  powerIntake(-70);
  pros::delay(500);
  powerIntake(0);

  // Score the second center tower
  // liftMotor->move_absolute(490, 100);
  pid->velocityMove(-6, 40);
  pros::delay(500);
  pid->velocityMove(8, 40);
  powerIntake(-100);
  pros::delay(950);
  powerIntake(0);

  // Move into position and face the tall tower
  // liftMotor->move_absolute(0, 80);
  pid->move(-31.2);
  pid->pivot(-89.2);
  pros::delay(100);
  pid->tareDesiredHeading();

  // Get the tall tower cube
  powerIntake(127);
  pid->velocityMove(14, 65);
  pros::delay(700);
  // Set the cube in the intake
  powerIntake(-70);
  pros::delay(600);
  powerIntake(0);

  // Score third the tower
  // liftMotor->move_absolute(614, 100);
  pid->velocityMove(-5, 40);
  pros::delay(800);
  pid->velocityMove(9.5, 40);
  powerIntake(-115);
  pros::delay(1000);
  powerIntake(0);

  // Get in position to intake the second row of cubes
  pid->move(-3.8);
  pid->pivot(90);
  // liftMotor->move_absolute(0, 100);
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
  powerIntake(127);
  pid->velocityMove(65, 70);
  pid->velocityMove(54, 48);

  pid->pivot(90);
  pid->move(20);

  // Strafe align for scoring
  steps = 88;
  gyro->tarePosition();
  pid->tareDesiredHeading();
  for (int i = 0; i < steps; i++) {
    pid->strafeStraight(-85);
    pros::delay(20);
  }

  // Move into position to score
  pid->move(10.6);

  powerIntake(-35);
  pros::delay(400);
  powerIntake(-13);
  // Score the stack
  pid->powerDrive(10, 10);
  // safeStack();
  powerIntake(0);
  pros::delay(1200);

  // Move the tray back and let go go of the stack
  pid->velocityMove(-18, 55, false);
  // tiltMotor->move_absolute(0, 60);
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
