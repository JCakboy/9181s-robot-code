#ifndef _BALLLAUNCHER_HPP_
#define _BALLLAUNCHER_HPP_

#include "main.h"

/*
 * A base class meant to interface a ball launcher to the opcontrol task
 *
 * Meant to be inherited and have the following methods overriden:
 *    - _load(): prepares the cannon for a launch, and will be called on creation
 *    - _launch(): launches the cannon
 *
 * The following methods should be called from the opcontrol task
 *    - start(): starts the launcher
 *    - load(): safely and concurrently calls _load()
 *    - launch(): safely and concurrently calls _launch()
 */

class BallLauncher {
  protected:
    pros::Task* launchTask;
    int state;

    // The task to control the launcher. Should never be called
    void task();

    // Meant to be overriden to implement the loading a specific type of launcher
    virtual void _load();

    // Meant to be overriden to implement the launching a specific type of launcher
    virtual void _launch();

  public:
    // Starts the launcher and its launcher task
    void start();

    // Returns whether or not the launcher is ready to launch
    bool ready();

    // Loads the launcher
    void load();

    // Launches the launcher
    void launch();
};

/*
 * An implementation of a ball launcher using pneumatics
 *
 * See the base class for documentation
 */

class PneumaticLauncher : public BallLauncher {
  private:
    pros::Mutex lock;
    pros::ADIDigitalOut* piston;

  protected:
    void _load();

    void _launch();

  public:
    explicit PneumaticLauncher(pros::Mutex & pistonLock, pros::ADIDigitalOut & pneumaticPort);
};

/*
 * An implementation of a ball launcher using tension stored by elastics
 *
 * See the base class for documentation
 */

class ElasticLauncher : public BallLauncher {
  private:
    pros::Mutex lock;
    pros::Motor* motor;

  protected:
    void _load();

    void _launch();

  public:
    explicit ElasticLauncher(pros::Mutex & motorLock, pros::Motor & motor);
  };

#endif
