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
  Watchdog::alert(LOG_INFO, "Watching " + name + " motor. Selected gearset: " + (motor.get_gearing() == GEARSET_100 ? "6:1 (100 RPM)" : (motor.get_gearing() == GEARSET_200 ? "1:18 (200 RPM)" : "1:36 (600 RPM)")), "MOTOR WATCH|" + MotorWatcher::name);

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
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is overheating! Power from this motor will be reduced", "MOTOR OVERHEAT|" + MotorWatcher::name);
    else if (motor->get_temperature() >= MOTOR_HOT_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is hot. Power will be reduced if the motor does not cool down", "MOTOR HOT|" + MotorWatcher::name);
    if (motor->is_over_current())
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is over current!", "MOTOR OVERCURR|" + MotorWatcher::name);
    if (motor->get_efficiency() <= MOTOR_OVERLOAD_THRESHOLD)
      Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is being overloaded!", "MOTOR OVERLOAD|" + MotorWatcher::name);
    else if (motor->get_efficiency() <= MOTOR_HIGH_LOAD_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is under high load", "MOTOR HIGH LOAD|" + MotorWatcher::name);
    else if (motor->get_efficiency() >= MOTOR_LOW_LOAD_THRESHOLD)
      Watchdog::alert(LOG_WARNING, MotorWatcher::name + " motor is under low load", "MOTOR LOW LOAD|" + MotorWatcher::name);
    lock->give();
  } else if (MotorWatcher::locked >= (MOTOR_LOCKED_UP_THRESHOLD * TASK_WATCHDOG_HZ)) {
    Watchdog::alert(LOG_ERROR, MotorWatcher::name + " motor is being locked up! Candidate: " + MotorWatcher::lockupCandidate, "MOTOR LOCKUP|" + MotorWatcher::name);
  } else {
     MotorWatcher::locked++;
     if (MotorWatcher::lockupCandidate.size() == 0)
       MotorWatcher::lockupCandidate = MotorWatcher::lastid;
    }
}

void MotorWatcher::notify(std::string identifier) {
  start:
    if (identifier.rfind("g", 0) == 0) {
      if (identifier.substr(1) == MotorWatcher::lastid.substr(1) && MotorWatcher::lastid.rfind("t", 0) == 0)
        MotorWatcher::lastid = "gUnknown";
      else
        Watchdog::alert(LOG_WARNING, MotorWatcher::name + " mutex was released by " + identifier.substr(1) + " when it was never taken by them", "MUTEX ID GIVE|" + identifier.substr(1));
    } else if (identifier.rfind("t", 0) == 0) {
      if (MotorWatcher::lastid == "gUnknown")
        MotorWatcher::lastid = identifier;
      else
        Watchdog::alert(LOG_WARNING, MotorWatcher::name + " mutex was taken by " + identifier.substr(1) + " when it has already been taken by " + MotorWatcher::lastid.substr(1), "MUTEX DB TAKE|" + identifier.substr(1));
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

bool MotorWatcher::takeMutex(std::string identifier, unsigned long waitTime) {
  if (MotorWatcher::lock->take(waitTime)) {
    MotorWatcher::lastid = identifier;
    return true;
  }
  return false;
}

bool MotorWatcher::giveMutex(std::string identifier) {
  if (identifier == MotorWatcher::lastid) {
    MotorWatcher::lock->give();
    MotorWatcher::lastid = "Unknown";
    return true;
  }
  return false;
}

BatteryWatcher::BatteryWatcher(std::string name, Battery & battery) : Watcher::Watcher(name) {
  Watchdog::alert(LOG_INFO, "Watching " + name + " battery. Current capacity: " + std::to_string(battery.capacity()), "BATTERY WATCH|" + BatteryWatcher::name);

  BatteryWatcher::battery = &battery;
  BatteryWatcher::level = 0;
}

void BatteryWatcher::runChecks() {
  double capacity = BatteryWatcher::battery->capacity();
  if (capacity == -1)
    return;
  if (capacity < VERY_LOW_BATTERY_THRESHOLD && BatteryWatcher::level < 2) {
    BatteryWatcher::level = 2;
    Watchdog::alert(LOG_ERROR, name + " battery is very low! Current capacity: " + std::to_string(capacity), "BATTERY LOW|" + BatteryWatcher::name);
    return;
  }
  if (capacity < LOW_BATTERY_THRESHOLD && BatteryWatcher::level < 1) {
    BatteryWatcher::level = 1;
    Watchdog::alert(LOG_WARNING, name + " battery is low. Current capacity: " + std::to_string(capacity), "BATTERY VLOW|" + BatteryWatcher::name);
    return;
  }
}

TaskWatcher::TaskWatcher(std::string name, int target_hz) : Watcher::Watcher(name) {
  Watchdog::alert(LOG_INFO, "Watching " + name + " task", "TASK WATCH|" + TaskWatcher::name);

  TaskWatcher::enabled = false;
  TaskWatcher::target = target_hz;
}

void TaskWatcher::runChecks() {
  if (!TaskWatcher::enabled)
    return;
  double ttarget = TaskWatcher::target / TASK_WATCHDOG_HZ;
  double pct = static_cast<double>(TaskWatcher::count) / ttarget;
  if (pct < TASK_HANG_THRESHOLD) {
    Watchdog::alert(LOG_ERROR, name + " task is hanging! Current speed: " + std::to_string(TaskWatcher::count) + " hz (target: " + std::to_string(TaskWatcher::target) + " hz)", "TASK HANG|" + TaskWatcher::name);
    this->timeout(3 * TASK_WATCHDOG_HZ);
    return;
  }
  if (pct < TASK_SLOW_THRESHOLD) {
    Watchdog::alert(LOG_WARNING, name + " task is slow. Current speed: " + std::to_string(TaskWatcher::count) + " hz (target: " + std::to_string(TaskWatcher::target) + " hz)", "TASK SLOW|" + TaskWatcher::name);
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
  Watchdog::alert(LOG_INFO, "Watching " + name + " controller", "CONTROL WATCH|" + ControllerWatcher::name);

  ControllerWatcher::controller = &controller;
}

void ControllerWatcher::runChecks() {
  if (!ControllerWatcher::controller->is_connected()) {
    Watchdog::alert(LOG_ERROR, ControllerWatcher::name + " has been disconnected!", "CONTROLLER DCON|" + ControllerWatcher::name);
    ControllerWatcher::timeout(3 * TASK_WATCHDOG_HZ);
  }
}

CompetitionWatcher::CompetitionWatcher() : Watcher::Watcher("Competition") {
  Watchdog::alert(LOG_INFO, "Watching competition", "COMP WATCH| ");
}

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
}

void Watchdog::alert(logging_levels level, std::string message, std::string controllerText) {
  Logger::log(level, message);
  if (ALERT_LEVEL == 0 || level > ALERT_LEVEL)
    return;

  std::vector<std::string> text;
  std::pair<std::string, std::string> pair = util::separateFirst(controllerText, "|");
  text.push_back("[{timediff}]" + util::getLoggingLevelName(level));
  text.push_back(pair.first);
  text.push_back(pair.second);

  ports::controllerMain->set_text(text);
  if (ALERT_MIRROR_PARTNER)
    ports::controllerPartner->set_text(text);

  if (VIBRATE_LEVEL == 0 || level > VIBRATE_LEVEL)
    return;
  // vibrate controller
}
