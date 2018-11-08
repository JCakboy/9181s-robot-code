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
  pros::Task * lt = new pros::Task(BallLauncher::task, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Ball launcher");
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
  pros::delay(PNEUMATIC_LAUNCHER_WAIT_TIME);
  if (lock->take(MUTEX_WAIT_TIME)) {
    PneumaticLauncher::piston->set_value(true);
    lock->give();
  }
}

void PneumaticLauncher::_load() {
  pros::delay(PNEUMATIC_LAUNCHER_WAIT_TIME);
  if (lock->take(MUTEX_WAIT_TIME)) {
    PneumaticLauncher::piston->set_value(false);
    lock->give();
  }}

ElasticLauncher::ElasticLauncher(TaskWatcher * watcher, MotorWatcher & motorLock, pros::Motor motor) : BallLauncher::BallLauncher(watcher) {
  motor.set_brake_mode(BRAKE_HOLD);
  ElasticLauncher::addMotor(motor);
  ElasticLauncher::lock = &motorLock;
}

void ElasticLauncher::_launch() {
  if (lock->takeMutex("Elastic ball launcher", MUTEX_WAIT_TIME)) {
    for (const auto & motor : ElasticLauncher::motors)
      motor.set_brake_mode(BRAKE_COAST);
    lock->giveMutex("Elastic ball launcher");
  }
}

void ElasticLauncher::_load() {
  if (lock->takeMutex("Elastic ball launcher", MUTEX_WAIT_TIME)) {
    for (const auto & motor : ElasticLauncher::motors) {
      motor.set_brake_mode(BRAKE_HOLD);
      motor.move(127);
    }
    pros::delay(ELASTIC_LAUNCHER_MOTOR_TIME);
    for (const auto & motor : ElasticLauncher::motors)
      motor.move(0);
    lock->giveMutex("Elastic ball launcher");
  }
}

void ElasticLauncher::addMotor(pros::Motor motor) {
  ElasticLauncher::motors.push_back(motor);
}

void ElasticLauncher::clearMotors() {
  ElasticLauncher::motors.clear();
}

ElasticSlipGearLauncher::ElasticSlipGearLauncher(TaskWatcher * watcher, MotorWatcher & motorLock, pros::Motor motor) : BallLauncher::BallLauncher(watcher) {
  motor.set_brake_mode(BRAKE_HOLD);
  ElasticSlipGearLauncher::addMotor(motor);
  ElasticSlipGearLauncher::lock = &motorLock;
}

void ElasticSlipGearLauncher::_launch() {
  if (lock->takeMutex("Elastic slip gear ball launcher", MUTEX_WAIT_TIME)) {
    for (const auto & motor : ElasticSlipGearLauncher::motors)
      motor.move(127);
    lock->giveMutex("Elastic slip gear ball launcher");
  }
}

void ElasticSlipGearLauncher::_load() {}

void ElasticSlipGearLauncher::addMotor(pros::Motor motor) {
  ElasticSlipGearLauncher::motors.push_back(motor);
}

void ElasticSlipGearLauncher::clearMotors() {
  ElasticSlipGearLauncher::motors.clear();
}
