#include "main.h"


pros::Task * Puncher::task = NULL;
bool Puncher::stopped = false;
bool Puncher::_shoot = false;

pros::Mutex * Puncher::lock = NULL;
pros::Motor * Puncher::motor = NULL;

void Puncher::_task(void * param) {
  while (true) {
    if (_shoot && Puncher::lock->take(MUTEX_WAIT_TIME)) {
      _shoot = false;
      motor->tare_position();
      motor->move_relative(360, 127);
      while (motor->get_position() < 340)
        pros::delay(10);
      Puncher::lock->give();
    }
    pros::delay(20);
  }
}

void Puncher::start(pros::Mutex * lock, pros::Motor * motor) {
  if (!Puncher::stopped)
    return;
  Puncher::stopped = false;
  Puncher::lock = lock;
  Puncher::motor = motor;
  pros::Task * task = new pros::Task(Puncher::_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Puncher");
  Puncher::task = task;
  task->resume();
}

void Puncher::prime() {
  motor->move_relative(180, 127);
}

bool Puncher::shoot() {
  bool ret = !_shoot;
  _shoot = true;
  return ret;
}

void Puncher::stop() {
  if (Puncher::stopped)
    return;
  task->suspend();
  delete task;
  Puncher::task = NULL;
}
