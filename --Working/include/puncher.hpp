#ifndef _PUNCHER_HPP_
#define _PUNCHER_HPP_

#include "main.h"

class Puncher {
  friend void ::highRoutine();
  friend void ::midRoutine();
  private:
    // The mutex to take before attempting to move the motor
    pros::Mutex * lock;
    // The motor assigned to the puncher
    pros::Motor * motor;

    // The absolute value the puncher is holding
    int puncherTarget;

    // Whether the puncher is currently moving
    bool punching;
    // Whether a punch has been requested
    bool punchWaiting;
    // Whether the puncher is pulled back and primed
    bool isprimed;

    // Moves the puncher motor forward the given degrees
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
