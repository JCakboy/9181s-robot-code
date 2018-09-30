#include "main.h"
#include "balllauncher.hpp"

BallLauncher::BallLauncher(TaskWatcher * watcher) {
  BallLauncher::stopped = true;
  BallLauncher::watcher = watcher;
}

void BallLauncher::start() {
  if (!BallLauncher::stopped)
    return;
  BallLauncher::stopped = false;
  pros::Task * lt = new pros::Task(BallLauncher::task, this);
  BallLauncher::launchTask = lt;
  BallLauncher::launchTask->resume();
  BallLauncher::load();
}

bool BallLauncher::ready() {
  return BallLauncher::state == 1;
}

void BallLauncher::load() {
  if (!BallLauncher::ready())
    BallLauncher::launchTask->notify_ext(1, NOTIFY_OWRITE, NULL);
}

void BallLauncher::launch() {
  if (BallLauncher::ready())
    BallLauncher::launchTask->notify_ext(2, NOTIFY_OWRITE, NULL);
}

void BallLauncher::stop() {
  if (BallLauncher::stopped)
    return;
  BallLauncher::stopped = true;
  BallLauncher::launchTask->suspend();
  delete BallLauncher::launchTask;
  BallLauncher::launchTask = NULL;
}

void BallLauncher::task(void* param) {
  BallLauncher * bl = static_cast<BallLauncher*>(param);
  while (flags::generalAlive) {
    bl->watcher->notify();
    int tstate = bl->launchTask->notify_take(true, 1000 / TASK_BALLLAUNCHER_HZ);
    if (tstate == bl->state || tstate == 0)
      continue;
    bl->watcher->timeout(2 * TASK_WATCHDOG_HZ);
    if (tstate == 1) {
      bl->_load();
    } else if (tstate == 2) {
      bl->_launch();
      if (BALL_LAUNCHER_AUTO_LOAD)
        bl->load();
    }
    bl->state = tstate;
  }
}


PneumaticLauncher::PneumaticLauncher(TaskWatcher * watcher, pros::Mutex & pistonLock, pros::ADIDigitalOut & piston) : BallLauncher::BallLauncher(watcher) {
  PneumaticLauncher::lock = &pistonLock;
  PneumaticLauncher::piston = &piston;
}

void PneumaticLauncher::_launch() {
  PneumaticLauncher::piston->set_value(true);
}

void PneumaticLauncher::_load() {
  PneumaticLauncher::piston->set_value(false);
}

ElasticLauncher::ElasticLauncher(TaskWatcher * watcher, pros::Mutex & motorLock, pros::Motor & motor) : BallLauncher::BallLauncher(watcher) {
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
