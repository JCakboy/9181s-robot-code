#include "main.h"
#include "watchdog.hpp"

Watcher::Watcher (std::string name) {
  Watcher::name = name;
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
  BatteryWatcher::battery = battery;
}

void BatteryWatcher::runChecks() {

}

TaskWatcher::TaskWatcher(std::string name, int target_hz) : Watcher::Watcher(name) {
  TaskWatcher::target = target_hz;
}

void TaskWatcher::runChecks() {

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
  while(true) {
    for (const auto & watch : Watchdog::watchers)
      watch->runChecks();
    pros::c::delay(1000 / TASK_WATCHDOG_HZ);
  }
}

void Watchdog::start() {
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
