#include "main.h"
#include "watchdog.hpp"

std::vector<Watcher*> Watchdog::watchers;
bool Watchdog::stopped = false;
pros::Task * Watchdog::watchTask;

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
  if (MOTORWATCHER_MUTEX_WAIT_TIME == -1 || MOTOR_LOCKED_UP_THRESHOLD == -1)
    return;

  if (lock->take(MOTORWATCHER_MUTEX_WAIT_TIME)) {
    MotorWatcher::locked = 0;
    MotorWatcher::lockupCandidate = "";
    if (motor->get_temperature() >= MOTOR_OVERHEATING_THRESHOLD)
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is overheating! Power from this motor will be reduced");
    else if (motor->get_temperature() >= MOTOR_HOT_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is hot. Power will be reduced if the motor does not cool down");
    if (motor->is_over_current())
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is over current!");
    if (motor->get_efficiency() <= MOTOR_OVERLOAD_THRESHOLD)
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is being overloaded!");
    else if (motor->get_efficiency() <= MOTOR_HIGH_LOAD_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is under high load");
    else if (motor->get_efficiency() >= MOTOR_LOW_LOAD_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is under low load");
    lock->give();
  } else if (MotorWatcher::locked >= (MOTOR_LOCKED_UP_THRESHOLD * TASK_WATCHDOG_HZ)) {
    Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is being locked up! Candidate: " + MotorWatcher::lockupCandidate);
  } else {
     MotorWatcher::locked++;
     if (MotorWatcher::lockupCandidate.size() == 0)
       MotorWatcher::lockupCandidate = MotorWatcher::lastid.substr(1);
    }
}

void MotorWatcher::notify(std::string identifier) {
  start:
    if (identifier.rfind("g", 0) == 0) {
      if (identifier.substr(1) == MotorWatcher::lastid.substr(1) && MotorWatcher::lastid.rfind("t", 0) == 0)
        MotorWatcher::lastid = "gUnknown";
      else
        Watchdog::alert(LOG_WARNING, MotorWatcher::name + " mutex was released by " + identifier.substr(1) + " when it was never taken by them");
    } else if (identifier.rfind("t", 0) == 0) {
      if (MotorWatcher::lastid == "gUnknown")
        MotorWatcher::lastid = identifier;
      else
        Watchdog::alert(LOG_WARNING, MotorWatcher::name + " mutex was taken by " + identifier.substr(1) + " when it has already been taken by " + MotorWatcher::lastid.substr(1));
    } else if (identifier.rfind("a", 0) == 0) {
        if (identifier.substr(1) == MotorWatcher::lastid.substr(1)) {
          if (MotorWatcher::lastid.rfind("t", 0) == 0)
            identifier = "g" + MotorWatcher::lastid.substr(1);
          else
            identifier = "t" + MotorWatcher::lastid.substr(1);
          goto start;
        }
    } else goto start;
}

BatteryWatcher::BatteryWatcher(std::string name, Battery & battery) : Watcher::Watcher(name) {
  Watchdog::alert(LOG_INFO, "Watching " + name + " battery. Current capacity: " + std::to_string(battery.capacity()));

  BatteryWatcher::battery = &battery;
  BatteryWatcher::level = 0;
}

void BatteryWatcher::runChecks() {
  double capacity = BatteryWatcher::battery->capacity();
  if (capacity == -1)
    return;
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

  TaskWatcher::enabled = false;
  TaskWatcher::target = target_hz;
}

void TaskWatcher::runChecks() {
  if (!TaskWatcher::enabled)
    return;
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

void TaskWatcher::enable() {
  TaskWatcher::enabled = true;
}

void TaskWatcher::disable() {
  TaskWatcher::enabled = false;
}

ControllerWatcher::ControllerWatcher(std::string name, RecordedController & controller) : Watcher::Watcher(name) {
  ControllerWatcher::controller = &controller;
}

void ControllerWatcher::runChecks() {
  if (!ControllerWatcher::controller->is_connected()) {
    Watchdog::alert(LOG_ERROR, ControllerWatcher::name + " has been disconnected!");
    ControllerWatcher::timeout(3 * TASK_WATCHDOG_HZ);
  }
}

CompetitionWatcher::CompetitionWatcher() : Watcher::Watcher("Competition") {}

void CompetitionWatcher::runChecks() {
  // Checks to run if competition is connected
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
  if (TASK_WATCHDOG_HZ == 0 || !Watchdog::stopped)
    return;
  Watchdog::stopped = false;
  pros::Task task = new pros::Task(Watchdog::task, NULL, 10, TASK_STACK_DEPTH_DEFAULT, "Watchdog");
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

BatteryWatcher & Watchdog::watch(std::string name, Battery & battery) {
  BatteryWatcher * watch = new BatteryWatcher(name, battery);
  Watchdog::addWatcher(watch);
  return *watch;
}

TaskWatcher & Watchdog::watch(std::string name, int target_hz) {
  TaskWatcher * watch = new TaskWatcher(name, target_hz);
  Watchdog::addWatcher(watch);
  return *watch;
}

ControllerWatcher & Watchdog::watch(std::string name, RecordedController & controller) {
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
  if (ALERT_LEVEL == 0 || level > ALERT_LEVEL)
    return;
  // set controller text
  if (VIBRATE_LEVEL == 0 || level > VIBRATE_LEVEL)
    return;
  // vibrate controller
}
