#ifndef _GYRO_HPP_
#define _GYRO_HPP_

#include "main.h"

/*
 * Better gyro implementation to account for averaging of multiple gyros and overflows
 */
class Gyro {
friend void ::gyroTask(void * param);
private:
  // Base PROS inertial sensor
  pros::Imu * imu;

  // This gyro's "zero" position
  double rollZero = 0;
  double pitchZero = 0;
  double headingZero = 0;

  // Variables to track how many full rotations to add to the final calculation
  int rollLastRead;
  int pitchLastRead;
  int headingLastRead;
  int rollRotations;
  int pitchRotations;
  int headingRotations;

  // Task to keep track of gyro overflowing
  void task();

public:
  // Initilizes the gyros given global pointers
  Gyro(pros::Imu * imu);

  // Returns the calculated value
  double getRoll();
  double getPitch();
  double getHeading();

  // Passthrough for acceleration
  pros::c::imu_accel_s_t getAcceleration(); // in g's (m/s^2)
  pros::c::imu_gyro_s_t getGyroRate(); // in dps

  // Sets the current "zero" position to the current one
  void tarePosition();
  // Sets the current "zero" position to the one at program start
  void fullTarePosition();
};

// Task to be given to the global Gyro object
extern void gyroTask(void * param);

#endif
