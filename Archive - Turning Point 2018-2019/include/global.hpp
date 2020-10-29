#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include "main.h"

/*
 * A placeholder class representing an unused port
 */

class Unused {
  public:
    Unused();

    Unused(int i);
};

/*
 * A namespace for holding port configuration
 */

namespace ports {

  // Controllers
  extern pros::Controller * controllerMain;
  extern pros::Controller * controllerPartner;

  // Ports
  extern Unused * port1;
  extern Unused * port2;
  extern Unused * port3;
  extern Unused * port4;
  extern Unused * port5;
  extern Unused * port6;
  extern Unused * port7;
  extern Unused * port8;
  extern Unused * port9;
  extern Unused * port10;
  extern Unused * port11;
  extern Unused * port12;
  extern Unused * port13;
  extern Unused * port14;
  extern Unused * port15;
  extern Unused * port16;
  extern Unused * port17;
  extern Unused * port18;
  extern Unused * port19;
  extern Unused * port20;
  extern Unused * port21;

  // Mapping
  extern pros::Motor * frontLeftDrive;
  extern pros::Motor * frontRightDrive;
  extern pros::Motor * intake;
  extern pros::Motor * arm;
  extern pros::Motor * puncherVariable;
  extern pros::Motor * puncherMotor;
  extern pros::Motor * backRightDrive;
  extern pros::Motor * backLeftDrive;

  // Vision
  extern pros::Vision * flagVision;

  // Mutexes
  extern pros::Mutex * driveLock;
  // Driving
  extern DriveControl * driveControl;
  extern DriveFunction * drive;

  // Fully initializes the port configuration. Should be called on intitalize()
  extern void init();

};

// Whether the autonomous completed successfully
extern bool autonomousComplete;
// The currently selected autonomous
extern int selectedAutonomous;
// Whether the operator control loop should run
extern bool runOperatorControlLoop;
// Driving sensitivity
extern double sensitivity;
extern double adjustingSensitivity;

#endif
