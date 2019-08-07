#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

// Create the default constructor
PID::PID() = default;

// Returns the calculated full gear ratio
double PID::getGearRatio() {
  double in = 1;
  double out = 2;
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

// Returns the power given the minimum and maximum power restraints
double PID::checkPower(double power) {
  if (!power);
  else if (util::abs(power) > maxPower) power = power / util::abs(power) * maxPower;
  else if (util::abs(power) < minPower) power = power / util::abs(power) * minPower;
  return power;
}

// Sets the power limits of PID
void PID::setPowerLimits(int maxPower, int minPower) {
  PID::maxPower = maxPower;
  PID::minPower = minPower;
}

// Sets the move PID values
void PID::setMovePID(double movekp, double movekd, double velocitykp, double velocitykd) {
  PID::movekp = movekp;
  PID::movekd = movekd;
  PID::velocitykp = velocitykp;
  PID::velocitykd = velocitykd;
}

// Sets the pivot PID values
void PID::setPivotPID(double rightAngleAmount, double pivotkp, double pivotkd) {
  PID::rightAngle = rightAngleAmount;
  PID::pivotkp = pivotkp;
  PID::pivotkd = pivotkd;
}

// Sets the forward acceleration values
void PID::setForwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay) {
  PID::accelerationForwardCoeff = accelerationCoeff;
  PID::accelerationForwardConst = accelerationConst;
  PID::accelerationForwardDelay = accelerationDelay;
}

// Sets the backward acceleration values
void PID::setBackwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay) {
  PID::accelerationBackwardCoeff = accelerationCoeff;
  PID::accelerationBackwardConst = accelerationConst;
  PID::accelerationBackwardDelay = accelerationDelay;
}

// Sets the gyro to be used during velocity PID
void PID::setVelocityGyro(Gyro * g) {
  PID::velocityGyro = g;
  PID::velocityGyroValue = 0;
}

// Resets the motor encoders
void PID::resetEncoders() {
  frontLeftDrive->tare_position();
  backLeftDrive->tare_position();
  frontRightDrive->tare_position();
  backRightDrive->tare_position();
}

// Powers the drive motors based on the given powers
void PID::powerDrive(int powerLeft, int powerRight) {
  frontLeftDrive->move(powerLeft);
  frontRightDrive->move(powerRight);
  backLeftDrive->move(powerLeft);
  backRightDrive->move(powerRight);
}

// Ensures the robot drives straight using velocity PID
void PID::driveStraight(int power) {
  double kp = velocitykp;
  double kd = velocitykd;
  int powerLeft = power;
  int powerRight = power;

  double error = 0;
  double derivative = 0;

  // If the gyro is being used, the error will simply be the gyro deviation
  if (PID::velocityGyro)
    error = PID::velocityGyro->getValue() - PID::velocityGyroValue;
  else
    error = frontLeftDrive->get_position() - frontRightDrive->get_position();

  // Determine how much to adjust based on the kp and kd values
  derivative = error - velocityle;
  double adjust = (error * kp - derivative * kd);

  // Reduce the power to the faster moving side, accounting for forwards and backwards movement
  if (power > 0)
    if (adjust > 0)
      powerLeft -= adjust;
    else if (adjust < 0)
      powerRight += adjust;
    else;
  else if (power < 0)
    if (adjust > 0)
      powerRight += adjust;
    else if (adjust < 0)
      powerLeft -= adjust;
    else;
  else;

  // Set the last error to the current error
  velocityle = error;

  // Issue the power to the motors
  powerDrive(powerLeft, powerRight);
}

// Moves the robot the given amount of inches to the desired location
void PID::move(double inches, double threshold, bool useDesiredHeading) {
  double kp = movekp;
  double kd = movekd;
  double currentDistance = 0;
  double error = 0;
  double derivative = 0;
  double lastError = 0;
  double power = minPower * util::abs(inches) / inches;

  // Convert targetDistance from inches to degrees
  double targetDistance = inches * getGearRatio();

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  // If gyro is used for velocity PID, prepare values
  if (velocityGyro)
    if (useDesiredHeading)
      velocityGyroValue = desiredHeading;
    else
      velocityGyroValue = velocityGyro->getValue();
  velocityle = 0;

  // Set the current error
  error = targetDistance - currentDistance;

  // Sets the acceleration parameters to prevent jerking
  double accelCoeff = 1;
  double accelConst = 1;
  double accelDelay = 50;

  // Determine which acceleration values to use
  if (power > 0) {
    accelCoeff = PID::accelerationForwardCoeff;
    accelConst = PID::accelerationForwardConst;
    accelDelay = PID::accelerationForwardDelay;
  } else if (power < 0) {
    accelCoeff = PID::accelerationBackwardCoeff;
    accelConst = PID::accelerationBackwardConst;
    accelDelay = PID::accelerationBackwardDelay;
  }

  // Accelerate to the max speed smoothly
  while (util::abs(power) < maxPower && util::abs(power) < util::abs(kp * error)) {
    // Increase the power
    power *= accelCoeff;
    power = power + accelConst * util::abs(inches) / inches;
    driveStraight(power);

    // Repeat with the set delay
    pros::delay(accelDelay);

    // Update the error and current distance
    currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;
    lastError = error;
  }

  // Enter the main PID loop
  while (util::abs(error) >= threshold || noStop) {
    // Calculate the derivative term and store the current error
    derivative = error - lastError;
    lastError = error;

    // Determine power and checks if power is within constraints
    power = (error * kp) + (derivative * kd);
    power = checkPower(power);

    // Passes the requested power to the velocity PID
    driveStraight(power);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;
  }

  // Stop the motors and exit
  powerDrive(0, 0);
}
void PID::move(double inches, bool useDesiredHeading) {
  PID::move(inches, 12, useDesiredHeading);
}

// Moves the robot the given amount of inches while only using velocity PID
void PID::velocityMove(double inches, double power, double threshold, bool useDesiredHeading) {
  double currentDistance = 0;
  double error = 0;

  // Convert the inches to degrees
  double targetDistance = inches * pid->getGearRatio();

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  // If gyro is used for velocity PID, prepare values
  if (velocityGyro)
    if (useDesiredHeading)
      velocityGyroValue = desiredHeading;
    else
      velocityGyroValue = velocityGyro->getValue();
  velocityle = 0;

  // Set the current error
  error = targetDistance - currentDistance;

  // While the target has not been reached, power the drive
  while (util::abs(error) >= threshold || noStop) {
    driveStraight(power * util::abs(targetDistance) / targetDistance);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    currentDistance = (frontRightDrive->get_position() + frontLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;
  }

  // Stop the motors and exit
  powerDrive(0, 0);
}
void PID::velocityMove(double inches, double power, bool useDesiredHeading) {
  PID::velocityMove(inches, power, 12, useDesiredHeading);
}

// Moves the robot with custom left and right targets while only using positional PID
void PID::customMove(double leftInches, double rightInches, double threshold) {
  double kp = movekp;
  double kd = movekd;
  double leftCurrentDistance = 0;
  double rightCurrentDistance = 0;
  double leftDerivative = 0;
  double rightDerivative = 0;

  // Convert the inches to degrees
  double leftTargetDistance = leftInches * getGearRatio();
  double rightTargetDistance = rightInches * getGearRatio();

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  // Set the current error
  double leftError = leftTargetDistance - leftCurrentDistance;
  double rightError = rightTargetDistance - rightCurrentDistance;
  double lastLeftError = leftTargetDistance;
  double lastRightError = rightTargetDistance;

  // While the target has not been reached, power the drive
  while (util::abs(leftError) >= threshold || util::abs(rightError) >= threshold || noStop) {
    // Calculate the derivative term
    leftDerivative = leftError - lastLeftError;
    rightDerivative = rightError - lastRightError;

    // Store the current error
    lastLeftError = leftError;
    lastRightError = rightError;

    // Determine power and checks if power is within constraints
    double leftPower = (leftPower * kp) + (leftDerivative * kd);
    double rightPower = (rightError * kp) + (rightDerivative * kd);
    leftPower = checkPower(leftPower);
    rightPower = checkPower(rightPower);

    // Passes the requested power to the motors
    powerDrive(leftPower, rightPower);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    leftCurrentDistance = (frontLeftDrive->get_position() + backLeftDrive->get_position()) / 2;
    rightCurrentDistance = (frontRightDrive->get_position() + backRightDrive->get_position()) / 2;
    leftError = leftTargetDistance - leftCurrentDistance;
    rightError = rightTargetDistance - rightCurrentDistance;
  }

  // Stop the motors and exit
  powerDrive(0, 0);
}

// Pivots the robot relative the given amount of degrees
void PID::pivot(double degrees, double threshold, bool modifyDesiredHeading) {
  // If requested, modify the current desired heading
  if (modifyDesiredHeading)
    PID::desiredHeading += degrees;

  // Pivot to the requeseted heading
  PID::pivotAbsolute(ports::gyro->getValue() + degrees, threshold, false);
}

// Pivots the robot relative the given amount of degrees factoring in the current and desired heading
void PID::pivotDesired(double degrees, double threshold) {
  // Modify the current desired heading
  PID::desiredHeading += degrees;

  PID::pivotAbsolute(desiredHeading, threshold);
}

// Pivots the robot to the heading given
void PID::pivotAbsolute(double heading, double threshold, bool modifyDesiredHeading) {
  double kp = pivotkp;
  double kd = pivotkd;
  double currentBearing = ports::gyro->getValue();
  double error = 10;
  double derivative = 0;
  double lastError = 0;
  int power = 0;

  // Converts targetBearing to a 10th of a degree
  double targetBearing = (heading * rightAngle / 90);

  while (util::abs(error) >= threshold || noStop) {
    // Calculate the derivative term and store the current error
    derivative = error - lastError;
    lastError = error;

    // Determines power and checks if power is within constraints
    power = (error * kp) + (derivative  * kd);
    power = checkPower(power);

    // Passes the requested power to the motors
    powerDrive(power, -power);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current bearing
    currentBearing = ports::gyro->getValue();
    error = targetBearing - currentBearing;
  }

  // If requested, modify the current desired heading
  if (modifyDesiredHeading)
    PID::desiredHeading += heading;
}

// Sets the desired heading to the current heading
void PID::tareDesiredHeading() {
  PID::setRelativeDesiredHeading(0);
}

// Sets the absolute desired heading
void PID::setAbsoluteDesiredHeading(double heading) {
  PID::desiredHeading = heading;
}

// Sets the desired heading relative to the current heading
void PID::setRelativeDesiredHeading(double heading) {
  PID::desiredHeading = ports::gyro->getValue() + heading;
}

#if ATTACH_DEBUGGING
// Set the debugging flag to force PID loops to run non-stop; useful for tuning
void PID::setNoStopDebug(bool flag) {
  PID::noStop = flag;
}
#else
// Ignore all calls to this method if debugging is not enabled
void PID::setNoStopDebug(bool flag) {}
#endif
