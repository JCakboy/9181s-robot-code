#include "main.h"
#include "balllauncher.hpp"

void BallLauncher::start() {
  //BallLauncher::launchTask = &pros::Task( &BallLauncher::task );
  BallLauncher::launchTask->resume();
  BallLauncher::load();
}

bool BallLauncher::ready() {
  return BallLauncher::state == 1;
}

void BallLauncher::load() {
  BallLauncher::launchTask->notify_ext(1, NOTIFY_OWRITE, NULL);
}

void BallLauncher::launch() {
  if (BallLauncher::ready())
    BallLauncher::launchTask->notify_ext(2, NOTIFY_OWRITE, NULL);
}

void BallLauncher::task() {
  while (true) {
    int tstate = BallLauncher::launchTask->notify_take(true, TIMEOUT_MAX);
    if (tstate == BallLauncher::state)
      continue;
    if (tstate == 1) {
      BallLauncher::_load();
    } else if (tstate == 2) {
      BallLauncher::_launch();
    }
    BallLauncher::state = tstate;
  }
}


PneumaticLauncher::PneumaticLauncher(pros::Mutex & pistonLock, pros::ADIDigitalOut & piston) {
  PneumaticLauncher::lock = pistonLock;
  PneumaticLauncher::piston = &piston;
}

void PneumaticLauncher::_launch() {
  PneumaticLauncher::piston->set_value(true);
}

void PneumaticLauncher::_load() {
  PneumaticLauncher::piston->set_value(false);
}

ElasticLauncher::ElasticLauncher(pros::Mutex & motorLock, pros::Motor & motor) {
  motor.set_brake_mode(BRAKE_HOLD);
  ElasticLauncher::motors.push_back(motor);
}

void ElasticLauncher::_launch() {
  for (const auto & motor : ElasticLauncher::motors)
    motor.set_brake_mode(BRAKE_COAST);
}

void ElasticLauncher::_load() {
  for (const auto & motor : ElasticLauncher::motors) {
    motor.set_brake_mode(BRAKE_HOLD);
    motor.move(127);
  }
  pros::delay(ELASTIC_LAUNCHER_MOTOR_TIME);
  for (const auto & motor : ElasticLauncher::motors)
    motor.move(0);
}
