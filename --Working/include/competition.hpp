#ifndef _COMPETITION_HPP_
#define _COMPETITION_HPP_

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

  extern BrainBattery * brainBattery;
  extern ControllerBattery * controllerMainBattery;
  extern ControllerBattery * controllerPartnerBattery;

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
  extern pros::Motor * port12;
  extern pros::Motor * port13;
  extern pros::Motor * port14;
  extern Unused * port15;
  extern Unused * port16;
  extern pros::Motor * port17;
  extern pros::Motor * port18;
  extern pros::Motor * port19;
  extern pros::Motor * port20;
  extern Unused * port21;

  extern pros::Motor * frontLeftDrive;
  extern pros::Motor * frontRightDrive;
  extern pros::Motor * intake;
  extern pros::Motor * puncherVariable;
  extern pros::Motor * puncherMotor;
  extern pros::Motor * backRightDrive;
  extern pros::Motor * backLeftDrive;

  extern pros::Mutex * driveLock;
  extern pros::Mutex * launcherLock;
  extern pros::Mutex * intakeLock;
  extern pros::Mutex * liftLock;

  extern DriveControl * driveControl;
  extern DriveFunction * drive;

  extern Puncher * puncher;

  // Fully initializes the port configuration. Should be called on intitalize()
  extern void init();

};

void highRoutine();
void midRoutine();

extern int selectedAutonomous;
extern bool runOperatorControlLoop;

#endif
