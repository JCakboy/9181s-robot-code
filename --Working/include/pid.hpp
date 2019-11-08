#ifndef _PID_HPP_
#define _PID_HPP_

#include "main.h"

class PID {
friend class LCD;
  // Debugging no-stop flag
  bool noStop = false;
  // Debugging controller X stop flag
  bool controllerXStop = false;
  // Debugging log errors flag
  bool logPIDErrors = false;

  // Power restraints
  int maxPower = 120;
  int minPower = 20;

  // Positional PID values
  double movekp = 0;
  double movekd = 0;
  // Velocity PID values
  double velocitykp = 0;
  double velocitykd = 0;
  double straightle = 0;
  // Strafe PID values
  double strafekp = 0;
  double strafekd = 0;
  // Strafe velocity PID values
  double strafevkp = 0;
  double strafevkd = 0;
  double strafevle = 0;
  // Pivoting PID values
  double pivotkp = 0;
  double pivotkd = 0;

  // Gyro desired heading
  double rightAngle = 975;
  double strafeInchAmount = 100;
  double desiredHeading = 0;

  // Acceleration values
  double accelerationForwardCoeff = 1;
  double accelerationForwardConst = 1;
  double accelerationForwardDelay = 50;
  double accelerationBackwardCoeff = 1;
  double accelerationBackwardConst = 1;
  double accelerationBackwardDelay = 50;

  // Gyro to use during velocity PID
  Gyro * velocityGyro = NULL;

  // The last error for velocity PID
  double velocityle = 0;
  double velocityGyroValue = 0;

  // Calculates and returns the gear ratio for the drive
  static double getGearRatio();

  // Converts to and from degrees to gyro units
  double toGyroUnits(double degrees);
  double toDegrees(double gyroUnits);

  // Sets the brake mode
  void setBrakeMode();
  // Returns the power given the minimum and maximum power restraints
  double checkPower(double power);

  // The logic to continue PID loops
  bool continuePIDLoop(bool expr);

public:
  // Constructs the PID object
  PID();

  // Set the debugging flag to force PID loops to run non-stop; useful for tuning
  void setNoStopDebug(bool flag);
  // Set the debugging flag to force exit of PID loops when X is pressed; overrides noStop
  void setControllerXStop(bool flag);
  // Set the debugging flag to force PID loops to log errors; useful for tuning
  void setLoggingDebug(bool flag);

  // Sets the power limits of PID
  void setPowerLimits(int maxPower, int minPower);
  // Sets the move PID values
  void setMovePID(double movekp, double movekd, double velocitykp, double velocitykd);
  // Sets the pivot PID values
  void setPivotPID(double rightAngleAmount, double pivotkp, double pivotkd);
  // Sets the strafe PID values
  void setStrafePID(double inchAmount, double strafekp, double strafekd, double strafevkp, double strafevkd);
  // Sets the forward acceleration values
  void setForwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay);
  // Sets the backward acceleration values
  void setBackwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay);
  // Sets the gyro to be used during velocity PID
  void setVelocityGyro(Gyro * g);

  // Resets the motor encoders
  void resetEncoders();

  // Sends the power commands to the motor
  void powerDrive(int powerLeft, int powerRight);
  // Ensures the robot drives straight
  void driveStraight(int power);
  // Ensures the robot strafes straight
  void strafeStraight(int strafePower, int movePower = 0);

  // Moves the robot the given amount of inches to the desired location
  void move(double inches, double threshold = 12, bool useDesiredHeading = false);
  void move(double inches, bool useDesiredHeading);
  // Moves the robot the given amount of inches while only using velocity PID
  void velocityMove(double inches, double power, double threshold = 12, bool useDesiredHeading = false);
  void velocityMove(double inches, double power, bool useDesiredHeading);
  // Moves the robot with custom left and right targets while only using positional PID
  void customMove(double leftInches, double rightInches, double threshold = 12);

  // Strafes the robot the given amount of inches to the desired position
  void strafe(double inches, double threshold = 12, bool useDesiredHeading = true);

  // Pivots the robot relative the given amount of degrees, based on the current desired heading
  void pivot(double degrees, double threshold = 10, bool modifyDesiredHeading = true);
  // Pivots the robot relative the given amount of degrees, based on the current heading of the robot
  void pivotRelative(double degrees, double threshold = 10, bool modifyDesiredHeading = true);
  // Pivots the robot to the heading given
  void pivotAbsolute(double heading, double threshold = 10, bool modifyDesiredHeading = true);

  // Sets the desired heading to the current heading
  void tareDesiredHeading();
  // Sets the absolute desired heading
  void setAbsoluteDesiredHeading(double heading);
  // Sets the desired heading relative to the current heading
  void setRelativeDesiredHeading(double heading);

};

#endif
