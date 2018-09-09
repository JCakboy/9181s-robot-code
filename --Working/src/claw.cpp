#include "main.h"
#include "claw.hpp"

void Claw::runActiveMotors(int voltage) {
  for (const auto & motor : Claw::activeMotors)
    motor.move(voltage);
}

Claw::Claw (pros::Mutex & motorLock, pros::Motor flipMotor) {
  Claw::lock = &motorLock;
  Claw::addFlipMotor(flipMotor);
}

Claw::Claw (pros::Mutex & motorLock, pros::Motor pullMotor, pros::Motor activeMotor) {
  Claw::lock = &motorLock;
  Claw::addPullMotor(pullMotor);
  Claw::addActiveMotor(activeMotor);
}

Claw::Claw (pros::Mutex & motorLock, pros::Motor flipMotor, pros::Motor pullMotor, pros::Motor activeMotor) {
  Claw::lock = &motorLock;
  Claw::addFlipMotor(flipMotor);
  Claw::addPullMotor(pullMotor);
  Claw::addActiveMotor(activeMotor);
}

void Claw::addActiveMotor(pros::Motor motor) {
  Claw::activeMotors.push_back(motor);
}

void Claw::addFlipMotor(pros::Motor motor) {
  motor.set_brake_mode(BRAKE_BRAKE);
  Claw::flipMotors.push_back(motor);
}

void Claw::addPullMotor(pros::Motor motor) {
  Claw::pullMotors.push_back(motor);
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool Claw::removeActiveMotor(pros::Motor motor) {

}

bool Claw::removeFlipMotor(pros::Motor motor) {

}
*/
void Claw::clearMotors() {
  Claw::clearActiveMotors();
  Claw::clearFlipMotors();
  Claw::clearPullMotors();
}

void Claw::clearActiveMotors(){
  Claw::activeMotors.clear();
}

void Claw::clearFlipMotors() {
  Claw::flipMotors.clear();
}

void Claw::clearPullMotors() {
  Claw::pullMotors.clear();
}

void Claw::clamp() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(127);
    lock->give();
  }
}

void Claw::coast() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(0);
    lock->give();
  }
}

void Claw::release() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(-127);
    lock->give();
  }
}

void Claw::pull() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::pullMotors)
      motor.move_absolute(180, 127);
    lock->give();
  }
}

void Claw::push() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::pullMotors)
      motor.move_absolute(0, 127);
    lock->give();
  }
}

void Claw::flip() {
  if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::flipMotors)
      motor.move_relative(180, 127);
    lock->give();
  }
}

ClawArmControl::ClawArmControl (ArmControl & ac, Claw & c) {
  ClawArmControl::arm = &ac;
  ClawArmControl::claw = &c;
}

Claw ClawArmControl::getClaw() {
  return *ClawArmControl::claw;
}

ArmControl ClawArmControl::getArm() {
  return *ClawArmControl::arm;
}

void ClawArmControl::run(bool clawClamp, bool clawPull, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity) {
  if (clawClamp)
    claw->clamp();
  else
    claw->release();
  if (clawPull)
    claw->pull();
  else
    claw->push();
  arm->run(armXVoltage, armYVoltage, armTankScale, armXSensitivity, armYSensitivity);
}

void ClawArmControl::run(bool clawPull, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity) {
  claw->coast();
  if (clawPull)
    claw->pull();
  else
    claw->push();
  arm->run(armXVoltage, armYVoltage, armTankScale, armXSensitivity, armYSensitivity);
}
