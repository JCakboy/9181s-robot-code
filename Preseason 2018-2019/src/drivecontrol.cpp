#include "main.h"
#include <vector>
#include "drivecontrol.hpp"

void DriveControl::runLeftMotors(int voltage) {
  for (const auto & motor : DriveControl::leftMotors)
    motor.move(voltage);
}

void DriveControl::runRightMotors(int voltage) {
  for (const auto & motor : DriveControl::rightMotors)
    motor.move(voltage);
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor & leftMotor, pros::Motor & rightMotor) {
  DriveControl::lock = motorLock;
  DriveControl::leftMotors.push_back(leftMotor);
  DriveControl::rightMotors.push_back(rightMotor);
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor & leftFrontMotor, pros::Motor & leftRearMotor, pros::Motor & rightFrontMotor, pros::Motor & rightRearMotor) {
  DriveControl::lock = motorLock;
  DriveControl::leftMotors.push_back(leftFrontMotor);
  DriveControl::leftMotors.push_back(leftRearMotor);
  DriveControl::rightMotors.push_back(rightFrontMotor);
  DriveControl::rightMotors.push_back(rightRearMotor);
}

void DriveControl::addLeftMotor(pros::Motor & motor) {
  DriveControl::leftMotors.push_back(motor);
}

void DriveControl::addRightMotor(pros::Motor & motor) {
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
bool DriveControl::removeLeftMotor(pros::Motor & motor) {
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

bool DriveControl::removeRightMotor(pros::Motor & motor) {
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
void DriveControl::run(double moveVoltage, double turnVoltage, bool tankScale, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  if (tankScale) {
    moveVoltage = emath::tankScaleJoystick(moveVoltage);
    turnVoltage = emath::tankScaleJoystick(turnVoltage);
  }

  bool flip = flipReverse && moveVoltage < -42.3;

  moveVoltage *= moveSensitivity;
  turnVoltage *= turnSensitivity;

  int leftVoltage = emath::limit127(!flip ? moveVoltage - turnVoltage : moveVoltage - (0 - turnVoltage));
  int rightVoltage = emath::limit127(!flip ? moveVoltage - (0 - turnVoltage) : moveVoltage - turnVoltage);

  if (lock.take(MUTEX_WAIT_TIME)) {
    DriveControl::runLeftMotors(leftVoltage);
    DriveControl::runRightMotors(rightVoltage);
    lock.give();
  }
}
