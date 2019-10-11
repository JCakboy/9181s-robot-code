#include "main.h"

// Ports initialization
namespace ports {

  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MASTER);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Motors
  pros::Motor * port1 = new pros::Motor(1, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port2 = new pros::Motor(2, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port3 = NULL;
  pros::Motor * port4 = new pros::Motor(4, GEARSET_100, FWD, ENCODER_DEGREES);
  pros::Motor * port5 = NULL;
  pros::Motor * port6 = NULL;
  pros::Motor * port7 = new pros::Motor(7, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port8 = new pros::Motor(8, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port9 = new pros::Motor(9, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port10 = new pros::Motor(10, GEARSET_200, REV, ENCODER_DEGREES);
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
  pros::Motor * frontLeftDrive = port8;
  pros::Motor * backLeftDrive = port7;
  pros::Motor * frontRightDrive = port10;
  pros::Motor * backRightDrive = port9;
  pros::Motor * intakeMotorLeft = port2;
  pros::Motor * intakeMotorRight = port1;
  pros::Motor * tiltMotor = port4;

  // Vision
  pros::Vision * frontVision = new pros::Vision(20);

  // ADI (3-wire) ports
  pros::ADIGyro * gyro1 = new pros::ADIGyro('A');
  pros::ADIGyro * gyro2 = new pros::ADIGyro('B');
  pros::ADIUltrasonic * leftUltrasonic = new pros::ADIUltrasonic('E', 'F');
  pros::ADIUltrasonic * rightUltrasonic = new pros::ADIUltrasonic('G', 'H');

  // Gyro manager
  Gyro * gyro = new Gyro(gyro1, gyro2);

  // PID manager
  PID * pid = new PID();

  // Debugging objects
  CompetitionTimer * competitionTimer = new CompetitionTimer();
  MessageHolder * messageHolder = new MessageHolder();

  // Tasks
  pros::Task * gyroTask = NULL; // To be initialized during the initialization routine
  pros::Task * mhTask = NULL; // To be initialized during the initialization routine

}

// Selected autonomous routine
int selectedAutonomous = 0;
