#include "main.h"

// Ports initialization
namespace ports {

  // Controllers
  pros::Controller * controllerMain = new pros::Controller(CONTROLLER_MASTER);
  pros::Controller * controllerPartner = new pros::Controller(CONTROLLER_PARTNER);

  // Motors
  pros::Motor * port1 = NULL;
  pros::Motor * port2 = NULL;
  pros::Motor * port3 = new pros::Motor(3, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port4 = NULL;
  pros::Motor * port5 = NULL;
  pros::Motor * port6 = new pros::Motor(6, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port7 = NULL;
  pros::Motor * port8 = new pros::Motor(8, GEARSET_100, REV, ENCODER_DEGREES);
  pros::Motor * port9 = NULL;
  pros::Motor * port10 = NULL;
  pros::Motor * port11 = NULL;
  pros::Motor * port12 = NULL;
  pros::Motor * port13 = NULL;
  pros::Motor * port14 = new pros::Motor(14, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port15 = new pros::Motor(15, GEARSET_200, FWD, ENCODER_DEGREES);
  pros::Motor * port16 = new pros::Motor(16, GEARSET_100, REV, ENCODER_DEGREES);
  pros::Motor * port17 = NULL;
  pros::Motor * port18 = new pros::Motor(18, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port19 = new pros::Motor(19, GEARSET_200, REV, ENCODER_DEGREES);
  pros::Motor * port20 = NULL;
  pros::Motor * port21 = NULL;

  // Port mapping
  pros::Motor * frontLeftDrive = port15;
  pros::Motor * backLeftDrive = port14;
  pros::Motor * frontRightDrive = port18;
  pros::Motor * backRightDrive = port19;
  pros::Motor * intakeMotorLeft = port3;
  pros::Motor * intakeMotorRight = port6;
  pros::Motor * tiltMotor = port16;
  pros::Motor * liftMotor = port8;

  // Vision sensor

  // Inertial sensor
  pros::Imu * imu = new pros::Imu(17);

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
