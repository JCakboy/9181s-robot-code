#include "main.h"
#include "watchdog.hpp"

std::vector<Watcher*> Watchdog::watchers;

Watcher::Watcher (std::string name) {
  Watcher::name = name;
  Watcher::currentTimeout = 0;
}

void Watcher::timeout(int cycles) {
  Watcher::currentTimeout += cycles;
}

MotorWatcher::MotorWatcher(std::string name, pros::Mutex & motorLock, pros::Motor & motor) : Watcher::Watcher(name) {
  MotorWatcher::lock = &motorLock;
  MotorWatcher::motor = &motor;
}

void MotorWatcher::runChecks() {

}

void MotorWatcher::notify(std::string identifier) {
  MotorWatcher::lastid = identifier;
}

BatteryWatcher::BatteryWatcher(std::string name, Battery battery) : Watcher::Watcher(name) {
  Watchdog::alert(LOG_INFO, "Watching " + name + " battery. Current capacity: " + std::to_string(battery.capacity()));

  BatteryWatcher::battery = battery;
  BatteryWatcher::level = 0;
}

void BatteryWatcher::runChecks() {
  double capacity = BatteryWatcher::battery.capacity();
  if (capacity < VERY_LOW_BATTERY_THRESHOLD && BatteryWatcher::level < 2) {
    BatteryWatcher::level = 2;
    Watchdog::alert(LOG_ERROR, name + " battery is very low! Current capacity: " + std::to_string(capacity));
    return;
  }
  if (capacity < LOW_BATTERY_THRESHOLD && BatteryWatcher::level < 1) {
    BatteryWatcher::level = 1;
    Watchdog::alert(LOG_WARNING, name + " battery is low. Current capacity: " + std::to_string(capacity));
    return;
  }
}

TaskWatcher::TaskWatcher(std::string name, int target_hz) : Watcher::Watcher(name) {
  Watchdog::alert(LOG_INFO, "Watching " + name + " task");

  TaskWatcher::target = target_hz;
}

void TaskWatcher::runChecks() {
  double ttarget = TaskWatcher::target / TASK_WATCHDOG_HZ;
  double pct = static_cast<double>(TaskWatcher::count) / ttarget;
  if (pct < TASK_HANG_THRESHOLD) {
    Watchdog::alert(LOG_ERROR, name + " task is hanging! Current speed: " + std::to_string(TaskWatcher::count) + " hz (target: " + std::to_string(TaskWatcher::target) + " hz)");
    this->timeout(3 * TASK_WATCHDOG_HZ);
    return;
  }
  if (pct < TASK_SLOW_THRESHOLD) {
    Watchdog::alert(LOG_WARNING, name + " task is slow. Current speed: " + std::to_string(TaskWatcher::count) + " hz (target: " + std::to_string(TaskWatcher::target) + " hz)");
    this->timeout(3 * TASK_WATCHDOG_HZ);
    return;
  }
}

void TaskWatcher::notify() {
  TaskWatcher::count++;
}

ControllerWatcher::ControllerWatcher(std::string name, pros::Controller & controller) : Watcher::Watcher(name) {
  ControllerWatcher::controller = &controller;
}

void ControllerWatcher::runChecks() {

}

CompetitionWatcher::CompetitionWatcher() : Watcher::Watcher("Competition") {}

void CompetitionWatcher::runChecks() {

}

void Watchdog::addWatcher(Watcher * watch) {
  Watchdog::watchers.push_back(watch);
}

void Watchdog::task(void *param) {
  while (flags::watchdogAlive) {
    for (Watcher * watch : Watchdog::watchers) {
      if (watch->currentTimeout > 0) {
        watch->currentTimeout--;
        continue;
      }
      watch->runChecks();
    }
    pros::c::delay(1000 / TASK_WATCHDOG_HZ);
  }
}

void Watchdog::start() {
  if (TASK_WATCHDOG_HZ == 0)
    return;
  Watchdog::stopped = false;
  pros::Task task = new pros::Task(Watchdog::task);
  Watchdog::watchTask = &task;
  Watchdog::watchTask->resume();
}

void Watchdog::stop() {
  if (Watchdog::stopped)
    return;
  Watchdog::stopped = true;
  Watchdog::watchTask->suspend();
  delete Watchdog::watchTask;
  Watchdog::watchTask = NULL;
}

MotorWatcher & Watchdog::watch(std::string name, pros::Mutex & motorLock, pros::Motor motor) {
  MotorWatcher * watch = new MotorWatcher(name, motorLock, motor);
  Watchdog::addWatcher(watch);
  return *watch;
}

BatteryWatcher & Watchdog::watch(std::string name, Battery battery) {
  BatteryWatcher * watch = new BatteryWatcher(name, battery);
  Watchdog::addWatcher(watch);
  return *watch;
}

TaskWatcher & Watchdog::watch(std::string name, int target_hz) {
  TaskWatcher * watch = new TaskWatcher(name, target_hz);
  Watchdog::addWatcher(watch);
  return *watch;
}

ControllerWatcher & Watchdog::watch(std::string name, pros::Controller & controller) {
  ControllerWatcher * watch = new ControllerWatcher(name, controller);
  Watchdog::addWatcher(watch);
  return *watch;
}

CompetitionWatcher & Watchdog::watchCompetition() {
  CompetitionWatcher * watch = new CompetitionWatcher();
  Watchdog::addWatcher(watch);
  return *watch;
}

void Watchdog::alert(logging_levels level, std::string message) {
  Logger::log(level, message);
}
