#ifndef _WATCHDOG_HPP_
#define _WATCHDOG_HPP_

#include "main.h"

class Watcher {
  protected:
    std::string name;

    explicit Watcher (std::string name);

    void timeout(int cycles);

  public:
    virtual void runChecks();
};

class MotorWatcher : public Watcher {
  private:
    pros::Motor * motor;
    pros::Mutex * lock;
    std::string lastid;

  public:
    explicit MotorWatcher (std::string name, pros::Mutex & motorLock, pros::Motor & motor);

    void runChecks();

    void notify(std::string identifier);
};

class BatteryWatcher : public Watcher {
  private:
    Battery battery;

  public:
    explicit BatteryWatcher (std::string name, Battery battery);

    void runChecks();

};

class TaskWatcher : public Watcher {
  private:
    int count;
    int target;

  public:
    explicit TaskWatcher (std::string name, int target_hz);

    void runChecks();

    void notify();

};

class ControllerWatcher : public Watcher {
  private:
    pros::Controller * controller;

  public:
    explicit ControllerWatcher (std::string name, pros::Controller & controller);

    void runChecks();

};

class CompetitionWatcher : public Watcher {
  public:
    CompetitionWatcher ();

    void runChecks();

};

class Watchdog {
  private:
    static bool stopped;
    static pros::Task * watchTask;
    static std::vector<Watcher *> watchers;

    static void addWatcher(Watcher * watch);

    static void task(void* param);

  public:
    static void start();

    static void stop();

    static MotorWatcher & watch(std::string name, pros::Mutex & motorLock, pros::Motor motor);

    static BatteryWatcher & watch(std::string name, Battery battery);

    static TaskWatcher & watch(std::string name, int target_hz);

    static ControllerWatcher & watch(std::string name, pros::Controller & controller);

    static CompetitionWatcher & watchCompetition();

    static void alert(logging_levels level, std::string message);

};

#endif
