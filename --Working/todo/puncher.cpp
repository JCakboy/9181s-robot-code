#include "main.h"


pros::Task * Puncher::task = NULL;
bool Puncher::stopped = false;

pros::Mutex * Puncher::lock = NULL;
pros::Motor * Puncher::motor = NULL;

void Puncher::_task(void * param) {
  while (true) {
    int notify = task->notify_take(true, TIMEOUT_MAX);

    if (notify == 1 && Puncher::lock->take(MUTEX_WAIT_TIME)) {
      motor->tare_position();
      motor->move_relative(180, 127);
      while (motor->get_position() < 340)
        pros::delay(10);
      Puncher::lock->give();
    } else if (notify == 2 && Puncher::lock->take(MUTEX_WAIT_TIME)) {
      motor->tare_position();
      motor->move_relative(360, 127);
      while (motor->get_position() < 340)
        pros::delay(10);
      Puncher::lock->give();
    }

    task->notify_clear();
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
  Puncher::task->notify_ext(1, NOTIFY_OWRITE, NULL);
}

void Puncher::shoot() {
  Puncher::task->notify_ext(2, NOTIFY_OWRITE, NULL);
}

void Puncher::stop() {
  if (Puncher::stopped)
    return;
  task->suspend();
  delete task;
  Puncher::task = NULL;
}
