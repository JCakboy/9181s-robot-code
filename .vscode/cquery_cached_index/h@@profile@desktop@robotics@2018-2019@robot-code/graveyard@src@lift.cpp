#include "main.h"
#include "lift.hpp"

void LiftControl::runXMotors(int voltage) {
  for (const auto & motor : LiftControl::xMotors)
    motor.move(voltage);
}

void LiftControl::runYMotors(int voltage) {
  for (const auto & motor : LiftControl::yMotors)
    motor.move(voltage);
}

LiftControl::LiftControl(MotorWatcher & motorLock) {
  LiftControl::lock = &motorLock;
}

LiftControl::LiftControl(MotorWatcher & motorLock, pros::Motor yMotor) {
  LiftControl::lock = &motorLock;
  LiftControl::yMotors.push_back(yMotor);
}

LiftControl::LiftControl(MotorWatcher & motorLock, pros::Motor xMotor, pros::Motor yMotor) {
  LiftControl::lock = &motorLock;
  LiftControl::xMotors.push_back(xMotor);
  LiftControl::yMotors.push_back(yMotor);
}

void LiftControl::addXMotor(pros::Motor motor) {
  LiftControl::xMotors.push_back(motor);
}

void LiftControl::addYMotor(pros::Motor motor) {
  LiftControl::yMotors.push_back(motor);
}

void LiftControl::clearMotors() {
  LiftControl::clearXMotors();
  LiftControl::clearYMotors();
}

void LiftControl::clearXMotors() {
  LiftControl::xMotors.clear();
}

void LiftControl::clearYMotors() {
  LiftControl::yMotors.clear();
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool LiftControl::removeXMotor(pros::Motor motor) {

}

bool LiftControl::removeYMotor(pros::Motor motor) {

}
*/
void LiftControl::run(double xVoltage, double yVoltage, bool tankScale, double xSensitivity, double ySensitivity) {
  if (tankScale) {
    xVoltage = util::tankScaleJoystick(xVoltage);
    yVoltage = util::tankScaleJoystick(yVoltage);
  }

  xVoltage = util::limit127(xVoltage * xSensitivity);
  yVoltage = util::limit127(yVoltage * ySensitivity);

  if (lock->takeMutex("Lift control", MUTEX_WAIT_TIME)) {
    LiftControl::runXMotors(xVoltage);
    LiftControl::runYMotors(yVoltage);
    lock->giveMutex("Lift control");
  }
}
