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

  // Batteries
  BrainBattery * brainBattery;
  ControllerBattery * controllerMainBattery;
  ControllerBattery * controllerPartnerBattery;

  // Ports
  Unused * port1 = new Unused(1);
  Unused * port2 = new Unused(2);
  Unused * port3 = new Unused(3);
  Unused * port4 = new Unused(4);
  Unused * port5 = new Unused(5);
  Unused * port6 = new Unused(6);
  Unused * port7 = new Unused(7);
  Unused * port8 = new Unused(8);
  pros::Vision * port9 = new pros::Vision(9);
  Unused * port10 = new Unused(10);
  pros::Motor * port11 = new pros::Motor(11, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port12 = new pros::Motor(12, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port13 = new pros::Motor(13, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port14 = new pros::Motor(14, GEARSET_200, REV, ENCODER_DEGREES);
  Unused * port15 = new Unused(15);
  pros::Motor * port16 = new pros::Motor(16, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port17 = new pros::Motor(17, GEARSET_100, FWD, ENCODER_DEGREES);
  pros::Motor * port18 = new pros::Motor(18, GEARSET_200, REV, ENCODER_DEGREES);
  Unused * port19 = new Unused(19);
  pros::Motor * port20 = new pros::Motor(20, GEARSET_200, REV, ENCODER_DEGREES);
  Unused * port21 = new Unused(21);

  // Mapping
  pros::Motor * backLeftDrive = ports::port11;
  pros::Motor * frontLeftDrive = ports::port12;
  pros::Motor * backRightDrive = ports::port13;
  pros::Motor * frontRightDrive = ports::port14;
  pros::Motor * puncherVariable = ports::port16;
  pros::Motor * puncherMotor = ports::port17;
  pros::Motor * intake = ports::port18;
  pros::Motor * arm = ports::port20;

  // Vision
  pros::Vision * flagVision = ports::port9;

  // Mutexes
  pros::Mutex * driveLock = new pros::Mutex();
  pros::Mutex * launcherLock = new pros::Mutex();
  pros::Mutex * intakeLock = new pros::Mutex();
  pros::Mutex * liftLock = new pros::Mutex();

  // Driving
  DriveControl * driveControl = new DriveControl(ports::driveLock, ports::frontLeftDrive, ports::backLeftDrive, ports::frontRightDrive, ports::backRightDrive);
  DriveFunction * drive = new DriveFunction(ports::driveControl);

  // Puncher
  Puncher * puncher = new Puncher(launcherLock, ports::puncherMotor);

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
