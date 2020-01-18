#include "main.h"

// Dump ports namespace for ease of use
using namespace ports;

// Create the default constructor
PID::PID() = default;

// Returns the calculated full gear ratio
double PID::getGearRatio() {
  double in = 1;
  double out = 1;
  double wheelDiameter = 4; // in inches
  // Calculates and returns the full gear ratio
  return (360 * out) / (wheelDiameter * PI * in);
}

// The logic to continue PID loops
bool PID::continuePIDLoop(bool expr) {
  if (controllerXStop && controllerMain->get_digital(BUTTON_X))
    return false;
  if (noStop)
    return true;
  return expr;
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

// Sets the move positional PID values
void PID::setMovePosPID(double movekp, double moveki, double movekd) {
  PID::movekp = movekp;
  PID::moveki = moveki;
  PID::movekd = movekd;
}

// Sets the move velocity PID values
void PID::setMoveVelPID(double velocitykp, double velocityki, double velocitykd) {
  PID::velocitykp = velocitykp;
  PID::velocityki = velocityki;
  PID::velocitykd = velocitykd;
}

// Sets the pivot PID values
void PID::setPivotPID(double pivotkp, double pivotki, double pivotkd) {
  PID::pivotkp = pivotkp;
  PID::pivotki = pivotki;
  PID::pivotkd = pivotkd;
}

// Sets the strafe positional PID values
void PID::setStrafePosPID(double inchAmount, double strafekp, double strafeki, double strafekd) {
  PID::strafekp = strafekp;
  PID::strafeki = strafeki;
  PID::strafekd = strafekd;
}

// Sets the strafe velocity PID values
void PID::setStrafeVelPID(double strafevkp, double strafevki, double strafevkd) {
  PID::strafevkp = strafevkp;
  PID::strafevki = strafevki;
  PID::strafevkd = strafevkd;
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
  double ki = velocityki;
  double kd = velocitykd;
  int powerLeft = power;
  int powerRight = power;

  double error = 0;
  double derivative = 0;

  // If the gyro is being used, the error will simply be the gyro deviation
  if (PID::velocityGyro)
    error = PID::velocityGyro->getHeading() - PID::velocityGyroValue;
  else
    error = backLeftDrive->get_position() - backRightDrive->get_position();

  // Determine how much to adjust based on the kp ki and kd values
  derivative = error - velocityle;
  velocityse += error;
  double adjust = (error * kp) + (velocityse * ki) + (derivative * kd);

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

  // Log it to the message holder if the flag is set
  if (logPIDErrors)
    messageHolder->appendLine("Vel Err: " + std::to_string(error));

  // Issue the power to the motors
  powerDrive(powerLeft, powerRight);
}

// Ensures the robot strafes straight using velocity PID
void PID::strafeStraight(int strafePower, int movePower) {
  double kp = strafevkp;
  double ki = strafevki;
  double kd = strafevkd;
  int powerFrontLeft = movePower + strafePower;
  int powerFrontRight = movePower - strafePower;
  int powerBackLeft = movePower - strafePower;
  int powerBackRight = movePower + strafePower;

  double error = 0;
  double derivative = 0;

  // If the gyro is being used, the error will simply be the gyro deviation
  if (PID::velocityGyro)
    error = PID::velocityGyro->getHeading() - PID::velocityGyroValue;
  else
    error = backLeftDrive->get_position() - backRightDrive->get_position();

  // Determine how much to adjust based on the kp and kd values
  derivative = error - strafevle;
  strafevse += error;
  double adjust = (error * kp) + (strafevse * ki) + (derivative * kd);

  // Reduce the power to the faster moving side, accounting for forwards and backwards movement
  if (strafePower > 0)
    if (adjust > 0) {
      powerFrontLeft -= adjust;
      powerBackLeft -= adjust;
    } else if (adjust < 0) {
      powerFrontRight += adjust;
      powerBackRight += adjust;
    } else;
  else if (strafePower < 0)
    if (adjust > 0) {
      powerFrontRight += adjust;
      powerBackRight += adjust;
    } else if (adjust < 0) {
      powerFrontLeft -= adjust;
      powerBackLeft -= adjust;
    } else;
  else;

  // Set the last error to the current error
  strafevle = error;

  // Log it to the message holder if the flag is set
  if (logPIDErrors)
    messageHolder->appendLine("Vel Err: " + std::to_string(error));

  LCD::setText(6, std::to_string(error));


  // Issue the power to the motors
  frontLeftDrive->move(powerFrontLeft);
  frontRightDrive->move(powerFrontRight);
  backLeftDrive->move(powerBackLeft);
  backRightDrive->move(powerBackRight);
}

// Moves the robot the given amount of inches to the desired location
void PID::move(double inches, double threshold, bool useDesiredHeading) {
  double kp = movekp;
  double ki = moveki;
  double kd = movekd;
  double currentDistance = 0;
  double error = 0;
  double errorsum = 0;
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
      velocityGyroValue = velocityGyro->getHeading();
  else;
  velocityle = 0;
  velocityse = 0;

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
    currentDistance = (backRightDrive->get_position() + backLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;
    lastError = error;
  }

  // Enter the main PID loop
  while (continuePIDLoop(util::abs(error) >= threshold)) {
    // Calculate the integral derivative term and store the current error
    derivative = error - lastError;
    errorsum += error;
    lastError = error;

    // Determine power and checks if power is within constraints
    power = (error * kp) + (derivative * kd);
    power = checkPower(power);

    // Passes the requested power to the velocity PID
    driveStraight(power);

    // Print the sensor debug information
    LCD::printDebugInformation();
    LCD::setText(6, std::to_string(error));

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    currentDistance = (backRightDrive->get_position() + backLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;

    // Log it to the message holder if the flag is set
    if (logPIDErrors)
      messageHolder->appendLine("Move Err: " + std::to_string(error));
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
      velocityGyroValue = velocityGyro->getHeading();
  else;
  velocityle = 0;
  velocityse = 0;

  // Set the current error
  error = targetDistance - currentDistance;

  // While the target has not been reached, power the drive
  while (continuePIDLoop(util::abs(error) >= threshold)) {
    driveStraight(power * util::abs(error) / error);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    currentDistance = (backRightDrive->get_position() + backLeftDrive->get_position()) / 2;
    error = targetDistance - currentDistance;

    // Log it to the message holder if the flag is set
    if (logPIDErrors)
      messageHolder->appendLine("VMove Err: " + std::to_string(error));
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
  double ki = moveki;
  double kd = movekd;
  double leftCurrentDistance = 0;
  double rightCurrentDistance = 0;
  double leftErrorSum = 0;
  double rightErrorSum = 0;
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
  while (continuePIDLoop(util::abs(leftError) >= threshold || util::abs(rightError) >= threshold)) {
    // Calculate the derivative term
    leftDerivative = leftError - lastLeftError;
    rightDerivative = rightError - lastRightError;

    // Calculate the integral term
    leftErrorSum += leftError;
    rightErrorSum += rightError;

    // Store the current error
    lastLeftError = leftError;
    lastRightError = rightError;

    // Determine power and checks if power is within constraints
    double leftPower = (leftPower * kp) + (leftErrorSum + ki) + (leftDerivative * kd);
    double rightPower = (rightError * kp) + (rightErrorSum + ki) + (rightDerivative * kd);
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

    // Log it to the message holder if the flag is set
    if (logPIDErrors)
      messageHolder->appendLine("CMove Err: " + std::to_string(leftError) + " | " + std::to_string(rightError));
  }

  // Stop the motors and exit
  powerDrive(0, 0);
}

// Strafes the robot the given amount of inches to the desired position
void PID::strafe(double inches, double threshold, bool useDesiredHeading) {
  double kp = strafekp;
  double ki = strafeki;
  double kd = strafekd;
  double currentDistance = 0;
  double error = 0;
  double errorsum = 0;
  double derivative = 0;
  double lastError = 0;
  double power = minPower * util::abs(inches) / inches;

  // Convert targetDistance from inches to degrees
  double targetDistance = inches * strafeInchAmount;

  // Prepares motors for movement
  setBrakeMode();
  resetEncoders();

  // If gyro is used for velocity PID, prepare values
  if (velocityGyro)
    if (useDesiredHeading)
      velocityGyroValue = desiredHeading;
    else
      velocityGyroValue = velocityGyro->getHeading();
  else;
  strafevle = 0;
  strafevse = 0;

  // Set the current error
  error = targetDistance - currentDistance;

  // Enter the main PID loop
  while (continuePIDLoop(util::abs(error) >= threshold)) {
    // Calculate the integral and derivative term and store the current error
    derivative = error - lastError;
    errorsum += error;
    lastError = error;

    // Determine power and checks if power is within constraints
    power = (error * kp) + (errorsum * ki) + (derivative * kd);
    power = checkPower(power);

    // Passes the requested power to the velocity PID
    strafeStraight(power * util::abs(error) / error);

    // Print the sensor debug information
    LCD::printDebugInformation();
    LCD::setText(6, std::to_string(error));

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current distance
    currentDistance = (backLeftDrive->get_position() - backRightDrive->get_position()) / 2;
    error = targetDistance - currentDistance;

    // Log it to the message holder if the flag is set
    if (logPIDErrors)
      messageHolder->appendLine("Strafe Err: " + std::to_string(error));
  }

  // Stop the motors and exit
  powerDrive(0, 0);
}

// Pivots the robot relative the given amount of degrees, based on the current desired heading
void PID::pivot(double degrees, double threshold, bool modifyDesiredHeading) {
  // Pivot to the requeseted heading
  PID::pivotAbsolute(desiredHeading + degrees, threshold, modifyDesiredHeading);
}

// Pivots the robot relative the given amount of degrees, based on the current heading of the robot
void PID::pivotRelative(double degrees, double threshold, bool modifyDesiredHeading) {
  // Pivot to the requeseted heading
  PID::pivotAbsolute(ports::gyro->getHeading() + degrees, threshold, modifyDesiredHeading);
}

// Pivots the robot to the heading given
void PID::pivotAbsolute(double heading, double threshold, bool modifyDesiredHeading) {
  double kp = pivotkp;
  double ki = pivotki;
  double kd = pivotkd;
  double currentBearing = ports::gyro->getHeading();
  double error = 10;
  double errorsum = 0;
  double derivative = 0;
  double lastError = 0;
  int power = 0;

  // Converts targetBearing to a 10th of a degree
  double targetBearing = heading;

  // Calculate the error before the loop
  error = targetBearing - currentBearing;

  while (continuePIDLoop(util::abs(error) >= threshold)) {
    // Calculate the integral and derivative term and store the current error
    derivative = error - lastError;
    errorsum += error;
    lastError = error;

    // Determines power and checks if power is within constraints
    power = (error * kp) + (errorsum * ki) + (derivative  * kd);
    power = checkPower(power);

    // Passes the requested power to the motors
    powerDrive(power, -power);

    // Print the sensor debug information
    LCD::printDebugInformation();

    // Run every 20 ms
    pros::delay(20);

    // Update the error and current bearing
    currentBearing = ports::gyro->getHeading();
    error = targetBearing - currentBearing;

    if (abs(error) < 2) errorsum = 0;

    // Log it to the message holder if the flag is set
    if (logPIDErrors)
      messageHolder->appendLine("Pivot Err: " + std::to_string(error));
  }

  // If requested, modify the current desired heading
  if (modifyDesiredHeading)
    PID::desiredHeading = heading;
}

// Sets the desired heading to the current heading
void PID::tareDesiredHeading() {
  PID::setRelativeDesiredHeading(0);
}

// Sets the absolute desired heading
void PID::setAbsoluteDesiredHeading(double heading) {
  PID::desiredHeading = heading;
  PID::velocityGyroValue = desiredHeading;
}

// Sets the desired heading relative to the current heading
void PID::setRelativeDesiredHeading(double heading) {
  PID::desiredHeading = velocityGyro->getHeading() + heading;
  PID::velocityGyroValue = desiredHeading;
}

#if ATTACH_DEBUGGING
// Set the debugging flag to force PID loops to run non-stop; useful for tuning
void PID::setNoStopDebug(bool flag) {
  PID::noStop = flag;
}
void PID::setControllerXStop(bool flag) {
  PID::controllerXStop = flag;
}
void PID::setLoggingDebug(bool flag) {
  PID::logPIDErrors = flag;
}
#else
// Ignore all calls to these methods if debugging is not enabled
void PID::setNoStopDebug(bool flag) {}
void PID::setControllerXStop(bool flag) {}
void PID::setLoggingDebug(bool flag) {}
#endif
