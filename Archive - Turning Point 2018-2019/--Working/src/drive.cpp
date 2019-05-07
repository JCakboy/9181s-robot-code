#include "main.h"
#include <cmath>
#include <vector>
#include "drive.hpp"

void DriveControl::runLeftMotors(int voltage) {
  // Iterate through and set the left motors to a given power
  for (const auto & motor : DriveControl::otherLeftMotors)
    motor->move(voltage);
  for (const auto & motor : DriveControl::frontLeftMotors)
    motor->move(voltage);
  for (const auto & motor : DriveControl::backLeftMotors)
    motor->move(voltage);
}

void DriveControl::runRightMotors(int voltage) {
  // Iterate through and set the right motors to a given power
  for (const auto & motor : DriveControl::otherRightMotors)
    motor->move(voltage);
  for (const auto & motor : DriveControl::frontRightMotors)
    motor->move(voltage);
  for (const auto & motor : DriveControl::backRightMotors)
    motor->move(voltage);
}

void DriveControl::runFrontLeftMotors(int power) {
  // Iterates and runs the front left motors at a given power
    for (const auto & motor : DriveControl::frontLeftMotors)
      motor->move(power);
}

void DriveControl::runBackLeftMotors(int power) {
  // Iterates and runs the back left motors at a given power
    for (const auto & motor : DriveControl::backLeftMotors)
      motor->move(power);
}

void DriveControl::runFrontRightMotors(int power) {
  // Iterates and runs the front right motors at a given power
    for (const auto & motor : DriveControl::frontRightMotors)
      motor->move(power);
}

void DriveControl::runBackRightMotors(int power) {
  // Iterates and runs the back right motors at a given power
    for (const auto & motor : DriveControl::backRightMotors)
      motor->move(power);
}

void DriveControl::setLeftBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the left motors to a given brake mode
  for (const auto & motor : DriveControl::otherLeftMotors)
    motor->set_brake_mode(mode);
  for (const auto & motor : DriveControl::frontLeftMotors)
    motor->set_brake_mode(mode);
  for (const auto & motor : DriveControl::backLeftMotors)
    motor->set_brake_mode(mode);
}

void DriveControl::setRightBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the right motors to a given brake mode
  for (const auto & motor : DriveControl::otherRightMotors)
    motor->set_brake_mode(mode);
  for (const auto & motor : DriveControl::frontRightMotors)
    motor->set_brake_mode(mode);
  for (const auto & motor : DriveControl::backRightMotors)
    motor->set_brake_mode(mode);
}

void DriveControl::setFrontLeftBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the front left motors to a given brake mode
  for (const auto & motor : DriveControl::frontLeftMotors)
    motor->set_brake_mode(mode);
}

void DriveControl::setBackLeftBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the back left motors to a given brake mode
  for (const auto & motor : DriveControl::backLeftMotors)
    motor->set_brake_mode(mode);
}

void DriveControl::setFrontRightBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the front right motors to a given brake mode
  for (const auto & motor : DriveControl::frontRightMotors)
    motor->set_brake_mode(mode);
}

void DriveControl::setBackRightBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the back right motors to a given brake mode
  for (const auto & motor : DriveControl::backRightMotors)
    motor->set_brake_mode(mode);
}
/*
PIDCommand DriveControl::runMotorsRelative(PID * pid, PIDCalc * calc, std::vector<pros::Motor *> motors, int target) {
  if (!usePID) if (lock->take(MUTEX_WAIT_TIME)) {
    // PID values are not set, issue simple move command
    for (const auto & motor : motors)
      motor->move(util::limitX(MOTOR_MOVE_RELATIVE_MAX_SPEED, target * MOTOR_MOVE_RELATIVE_KP));
    lock->give();
    return PIDCommand(E_COMMAND_NO_CALCULATION, 0);
  } else; else if (motors.size() > 0) {
    // PID values are set, calculate the output with PID
    // Calculate the average motor position, giving a better sense of the robot position
    int position = 0;
    for (const auto & motor : motors)
      position += motor->get_position();
    position = position / util::sign(motors.size());

    // Calculate the output
    return pid->calculate(calc, position, target);
  }
}
*/
DriveControl::DriveControl(pros::Mutex * motorLock, pros::Motor * leftMotor, pros::Motor * rightMotor) {
  // Two wheel drive initialization
  // Store the mutex
  DriveControl::lock = motorLock;
  // Add the left motor
  DriveControl::addLeftMotor(leftMotor);
  // Add the right motor
  DriveControl::addRightMotor(rightMotor);
}

DriveControl::DriveControl(pros::Mutex * motorLock, pros::Motor * frontLeftMotor, pros::Motor * rearLeftMotor, pros::Motor * frontRightMotor, pros::Motor * rearRightMotor) {
  // Four wheel drive initialization
  // Store the mutex
  DriveControl::lock = motorLock;
  // Add the left motors
  DriveControl::addFrontLeftMotor(frontLeftMotor);
  DriveControl::addBackLeftMotor(rearLeftMotor);
  // Add the right motors
  DriveControl::addFrontRightMotor(frontRightMotor);
  DriveControl::addBackRightMotor(rearRightMotor);
}

void DriveControl::reinitialize(pros::Motor * leftMotor, pros::Motor * rightMotor) {
  // Clear the existing drive configuration
  DriveControl::clearMotors();
  // Two wheel drive initialization
  // Add the new left motor
  DriveControl::addLeftMotor(leftMotor);
  // Add the new right motor
  DriveControl::addRightMotor(rightMotor);
}

void DriveControl::reinitialize(pros::Motor * frontLeftMotor, pros::Motor * rearLeftMotor, pros::Motor * frontRightMotor, pros::Motor * rearRightMotor) {
  // Clear the existing drive configuration
  DriveControl::clearMotors();
  // Four wheel drive initialization
  // Add the new left motors
  DriveControl::addFrontLeftMotor(frontLeftMotor);
  DriveControl::addBackLeftMotor(rearLeftMotor);
  // Add the new right motors
  DriveControl::addFrontRightMotor(frontRightMotor);
  DriveControl::addBackRightMotor(rearRightMotor);
}

void DriveControl::addLeftMotor(pros::Motor * motor) {
  // Add the motor to the list of left motors
  DriveControl::otherLeftMotors.push_back(motor);
}

void DriveControl::addFrontLeftMotor(pros::Motor * motor) {
  // Adds a motor to the front left motors list
  DriveControl::frontLeftMotors.push_back(motor);
}

void DriveControl::addBackLeftMotor(pros::Motor * motor) {
  // Adds a motor to the back left motors list
  DriveControl::backLeftMotors.push_back(motor);
}

void DriveControl::addRightMotor(pros::Motor * motor) {
  // Add the motor to the list of right motors
  DriveControl::otherRightMotors.push_back(motor);
}

void DriveControl::addFrontRightMotor(pros::Motor * motor) {
  // Adds a motor to the front right motors list
  DriveControl::frontRightMotors.push_back(motor);
}

void DriveControl::addBackRightMotor(pros::Motor * motor) {
  // Adds a motor to the back right motors list
  DriveControl::backRightMotors.push_back(motor);
}

void DriveControl::clearMotors() {
  // Clear all left motors
  DriveControl::clearLeftMotors();
  // Clear all right motors
  DriveControl::clearRightMotors();
}

void DriveControl::clearLeftMotors() {
  // Clear all left motors
  DriveControl::otherLeftMotors.clear();
  DriveControl::clearFrontLeftMotors();
  DriveControl::clearBackLeftMotors();
}

void DriveControl::clearRightMotors() {
  // Clear all right motors
  DriveControl::otherRightMotors.clear();
  DriveControl::clearFrontRightMotors();
  DriveControl::clearBackRightMotors();
}

void DriveControl::clearFrontLeftMotors() {
  // Empty the list of front left motors
  DriveControl::frontLeftMotors.clear();
}

void DriveControl::clearBackLeftMotors() {
  // Empty the list of back left motors
  DriveControl::backLeftMotors.clear();
}

void DriveControl::clearFrontRightMotors() {
  // Empty the list of front right motors
  DriveControl::frontRightMotors.clear();
}

void DriveControl::clearBackRightMotors() {
  // Empty the list of back right motors
  DriveControl::backRightMotors.clear();
}
/*
void DriveControl::moveRelative(int frontLeftDegrees, int backLeftDegrees, int frontRightDegrees, int backRightDegrees) {
  int leftAverage = (frontLeftDegrees + backLeftDegrees) / 2;
  int rightAverage = (frontRightDegrees + backRightDegrees) / 2;

  // The amount of disconnected motors
  int lDisconnect = 0;
  int rDisconnect = 0;

  /*
   * Iterates through the left and right motors to check whether they are connected
   * If they are connected, proceed
   * If not, an event is logged and the motor is temporarily removed from the list
   * If no motors are plugged in on either side, abort
   *//*
  for (const auto & motor : DriveControl::otherLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || frontLeftPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveControl::frontLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || frontLeftPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveControl::backLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || backLeftPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveControl::otherRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || frontRightPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }
  for (const auto & motor : DriveControl::frontRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || frontRightPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }
  for (const auto & motor : DriveControl::backRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    motor->set_brake_mode(!usePID || backRightPID->brake ? BRAKE_HOLD : BRAKE_COAST);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }

  // Whether to abort
  bool abort = false;

  // Display disconnect errors
  if (lDisconnect > 0) {
    int totalsize = DriveControl::otherLeftMotors.size() + DriveControl::frontLeftMotors.size() + DriveControl::backLeftMotors.size();
    if (totalsize == lDisconnect)
      Logger::log(LOG_ERROR, "All of the left side drive motors have been disconnected! Aborting...");
    else
      Logger::log(LOG_ERROR, "Some of the left side drive motors have been disconnected! Aborting...");
    abort = true;
  }
  if (rDisconnect > 0) {
    int totalsize = DriveControl::otherRightMotors.size() + DriveControl::frontRightMotors.size() + DriveControl::backRightMotors.size();
    if (totalsize == rDisconnect)
      Logger::log(LOG_ERROR, "All of the right side drive motors have been disconnected! Aborting...");
    else
      Logger::log(LOG_ERROR, "Some of the right side drive motors have been disconnected! Aborting...");
    abort = true;
  }
  if (abort) return;

  // Display and log for debugging purposes
  LCD::setStatus("Auto driving: L:" + std::to_string(leftAverage) + ", R" + std::to_string(rightAverage));
  Logger::log(LOG_INFO, "Auto Driving - \t" + std::to_string(frontLeftDegrees) + "\t\t" + std::to_string(frontRightDegrees));
  Logger::log(LOG_INFO, "Auto Driving - \t" + std::to_string(backLeftDegrees) + "\t\t" + std::to_string(backRightDegrees));

  if (!usePID) {
    // PID values have not been set, issue simple move commands
    if (leftAverage != 0) DriveControl::runMotorsRelative(NULL, NULL, otherLeftMotors, leftAverage);
    if (rightAverage != 0) DriveControl::runMotorsRelative(NULL, NULL, otherRightMotors, rightAverage);

    // Loop until done
    while (true) {
      bool done = true;
      int count = 0;
      long total = 0;

      // Run the front left motors
      for (const auto & motor : frontLeftMotors) {
        if (util::abs(frontLeftDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::frontLeftMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, frontLeftMotors, frontLeftDegrees - (total / util::sign(frontLeftMotors.size())));
      } else for (const auto & motor : frontLeftMotors) motor->move(0);

      count = 0;
      total = 0;
      // Run the front right motors
      for (const auto & motor : frontRightMotors) {
        if (util::abs(frontRightDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::frontRightMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, frontRightMotors, frontRightDegrees - (total / util::sign(frontRightMotors.size())));
      } else for (const auto & motor : frontRightMotors) motor->move(0);

      count = 0;
      total = 0;
      // Run the back left motors
      for (const auto & motor : backLeftMotors) {
        if (util::abs(backLeftDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::backLeftMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, backLeftMotors, backLeftDegrees - (total / util::sign(backLeftMotors.size())));
      } else for (const auto & motor : backLeftMotors) motor->move(0);

      count = 0;
      total = 0;
      // Run the back right motors
      for (const auto & motor : backRightMotors) {
        if (util::abs(backRightDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::backRightMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, backRightMotors, backRightDegrees - (total / util::sign(backRightMotors.size())));
      } else for (const auto & motor : backRightMotors) motor->move(0);

      count = 0;
      total = 0;
      // Run the other left motors
      for (const auto & motor : otherLeftMotors) {
        if (util::abs(leftAverage - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::otherLeftMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, otherLeftMotors, leftAverage - (total / util::sign(otherLeftMotors.size())));
      } else for (const auto & motor : otherLeftMotors) motor->move(0);

      count = 0;
      total = 0;
      // Run the other right motors
      for (const auto & motor : otherRightMotors) {
        if (util::abs(rightAverage - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          count++;
        total += motor->get_position();
      }
      // Check for completion
      if (count == DriveControl::otherRightMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(NULL, NULL, otherRightMotors, rightAverage - (total / util::sign(otherRightMotors.size())));
      } else for (const auto & motor : otherRightMotors) motor->move(0);

      // If both sides are done, exit
      if (done) break;
      // Run every 20ms
      pros::delay(20);
    }
    // Stop the motors together
    DriveControl::runLeftMotors(0);
    DriveControl::runRightMotors(0);
    // Log the completion
    LCD::setStatus("Movement Complete");
    Logger::log(LOG_INFO, "Movement Complete");
  } else {
    // Create calculation classes
    PIDCalc * frontLeftPIDCalc = new PIDCalc();
    PIDCalc * backLeftPIDCalc = new PIDCalc();
    PIDCalc * frontRightPIDCalc = new PIDCalc();
    PIDCalc * backRightPIDCalc = new PIDCalc();
    PIDCalc * otherLeftPIDCalc = new PIDCalc();
    PIDCalc * otherRightPIDCalc = new PIDCalc();
    // Check whether to move the motors
    bool frontLeftComplete = frontLeftDegrees == 0;
    bool backLeftComplete = backLeftDegrees == 0;
    bool frontRightComplete = frontRightDegrees == 0;
    bool backRightComplete = backRightDegrees == 0;
    bool otherLeftComplete = leftAverage == 0;
    bool otherRightComplete = rightAverage == 0;
    int frontLeftPower = 0;
    int backLeftPower = 0;
    int frontRightPower = 0;
    int backRightPower = 0;
    int otherLeftPower = 0;
    int otherRightPower = 0;

    // Completion string
    std::string message;
    while (!(frontLeftComplete && backLeftComplete && frontRightComplete && backRightComplete)) {
      // Calculate individual powers for each side

      if (frontLeftMotors.size() != 0) {
        // Calculate a power for the front left motors
        PIDCommand frontLeft = DriveControl::runMotorsRelative(frontLeftPID, frontLeftPIDCalc, frontLeftMotors, frontLeftDegrees);
        frontLeftPower = frontLeft.result;

        if (!frontLeftComplete) {
          // Front left side is not complete, check for completion signal
          if (frontLeft.type == E_COMMAND_EXIT_FAILURE || frontLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (frontLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Front left side failed to complete, stuck on an object
              message += "FLF";
              Logger::log(LOG_WARNING, "Front Left has existed with a failure status! Threshold: " + std::to_string(frontLeftPID->dThreshold) + ", Error: " + std::to_string(frontLeftPIDCalc->lastError));
            } else {
              // Front left side successfully completed
              message += "FLS";
              Logger::log(LOG_INFO, "Front Left has existed with a success status. Error: " + std::to_string(frontLeftPIDCalc->lastError));
            }
            frontLeftComplete = true;
          }
        }
      }

      if (frontRightMotors.size() != 0) {
        // Calculate a power for the front left motors
        PIDCommand frontRight = DriveControl::runMotorsRelative(frontRightPID, frontRightPIDCalc, frontRightMotors, frontRightDegrees);
        frontRightPower = frontRight.result;

        if (!frontRightComplete) {
          // Front right side is not complete, check for completion signal
          if (frontRight.type == E_COMMAND_EXIT_FAILURE || frontRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (frontRight.type == E_COMMAND_EXIT_FAILURE) {
              // Front right side failed to complete, stuck on an object
              message += "FRF";
              Logger::log(LOG_WARNING, "Front Right has existed with a failure status! Threshold: " + std::to_string(frontRightPID->dThreshold) + ", Error: " + std::to_string(frontRightPIDCalc->lastError));
            } else {
              // Front right side successfully completed
              message += "FRS";
              Logger::log(LOG_INFO, "Front Right has existed with a success status. Error: " + std::to_string(frontRightPIDCalc->lastError));
            }
            frontRightComplete = true;
          }
        }
      }

      if (backLeftMotors.size() != 0) {
        // Calculate a power for the back left motors
        PIDCommand backLeft = DriveControl::runMotorsRelative(backLeftPID, backLeftPIDCalc, backLeftMotors, backLeftDegrees);
        backLeftPower = backLeft.result;

        if (!backLeftComplete) {
          // Back left side is not complete, check for completion signal
          if (backLeft.type == E_COMMAND_EXIT_FAILURE || backLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (backLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Back left side failed to complete, stuck on an object
              message += "FLF";
              Logger::log(LOG_WARNING, "Back Left has existed with a failure status! Threshold: " + std::to_string(backLeftPID->dThreshold) + ", Error: " + std::to_string(backLeftPIDCalc->lastError));
            } else {
              // Back left side successfully completed
              message += "FLS";
              Logger::log(LOG_INFO, "Back Left has existed with a success status. Error: " + std::to_string(backLeftPIDCalc->lastError));
            }
            backLeftComplete = true;
          }
        }
      }

      if (backRightMotors.size() != 0) {
        // Calculate a power for the back left motors
        PIDCommand backRight = DriveControl::runMotorsRelative(backRightPID, backRightPIDCalc, backRightMotors, backRightDegrees);
        backRightPower = backRight.result;

        if (!backRightComplete) {
          // Back right side is not complete, check for completion signal
          if (backRight.type == E_COMMAND_EXIT_FAILURE || backRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (backRight.type == E_COMMAND_EXIT_FAILURE) {
              // Back right side failed to complete, stuck on an object
              message += "FRF";
              Logger::log(LOG_WARNING, "Back Right has existed with a failure status! Threshold: " + std::to_string(backRightPID->dThreshold) + ", Error: " + std::to_string(backRightPIDCalc->lastError));
            } else {
              // Back right side successfully completed
              message += "FRS";
              Logger::log(LOG_INFO, "Back Right has existed with a success status. Error: " + std::to_string(backRightPIDCalc->lastError));
            }
            backRightComplete = true;
          }
        }
      }

      if (otherLeftMotors.size() != 0) {
        // Calculate a power for the other left motors
        PIDCommand otherLeft = DriveControl::runMotorsRelative(frontLeftPID, otherLeftPIDCalc, otherLeftMotors, leftAverage);
        otherLeftPower = otherLeft.result;

        if (!otherLeftComplete) {
          // Other left side is not complete, check for completion signal
          if (otherLeft.type == E_COMMAND_EXIT_FAILURE || otherLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (otherLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Other left side failed to complete, stuck on an object
              message += "OLF";
              Logger::log(LOG_WARNING, "Other Left has existed with a failure status! Threshold: " + std::to_string(frontLeftPID->dThreshold) + ", Error: " + std::to_string(otherLeftPIDCalc->lastError));
            } else {
              // Other left side successfully completed
              message += "OLS";
              Logger::log(LOG_INFO, "Other Left has existed with a success status. Error: " + std::to_string(otherLeftPIDCalc->lastError));
            }
            otherLeftComplete = true;
          }
        }
      }

      if (otherRightMotors.size() != 0) {
        // Calculate a power for the other left motors
        PIDCommand otherRight = DriveControl::runMotorsRelative(frontRightPID, otherRightPIDCalc, otherRightMotors, rightAverage);
        otherRightPower = otherRight.result;

        if (!otherRightComplete) {
          // Other right side is not complete, check for completion signal
          if (otherRight.type == E_COMMAND_EXIT_FAILURE || otherRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (otherRight.type == E_COMMAND_EXIT_FAILURE) {
              // Other right side failed to complete, stuck on an object
              message += "ORF";
              Logger::log(LOG_WARNING, "Other Right has existed with a failure status! Threshold: " + std::to_string(frontRightPID->dThreshold) + ", Error: " + std::to_string(otherRightPIDCalc->lastError));
            } else {
              // Other right side successfully completed
              message += "ORS";
              Logger::log(LOG_INFO, "Other Right has existed with a success status. Error: " + std::to_string(otherRightPIDCalc->lastError));
            }
            otherRightComplete = true;
          }
        }
      }

      if (lock->take(MUTEX_WAIT_TIME)) {
        // Iterate through and issue the commands to the motors
        for (const auto & motor : DriveControl::frontLeftMotors)
          motor->move(frontLeftPower);
        for (const auto & motor : DriveControl::frontRightMotors)
          motor->move(frontRightPower);
        for (const auto & motor : DriveControl::backLeftMotors)
          motor->move(backLeftPower);
        for (const auto & motor : DriveControl::backRightMotors)
          motor->move(backRightPower);
        for (const auto & motor : DriveControl::otherLeftMotors)
          motor->move(otherLeftPower);
        for (const auto & motor : DriveControl::otherRightMotors)
          motor->move(otherRightPower);
        lock->give();
      }

      // Delay for the average time delta across all PID constants
      pros::delay((frontLeftPID->dt + backLeftPID->dt + frontRightPID->dt + backRightPID->dt) / 4.0);
    }
    // Stop the motors together
    DriveControl::runLeftMotors(0);
    DriveControl::runRightMotors(0);
    // Log the completion
    LCD::setStatus("PID Complete " + message);
    Logger::log(LOG_INFO, "PID Complete");
    // Free up memory
    delete frontLeftPIDCalc;
    delete backLeftPIDCalc;
    delete frontRightPIDCalc;
    delete backRightPIDCalc;
    delete otherLeftPIDCalc;
    delete otherRightPIDCalc;
  }
}
*/
void DriveControl::run(double moveVoltage, double turnVoltage, bool brake, bool flipReverse) {
  // Pass the call to the longer run() function
  DriveControl::runH(moveVoltage, turnVoltage, brake, flipReverse, 1.0, 1.0);
}

void DriveControl::runH(double moveVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  // Flip the left and right outputs if reversing, meant to better map the analog stick positions to actual robot movement
  bool flip = flipReverse && moveVoltage < MOTOR_REVERSE_FLIP_THRESHOLD;

  // Multiply the voltages by the sensitivity
  moveVoltage *= moveSensitivity;
  turnVoltage *= turnSensitivity;

  // Calculate the left and right outputs
  int leftVoltage = util::limit127(!flip ? moveVoltage + turnVoltage : moveVoltage - turnVoltage);
  int rightVoltage = util::limit127(!flip ? moveVoltage - turnVoltage : moveVoltage + turnVoltage);

  if (lock->take(MUTEX_WAIT_TIME)) {
    // Issue the move and brake commands to the motors
    setLeftBrake(brake ? BRAKE_BRAKE : BRAKE_COAST);
    setRightBrake(brake ? BRAKE_BRAKE : BRAKE_COAST);
    runLeftMotors(leftVoltage);
    runRightMotors(rightVoltage);
    lock->give();
  }
}

void DriveControl::runStrafe(double moveVoltage, double turnStrafeVoltage, bool strafe, bool brake, bool flipReverse, double moveSensitivity, double turnStrafeSensitivity) {
    // Pass the call to runX while interpreting the turnStrafeVoltage
    DriveControl::runX(moveVoltage, strafe ? turnStrafeVoltage : 0, strafe ? 0 : turnStrafeVoltage, brake, flipReverse, moveSensitivity, strafe ? turnStrafeSensitivity : 0, strafe ? 0 : turnStrafeSensitivity);
}

void DriveControl::runX(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double strafeSensitivity, double turnSensitivity) {
  // Flip the left and right outputs if reversing, meant to better map the analog stick positions to actual robot movement
  bool flip = flipReverse && moveVoltage < MOTOR_REVERSE_FLIP_THRESHOLD;

  // Multiply the voltages by the sensitivity
  moveVoltage *= moveSensitivity;
  strafeVoltage *= strafeSensitivity;
  turnVoltage *= turnSensitivity;

  // Variables for calculating motor outputs
  double frontLeftVoltage = 0;
  double backLeftVoltage = 0;
  double frontRightVoltage = 0;
  double backRightVoltage = 0;

  // Calculate the motor outputs
  if (moveVoltage != 0) {
    frontLeftVoltage += moveVoltage;
    backLeftVoltage += moveVoltage;
    frontRightVoltage += moveVoltage;
    backRightVoltage += moveVoltage;
  }
  if (strafeVoltage != 0) {
    // Flip the left and right outputs if reversing, meant to better map the analog stick positions to actual robot movement
    frontLeftVoltage += strafeVoltage;
    backLeftVoltage -= strafeVoltage;
    frontRightVoltage -= strafeVoltage;
    backRightVoltage += strafeVoltage;
    frontLeftVoltage += 20;
    backLeftVoltage += 20;
    frontRightVoltage += 20;
    backRightVoltage += 20;
  }
  if (turnVoltage != 0) {
    if (flip) turnVoltage = -turnVoltage;
    frontLeftVoltage += turnVoltage;
    backLeftVoltage += turnVoltage;
    frontRightVoltage -= turnVoltage;
    backRightVoltage -= turnVoltage;
  }

  if (lock->take(MUTEX_WAIT_TIME)) {
    // Issue the move and brake commands to the motors
    setLeftBrake(brake ? BRAKE_BRAKE : BRAKE_COAST);
    setRightBrake(brake ? BRAKE_BRAKE : BRAKE_COAST);
    runFrontLeftMotors(frontLeftVoltage);
    runBackLeftMotors(backLeftVoltage);
    runFrontRightMotors(frontRightVoltage);
    runBackRightMotors(backRightVoltage);
    lock->give();
  }
}

void DriveControl::stop(bool brake) {
  DriveControl::runX(0, 0, 0, brake, false, 1.0, 1.0, 1.0);
}


DriveFunction::DriveFunction(DriveControl * driveControl) {
  // Store the DriveControl object to wrap/call
  DriveFunction::driveControl = driveControl;

  // Gear ratio has not been set
  DriveFunction::in = 0;
  DriveFunction::out = 0;
  DriveFunction::wheelDiameter = 0;

  // Turn values has not been set
  DriveFunction::pt = 0;
  DriveFunction::kt = 0;

  // Strafe value has not been set
  DriveFunction::ks = 0;

  // PID values have not been set
  DriveFunction::useForwardPID = false;
  DriveFunction::useBackwardPID = false;
  DriveFunction::usePivotPID = false;
  DriveFunction::useStrafePID = false;

  DriveFunction::forwardFrontLeftPID = NULL;
  DriveFunction::forwardBackLeftPID = NULL;
  DriveFunction::forwardFrontRightPID = NULL;
  DriveFunction::forwardBackRightPID = NULL;

  DriveFunction::backwardFrontLeftPID = NULL;
  DriveFunction::backwardBackLeftPID = NULL;
  DriveFunction::backwardFrontRightPID = NULL;
  DriveFunction::backwardBackRightPID = NULL;

  DriveFunction::pivotFrontLeftPID = NULL;
  DriveFunction::pivotBackLeftPID = NULL;
  DriveFunction::pivotFrontRightPID = NULL;
  DriveFunction::pivotBackRightPID = NULL;

  DriveFunction::strafeFrontLeftPID = NULL;
  DriveFunction::strafeBackLeftPID = NULL;
  DriveFunction::strafeFrontRightPID = NULL;
  DriveFunction::strafeBackRightPID = NULL;
}

DriveControl * DriveFunction::getDriveControl() {
  // Returns the stored DriveControl Object pointer
  return DriveFunction::driveControl;
}

void DriveFunction::setStrafeValue(int ks) {
  DriveFunction::ks = ks;
}

int DriveFunction::getStrafeValue() {
  return ks;
}

void DriveFunction::setTurnValues(int pt, int kt) {
  // Sets the turn proportion and the turn constant
  DriveFunction::pt = pt;
  DriveFunction::kt = kt;
}

std::pair<int, int> DriveFunction::getTurnValues() {
  // Returns the turn proportion and the turn constant
  return std::pair<int, int>(pt, kt);
}

void DriveFunction::setGearRatio(double in, double out, double wheelDiameter) {
  // Sets the gear ratio
  DriveFunction::in = in;
  DriveFunction::out = out;
  DriveFunction::wheelDiameter = wheelDiameter;
}

double DriveFunction::getFullRatio() {
  // Calculates and returns the full gear ratio
  return (360 * out) / (wheelDiameter * PI * in);
}

double DriveFunction::getInputRatio() {
  // Returns the set input ratio
  return DriveFunction::in;
}

double DriveFunction::getOutputRatio() {
  // Returns the set output ratio
  return DriveFunction::out;
}

double DriveFunction::getWheelDiameter() {
  // Returns the set input wheel diameter
  return DriveFunction::wheelDiameter;
}

void DriveFunction::setForwardPID(PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID) {
  if (useForwardPID)
    DriveFunction::clearForwardPID();
  DriveFunction::useForwardPID = true;
  DriveFunction::forwardFrontLeftPID = frontLeftPID;
  DriveFunction::forwardBackLeftPID = backLeftPID;
  DriveFunction::forwardFrontRightPID = frontRightPID;
  DriveFunction::forwardBackRightPID = backRightPID;
}

void DriveFunction::setBackwardPID(PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID) {
  if (useBackwardPID)
    DriveFunction::clearBackwardPID();
  DriveFunction::useBackwardPID = true;
  DriveFunction::backwardFrontLeftPID = frontLeftPID;
  DriveFunction::backwardBackLeftPID = backLeftPID;
  DriveFunction::backwardFrontRightPID = frontRightPID;
  DriveFunction::backwardBackRightPID = backRightPID;
}

void DriveFunction::setPivotPID(PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID) {
  if (usePivotPID)
    DriveFunction::clearPivotPID();
  DriveFunction::usePivotPID = true;
  DriveFunction::pivotFrontLeftPID = frontLeftPID;
  DriveFunction::pivotBackLeftPID = backLeftPID;
  DriveFunction::pivotFrontRightPID = frontRightPID;
  DriveFunction::pivotBackRightPID = backRightPID;
}

void DriveFunction::setStrafePID(PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID) {
  if (useStrafePID)
    DriveFunction::clearStrafePID();
  DriveFunction::useStrafePID = true;
  DriveFunction::strafeFrontLeftPID = frontLeftPID;
  DriveFunction::strafeBackLeftPID = backLeftPID;
  DriveFunction::strafeFrontRightPID = frontRightPID;
  DriveFunction::strafeBackRightPID = backRightPID;
}

void DriveFunction::clearForwardPID() {
  if (!useForwardPID)
    return;
  DriveFunction::useForwardPID = false;
  delete DriveFunction::forwardFrontLeftPID;
  delete DriveFunction::forwardBackLeftPID;
  delete DriveFunction::forwardFrontRightPID;
  delete DriveFunction::forwardBackRightPID;
  DriveFunction::forwardFrontLeftPID = NULL;
  DriveFunction::forwardBackLeftPID = NULL;
  DriveFunction::forwardFrontRightPID = NULL;
  DriveFunction::forwardBackRightPID = NULL;
}

void DriveFunction::clearBackwardPID() {
  if (!useBackwardPID)
    return;
  DriveFunction::useBackwardPID = false;
  delete DriveFunction::backwardFrontLeftPID;
  delete DriveFunction::backwardBackLeftPID;
  delete DriveFunction::backwardFrontRightPID;
  delete DriveFunction::backwardBackRightPID;
  DriveFunction::backwardFrontLeftPID = NULL;
  DriveFunction::backwardBackLeftPID = NULL;
  DriveFunction::backwardFrontRightPID = NULL;
  DriveFunction::backwardBackRightPID = NULL;
}

void DriveFunction::clearPivotPID() {
  if (!usePivotPID)
    return;
  DriveFunction::usePivotPID = false;
  delete DriveFunction::pivotFrontLeftPID;
  delete DriveFunction::pivotBackLeftPID;
  delete DriveFunction::pivotFrontRightPID;
  delete DriveFunction::pivotBackRightPID;
  DriveFunction::pivotFrontLeftPID = NULL;
  DriveFunction::pivotBackLeftPID = NULL;
  DriveFunction::pivotFrontRightPID = NULL;
  DriveFunction::pivotBackRightPID = NULL;
}

void DriveFunction::clearStrafePID() {
  if (!useStrafePID)
    return;
  DriveFunction::useStrafePID = false;
  delete DriveFunction::strafeFrontLeftPID;
  delete DriveFunction::strafeBackLeftPID;
  delete DriveFunction::strafeFrontRightPID;
  delete DriveFunction::strafeBackRightPID;
  DriveFunction::strafeFrontLeftPID = NULL;
  DriveFunction::strafeBackLeftPID = NULL;
  DriveFunction::strafeFrontRightPID = NULL;
  DriveFunction::strafeBackRightPID = NULL;
}

double DriveFunction::averagePosition(std::vector<pros::Motor *> motors) {
  double position = 0;
  for (const auto & motor : motors)
    position += motor->get_position();
  return position / util::sign(motors.size());
}

bool DriveFunction::movementReset() {
  // The amount of disconnected motors
  int lDisconnect = 0;
  int rDisconnect = 0;

  /*
   * Iterates through the left and right motors to check whether they are connected
   * If they are connected, proceed
   * If not, an event is logged and the motor is temporarily removed from the list
   * If no motors are plugged in on either side, abort
   */
  for (const auto & motor : DriveFunction::driveControl->otherLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveFunction::driveControl->frontLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveFunction::driveControl->backLeftMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      lDisconnect++;
  }
  for (const auto & motor : DriveFunction::driveControl->otherRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }
  for (const auto & motor : DriveFunction::driveControl->frontRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }
  for (const auto & motor : DriveFunction::driveControl->backRightMotors) {
    motor->tare_position();
    motor->set_encoder_units(ENCODER_DEGREES);
    if (motor->get_efficiency() > 1000)
      rDisconnect++;
  }

  // Whether to abort
  bool abort = false;

  // Display disconnect errors
  if (lDisconnect > 0) {
    int totalsize = driveControl->otherLeftMotors.size() + driveControl->frontLeftMotors.size() + driveControl->backLeftMotors.size();
    if (totalsize == lDisconnect)
      Logger::log(LOG_ERROR, "All of the left side drive motors have been disconnected! Aborting...");
    else
      Logger::log(LOG_ERROR, "Some of the left side drive motors have been disconnected! Aborting...");
    abort = true;
  }
  if (rDisconnect > 0) {
    int totalsize = driveControl->otherRightMotors.size() + driveControl->frontRightMotors.size() + driveControl->backRightMotors.size();
    if (totalsize == rDisconnect)
      Logger::log(LOG_ERROR, "All of the right side drive motors have been disconnected! Aborting...");
    else
      Logger::log(LOG_ERROR, "Some of the right side drive motors have been disconnected! Aborting...");
    abort = true;
  }
  return !abort;
}

void DriveFunction::moveRelative(bool usePID, PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID, int frontLeftDegrees, int backLeftDegrees, int frontRightDegrees, int backRightDegrees) {
  // If resetting failed, exit this call
  if (!movementReset()) return;

  // Calculate left and right averages
  int leftDegrees = (frontLeftDegrees + backLeftDegrees) / 2.0;
  int rightDegrees = (frontRightDegrees + backRightDegrees) / 2.0;

  // Set the brake mode of this PID
  driveControl->setLeftBrake((frontLeftPID == NULL || !frontLeftPID->brake) ? BRAKE_COAST : BRAKE_BRAKE);
  driveControl->setBackLeftBrake((backLeftPID == NULL || !backLeftPID->brake) ? BRAKE_COAST : BRAKE_BRAKE);
  driveControl->setRightBrake((frontRightPID == NULL || !frontRightPID->brake) ? BRAKE_COAST : BRAKE_BRAKE);
  driveControl->setBackRightBrake((backRightPID == NULL || !backRightPID->brake) ? BRAKE_COAST : BRAKE_BRAKE);

  if (!usePID) {
    // PID values have not been set, issue simple move commands
    for (const auto & motor : driveControl->otherLeftMotors)
      motor->move_relative(leftDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);
    for (const auto & motor : driveControl->otherRightMotors)
      motor->move_relative(rightDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);
    for (const auto & motor : driveControl->frontLeftMotors)
      motor->move_relative(frontLeftDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);
    for (const auto & motor : driveControl->frontRightMotors)
      motor->move_relative(frontRightDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);
    for (const auto & motor : driveControl->backLeftMotors)
      motor->move_relative(backLeftDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);
    for (const auto & motor : driveControl->backRightMotors)
      motor->move_relative(backRightDegrees, MOTOR_MOVE_RELATIVE_MAX_SPEED);

    // Loop and check for completion
    while (true) {
      bool done = true;

      // Check the other left motors for completion
      for (const auto & motor : driveControl->otherLeftMotors) {
        if (util::abs(leftDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // Check the other right motors for completion
      for (const auto & motor : driveControl->otherRightMotors) {
        if (util::abs(rightDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // Check the front left motors for completion
      for (const auto & motor : driveControl->frontLeftMotors) {
        if (util::abs(frontLeftDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // Check the front right motors for completion
      for (const auto & motor : driveControl->frontRightMotors) {
        if (util::abs(frontRightDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // Check the back left motors for completion
      for (const auto & motor : driveControl->backLeftMotors) {
        if (util::abs(backLeftDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // Check the back right motors for completion
      for (const auto & motor : driveControl->backRightMotors) {
        if (util::abs(backRightDegrees - motor->get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD)
          done = false;
      }

      // If done, exit the loop
      if (done) break;

      // Otherwise, wait and check again
      pros::delay(20);
    }

    // Stop the motors together
    driveControl->runLeftMotors(0);
    driveControl->runRightMotors(0);
    // Log the completion
    LCD::setStatus("Movement Complete");
    Logger::log(LOG_INFO, "Movement Complete");
  } else {
    // Create calculation classes
    PIDCalc * frontLeftPIDCalc = new PIDCalc();
    PIDCalc * backLeftPIDCalc = new PIDCalc();
    PIDCalc * frontRightPIDCalc = new PIDCalc();
    PIDCalc * backRightPIDCalc = new PIDCalc();
    PIDCalc * otherLeftPIDCalc = new PIDCalc();
    PIDCalc * otherRightPIDCalc = new PIDCalc();
    // Check whether to move the motors
    bool frontLeftComplete = frontLeftDegrees == 0;
    bool backLeftComplete = backLeftDegrees == 0;
    bool frontRightComplete = frontRightDegrees == 0;
    bool backRightComplete = backRightDegrees == 0;
    bool otherLeftComplete = leftDegrees == 0;
    bool otherRightComplete = rightDegrees == 0;
    int frontLeftPower = 0;
    int backLeftPower = 0;
    int frontRightPower = 0;
    int backRightPower = 0;
    int otherLeftPower = 0;
    int otherRightPower = 0;

    // Completion string
    std::string message;
    while (!(frontLeftComplete && backLeftComplete && frontRightComplete && backRightComplete)) {
      // Calculate individual powers for each side

      if (driveControl->frontLeftMotors.size() != 0) {
        // Calculate a power for the front left motors
        PIDCommand frontLeft = frontLeftPID->calculate(frontLeftPIDCalc, averagePosition(driveControl->frontLeftMotors), frontLeftDegrees);
        frontLeftPower = frontLeft.result;

        if (!frontLeftComplete) {
          // Front left side is not complete, check for completion signal
          if (frontLeft.type == E_COMMAND_EXIT_FAILURE || frontLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (frontLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Front left side failed to complete, stuck on an object
              message += "FLF";
              Logger::log(LOG_WARNING, "Front Left has existed with a failure status! Threshold: " + std::to_string(frontLeftPID->dThreshold) + ", Error: " + std::to_string(frontLeftPIDCalc->lastError));
            } else {
              // Front left side successfully completed
              message += "FLS";
              Logger::log(LOG_INFO, "Front Left has existed with a success status. Error: " + std::to_string(frontLeftPIDCalc->lastError));
            }
            frontLeftComplete = true;
          }
        }
      }

      if (driveControl->frontRightMotors.size() != 0) {
        // Calculate a power for the front left motors
        PIDCommand frontRight = frontRightPID->calculate(frontRightPIDCalc, averagePosition(driveControl->frontRightMotors), frontRightDegrees);
        frontRightPower = frontRight.result;

        if (!frontRightComplete) {
          // Front right side is not complete, check for completion signal
          if (frontRight.type == E_COMMAND_EXIT_FAILURE || frontRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (frontRight.type == E_COMMAND_EXIT_FAILURE) {
              // Front right side failed to complete, stuck on an object
              message += "FRF";
              Logger::log(LOG_WARNING, "Front Right has existed with a failure status! Threshold: " + std::to_string(frontRightPID->dThreshold) + ", Error: " + std::to_string(frontRightPIDCalc->lastError));
            } else {
              // Front right side successfully completed
              message += "FRS";
              Logger::log(LOG_INFO, "Front Right has existed with a success status. Error: " + std::to_string(frontRightPIDCalc->lastError));
            }
            frontRightComplete = true;
          }
        }
      }

      if (driveControl->backLeftMotors.size() != 0) {
        // Calculate a power for the back left motors
        PIDCommand backLeft = backLeftPID->calculate(backLeftPIDCalc, averagePosition(driveControl->backLeftMotors), backLeftDegrees);
        backLeftPower = backLeft.result;

        if (!backLeftComplete) {
          // Back left side is not complete, check for completion signal
          if (backLeft.type == E_COMMAND_EXIT_FAILURE || backLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (backLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Back left side failed to complete, stuck on an object
              message += "FLF";
              Logger::log(LOG_WARNING, "Back Left has existed with a failure status! Threshold: " + std::to_string(backLeftPID->dThreshold) + ", Error: " + std::to_string(backLeftPIDCalc->lastError));
            } else {
              // Back left side successfully completed
              message += "FLS";
              Logger::log(LOG_INFO, "Back Left has existed with a success status. Error: " + std::to_string(backLeftPIDCalc->lastError));
            }
            backLeftComplete = true;
          }
        }
      }

      if (driveControl->backRightMotors.size() != 0) {
        // Calculate a power for the back left motors
        PIDCommand backRight = backRightPID->calculate(backRightPIDCalc, averagePosition(driveControl->backRightMotors), backRightDegrees);
        backRightPower = backRight.result;

        if (!backRightComplete) {
          // Back right side is not complete, check for completion signal
          if (backRight.type == E_COMMAND_EXIT_FAILURE || backRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (backRight.type == E_COMMAND_EXIT_FAILURE) {
              // Back right side failed to complete, stuck on an object
              message += "FRF";
              Logger::log(LOG_WARNING, "Back Right has existed with a fai lure status! Threshold: " + std::to_string(backRightPID->dThreshold) + ", Error: " + std::to_string(backRightPIDCalc->lastError));
            } else {
              // Back right side successfully completed
              message += "FRS";
              Logger::log(LOG_INFO, "Back Right has existed with a success status. Error: " + std::to_string(backRightPIDCalc->lastError));
            }
            backRightComplete = true;
          }
        }
      }

      if (driveControl->otherLeftMotors.size() != 0) {
        // Calculate a power for the other left motors
        PIDCommand otherLeft = frontLeftPID->calculate(otherLeftPIDCalc, averagePosition(driveControl->otherLeftMotors), leftDegrees);
        otherLeftPower = otherLeft.result;

        if (!otherLeftComplete) {
          // Other left side is not complete, check for completion signal
          if (otherLeft.type == E_COMMAND_EXIT_FAILURE || otherLeft.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (otherLeft.type == E_COMMAND_EXIT_FAILURE) {
              // Other left side failed to complete, stuck on an object
              message += "OLF";
              Logger::log(LOG_WARNING, "Other Left has existed with a failure status! Threshold: " + std::to_string(frontLeftPID->dThreshold) + ", Error: " + std::to_string(otherLeftPIDCalc->lastError));
            } else {
              // Other left side successfully completed
              message += "OLS";
              Logger::log(LOG_INFO, "Other Left has existed with a success status. Error: " + std::to_string(otherLeftPIDCalc->lastError));
            }
            otherLeftComplete = true;
          }
        }
      }

      if (driveControl->otherRightMotors.size() != 0) {
        // Calculate a power for the other left motors
        PIDCommand otherRight = frontRightPID->calculate(otherRightPIDCalc, averagePosition(driveControl->otherRightMotors), rightDegrees);
        otherRightPower = otherRight.result;

        if (!otherRightComplete) {
          // Other right side is not complete, check for completion signal
          if (otherRight.type == E_COMMAND_EXIT_FAILURE || otherRight.type == E_COMMAND_EXIT_SUCCESS) {
            // An exit command was issued, signifying either success or failure
            if (otherRight.type == E_COMMAND_EXIT_FAILURE) {
              // Other right side failed to complete, stuck on an object
              message += "ORF";
              Logger::log(LOG_WARNING, "Other Right has existed with a failure status! Threshold: " + std::to_string(frontRightPID->dThreshold) + ", Error: " + std::to_string(otherRightPIDCalc->lastError));
            } else {
              // Other right side successfully completed
              message += "ORS";
              Logger::log(LOG_INFO, "Other Right has existed with a success status. Error: " + std::to_string(otherRightPIDCalc->lastError));
            }
            otherRightComplete = true;
          }
        }
      }

      if (driveControl->lock->take(MUTEX_WAIT_TIME)) {
        // Iterate through and issue the commands to the motors
        for (const auto & motor : driveControl->frontLeftMotors)
          motor->move(frontLeftPower);
        for (const auto & motor : driveControl->frontRightMotors)
          motor->move(frontRightPower);
        for (const auto & motor : driveControl->backLeftMotors)
          motor->move(backLeftPower);
        for (const auto & motor : driveControl->backRightMotors)
          motor->move(backRightPower);
        for (const auto & motor : driveControl->otherLeftMotors)
          motor->move(otherLeftPower);
        for (const auto & motor : driveControl->otherRightMotors)
          motor->move(otherRightPower);
        driveControl->lock->give();
      }

      // Delay for the average time delta across all PID constants
      pros::delay((frontLeftPID->dt + backLeftPID->dt + frontRightPID->dt + backRightPID->dt) / 4.0);
    }
    // Stop the motors together
    driveControl->runLeftMotors(0);
    driveControl->runRightMotors(0);
    // Log the completion
    LCD::setStatus("PID Complete " + message);
    Logger::log(LOG_INFO, "PID Complete");
    // Free up memory
    delete frontLeftPIDCalc;
    delete backLeftPIDCalc;
    delete frontRightPIDCalc;
    delete backRightPIDCalc;
    delete otherLeftPIDCalc;
    delete otherRightPIDCalc;
  }
}

void DriveFunction::pivot(int degrees) {
  // Using the turn values, calculate and call the DriveControl object to move a certain amount to pivot the robot the given degrees
  int amt = 0;
  if (degrees > 0)
    amt = 0.5 * (((degrees / 90.0) * pt) + kt);
  else
    amt = 0.5 * (((degrees / 90.0) * pt) - kt);

  // Display and log for debugging purposes
  LCD::setStatus("Pivoting: " + std::to_string(degrees) + " degrees");
  Logger::log(LOG_INFO, "Pivoting: " + std::to_string(degrees) + " degrees");

  // Run the drive command
  DriveFunction::moveRelative(usePivotPID, pivotFrontLeftPID, pivotBackLeftPID, pivotFrontRightPID, pivotBackRightPID, amt, amt, -amt, -amt);
}

void DriveFunction::strafe(double inches) {
  // Using the strafe value, calculate and call the DriveControl objects to strafe the given amount of inches
  double amt = DriveFunction::ks * inches;
  DriveFunction::moveRelative(useStrafePID, strafeFrontLeftPID, strafeBackLeftPID, strafeFrontRightPID, strafeBackRightPID, -amt, amt, amt, -amt);

  // Display and log for debugging purposes
  LCD::setStatus("Strafing: " + std::to_string(inches) + " inches");
  Logger::log(LOG_INFO, "Strafing: " + std::to_string(inches) + " inches");
}

void DriveFunction::move(double inches) {
  // Using the gear ratio, calculate and move forward the amount of degrees
  double amt = DriveFunction::getFullRatio() * inches;
  DriveFunction::moveDegrees(amt);
}

void DriveFunction::moveRevolutions(double revolutions, int degrees) {
  // Using the gear ratio, calculate and call the DriveControl object to move forward the given amount of revolutions
  DriveFunction::moveDegrees(revolutions * 360 + degrees);
}

void DriveFunction::moveDegrees(int degrees) {
  // Determines whether to use the forward or backwards PID
  bool usePID = false;
  PID * frontLeftPID = NULL;
  PID * backLeftPID = NULL;
  PID * frontRightPID = NULL;
  PID * backRightPID = NULL;
  if (degrees > 0) {
    usePID = useForwardPID;
    frontLeftPID = forwardFrontLeftPID;
    backLeftPID = forwardBackLeftPID;
    frontRightPID = forwardFrontRightPID;
    backRightPID = forwardBackRightPID;
  } else if (degrees < 0) {
    usePID = useBackwardPID;
    frontLeftPID = backwardFrontLeftPID;
    backLeftPID = backwardBackLeftPID;
    frontRightPID = backwardFrontRightPID;
    backRightPID = backwardBackRightPID;
  } else {
    Logger::log(LOG_WARNING, "Drive attempted to move 0. Not moving");
  }

  DriveFunction::moveRelative(usePID, frontLeftPID, backLeftPID, frontRightPID, backLeftPID, degrees, degrees, degrees, degrees);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool brake, bool flipReverse) {
  // Passes the call to the DriveControl object
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, brake, flipReverse);
}

void DriveFunction::runH(double moveVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  // Passes the call to the DriveControl object
  DriveFunction::driveControl->runH(moveVoltage, turnVoltage, brake, flipReverse, moveSensitivity,turnSensitivity);
}

void DriveFunction::runStrafe(double moveVoltage, double turnStrafeVoltage, bool strafe, bool brake, bool flipReverse, double moveSensitivity, double turnStrafeSensitivity) {
  // Passes the call to the DriveControl object
  DriveFunction::driveControl->runStrafe(moveVoltage, turnStrafeVoltage, strafe, brake, flipReverse,moveSensitivity, turnStrafeSensitivity);
}

void DriveFunction::runX(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double strafeSensitivity, double turnSensitivity) {
  // Passes the call to the DriveControl object
  DriveFunction::driveControl->runX(moveVoltage, strafeVoltage, turnVoltage, brake, flipReverse,moveSensitivity, strafeSensitivity, turnSensitivity);
}

void DriveFunction::stop() {
  // Moves the robot with a power of 0, stopping it
  DriveFunction::driveControl->stop(MOTOR_DEFAULT_BRAKE);
}

void DriveFunction::stop(bool brake) {
  // Moves the robot with a power of 0, stopping it
  DriveFunction::driveControl->stop(brake);
}
