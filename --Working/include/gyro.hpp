#ifndef _GYRO_HPP_
#define _GYRO_HPP_

#include "main.h"

/*
 * Better gyro implementation to account for averaging of multiple gyros and overflows
 */
class Gyro {
friend void ::gyroTask(void * param);
private:
  // Base PROS API gyros
  pros::ADIGyro * gyro1;
  pros::ADIGyro * gyro2;

  // Whether or not to use the given gyros
  bool useFirst;
  bool useSecond;

  // Variables to track how many full rotations to add to the final calculation
  int lastReadFirst;
  int lastReadSecond;
  int firstRotations;
  int secondRotations;

  // Task to keep track of gyro overflowing
  void task();

public:
  // Initilizes the gyros given global pointers
  Gyro(pros::ADIGyro * gyro1, pros::ADIGyro * gyro2);

  // Sets whether or not to use the first gyro given
  void useFirstGyro(bool flag);

  // Sets whether or not to use the second gyro given
  void useSecondGyro(bool flag);

  // Returns whether or not the first gyro given is being used
  bool usingFirstGyro();

  // Returns whether or not the second gyro given is being used
  bool usingSecondGyro();

  // Returns the calculated gyroscope value
  double getValue();
};

// Task to be given to the global Gyro object
extern void gyroTask(void * param);

#endif
