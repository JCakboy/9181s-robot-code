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
    if (useFirst) {
      // Get the gyroscope current value
      double v1 = gyro1->get_value();

      // Handle overflowing
      if (v1 < 900 && Gyro::lastReadFirst > 2700)
        firstRotations++;
      if (v1 > 2700 && Gyro::lastReadFirst < 900)
        firstRotations--;
      if (v1 > -900 && Gyro::lastReadFirst < -2700)
        firstRotations--;
      if (v1 < -2700 && Gyro::lastReadFirst > -900)
        firstRotations++;

      // Store the last gyroscope value
      Gyro::lastReadFirst = v1;
    }

    if (useSecond) {
      // Get the gyroscope current value
      double v2 = gyro2->get_value();

      // Handle overflowing
      if (v2 < 900 && Gyro::lastReadSecond > 2700)
        secondRotations++;
      if (v2 > 2700 && Gyro::lastReadSecond < 900)
        secondRotations--;
      if (v2 > -900 && Gyro::lastReadSecond < -2700)
        secondRotations--;
      if (v2 < -2700 && Gyro::lastReadSecond > -900)
        secondRotations++;

      // Store the last gyroscope value
      Gyro::lastReadSecond = v2;
    }

    // Run every 15 ms
    pros::delay(15);
  }
}

Gyro::Gyro(pros::ADIGyro * gyro1, pros::ADIGyro * gyro2) {
  // Sets the gyros to the given ones
  Gyro::gyro1 = gyro1;
  Gyro::gyro2 = gyro2;

  // Use all gyroscopes provided if they are set
  Gyro::useFirst = false;
  Gyro::useSecond = false;
  if (gyro1 != NULL)
    Gyro::useFirst = true;
  if (gyro2 != NULL)
    Gyro::useSecond = true;
}

// Set swhether or not the first gyro given is being used
void Gyro::useFirstGyro(bool flag) {
  Gyro::useFirst = flag;
}

// Sets whether or not the second gyro given is being used
void Gyro::useSecondGyro(bool flag) {
  Gyro::useSecond = flag;
}

// Returns whether or not the first gyro given is being used
bool Gyro::usingFirstGyro() {
  return Gyro::useFirst;
}

// Returns whether or not the second gyro given is being used
bool Gyro::usingSecondGyro() {
  return Gyro::useSecond;
}

// Returns the calculated gyroscope value
double Gyro::getValue() {
  double sum = 0;
  double amount = 0;

  // Use the first gyroscope given if the flag is set
  if (Gyro::useFirst) {
    sum += gyro1->get_value() + 3600 * firstRotations;
    amount++;
  }

  // Use the second gyroscope given if the flag is set
  if (Gyro::useSecond) {
    sum += gyro2->get_value() + 3600 * secondRotations;
    amount++;
  }

  // Return the avrerage of the used gyroscopes
  if (amount == 0) amount = 1;
  return sum / amount - zeroPosition;
}

// Sets the current "zero" position to the current one
void Gyro::tarePosition() {
  Gyro::zeroPosition = Gyro::getValue();
}

// Sets the current "zero" position to the one at program start
void Gyro::fullTarePosition() {
  Gyro::zeroPosition = 0;
}
