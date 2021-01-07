#include "main.h"

// Ports initialization
namespace ports {

  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MASTER);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Motors
  pros::Motor * emptyPort = new pros::Motor(2);
  pros::Motor * port1 = new pros::Motor(1, GEARSET_200, FWD, ENCODER_DEGREES); 
  pros::Motor * port2 = NULL; 
  pros::Motor * port3 = NULL;
  pros::Motor * port4 = NULL; 
  pros::Motor * port5 = new pros::Motor(5, GEARSET_200, FWD, ENCODER_DEGREES); 
  pros::Motor * port6 = new pros::Motor(6, GEARSET_200, FWD, ENCODER_DEGREES); 
  pros::Motor * port7 = new pros::Motor(7, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port8 = NULL;
  pros::Motor * port9 = NULL;
  pros::Motor * port10 = new pros::Motor(10, GEARSET_200, REV, ENCODER_DEGREES); 
  pros::Motor * port11 = NULL;
  pros::Motor * port12 = new pros::Motor(12, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port13 = NULL;
  pros::Motor * port14 = NULL;
  pros::Motor * port15 = NULL; 
  pros::Motor * port16 = new pros::Motor(16, GEARSET_200, FWD, ENCODER_DEGREES); 
  pros::Motor * port17 = NULL;
  pros::Motor * port18 = NULL;
  pros::Motor * port19 = NULL;
  pros::Motor * port20 = new pros::Motor(20, GEARSET_600, REV, ENCODER_DEGREES);
  pros::Motor * port21 = NULL;

  // Port mapping
  pros::Motor * frontLeftDrive = port6;
  pros::Motor * backLeftDrive = port5;
  pros::Motor * frontRightDrive = port10;
  pros::Motor * backRightDrive = port20;
  pros::Motor * intakeMotorLeft = port1;
  pros::Motor * intakeMotorRight = port7;
  pros::Motor * indexer = port12;
  pros::Motor * flywheel = port16;

  // Vision sensor

  // Inertial sensor
  pros::Imu * imu = new pros::Imu(15);

  // ADI (3-wire) ports
  pros::ADIUltrasonic * leftUltrasonic = new pros::ADIUltrasonic('A', 'B');
  pros::ADIUltrasonic * rightUltrasonic = new pros::ADIUltrasonic('C', 'D');
  pros::ADIUltrasonic * frontUltrasonic = new pros::ADIUltrasonic('E', 'F');

  // Gyro manager
  Gyro * gyro = new Gyro(imu);

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
