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
  int maxPower = 80;
  int minPower = 20;

  // Positional PID values
  double movekp = 0;
  double moveki = 0;
  double movekd = 0;
  // Velocity PID values
  double velocitykp = 0;
  double velocityki = 0;
  double velocitykd = 0;
  double velocityle = 0;
  double velocityse = 0;
  // Strafe PID values
  double strafekp = 0;
  double strafeki = 0;
  double strafekd = 0;
  // Strafe velocity PID values
  double strafevkp = 0;
  double strafevki = 0;
  double strafevkd = 0;
  double strafevle = 0;
  double strafevse = 0;
  // Pivoting PID values
  double pivotkp = 0;
  double pivotki = 0;
  double pivotkd = 0;

  // Gyro desired heading
  double strafeInchAmount = 70;
  double desiredHeading = 0;

  // Acceleration values
  double accelerationForwardCoeff = 1;
  double accelerationForwardConst = 1;
  double accelerationForwardDelay = 50;
  double accelerationBackwardCoeff = 1;
  double accelerationBackwardConst = 1;
  double accelerationBackwardDelay = 50;
  double accelerationPivotCoeff = 1;
  double accelerationPivotConst = 1;
  double accelerationPivotDelay = 50;
  // Gyro to use during velocity PID
  Gyro * velocityGyro = NULL;

  double velocityGyroValue = 0;

  // Calculates and returns the gear ratio for the drive
  static double getGearRatio();

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
  // Sets the move positional PID values
  void setMovePosPID(double movekp, double moveki, double movekd);
  // Sets the move velocity PID values
  void setMoveVelPID(double velocitykp, double velocityki, double velocitykd);
  // Sets the pivot PID values
  void setPivotPID(double pivotkp, double pivotki, double pivotkd);
  // Sets the strafe positional PID values
  void setStrafePosPID(double inchAmount, double strafekp, double strafeki, double strafekd);
  // Sets the strafe velocity PID values
  void setStrafeVelPID(double strafevkp, double strafevki, double strafevkd);
  // Sets the forward acceleration values
  void setForwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay);
  // Sets the backward acceleration values
  void setBackwardAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay);
  // Sets the pivoting acceleration values
  void setPivotAcceleration(double accelerationCoeff, double accelerationConst, double accelerationDelay);
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
  void move(double inches, double threshold = 8, bool useDesiredHeading = true);
  void move(double inches, bool useDesiredHeading);
  // Moves the robot the given amount of inches while only using velocity PID
  void velocityMove(double inches, double power, double threshold = 8, bool useDesiredHeading = true);
  void velocityMove(double inches, double power, bool useDesiredHeading);
  // Moves the robot with custom left and right targets while only using positional PID
  void customMove(double leftInches, double rightInches, double threshold = 8);

  // Strafes the robot the given amount of inches to the desired position
  void strafe(double inches, double threshold = 25, bool useDesiredHeading = true);

  // Pivots the robot relative the given amount of degrees, based on the current desired heading
  void pivot(double degrees, double threshold = 2, bool modifyDesiredHeading = true);
  // Pivots the robot relative the given amount of degrees, based on the current heading of the robot
  void pivotRelative(double degrees, double threshold = 2, bool modifyDesiredHeading = true);
  // Pivots the robot to the heading given
  void pivotAbsolute(double heading, double threshold = 2, bool modifyDesiredHeading = true);

  // Sets the desired heading to the current heading
  void tareDesiredHeading();
  // Sets the absolute desired heading
  void setAbsoluteDesiredHeading(double heading);
  // Sets the desired heading relative to the current heading
  void setRelativeDesiredHeading(double heading);

};

#endif
