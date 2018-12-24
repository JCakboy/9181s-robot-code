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

  //static Battery * brainBattery;
  //static Battery * controllerMainBattery;
  // static Battery * controllerPartnerBattery;

  // Ports
  static pros::Motor * frontLeftDrive;
  static pros::Motor * frontRightDrive;
  static Unused * port3;
  static Unused * port4;
  static Unused * port5;
  static Unused * port6;
  static Unused * port7;
  static Unused * port8;
  static pros::Motor * backRightDrive;
  static pros::Motor * backLeftDrive;
  static Unused * port11;
  static Unused * port12;
  static Unused * port13;
  static Unused * port14;
  static Unused * port15;
  static Unused * port16;
  static Unused * port17;
  static Unused * port18;
  static Unused * port19;
  static Unused * port20;
  static Unused * port21;

  static pros::Mutex * driveLock;
  static pros::Mutex * launcherLock;
  static pros::Mutex * intakeLock;
  static pros::Mutex * liftLock;

  static DriveControl * driveControl;
  static DriveFunction * drive;

  static void init();

};

extern int selectedAutonomous;

#endif
