#include "main.h"

// Ports initialization
namespace ports {

  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MASTER);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Motors
  pros::Motor * port1 = new pros::Motor(1, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port2 = NULL;
  pros::Motor * port3 = NULL;
  pros::Motor * port4 = NULL;
  pros::Motor * port5 = NULL;
  pros::Motor * port6 = NULL;
  pros::Motor * port7 = NULL;
  pros::Motor * port8 = NULL;
  pros::Motor * port9 = NULL;
  pros::Motor * port10 = NULL;
  pros::Motor * port11 = new pros::Motor(11, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port12 = new pros::Motor(12, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port13 = new pros::Motor(13, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port14 = new pros::Motor(14, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port15 = new pros::Motor(15, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port16 = NULL;
  pros::Motor * port17 = NULL;
  pros::Motor * port18 = new pros::Motor(18, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port19 = new pros::Motor(19, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port20 = NULL;
  pros::Motor * port21 = NULL;

  // Port mapping
  pros::Motor * frontLeftDrive = port12;
  pros::Motor * backLeftDrive = port14;
  pros::Motor * frontRightDrive = port11;
  pros::Motor * backRightDrive = port13;
  pros::Motor * intakeMotor = port1;
  pros::Motor * tiltMotor = port15;
  pros::Motor * liftMotor = port19;
  pros::Motor * clawMotor = port18;

  // Vision
  pros::Vision * frontVision = new pros::Vision(20);

  // ADI (3-wire) ports
  pros::ADIGyro * gyro1 = new pros::ADIGyro('A');
  pros::ADIGyro * gyro2 = new pros::ADIGyro('B');
  pros::ADIUltrasonic * frontUltrasonic = new pros::ADIUltrasonic('C', 'D');

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
