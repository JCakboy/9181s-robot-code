#include "main.h"
#include <cmath>
#include <vector>
#include "drive.hpp"

void DriveControl::runLeftMotors(int voltage) {
  for (const auto & motor : DriveControl::leftMotors)
    motor.move(voltage);
}

void DriveControl::runRightMotors(int voltage) {
  for (const auto & motor : DriveControl::rightMotors)
    motor.move(voltage);
}

void DriveControl::setLeftBrake(pros::motor_brake_mode_e_t mode) {
  for (const auto & motor : DriveControl::leftMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

void DriveControl::setRightBrake(pros::motor_brake_mode_e_t mode) {
  for (const auto & motor : DriveControl::rightMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

bool DriveControl::runLeftMotorsRelative(int target, int threshold) {
  return DriveControl::runMotorsRelative(DriveControl::leftPID, DriveControl::leftMotors, target, threshold);
}

bool DriveControl::runRightMotorsRelative(int target, int threshold) {
  return DriveControl::runMotorsRelative(DriveControl::rightPID, DriveControl::rightMotors, target, threshold);
}

bool DriveControl::runMotorsRelative(PID * pid, std::vector<pros::Motor> motors, int target, int threshold) {
  if (!usePID) if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : motors)
      motor.move_relative(target, 127);
    lock->give();
  } else; else if (motors.size() > 0) {

    // Calculate the current error
    int error = 0;
    for (const auto & motor : motors)
      error += motor.get_position();
    error = error / motors.size() - target;

    pid->Se += error; // Add the current error to the sum of all errors
    int de = (error - pid->lastError) / pid->dt; // Calculate the change in error

    if (error <= threshold) return true;

    int p = error * pid->kp;
    int i = util::limitX(pid->limit, pid->Se * pid->ki);
    int d = de * pid->kd;

    if (lock->take(MUTEX_WAIT_TIME)) {
      for (const auto & motor : motors)
        motor.move(p + i + d);
      lock->give();
    }
    pid->lastError = error;

    return false;
  }
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor leftMotor, pros::Motor rightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::leftMotors.push_back(leftMotor);
  DriveControl::rightMotors.push_back(rightMotor);

  DriveControl::usePID = false;
  DriveControl::leftPID = NULL;
  DriveControl::rightPID = NULL;
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::addLeftMotor(frontLeftMotor);
  DriveControl::addLeftMotor(rearLeftMotor);
  DriveControl::addRightMotor(frontRightMotor);
  DriveControl::addRightMotor(rearRightMotor);

  DriveControl::usePID = false;
  DriveControl::leftPID = NULL;
  DriveControl::rightPID = NULL;
}

void DriveControl::addLeftMotor(pros::Motor motor) {
  DriveControl::leftMotors.push_back(motor);
}

void DriveControl::addRightMotor(pros::Motor motor) {
  DriveControl::rightMotors.push_back(motor);
}

void DriveControl::clearMotors() {
  DriveControl::clearLeftMotors();
  DriveControl::clearRightMotors();
}

void DriveControl::clearLeftMotors() {
  DriveControl::leftMotors.clear();
}

void DriveControl::clearRightMotors() {
  DriveControl::rightMotors.clear();
}
/* currently not working, may work when pros 3 is released
bool DriveControl::removeLeftMotor(pros::Motor motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::leftMotors.size(); i++)
    if (DriveControl::leftMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::leftMotors.erase(DriveControl::leftMotors.begin()+i);
  return found;
}

bool DriveControl::removeRightMotor(pros::Motor motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::rightMotors.size(); i++)
    if (DriveControl::rightMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::rightMotors.erase(DriveControl::rightMotors.begin()+i);
  return found;
}
*/

void DriveControl::setPID(int dt, double kp, double ki, double kd, int limit) {
  if (usePID) clearPID();
  DriveControl::usePID = true;
  DriveControl::leftPID = new PID(dt, kp, ki, kd, limit);
  DriveControl::rightPID = new PID(dt, kp, ki, kd, limit);
}

PID * DriveControl::getLeftPID() {
  return DriveControl::leftPID;
}

PID * DriveControl::getRightPID() {
  return DriveControl::rightPID;
}

void DriveControl::clearPID() {
  if (!usePID) return;
  DriveControl::usePID = false;
  delete DriveControl::leftPID;
  delete DriveControl::rightPID;
}

void DriveControl::moveRelative(double revolutions, int degrees, int threshold) {
  DriveControl::moveRelative(revolutions, degrees, revolutions, degrees, threshold);
}

void DriveControl::moveRelative(double leftRevolutions, int leftDegrees, double rightRevolutions, int rightDegrees, int threshold) {
  long leftTarget = std::lround(leftRevolutions * 360) + leftDegrees;
  long rightTarget = std::lround(rightRevolutions * 360) + rightDegrees;

  if (lock->take(MUTEX_WAIT_TIME)) {
    if (leftTarget != 0)
      for (const auto & motor : DriveControl::leftMotors) {
        motor.tare_position();
        motor.set_encoder_units(ENCODER_DEGREES);
        motor.set_brake_mode(BRAKE_BRAKE);
      }
    if (rightTarget != 0)
      for (const auto & motor : DriveControl::rightMotors) {
        motor.tare_position();
        motor.set_encoder_units(ENCODER_DEGREES);
        motor.set_brake_mode(BRAKE_BRAKE);
      }
    lock->give();
  }

  if (!usePID) {
    if (leftTarget != 0) DriveControl::runLeftMotorsRelative(leftTarget, threshold);
    if (rightTarget != 0) DriveControl::runRightMotorsRelative(rightTarget, threshold);

    //DriveControl::runLeftMotors((leftTarget > 0) ? 127 : -127);
    //DriveControl::runRightMotors((rightTarget > 0) ? 127 : -127);

    while (true) {
      for (const auto & motor : DriveControl::leftMotors)
        if (std::abs(motor.get_position() - leftTarget) > threshold)
          pros::delay(1);
      for (const auto & motor : DriveControl::rightMotors)
        if (std::abs(motor.get_position() - rightTarget) > threshold)
          pros::delay(1);
      /*
      bool done = false;
      for (const auto & motor : DriveControl::leftMotors)
        if (std::abs(leftTarget - motor.get_position()) < threshold)
          done = true;
      for (const auto & motor : DriveControl::rightMotors)
        if (std::abs(rightTarget - motor.get_position()) < threshold)
          done = true;
      if (done) break;
    }*/
    }
  } else {
    bool done = false;
    while (!done) {
      done = true;
      if (leftTarget != 0) done = done && DriveControl::runLeftMotorsRelative(leftTarget, threshold);
      if (rightTarget != 0) done = done && DriveControl::runRightMotorsRelative(rightTarget, threshold);
      pros::delay((leftPID->dt + rightPID->dt) / 2);
    }
  }
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  DriveControl::run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, 1.0, 1.0);
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  bool flip = flipReverse && moveVoltage < MOTOR_REVERSE_FLIP_THRESHOLD;

  moveVoltage *= moveSensitivity;
  turnVoltage *= turnSensitivity;

  int leftVoltage = util::limit127(!flip ? moveVoltage + turnVoltage : moveVoltage - turnVoltage);
  int rightVoltage = util::limit127(!flip ? moveVoltage - turnVoltage : moveVoltage + turnVoltage);

  if (lock->take(MUTEX_WAIT_TIME)) {
    setLeftBrake(leftBrake ? BRAKE_BRAKE : BRAKE_COAST);
    setRightBrake(rightBrake ? BRAKE_BRAKE : BRAKE_COAST);
    runLeftMotors(leftBrake ? 0 : leftVoltage);
    runRightMotors(rightBrake ? 0 : rightVoltage);
    lock->give();
  }
}

DriveFunction::DriveFunction(DriveControl * driveControl) {
  DriveFunction::driveControl = driveControl;
}

DriveControl & DriveFunction::getDriveControl() {
  return *(DriveFunction::driveControl);
}

void DriveFunction::turn(int degrees) {
  DriveFunction::turn(false, degrees);
}

void DriveFunction::turn(bool backward, int degrees) {
  if (backward)
    if (degrees > 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -(degrees / 90 * kt), MOTOR_MOVE_RELATIVE_THRESHOLD);
    else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, degrees / 90 * kt, 0, 0, MOTOR_MOVE_RELATIVE_THRESHOLD);
    else;
  else if (degrees > 0) DriveFunction::driveControl->moveRelative(0, degrees / 90 * kt, 0, 0, MOTOR_MOVE_RELATIVE_THRESHOLD);
  else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -(degrees / 90 * kt), MOTOR_MOVE_RELATIVE_THRESHOLD);
}

void DriveFunction::pivot(int degrees) {
  DriveFunction::driveControl->moveRelative(0, 0.5 * (degrees / 90 * kt), 0, -0.5 * (degrees / 90 * kt), MOTOR_MOVE_RELATIVE_THRESHOLD);
}

void DriveFunction::move(int degrees) {
  DriveFunction::move(0, degrees, MOTOR_MOVE_RELATIVE_THRESHOLD);
}

void DriveFunction::move(double revolutions, int degrees, int threshold) {
  DriveFunction::driveControl->moveRelative(revolutions, degrees, threshold);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, moveSensitivity, turnSensitivity);
}
