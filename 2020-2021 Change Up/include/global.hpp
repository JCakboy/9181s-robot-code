#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include "main.h"

namespace ports {

  // Controllers
  extern pros::Controller * controllerMain;
  extern pros::Controller * controllerPartner;

  // Motors
  extern pros::Motor * emptyPort;
  extern pros::Motor * port1;
  extern pros::Motor * port2;
  extern pros::Motor * port3;
  extern pros::Motor * port4;
  extern pros::Motor * port5;
  extern pros::Motor * port6;
  extern pros::Motor * port7;
  extern pros::Motor * port8;
  extern pros::Motor * port9;
  extern pros::Motor * port10;
  extern pros::Motor * port11;
  extern pros::Motor * port12;
  extern pros::Motor * port13;
  extern pros::Motor * port14;
  extern pros::Motor * port15;
  extern pros::Motor * port16;
  extern pros::Motor * port17;
  extern pros::Motor * port18;
  extern pros::Motor * port19;
  extern pros::Motor * port20;
  extern pros::Motor * port21;

  // Port mapping
  extern pros::Motor * frontLeftDrive;
  extern pros::Motor * backLeftDrive;
  extern pros::Motor * frontRightDrive;
  extern pros::Motor * backRightDrive;
  extern pros::Motor * intakeMotorLeft;
  extern pros::Motor * intakeMotorRight;
  extern pros::Motor * indexer;
  extern pros::Motor * flywheel;

  // Vision sensor

  // Inertial sensor
  extern pros::Imu * imu;

  // ADI (3-wire) ports
  extern pros::ADIGyro * gyro1;
  extern pros::ADIGyro * gyro2;
  extern pros::ADIUltrasonic * intakeUltrasonic;

  // Gyro manager
  extern Gyro * gyro;

  // PID manager
  extern PID * pid;

  // Debugging objects
  extern CompetitionTimer * competitionTimer;
  extern MessageHolder * messageHolder;

  // Tasks
  extern pros::Task * gyroTask;
  extern pros::Task * mhTask;
}

// Selected autonomous routine
extern int selectedAutonomous;

// POST result
extern bool postPass;
extern bool postNotified;
extern std::string postResult;

// Driver skills auto running
extern bool driverSkillsRunning;

#endif
