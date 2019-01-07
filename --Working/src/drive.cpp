#include "main.h"
#include <cmath>
#include <vector>
#include "drive.hpp"

void DriveControl::runLeftMotors(int voltage) {
  // Iterate through and set the left motors to a given power
  for (const auto & motor : DriveControl::leftMotors)
    motor.move(voltage);
}

void DriveControl::runRightMotors(int voltage) {
  // Iterate through and set the right motors to a given power
  for (const auto & motor : DriveControl::rightMotors)
    motor.move(voltage);
}

void DriveControl::setLeftBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the left motors to a given brake mode
  for (const auto & motor : DriveControl::leftMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

void DriveControl::setRightBrake(pros::motor_brake_mode_e_t mode) {
  // Iterate through and set the right motors to a given brake mode
  for (const auto & motor : DriveControl::rightMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

PIDCommand DriveControl::runMotorsRelative(PIDCalc * calc, std::vector<pros::Motor> motors, int target) {
  if (!usePID) if (lock->take(MUTEX_WAIT_TIME)) {
    // PID values are not set, issue simple move command
    for (const auto & motor : motors)
      motor.move(util::limitX(MOTOR_MOVE_RELATIVE_MAX_SPEED, target * MOTOR_MOVE_RELATIVE_KP));
    lock->give();
    return PIDCommand(E_COMMAND_NO_CALCULATION, 0);
  } else; else if (motors.size() > 0) {
    // PID values are set, calculate the output with PID
    // Calculate the average motor position, giving a better sense of the robot position
    int position = 0;
    for (const auto & motor : motors)
      position += motor.get_position();
    position = position / util::sign(motors.size());

    // Calculate the output
    return pid->calculate(calc, position, target);
  }
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor leftMotor, pros::Motor rightMotor) {
  // Two wheel drive initialization
  // Store the mutex
  DriveControl::lock = &motorLock;
  // Add the left motor
  DriveControl::addLeftMotor(leftMotor);
  // Add the right motor
  DriveControl::addRightMotor(rightMotor);

  // PID values have not been set
  DriveControl::usePID = false;
  DriveControl::pid = NULL;
  DriveControl::leftPIDCalc = NULL;
  DriveControl::rightPIDCalc = NULL;
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor) {
  // Four wheel drive initialization
  // Store the mutex
  DriveControl::lock = &motorLock;
  // Add the left motors
  DriveControl::addLeftMotor(frontLeftMotor);
  DriveControl::addLeftMotor(rearLeftMotor);
  // Add the right motors
  DriveControl::addRightMotor(frontRightMotor);
  DriveControl::addRightMotor(rearRightMotor);

  // PID values have not been set
  DriveControl::usePID = false;
  DriveControl::pid = NULL;
  DriveControl::leftPIDCalc = NULL;
  DriveControl::rightPIDCalc = NULL;
}

void DriveControl::addLeftMotor(pros::Motor motor) {
  // Add the motor to the list of left motors
  DriveControl::leftMotors.push_back(motor);
}

void DriveControl::addRightMotor(pros::Motor motor) {
  // Add the motor to the list of right motors
  DriveControl::rightMotors.push_back(motor);
}

void DriveControl::clearMotors() {
  // Clear all left motors
  DriveControl::clearLeftMotors();
  // Clear all right motors
  DriveControl::clearRightMotors();
}

void DriveControl::clearLeftMotors() {
  // Empty the list of left motors
  DriveControl::leftMotors.clear();
}

void DriveControl::clearRightMotors() {
  // Empty the list of right motors
  DriveControl::rightMotors.clear();
}

void DriveControl::setPID(int dt, double kp, double ki, double kd, bool brake, int tLimit, int iLimit, int iZone, int dThreshold, int tThreshold, int de0) {
  // If PID values have been set, clear them
  if (usePID) clearPID();
  // Set PID values to the new ones
  DriveControl::usePID = true;
  DriveControl::pid = new PID(dt, kp, ki, kd, brake, tLimit, iLimit, iZone, true, dThreshold, tThreshold, de0);
}

PID * DriveControl::getPID() {
  // Returns a pointer to the PID object, allowing for them to be changed
  return DriveControl::pid;
}

void DriveControl::clearPID() {
  // If PID values have been set, clear them
  if (!usePID) return;
  DriveControl::usePID = false;
  // Free up the memory
  delete DriveControl::pid;
  DriveControl::pid = NULL;
}

void DriveControl::moveRelative(double leftRevolutions, int leftDegrees, double rightRevolutions, int rightDegrees) {
  // Calculate the total target, factoring revolutions as well as degrees
  long leftTarget = std::lround(leftRevolutions * 360) + leftDegrees;
  long rightTarget = std::lround(rightRevolutions * 360) + rightDegrees;

  // The dynamic list of left and right motors to use
  std::vector<pros::Motor> leftMotors;
  std::vector<pros::Motor> rightMotors;

  // The list of motors not connected, but stored in the lists
  std::vector<pros::Motor> lDisconnect;
  std::vector<pros::Motor> rDisconnect;

  /*
   * Iterates through the left and right motors to check whether they are connected
   * If they are connected, proceed
   * If not, an event is logged and the motor is temporarily removed from the list
   * If no motors are plugged in on either side, abort
   */
  for (const auto & motor : DriveControl::leftMotors) {
    motor.tare_position();
    motor.set_encoder_units(ENCODER_DEGREES);
    motor.set_brake_mode((!usePID || pid->brake) ? BRAKE_BRAKE : BRAKE_COAST);
    if (motor.get_efficiency() > 1000)
      lDisconnect.push_back(motor);
    else
      leftMotors.push_back(motor);
  }
  for (const auto & motor : DriveControl::rightMotors) {
    motor.tare_position();
    motor.set_encoder_units(ENCODER_DEGREES);
    motor.set_brake_mode((!usePID || !(pid->brake)) ? BRAKE_BRAKE : BRAKE_COAST);
    if (motor.get_efficiency() > 1000)
      rDisconnect.push_back(motor);
    else
      rightMotors.push_back(motor);
  }

  // Whether to abort
  bool abort = false;

  // Display disconnect errors
  if (lDisconnect.size() > 0 && leftTarget) {
    if (leftMotors.size() == 0) {
      Logger::log(LOG_ERROR, "All of the left side drive motors have been disconnected! Aborting...");
      abort = true;
    }
    else
      Logger::log(LOG_ERROR, "Some of the left side drive motors have been disconnected! They will be ignored during this drive movement");
  }
  if (rDisconnect.size() > 0 && rightTarget) {
    if (rightMotors.size() == 0) {
      Logger::log(LOG_ERROR, "All of the right side drive motors have been disconnected! Aborting...");
      abort = true;
    }
    else
      Logger::log(LOG_ERROR, "Some of the right side drive motors have been disconnected! They will be ignored during this drive movement");
  }
  if (abort) return;

  // Display and log for debugging purposes
  LCD::setStatus("Auto driving: L" + std::to_string(leftTarget) + ", R" + std::to_string(rightTarget));
  Logger::log(LOG_INFO, "Auto Driving - Left Target: " + std::to_string(leftTarget) + ", Right Target:" + std::to_string(rightTarget));

  if (!usePID) {
    // PID values have not been set, issue simple move commands
    if (leftTarget != 0) DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget);
    if (rightTarget != 0) DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget);

    // Loop until done
    while (true) {
      bool done = true;
      int count = 0;
      long total = 0;
      // Run the left motors
      for (const auto & motor : leftMotors) {
        if (util::abs(leftTarget - motor.get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD) {
          count++;
          LCD::setText(2, "Left error (T" + std::to_string(MOTOR_MOVE_RELATIVE_THRESHOLD) + "): " + std::to_string(leftTarget - motor.get_position()));
        }
        total += motor.get_position();
      }
      // Check for completion
      if (count == DriveControl::leftMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget - (total / util::sign(leftMotors.size())));
      } else DriveControl::runLeftMotors(0);

      LCD::setText(4, "Left drive avg " + std::to_string(total / util::sign(DriveControl::rightMotors.size())));

      count = 0;
      total = 0;
      // Run the right motors
      for (const auto & motor : rightMotors) {
        if (util::abs(rightTarget - motor.get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD) {
          count++;
          LCD::setText(2, "Right error (T" + std::to_string(MOTOR_MOVE_RELATIVE_THRESHOLD) + "): " + std::to_string(rightTarget - motor.get_position()));
        }
        total += motor.get_position();
      }
      // Check for completion
      if (count == DriveControl::rightMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget - (total / util::sign(rightMotors.size())));
      } else DriveControl::runRightMotors(0);

      LCD::setText(5, "Right drive avg " + std::to_string(total / util::sign(DriveControl::rightMotors.size())));

      // If both sides are done, exit
      if (done) break;
      // Run every 20ms
      pros::delay(20);
    }

  } else {
    // Create calculation classes
    DriveControl::leftPIDCalc = new PIDCalc();
    DriveControl::rightPIDCalc = new PIDCalc();
    // Check whether to move the motors
    bool leftComplete = leftTarget == 0;
    bool rightComplete = rightTarget == 0;
    int leftPower = 0;
    int rightPower = 0;

    // Completion string
    std::string message;

    while (!(leftComplete && rightComplete)) {
      if (!leftComplete) {
        // Left is not complete, calculate another power to the motors
        PIDCommand ret = DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget);
        if (ret.type == E_COMMAND_EXIT_FAILURE || ret.type == E_COMMAND_EXIT_SUCCESS) {
          // An exit command was issued, signifying either success or failure
          if (ret.type == E_COMMAND_EXIT_FAILURE) {
            // Left side failed to complete, stuck on an object
            message += "LF";
            Logger::log(LOG_WARNING, "Left Side existed with a failure status! Threshold: " + std::to_string(pid->dThreshold) + ", Error: " + std::to_string(leftPIDCalc->lastError));
          } else {
            // Left side successfully completed
            message += "LS";
            Logger::log(LOG_INFO, "Left Side existed with a success status. Error: " + std::to_string(leftPIDCalc->lastError));
          }
          leftComplete = true;
          leftPower = 0;
        } else
          leftPower = ret.result;
      }

      if (!rightComplete) {
        // Right is not complete, calculate another power to the motors
        PIDCommand ret = DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget);
        if (ret.type == E_COMMAND_EXIT_FAILURE || ret.type == E_COMMAND_EXIT_SUCCESS) {
          // An exit command was issued, signifying either success or failure
          if (ret.type == E_COMMAND_EXIT_FAILURE) {
            // Right side failed to complete, stuck on an object
            message += "RF";
            Logger::log(LOG_WARNING, "Right Side existed with a failure status! Threshold: " + std::to_string(pid->dThreshold) + ", Error: " + std::to_string(rightPIDCalc->lastError));
          } else {
            // Right side successfully completed
            message += "RS";
            Logger::log(LOG_INFO, "Right Side existed with a success status. Error: " + std::to_string(rightPIDCalc->lastError));
          }
          rightComplete = true;
          rightPower = 0;
        } else
          rightPower = ret.result;
      }

      if (lock->take(MUTEX_WAIT_TIME)) {
        // Iterate through and issue the commands to the motors
        for (const auto & motor : leftMotors)
          motor.move(leftPower);
        for (const auto & motor : rightMotors)
          motor.move(rightPower);
        lock->give();
      }

      pros::delay(pid->dt);
    }
    // Log the completion
    LCD::setStatus("PID Complete " + message);
    Logger::log(LOG_INFO, "PID Complete");
    // Free up memory
    delete DriveControl::leftPIDCalc;
    delete DriveControl::rightPIDCalc;
    DriveControl::leftPIDCalc = NULL;
    DriveControl::rightPIDCalc = NULL;
  }
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  // Pass the call to the longer run() function
  DriveControl::run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, 1.0, 1.0);
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
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
    setLeftBrake(leftBrake ? BRAKE_BRAKE : BRAKE_COAST);
    setRightBrake(rightBrake ? BRAKE_BRAKE : BRAKE_COAST);
    runLeftMotors(leftBrake ? 0 : leftVoltage);
    runRightMotors(rightBrake ? 0 : rightVoltage);
    lock->give();
  }
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
}

DriveControl * DriveFunction::getDriveControl() {
  // Returns the stored DriveControl Object pointer
  return DriveFunction::driveControl;
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

void DriveFunction::turn(int degrees) {
  // Passes the call to the larger turn() function
  DriveFunction::turn(false, degrees);
}

void DriveFunction::turn(bool backward, int degrees) {
  // Using the turn values, calculate and call the DriveControl object to move a certain amount to turn the robot the given degrees
  if (backward)
    if (degrees > 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -((degrees / 90 * pt) + kt));
    else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, ((degrees / 90 * pt) - kt), 0, 0);
    else;
  else
    if (degrees > 0) DriveFunction::driveControl->moveRelative(0, (degrees / 90 * pt) + kt, 0, 0);
    else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -((degrees / 90 * pt) - kt));
}

void DriveFunction::pivot(int degrees) {
  // Using the turn values, calculate and call the DriveControl object to move a certain amount to pivot the robot the given degrees
  if (degrees > 0)
    DriveFunction::driveControl->moveRelative(0, 0.5 * (((degrees / 90.0) * pt) + kt), 0, -0.5 * ((degrees / 90.0 * pt) + kt));
  else
    DriveFunction::driveControl->moveRelative(0, 0.5 * (((degrees / 90.0) * pt) - kt), 0, -0.5 * ((degrees / 90.0 * pt) - kt));
}

void DriveFunction::move(double inches) {
  // Using the gear ratio, calculate and call the DriveControl object to move forward the given amount of inches
  double gearRatio = DriveFunction::getFullRatio();
  DriveFunction::driveControl->moveRelative(0, inches * gearRatio, 0, inches * gearRatio);
}

void DriveFunction::moveDegrees(int degrees) {
  // Using the gear ratio, calculate and call the DriveControl object to move forward the given amount of degrees
  DriveFunction::driveControl->moveRelative(0, degrees, 0, degrees);
}

void DriveFunction::moveRevolutions(double revolutions, int degrees) {
  // Using the gear ratio, calculate and call the DriveControl object to move forward the given amount of revolutions
  DriveFunction::driveControl->moveRelative(revolutions, degrees, revolutions, degrees);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  // Passes the call to the DriveControl object run()
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  // Passes the call to the DriveControl object run()
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, moveSensitivity, turnSensitivity);
}
