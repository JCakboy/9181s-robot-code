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
  static pros::Controller * controllerMain;
  static pros::Controller * controllerPartner;

  static BrainBattery * brainBattery;
  static ControllerBattery * controllerMainBattery;
  static ControllerBattery * controllerPartnerBattery;

  // Ports
  static Unused * port1;
  static Unused * port2;
  static Unused * port3;
  static Unused * port4;
  static Unused * port5;
  static Unused * port6;
  static Unused * port7;
  static Unused * port8;
  static Unused * port9;
  static Unused * port10;
  static pros::Motor * port11;
  static pros::Motor * port12;
  static pros::Motor * port13;
  static Unused * port14;
  static Unused * port15;
  static Unused * port16;
  static pros::Motor * port17;
  static pros::Motor * port18;
  static pros::Motor * port19;
  static pros::Motor * port20;
  static Unused * port21;

  static pros::Motor * frontLeftDrive;
  static pros::Motor * frontRightDrive;
  static pros::Motor * intake;
  static pros::Motor * puncherVariable;
  static pros::Motor * puncherMotor;
  static pros::Motor * backRightDrive;
  static pros::Motor * backLeftDrive;

  static pros::Mutex * driveLock;
  static pros::Mutex * launcherLock;
  static pros::Mutex * intakeLock;
  static pros::Mutex * liftLock;

  static DriveControl * driveControl;
  static DriveFunction * drive;

  static Puncher * puncher;

  static void init();

};

extern int selectedAutonomous;
extern bool runOperaterControlLoop;

#endif
