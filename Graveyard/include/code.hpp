#ifndef _CODE_HPP_
#define _CODE_HPP_

#include "main.h"

/*
 * A namespace for holding global flag variables
 */

namespace flags {

  // Whether or not the main code shoudl be alive; a master switch
  static bool generalAlive;

  // Whether or not the opcontrol task should be alive
  static bool opcontrolAlive;

  // Whether or not the watchdog task should be alive
  static bool watchdogAlive;

  // The selected autonomous
  static int autonomousSelected;

  // Whether or not autonomous successfully completed
  static bool autonomousComplete;

  // Sets all alive variables to true
  static void aliveAll();

  // Sets all alive variables to false
  static void killAll();

};

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

  static TaskWatcher * opcontrolTaskWatcher;
  static TaskWatcher * ballLauncherTaskWatcher;

  static MotorWatcher * leftDriveMotorWatcher;
  static MotorWatcher * rightDriveMotorWatcher;
  static MotorWatcher * elasticLaunchMotorWatcher;
  static MotorWatcher * intakeMotorWatcher;
  static MotorWatcher * leftLiftMotorWatcher;
  static MotorWatcher * rightLiftMotorWatcher;

  static Battery * brainBattery;
  static Battery * controllerMainBattery;
  static Battery * controllerPartnerBattery;

  static RecordedController * controllerMain;
  static RecordedController * controllerPartner;

  static pros::Motor * leftDriveMotor;
  static pros::Motor * intakeMotor;
  static Unused * port3;
  static Unused * port4;
  static Unused * port5;
  static Unused * port6;
  static Unused * port7;
  static Unused * port8;
  static pros::Motor * elasticLaunchMotor;
  static pros::Motor * rightDriveMotor;
  static pros::Motor * leftLiftMotor;
  static Unused * port12;
  static Unused * port13;
  static Unused * port14;
  static Unused * port15;
  static Unused * port16;
  static Unused * port17;
  static Unused * port18;
  static Unused * port19;
  static Unused * port20;
  static pros::Motor * rightLiftMotor;

  static pros::Mutex * driveLock;
  static pros::Mutex * launcherLock;
  static pros::Mutex * intakeLock;
  static pros::Mutex * liftLock;

  static DriveControl * driveControl;
  static BallLauncher * ballLauncher;
  static LiftControl * liftControl;

  static void init();

};

void initilize();

void competition_initialize();

void autonomous();

void autonomousSafe();

void operatorControl();

void operatorControlSafe();

void operatorControlBackup();

void disabled();

#endif
