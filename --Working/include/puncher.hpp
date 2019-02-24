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

    // Whether an aligner is set for the puncher
    bool useAligner;
    // The DriveControl object to align with
    DriveControl * driveControl;
    // The Vision sensor to align with
    pros::Vision * vision;
    // Whether to strafe when aligning
    bool strafeAlign;
    // The time at the last successful alignment
    unsigned int alignTime;

    // The center position of the vision sensor
    static const int visionMidXPos = 152;
    // The signiture ID for the blue flag
    static const int blueSigID = 1;
    // The signiture ID for the red flag
    static const int redSigID = 2;

    // Moves the puncher motor forward the given degrees
    void move(int degrees);

  public:
    // Creates the puncher object given a mutex to respect and a puncher motor
    explicit Puncher(pros::Mutex * lock, pros::Motor * motor);

    // Reinitializes the Puncher object with a new motor
    void reinitialize(pros::Motor * motor);

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

    // Sets the aligner
    void setAligner(bool enabled, DriveControl * driveControl, pros::Vision * vision, bool strafeAlign);

    // Aligns the puncher
    void align(bool pause);

    // Runs the puncher, meant to be called on each pass of the operator control loop
    void run();

};

#endif
