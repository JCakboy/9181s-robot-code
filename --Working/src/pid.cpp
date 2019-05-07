#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

// Create the default constructor
PID::PID() = default;

// Sets the move PID values
void PID::setMovePID(double movekp, double movekd, double straightkp, double straightkd) {
  PID::movekp = movekp;
  PID::movekd = movekd;
  PID::straightkp = straightkp;
  PID::straightkd = straightkd;
}

// Sets the pivot PID values
void PID::setPivotPID(double pivotkp, double pivotkd) {
  PID::pivotkp = pivotkp;
  PID::pivotkd = pivotkd;
}

// Returns the calculated full gear ratio
double PID::getGearRatio() {
  double in = 1;
  double out = 1;
  double wheelDiameter = 4; // in inches
  // Calculates and returns the full gear ratio
  return (360 * out) / (wheelDiameter * PI * in);
}

// Sets the brake mode
void PID::setBrakeMode() {
  frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
  backLeftDrive->set_brake_mode(BRAKE_BRAKE);
  frontRightDrive->set_brake_mode(BRAKE_BRAKE);
  backRightDrive->set_brake_mode(BRAKE_BRAKE);
}

// Resets the motor encoders
void PID::resetEncoders() {
  frontLeftDrive->tare_position();
  backLeftDrive->tare_position();
  frontRightDrive->tare_position();
  backRightDrive->tare_position();
}

// Returns the power given the minimum and maximum power restraints
double PID::checkPower(double power) {
  if (!power);
  else if (util::abs(power) > MAX_POWER) power = power / util::abs(power) * MAX_POWER;
  else if (util::abs(power) < MIN_POWER) power = power / util::abs(power) * MIN_POWER;
  return power;
}

// Powers the drive motors based on the given powers
void PID::powerDrive(int powerLeft, int powerRight) {
  frontLeftDrive->move(powerLeft);
  frontRightDrive->move(powerRight);
  backLeftDrive->move(powerLeft);
  backRightDrive->move(powerRight);
}

// Ensures the robot drives straight
void PID::driveStraight(int power) {
  double kp = straightkp;
  double kd = straightkd;
  double master = 0;
  double partner = 0;
  int powerLeft = 0;
  int powerRight = 0;

  // Assign master to the faster moving side
  if (util::abs(frontLeftDrive->get_position()) > util::abs(frontRightDrive->get_position())) {
    master = frontLeftDrive->get_position();
    partner = frontRightDrive->get_position();
  } else if (util::abs(frontLeftDrive->get_position()) < util::abs(frontRightDrive->get_position())) {
    master = frontRightDrive->get_position();
    partner = frontLeftDrive->get_position();
  }

  // Calculate the difference between the wheels from the two sides of the chassis
  double error = master - partner;
  double derivative = error - straightle;
  straightle = error;

  // Decrease the side that is moving faster than the other
  if (util::abs(frontLeftDrive->get_position()) >= util::abs(frontRightDrive->get_position())) {
    powerLeft = power - (error * kp - derivative * kd);
    powerRight = power;
  }
  else if (util::abs(frontLeftDrive->get_position())  < util::abs(frontRightDrive->get_position())) {
    powerLeft = power;
    powerRight = power - (error * kp - derivative * kd);
  }

  // Ensure that power is within constraints
  powerLeft = checkPower(powerLeft);
  powerRight = checkPower(powerRight);

  // Issue the power to the motors
  powerDrive(powerLeft, powerRight);
}

// Moves the robot the given amount of inches to the desired location
void PID::move(double inches) {
  double kp = movekp;
  double kd = movekd;
  double currentDistance = 0;
  double error = 0;
  double derivative = 0;
  double lastError = 0;
  double power = MIN_POWER * util::abs(inches) / inches;

  // Convert targetDistance from inches to degrees
  double targetDistance = inches * getGearRatio();

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  error = targetDistance - currentDistance;

  // Accelerates to the max speed smoothly
  if (power > 0)
    while (util::abs(power) < MAX_POWER && util::abs(power) < util::abs(kp * error)) {
      power *= 1.4;
      driveStraight(power);
      pros::delay(60);

      currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
      error = targetDistance - currentDistance;
    }
  else if (power < 0) {
    while (util::abs(power) < MAX_POWER && util::abs(power) < util::abs(kp * error)) {
      power *= 1.1;
      driveStraight(power);
      pros::delay(50);

      currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
      error = targetDistance - currentDistance;
    }
  }

  currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
  error = targetDistance - currentDistance;

  while (util::abs(error) >= 5) {
    currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
    // Calculate proportional error term and the derivative term
    error = targetDistance - currentDistance;
    derivative = error - lastError;
    lastError = error;

    // Determine power and checks if power is within constraints
    power = (error * kp) + (derivative * kd);
    power = checkPower(power);

    if(error <= 200) {
      powerDrive(power - 3, power);
    }
    else {
      // Passes the requested power to the straight drive method
      driveStraight(power);
      LCD::printDebugInformation();
    }

    pros::delay(20);
  }
  powerDrive(0, 0);
}

void PID::velocityMove(double inches, double power) {
  // Convert the inches to degrees
  double degrees = inches * pid->getGearRatio();

  pid->resetEncoders();
  // While the target has not been reached, power the drive
  while (util::abs(frontLeftDrive->get_position()) < util::abs(degrees) || util::abs(frontRightDrive->get_position()) < util::abs(degrees)) {
    driveStraight(power * util::abs(degrees) / degrees);
    LCD::printDebugInformation();
    pros::delay(20);
  }
  // When the target is reached, stop
  powerDrive(0, 0);
}


void PID::customMove(double leftInches, double rightInches) {
  double kp = movekp;
  double kd = movekd;

  double leftTarget = leftInches * getGearRatio();
  double rightTarget = rightInches * getGearRatio();

  double leftError = leftTarget;
  double rightError = rightTarget;
  double lastLeftError = leftTarget;
  double lastRightError = rightTarget;

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  while (util::abs(leftError) >= 5 || util::abs(rightError) >= 5) {
    // Calculate proportional error term and the derivative term
    leftError = leftTarget - frontLeftDrive->get_position();
    rightError = rightTarget - frontRightDrive->get_position();

    double leftDerivative = leftError - lastLeftError;
    double rightDerivative = rightError - lastRightError;

    lastLeftError = leftError;
    lastRightError = rightError;

    // Determine power and checks if power is within constraints
    double leftPower = (leftPower * kp) + (leftDerivative * kd);
    double rightPower = (rightError * kp) + (rightDerivative * kd);
    leftPower = checkPower(leftPower);
    rightPower = checkPower(rightPower);

    // Passes the requested power to the motors
    powerDrive(leftPower, rightPower);
    LCD::printDebugInformation();
    pros::delay(20);
  }
}

// Pivots the robot the given amount of degrees
void PID::pivot(double degrees) {
  double kp = pivotkp;
  double kd = pivotkd;
  double currentBearing = gyro1->get_value();
  double error = 10;
  double derivative = 0;
  double lastError = 0;
  int power = 0;

  gyro1->reset();

  /* Converts targetBearing to a 10th of a degree.
     Gyro is never reset, so currentBearing is added to targetBearing */
  double targetBearing = (degrees * 982 / 90);

  while (util::abs(error) >= 10) {
    currentBearing = gyro1->get_value();
    // Calculates difference from targetBearing
    error = targetBearing - currentBearing;
    derivative = error - lastError;
    lastError = error;

    // Determines power and checks if power is within constraints
    power = (error * kp) + (derivative  * kd);
    power = checkPower(power);

    // Powers drive motors to pivot
    powerDrive(power, -power);
    LCD::printDebugInformation();
    pros::delay(20);
  }

}

// Front resets the robot using the ultrasonics
void PID::frontReset(double inches) {
  // Get the current front ultrasonic distance
  double value = frontUltrasonic->get_value();

  // If no object is detected, ignore this call
  if (value == -1) return;

  // Apply a constant value to the ultrasonic
  value -= 92;
  // Convert that distance to inches
  double currentInches = value / 25.4;
  // Calculate the error
  double error = currentInches - inches;
  // Drive to the desired position
  move(error);
}

// Back resets the robot using the ultrasonics
void PID::backReset(double inches) {
  // Get the current ultrasonic distances
  double valueLeft = backLeftUltrasonic->get_value();
  double valueRight = backRightUltrasonic->get_value();
  double valueAvg = (valueLeft + valueRight) / 2;

  // If no object is detected, ignore this call
  if (valueLeft == -1 || valueRight == -1) return;

  // Apply a constant value to the ultrasonic
  valueAvg -= 45;
  // Convert that distance to inches
  double currentInches = valueAvg / 25.4;
  // Calculate the error
  double error = inches - currentInches;
  // Drive to the desired position
  move(error);
}

// Front resets the robot against the wall using ultrasonics, aligning it
void PID::frontAlignReset() {
  while (frontUltrasonic->get_value() > 400) {
    powerDrive(95, 95);
    LCD::printDebugInformation();
    pros::delay(20);
  }
  while (frontUltrasonic->get_value() > 110) {
    powerDrive(60, 60);
    LCD::printDebugInformation();
    pros::delay(20);
  }
  pros::delay(100);
  powerDrive(0, 0);
}

// Back resets the robot against the wall using ultrasonics, aligning it
void PID::backAlignReset() {
  while (backLeftUltrasonic->get_value() > 48 || backRightUltrasonic->get_value() > 48) {
    powerDrive(-60, -60);
    LCD::printDebugInformation();
    pros::delay(20);
  }
  powerDrive(0, 0);
}
