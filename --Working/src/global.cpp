#include "main.h"

/*
 * This file includes all definitions that do NOT require linking to the hot image
 *
 * This is a general-purpose file and code from this file can
 * refer to any part of robot control
 */

Unused::Unused() {}
Unused::Unused(int i) {}

namespace ports {
  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MAIN);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Ports
  Unused * port1 = new Unused(1);
  Unused * port2 = new Unused(2);
  Unused * port3 = new Unused(3);
  Unused * port4 = new Unused(4);
  Unused * port5 = new Unused(5);
  Unused * port6 = new Unused(6);
  Unused * port7 = new Unused(7);
  Unused * port8 = new Unused(8);
  Unused * port9 = new Unused(9);
  Unused * port10 = new Unused(10);
  Unused * port11 = new Unused(11);
  Unused * port12 = new Unused(12);
  Unused * port13 = new Unused(13);
  Unused * port14 = new Unused(14);
  Unused * port15 = new Unused(15);
  Unused * port16 = new Unused(16);
  Unused * port17 = new Unused(17);
  Unused * port18 = new Unused(18);
  Unused * port19 = new Unused(19);
  Unused * port20 = new Unused(20);
  Unused * port21 = new Unused(21);

  // Mapping
  pros::Motor * backLeftDrive = new pros::Motor(1);
  pros::Motor * frontLeftDrive = new pros::Motor(1);
  pros::Motor * backRightDrive = new pros::Motor(1);
  pros::Motor * frontRightDrive = new pros::Motor(1);

  // Vision
  pros::Vision * flagVision = new pros::Vision(1);

  // Mutexes
  pros::Mutex * driveLock = new pros::Mutex();

  // Driving
  DriveControl * driveControl = new DriveControl(ports::driveLock, ports::frontLeftDrive, ports::backLeftDrive, ports::frontRightDrive, ports::backRightDrive);
  DriveFunction * drive = new DriveFunction(ports::driveControl);

}

// The currently selected autonomous, has external linkage
int selectedAutonomous = 0;
// Whether the autonomous was completed successfully. Used to determine whether the autonomous routine takes too long to complete
bool autonomousComplete = true;
 // Whether the operator control loop should run, has external linkage
bool runOperatorControlLoop = true;
// Driving sensitivity, has external linkage
// These are fallback values, explicit statements should be made in ports::init()
double sensitivity = 1.0;
double adjustingSensitivity = 0.45;
