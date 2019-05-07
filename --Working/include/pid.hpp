#ifndef _PID_HPP_
#define _PID_HPP_

#include "main.h"
extern void doubleShot();
class PID {

  // Power restraints
  const int MAX_POWER = 120;
  const int MIN_POWER = 20;

  // PID values
  double movekp = 0;
  double movekd = 0;
  double straightkp = 0;
  double straightkd = 0;
  double pivotkp = 0;
  double pivotkd = 0;

  double straightle = 0;

  // Calculates and returns the gear ratio for the drive
  static double getGearRatio();

  // Sets the brake mode
  void setBrakeMode();
  // Returns the power given the minimum and maximum power restraints
  double checkPower(double power);
public:
  // Constructs the PID object
  PID();

  // Resets the motor encoders
  void resetEncoders();

  // Sets the move PID values
  void setMovePID(double movekp, double movekd, double straightkp, double straightkd);
  // Sets the pivot PID values
  void setPivotPID(double pivotkp, double pivotkd);

  // Ensures the robot drives straight
  void driveStraight(int power);
  // Sends the power commands to the motor
  void powerDrive(int powerLeft, int powerRight);

  // Moves the robot the given amount of inches to the desired location
  void move(double inches);
  // Moves the robot the given amount of inches, while only using velocity PID
  void velocityMove(double inches, double power);
  // Moves the robot with custom left and right targets
  void customMove(double leftInches, double rightInches);
  // Pivots the robot the given amount of degrees
  void pivot(double degrees);

  // Front resets the robot using the ultrasonics
  void frontReset(double inches);
  // Back resets the robot using the ultrasonics
  void backReset(double inches);
  // Front resets the robot against the wall using ultrasonics, aligning it
  void frontAlignReset();
  // Back resets the robot against the wall using ultrasonics, aligning it
  void backAlignReset();
};

#endif
