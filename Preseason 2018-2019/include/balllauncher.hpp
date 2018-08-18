#ifndef _BALLLAUNCHER_HPP_
#define _BALLLAUNCHER_HPP_

#include "main.h"

class BallLauncher {
  protected:
    pros::Task* launchTask;
    int state;

    void task();

    virtual void _load();

    virtual void _launch();

  public:
    void start();

    void load();

    void launch();
};

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
