#include "main.h"

// Task to be given to the global Gyro object
void gyroTask(void * param) {
  ports::gyro->task();
}

// Task to keep track of gyro overflowing
void Gyro::task() {
  Gyro::rollLastRead = 0;
  Gyro::pitchLastRead = 0;
  Gyro::headingLastRead = 0;
  Gyro::rollRotations = 0;
  Gyro::pitchRotations = 0;
  Gyro::headingRotations = 0;

  while (true) {

    // Handle roll overflowing
    double roll = imu->get_roll();
    if (roll < -90.0 && Gyro::rollLastRead > 90.0)
      rollRotations++;
    if (roll > 90.0 && Gyro::rollLastRead < -90.0)
      rollRotations--;
    // Store the last roll
    Gyro::rollLastRead = roll;

    // Handle pitch overflowing
    double pitch = imu->get_pitch();
    if (pitch < -90.0 && Gyro::pitchLastRead > 90.0)
      pitchRotations++;
    if (pitch > 90.0 && Gyro::pitchLastRead < -90.0)
      pitchRotations--;
    // Store the last pitch
    Gyro::pitchLastRead = pitch;

    // Handle pitch overflowing
    double yaw = imu->get_yaw();
    if (yaw < -90.0 && Gyro::headingLastRead > 90.0)
      headingRotations++;
    if (yaw > 90.0 && Gyro::headingLastRead < -90.0)
      headingRotations--;

    // Store the last heading
    Gyro::headingLastRead = yaw;

    // Run every 15 ms
    pros::delay(15);
  }
}

Gyro::Gyro(pros::Imu * imu) {
  // Sets the imu to the given one
  Gyro::imu = imu;
}

// Returns the calculated roll value
double Gyro::getRoll() {
  return Gyro::imu->get_roll() + 360.0 * Gyro::rollRotations - rollZero;
}

// Returns the calculated pitch value
double Gyro::getPitch() {
  return Gyro::imu->get_pitch() + 360.0 * Gyro::pitchRotations - pitchZero;
}

// Returns the calculated heading value
double Gyro::getHeading() {
  return Gyro::imu->get_yaw() + 360.0 * Gyro::headingRotations - headingZero;
}

// Passthrough for acceleration
pros::c::imu_accel_s_t Gyro::getAcceleration() {
  return Gyro::imu->get_accel();
}

pros::c::imu_gyro_s_t Gyro::getGyroRate() {
  return Gyro::imu->get_gyro_rate();
}

// Sets the current "zero" position to the current one
void Gyro::tarePosition() {
  Gyro::rollZero = Gyro::getRoll();
  Gyro::pitchZero = Gyro::getPitch();
  Gyro::headingZero = Gyro::getHeading();
}

// Sets the current "zero" position to the one at program start
void Gyro::fullTarePosition() {
  // Gyro::zeroPosition = 0;
}
