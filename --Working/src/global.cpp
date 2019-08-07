#include "main.h"

// Ports initialization
namespace ports {

  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MASTER);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Motors
  pros::Motor * port1 = NULL;
  pros::Motor * port2 = NULL;
  pros::Motor * port3 = NULL;
  pros::Motor * port4 = NULL;
  pros::Motor * port5 = NULL;
  pros::Motor * port6 = NULL;
  pros::Motor * port7 = new pros::Motor(7, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port8 = new pros::Motor(8, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port9 = new pros::Motor(9, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port10 = new pros::Motor(10, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port11 = NULL;
  pros::Motor * port12 = NULL;
  pros::Motor * port13 = NULL;
  pros::Motor * port14 = NULL;
  pros::Motor * port15 = NULL;
  pros::Motor * port16 = new pros::Motor(16, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port17 = new pros::Motor(17, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port18 = NULL;
  pros::Motor * port19 = NULL;
  pros::Motor * port20 = NULL;
  pros::Motor * port21 = NULL;

  // Port mapping
  pros::Motor * frontLeftDrive = port7;
  pros::Motor * backLeftDrive = port16;
  pros::Motor * frontRightDrive = port8;
  pros::Motor * backRightDrive = port10;
  pros::Motor * intakeMotor = port17;
  pros::Motor * tiltMotor = port9;

  // Vision

  // ADI (3-wire) ports
  pros::ADIGyro * gyro1 = new pros::ADIGyro('A');
  pros::ADIGyro * gyro2 = new pros::ADIGyro('B');

  // Gyro manager
  Gyro * gyro = new Gyro(gyro1, gyro2);
  pros::Task * gyroTask = NULL; // To be initialized during the initialization routine

  // PID manager
  PID * pid = new PID();

}

// Selected autonomous routine
int selectedAutonomous = 0;
