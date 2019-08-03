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
  pros::Motor * port7 = NULL;
  pros::Motor * port8 = NULL;
  pros::Motor * port9 = NULL;
  pros::Motor * port10 = NULL;
  pros::Motor * port11 = NULL;
  pros::Motor * port12 = NULL;
  pros::Motor * port13 = NULL;
  pros::Motor * port14 = NULL;
  pros::Motor * port15 = NULL;
  pros::Motor * port16 = NULL;
  pros::Motor * port17 = NULL;
  pros::Motor * port18 = NULL;
  pros::Motor * port19 = NULL;
  pros::Motor * port20 = NULL;
  pros::Motor * port21 = NULL;

  // Port mapping
  pros::Motor * frontLeftDrive = NULL;
  pros::Motor * backLeftDrive = NULL;
  pros::Motor * frontRightDrive = NULL;
  pros::Motor * backRightDrive = NULL;

  // Vision

  // ADI (3-wire) ports

  // PID manager

}

// Selected autonomous routine
int selectedAutonomous = 0;
