#include "main.h"
#include "arm.hpp"

void ArmControl::runXMotors(int voltage) {
  for (const auto & motor : ArmControl::xMotors)
    motor.move(voltage);
}

void ArmControl::runYMotors(int voltage) {
  for (const auto & motor : ArmControl::yMotors)
    motor.move(voltage);
}

ArmControl::ArmControl(pros::Mutex & motorLock, pros::Motor yMotor) {
  ArmControl::lock = &motorLock;
  ArmControl::yMotors.push_back(yMotor);
}

ArmControl::ArmControl(pros::Mutex & motorLock, pros::Motor xMotor, pros::Motor yMotor) {
  ArmControl::lock = &motorLock;
  ArmControl::xMotors.push_back(xMotor);
  ArmControl::yMotors.push_back(yMotor);
}

void ArmControl::addXMotor(pros::Motor motor) {
  ArmControl::xMotors.push_back(motor);
}

void ArmControl::addYMotor(pros::Motor motor) {
  ArmControl::yMotors.push_back(motor);
}

void ArmControl::clearMotors() {
  ArmControl::clearXMotors();
  ArmControl::clearYMotors();
}

void ArmControl::clearXMotors() {
  ArmControl::xMotors.clear();
}

void ArmControl::clearYMotors() {
  ArmControl::yMotors.clear();
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool ArmControl::removeXMotor(pros::Motor motor) {

}

bool ArmControl::removeYMotor(pros::Motor motor) {

}
*/
void ArmControl::run(double xVoltage, double yVoltage, bool tankScale, double xSensitivity, double ySensitivity) {
  if (tankScale) {
    xVoltage = emath::tankScaleJoystick(xVoltage);
    yVoltage = emath::tankScaleJoystick(yVoltage);
  }

  xVoltage = emath::limit127(xVoltage * xSensitivity);
  yVoltage = emath::limit127(yVoltage * ySensitivity);

  if (lock->take(MUTEX_WAIT_TIME)) {
    ArmControl::runXMotors(xVoltage);
    ArmControl::runYMotors(yVoltage);
    lock->give();
  }
}
