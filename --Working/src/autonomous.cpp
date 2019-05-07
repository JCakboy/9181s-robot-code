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

void doubleShot() {
  flywheelMotor->move(127);
  while (flywheelMotor->get_actual_velocity() < 580)
    pros::delay(1);
  indexMotor->move(127);
  pros::delay(80);
  indexMotor->move(0);
  pros::delay(30);
  pid->move(25);
  indexMotor->move(127);
  intakeMotor->move(127);
  pros::delay(300);
  indexMotor->move(0);
  intakeMotor->move(0);
}

void visionAlign() {
  return;
  while (true) {
    if (flagVision != NULL) {
      // Prepare variables for decision
		int sigID = LCD::isAutonomousBlue() ? 1 : 2;
		pros::vision_object_s_t sig = flagVision->get_by_sig(0, sigID);
		int middle = util::sign(sig.x_middle_coord);
		int diff = middle - 158;
		// If a signature is detected, lock to it. Otherwise, give control back over to the driver
		if (middle > -2000)
			if (util::abs(diff) > 5) {
				int turnPower = diff / 3 + 10 * util::abs(diff) / diff;
        int leftPower = turnPower;
      	int rightPower = -turnPower;
      	frontLeftDrive->move(leftPower);
      	backLeftDrive->move(leftPower);
      	frontRightDrive->move(rightPower);
      	backRightDrive->move(rightPower);
      } else
				break;
		else;
  	}
  }
}

void autonomousSkills() {}

void autonomousBlueFlags() {
  // Grab the ball in the angled cap
  flywheelMotor->move(127);
  intakeMotor->move(127);
  pid->move(43.4);
  pid->move(-34.4);
  pid->pivot(91);
  intakeMotor->move(0);
  pid->powerDrive(0, 0);

  // Shake the lift
  liftMotor->move(127);
  pros::delay(150);
  liftMotor->move(-127);
  pros::delay(500);
  liftMotor->move_absolute(260, 127);

  // Vision align to the flags and shoot
  visionAlign();
  doubleShot();
  flywheelMotor->move(0);

  // Drive forward and toggle the low flag
  pid->move(10);
  pid->move(-7.5);

  // Grab the ball on the flat cap
  intakeMotor->move(127);
  indexMotor->move(13);
  pid->pivot(-93);
  pid->resetEncoders();
  pid->velocityMove(6.9, 50);
  flywheelMotor->move(88);
  pid->move(-8.5);
  pros::delay(750);

  // Flip the flat cap
  indexMotor->move(0);
  intakeMotor->move(-100);
  pid->move(30.5);

  // Align to the middle flags
  pid->pivot(60);
  visionAlign();
  pid->move(5.5);

  // Shoot the ball
  intakeMotor->move(127);
  indexMotor->move(127);
  pros::delay(200);
  intakeMotor->move(0);
  indexMotor->move(0);

  // Drive forward and toggle the low flag
  pid->move(8.75);
}

void autonomousRedFlags() {
  // Grab the ball in the angled cap
  flywheelMotor->move(127);
  intakeMotor->move(127);
  pid->move(44.8);
  pid->move(-36.4);
  pid->pivot(-90);
  intakeMotor->move(0);
  pid->powerDrive(0, 0);

  // Shake the lift
  liftMotor->move(127);
  pros::delay(150);
  liftMotor->move(-127);
  pros::delay(500);
  liftMotor->move_absolute(260, 127);

  // Vision align to the flags and shoot
  visionAlign();
  doubleShot();
  flywheelMotor->move(0);

  // Drive forward and toggle the low flag
  pid->move(10.55);
  pid->move(-7.65);
  intakeMotor->move(127);
  indexMotor->move(13);

  // Grab the ball on the flat cap
  pid->pivot(93);
  pid->resetEncoders();
  pid->velocityMove(6.4, 50);
  flywheelMotor->move(88);
  pid->move(-8.5);
  pros::delay(750);

  // Flip the flat cap
  indexMotor->move(0);
  intakeMotor->move(-100);
  pid->move(30.5);

  // Align to the middle flags
  pid->pivot(-52);
  visionAlign();
  pid->move(6);

  // Shoot the ball
  intakeMotor->move(127);
  indexMotor->move(127);
  pros::delay(200);
  intakeMotor->move(0);
  indexMotor->move(0);

  // Drive forward and toggle the low flag
  pid->move(9);
}

void autonomousBlueFar() {
  // Grab the ball in the angled cap
  intakeMotor->move(127);
  pid->move(43.4);
  pid->move(-4.5);
  pid->pivot(-66);
  pid->powerDrive(0, 0);

  // Shake the lift
  liftMotor->move(127);
  pros::delay(150);
  liftMotor->move(-127);
  pros::delay(800);

  // Flip the flat cap
  pid->move(9);
  liftMotor->move(127);
  pid->powerDrive(0, 0);
  pros::delay(1000);

  // Start the flywheel
  flywheelMotor->move(98);

  // Park
  pid->move(-38.75);

  // Turn and reset against the center platform
  pid->pivot(90);
  pid->powerDrive(50, 50);
  pros::delay(650);

  // Turn and face the far flags
  pid->move(-3);
  pid->pivot(47.8);
  pros::delay(20);
  pid->powerDrive(0, 0);

  // Wait until the last second to shoot the balls
  pros::delay(1850);

  // Shoot the two balls
  indexMotor->move(127);
  pros::delay(80);
  indexMotor->move(0);
  flywheelMotor->move(86);
  pros::delay(1000);
  intakeMotor->move(127);
  indexMotor->move(127);
  pros::delay(750);
  intakeMotor->move(0);
  indexMotor->move(0);

  // Power down the flywheel
  flywheelMotor->move(0);
}

void autonomousRedFar() {
  // Grab the ball in the angled cap
  intakeMotor->move(127);
  pid->move(43.4);
  pid->move(-3.85);
  pid->pivot(71.5);
  pid->powerDrive(0, 0);

  // Shake the lift
  liftMotor->move(127);
  pros::delay(150);
  liftMotor->move(-127);
  pros::delay(800);

  // Flip the flat cap
  pid->move(8.5);
  liftMotor->move(127);
  pid->powerDrive(0, 0);
  pros::delay(1000);

  // Start the flywheel
  flywheelMotor->move(97);

  // Park
  pid->move(-41.85);

  // Turn and reset against the center platform
  pid->pivot(-90);
  pid->powerDrive(50, 50);
  pros::delay(650);

  // Turn and face the far flags
  pid->move(-3);
  pid->pivot(-47);
  pid->powerDrive(0, 0);

  // Wait until the last second to shoot the balls
  pros::delay(1850);

  // Shoot the two balls
  indexMotor->move(127);
  pros::delay(78);
  indexMotor->move(0);
  flywheelMotor->move_voltage(84.75/127*12000);
  pros::delay(1000);
  intakeMotor->move(127);
  indexMotor->move(127);
  pros::delay(750);
  intakeMotor->move(0);
  indexMotor->move(0);

  // Power down the flywheel
  flywheelMotor->move(0);
}

void autonomousOther(int selected) {
  if (selectedAutonomous == -1) {
    pid->move(40);
  } else if (selectedAutonomous == -2)
  pid->move(-40);
}

void autonomous() {
  switch (selectedAutonomous) {
    case 0:
      autonomousSkills();
      break;
    case 1:
      autonomousBlueFlags();
      break;
    case 2:
      autonomousRedFlags();
      break;
    case 3:
      autonomousBlueFar();
      break;
    case 4:
      autonomousRedFar();
      break;
    default:
      autonomousOther(selectedAutonomous);
      break;
  }
}
