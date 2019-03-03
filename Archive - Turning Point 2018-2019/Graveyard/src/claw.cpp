#include "main.h"
#include "claw.hpp"

void Claw::runActiveMotors(int voltage) {
  for (const auto & motor : Claw::activeMotors)
    motor.move(voltage);
}

Claw::Claw (MotorWatcher & motorLock, pros::Motor flipMotor) {
  Claw::lock = &motorLock;
  Claw::addFlipMotor(flipMotor);
}

Claw::Claw (MotorWatcher & motorLock, pros::Motor pullMotor, pros::Motor activeMotor) {
  Claw::lock = &motorLock;
  Claw::addPullMotor(pullMotor);
  Claw::addActiveMotor(activeMotor);
}

Claw::Claw (MotorWatcher & motorLock, pros::Motor flipMotor, pros::Motor pullMotor, pros::Motor activeMotor) {
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
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(127);
    lock->giveMutex("Claw");
  }
}

void Claw::coast() {
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(0);
    lock->giveMutex("Claw");
  }
}

void Claw::release() {
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    Claw::runActiveMotors(-127);
    lock->giveMutex("Claw");
  }
}

void Claw::pull() {
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::pullMotors)
      motor.move_absolute(180, 127);
    lock->giveMutex("Claw");
  }
}

void Claw::push() {
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::pullMotors)
      motor.move_absolute(0, 127);
    lock->giveMutex("Claw");
  }
}

void Claw::flip() {
  if (lock->takeMutex("Claw", MUTEX_WAIT_TIME)) {
    for (const auto & motor : Claw::flipMotors)
      motor.move_relative(180, 127);
    lock->giveMutex("Claw");
  }
}

ClawLiftControl::ClawLiftControl (LiftControl & lc, Claw & c) {
  ClawLiftControl::lift = &lc;
  ClawLiftControl::claw = &c;
}

Claw & ClawLiftControl::getClaw() {
  return *ClawLiftControl::claw;
}

LiftControl & ClawLiftControl::getLift() {
  return *ClawLiftControl::lift;
}

void ClawLiftControl::run(bool clawClamp, bool clawPull, double liftXVoltage, double liftYVoltage, bool liftTankScale, double liftXSensitivity, double liftYSensitivity) {
  if (clawClamp)
    claw->clamp();
  else
    claw->release();
  if (clawPull)
    claw->pull();
  else
    claw->push();
  lift->run(liftXVoltage, liftYVoltage, liftTankScale, liftXSensitivity, liftYSensitivity);
}

void ClawLiftControl::run(bool clawPull, double liftXVoltage, double liftYVoltage, bool liftTankScale, double liftXSensitivity, double liftYSensitivity) {
  claw->coast();
  if (clawPull)
    claw->pull();
  else
    claw->push();
  lift->run(liftXVoltage, liftYVoltage, liftTankScale, liftXSensitivity, liftYSensitivity);
}
