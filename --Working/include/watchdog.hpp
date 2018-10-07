#ifndef _WATCHDOG_HPP_
#define _WATCHDOG_HPP_

#include "main.h"

/*
 * A base class meant to watch and/or run checks on a specific object
 *
 * Meant to be inherited and have the following methods overriden:
 *    - runChecks(): checks the object
 *
 * The following methods should be called from tasks when necessary:
 *    - timeout(): prevent checking for the number of watchdog cycles
 */

class Watcher {
  friend class Watchdog;
  protected:
    std::string name;
    int currentTimeout;

    explicit Watcher (std::string name);

  public:
    void timeout(int cycles);

    virtual void runChecks() = 0;
};

/*
 * A class meant to watch and run checks a given motor and its associated mutex
 *
 * See the base class for documentation
 */

class MotorWatcher : public Watcher {
  private:
    pros::Motor * motor;
    pros::Mutex * lock;
    std::string lastid;

    int locked;
    std::string lockupCandidate;

  public:
    explicit MotorWatcher (std::string name, pros::Mutex & motorLock, pros::Motor & motor);

    void runChecks() override;

    void notify(std::string identifier);
};

/*
 * A class meant to run checks on a given battery
 *
 * See the base class for documentation
 */

class BatteryWatcher : public Watcher {
  private:
    Battery * battery;
    int level; // 0: no warning, 1: low warning, 2: very low warning

  public:
    explicit BatteryWatcher (std::string name, Battery & battery);

    void runChecks() override;

};

/*
 * A class meant to watch a task
 *
 * See the base class for documentation
 */

class TaskWatcher : public Watcher {
  private:
    bool enabled;
    double count;
    int target;

  public:
    explicit TaskWatcher (std::string name, int target_hz);

    void runChecks() override;

    void notify();

    // Enables watching
    void enable();

    // Disables watching
    void disable();

};

/*
 * A class meant to watch and run checks on a given controller
 *
 * See the base class for documentation
 */

class ControllerWatcher : public Watcher {
  private:
    RecordedController * controller;

  public:
    explicit ControllerWatcher (std::string name, RecordedController & controller);

    void runChecks() override;

};

/*
 * A class meant to watch the competition
 *
 * See the base class for documentation
 */

class CompetitionWatcher : public Watcher {
  public:
    CompetitionWatcher ();

    void runChecks() override;

};

/*
 * A class meant to interface Watchers
 *
 * The following methods should be called from initialize()
 *    - start(): starts all watchers
 *    - stop(): stops all watchers
 *    - watch(): creates a watcher and adds it to the list of active watchers
 */

class Watchdog {
  private:
    static bool stopped;
    static pros::Task * watchTask;
    static std::vector<Watcher*> watchers;

    static void addWatcher(Watcher * watch);

    static void task(void* param);

  public:
    // Starts all watchers
    static void start();

    // Stops all watchers
    static void stop();

    // Watches a given motor and its associated mutex
    static MotorWatcher & watch(std::string name, pros::Mutex & motorLock, pros::Motor motor);

    // Watches a given battery
    static BatteryWatcher & watch(std::string name, Battery & battery);

    // Watches given task
    static TaskWatcher & watch(std::string name, int target_hz);

    // Watches a given controller
    static ControllerWatcher & watch(std::string name, RecordedController & controller);

    // Watches the competition
    static CompetitionWatcher & watchCompetition();

    // Alerts the user and logs an event
    static void alert(logging_levels level, std::string message);

};

#endif
