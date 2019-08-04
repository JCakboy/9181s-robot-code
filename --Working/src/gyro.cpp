#include "main.h"

// Task to be given to the global Gyro object
void gyroTask(void * param) {
  ports::gyro->task();
}

// Task to keep track of gyro overflowing
void Gyro::task() {
  Gyro::firstRotations = 0;
  Gyro::secondRotations = 0;
  Gyro::lastReadFirst = 0;
  Gyro::lastReadSecond = 0;
  while (true) {
    double v1 = gyro1->get_value();
    double v2 = gyro2->get_value();
    if (v1 < 900 && Gyro::lastReadFirst > 2700)
      firstRotations++;
    if (v1 > 2700 && Gyro::lastReadFirst < 900)
      firstRotations--;
    if (v1 > -900 && Gyro::lastReadFirst < -2700)
      firstRotations--;
    if (v1 < -2700 && Gyro::lastReadFirst > -900)
      firstRotations++;

    if (v2 < 900 && Gyro::lastReadSecond > 2700)
      secondRotations++;
    if (v2 > 2700 && Gyro::lastReadSecond < 900)
      secondRotations--;
    if (v2 > -900 && Gyro::lastReadSecond < -2700)
      secondRotations--;
    if (v2 < -2700 && Gyro::lastReadSecond > -900)
      secondRotations++;

    // Store the last gyroscope value
    Gyro::lastReadFirst = v1;
    Gyro::lastReadSecond = v2;

    // Run every 15 ms
    pros::delay(15);
  }
}

Gyro::Gyro(pros::ADIGyro * gyro1, pros::ADIGyro * gyro2) {
  // Sets the gyros to the given ones
  Gyro::gyro1 = gyro1;
  Gyro::gyro2 = gyro2;
  Gyro::useFirst = true;
  Gyro::useSecond = true;
}

void Gyro::useFirstGyro(bool flag) {
  // Set swhether or not the first gyro given is being used
  Gyro::useFirst = flag;
}

void Gyro::useSecondGyro(bool flag) {
  // Sets whether or not the second gyro given is being used
  Gyro::useSecond = flag;
}

bool Gyro::usingFirstGyro() {
  // Returns whether or not the first gyro given is being used
  return Gyro::useFirst;
}

bool Gyro::usingSecondGyro() {
  // Returns whether or not the second gyro given is being used
  return Gyro::useSecond;
}

// Returns the calculated gyroscope value
double Gyro::getValue() {
  double sum = 0;
  double amount = 0;

  if (Gyro::useFirst) {
    sum += gyro1->get_value() + 3600 * firstRotations;
    amount++;
  }
  if (Gyro::useSecond) {
    sum += gyro2->get_value() + 3600 * secondRotations;
    amount++;
  }

  if (amount == 0) amount = 1;

  return sum / amount;
}
