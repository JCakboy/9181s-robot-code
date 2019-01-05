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
    bool isprimed;

    void move(int degrees);

  public:
    // Creates the puncher object given a mutex to respect and a puncher motor
    explicit Puncher(pros::Mutex * lock, pros::Motor * motor);

    // Brakes and primes the motor for a punch, reducing firing time from button press
    void prime();

    // Releases any brakes set on the motor and removes the prime status from the motor
    void unprime();

    // Toggles whether or not the motor is primed
    void togglePrime();

    // Returns whether or not the motor is primed
    bool primed();

    // Fires the puncher
    void shoot();

    // Runs the puncher, meant to be called on each pass of the operator control loop
    void run();

};

#endif
