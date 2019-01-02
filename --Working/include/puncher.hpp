#ifndef _PUNCHER_HPP_
#define _PUNCHER_HPP_

#include "main.h"

class Puncher {
  friend void ::opcontrol();
  private:
    pros::Mutex * lock;
    pros::Motor * motor;

    int puncherTarget;

    bool punching;
    bool punchWaiting;
    bool primed;

    void move(int degrees);

  public:
    explicit Puncher(pros::Mutex * lock, pros::Motor * motor);

    void prime();

    void unprime();

    void togglePrime();

    void shoot();

    void run();

};

#endif
