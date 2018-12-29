#include "main.h"
#include <cmath>
#include <vector>
#include "drive.hpp"

void DriveControl::runLeftMotors(int voltage) {
  for (const auto & motor : DriveControl::leftMotors)
    motor.move(voltage);
}

void DriveControl::runRightMotors(int voltage) {
  for (const auto & motor : DriveControl::rightMotors)
    motor.move(voltage);
}

void DriveControl::setLeftBrake(pros::motor_brake_mode_e_t mode) {
  for (const auto & motor : DriveControl::leftMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

void DriveControl::setRightBrake(pros::motor_brake_mode_e_t mode) {
  for (const auto & motor : DriveControl::rightMotors)
    if (motor.get_brake_mode() != mode)
      motor.set_brake_mode(mode);
}

PIDCommand DriveControl::runMotorsRelative(PIDCalc * calc, std::vector<pros::Motor> motors, int target) {
  if (!usePID) if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : motors)
      motor.move(util::limitX(MOTOR_MOVE_RELATIVE_MAX_SPEED, target * MOTOR_MOVE_RELATIVE_KP));
    lock->give();
    return PIDCommand(E_COMMAND_NO_CALCULATION, 0);
  } else; else if (motors.size() > 0) {
    int position = 0;
    for (const auto & motor : motors)
      position += motor.get_position();
    position = position / util::sign(motors.size());

    return pid->calculate(calc, position, target);
  }
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor leftMotor, pros::Motor rightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::leftMotors.push_back(leftMotor);
  DriveControl::rightMotors.push_back(rightMotor);

  DriveControl::usePID = false;
  DriveControl::pid = NULL;
  DriveControl::leftPIDCalc = NULL;
  DriveControl::rightPIDCalc = NULL;
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::addLeftMotor(frontLeftMotor);
  DriveControl::addLeftMotor(rearLeftMotor);
  DriveControl::addRightMotor(frontRightMotor);
  DriveControl::addRightMotor(rearRightMotor);

  DriveControl::usePID = false;
  DriveControl::pid = NULL;
  DriveControl::leftPIDCalc = NULL;
  DriveControl::rightPIDCalc = NULL;
}

void DriveControl::addLeftMotor(pros::Motor motor) {
  DriveControl::leftMotors.push_back(motor);
}

void DriveControl::addRightMotor(pros::Motor motor) {
  DriveControl::rightMotors.push_back(motor);
}

void DriveControl::clearMotors() {
  DriveControl::clearLeftMotors();
  DriveControl::clearRightMotors();
}

void DriveControl::clearLeftMotors() {
  DriveControl::leftMotors.clear();
}

void DriveControl::clearRightMotors() {
  DriveControl::rightMotors.clear();
}

void DriveControl::setPID(int dt, double kp, double ki, double kd, bool brake, int tLimit, int iLimit, int iZone, int dThreshold, int tThreshold, int de0) {
  if (usePID) clearPID();
  DriveControl::usePID = true;
  DriveControl::pid = new PID(dt, kp, ki, kd, brake, tLimit, iLimit, iZone, true, dThreshold, tThreshold, de0);
}

PID * DriveControl::getPID() {
  return DriveControl::pid;
}

void DriveControl::clearPID() {
  if (!usePID) return;
  DriveControl::usePID = false;
  delete DriveControl::pid;
}

void DriveControl::moveRelative(double leftRevolutions, int leftDegrees, double rightRevolutions, int rightDegrees) {
  long leftTarget = std::lround(leftRevolutions * 360) + leftDegrees;
  long rightTarget = std::lround(rightRevolutions * 360) + rightDegrees;

  for (const auto & motor : DriveControl::leftMotors) {
    motor.tare_position();
    motor.set_encoder_units(ENCODER_DEGREES);
    motor.set_brake_mode((!usePID || pid->brake) ? BRAKE_BRAKE : BRAKE_COAST);
  }
  for (const auto & motor : DriveControl::rightMotors) {
    motor.tare_position();
    motor.set_encoder_units(ENCODER_DEGREES);
    motor.set_brake_mode((!usePID || !(pid->brake)) ? BRAKE_BRAKE : BRAKE_COAST);
  }

  LCD::setStatus("Auto driving: L" + std::to_string(leftTarget) + ", R" + std::to_string(rightTarget));
  pros::delay(1000);

  if (!usePID) {
    if (leftTarget != 0) DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget);
    if (rightTarget != 0) DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget);

    while (true) {
      bool done = true;
      int count = 0;
      long total = 0;
      for (const auto & motor : DriveControl::leftMotors) {
        if (util::abs(leftTarget - motor.get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD) {
          count++;
          LCD::setText(2, "Left error (T" + std::to_string(MOTOR_MOVE_RELATIVE_THRESHOLD) + "): " + std::to_string(leftTarget - motor.get_position()));
        }
        total += motor.get_position();
      }
      if (count == DriveControl::leftMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget - (total / util::sign(DriveControl::leftMotors.size())));
      } else DriveControl::runLeftMotors(0);

      LCD::setText(4, "Left drive avg " + std::to_string(total / util::sign(DriveControl::rightMotors.size())));

      count = 0;
      total = 0;
      for (const auto & motor : DriveControl::rightMotors) {
        if (util::abs(rightTarget - motor.get_position()) > MOTOR_MOVE_RELATIVE_THRESHOLD) {
          count++;
          LCD::setText(2, "Right error (T" + std::to_string(MOTOR_MOVE_RELATIVE_THRESHOLD) + "): " + std::to_string(rightTarget - motor.get_position()));
        }
        total += motor.get_position();
      }
      if (count == DriveControl::rightMotors.size()) {
        done = false;
        DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget - (total / util::sign(DriveControl::rightMotors.size())));
      } else DriveControl::runRightMotors(0);

      LCD::setText(5, "Right drive avg " + std::to_string(total / util::sign(DriveControl::rightMotors.size())));

      if (done) break;
      pros::delay(10);
    }

  } else {
    pros::delay(2000);

    DriveControl::leftPIDCalc = new PIDCalc();
    DriveControl::rightPIDCalc = new PIDCalc();

    bool leftComplete = leftTarget == 0;
    bool rightComplete = rightTarget == 0;
    int leftPower = 0;
    int rightPower = 0;

    std::string message;

    while (!(leftComplete && rightComplete)) {
      if (!leftComplete) {
        PIDCommand ret = DriveControl::runMotorsRelative(leftPIDCalc, leftMotors, leftTarget);
        if (ret.type == E_COMMAND_EXIT_FAILURE || ret.type == E_COMMAND_EXIT_SUCCESS) {
          if (ret.type == E_COMMAND_EXIT_FAILURE)
            message += "LF";
          else
            message += "LS";
          leftComplete = true;
          leftPower = 0;
        } else
          leftPower = ret.result;
      }

      if (!rightComplete) {
        PIDCommand ret = DriveControl::runMotorsRelative(rightPIDCalc, rightMotors, rightTarget);
        if (ret.type == E_COMMAND_EXIT_FAILURE || ret.type == E_COMMAND_EXIT_SUCCESS) {
          if (ret.type == E_COMMAND_EXIT_FAILURE)
            message += "RF";
          else
            message += "RS";
          rightComplete = true;
          rightPower = 0;
        } else
          rightPower = ret.result;
      }

      if (lock->take(MUTEX_WAIT_TIME)) {
        for (const auto & motor : DriveControl::leftMotors)
          motor.move(leftPower);
        for (const auto & motor : DriveControl::rightMotors)
          motor.move(rightPower);
        lock->give();
      }

      pros::delay(20);
    }
    LCD::setStatus("PID complete " + message);
    pros::delay(1000);
    delete DriveControl::leftPIDCalc;
    delete DriveControl::rightPIDCalc;
    DriveControl::leftPIDCalc = NULL;
    DriveControl::rightPIDCalc = NULL;
  }
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  DriveControl::run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, 1.0, 1.0);
}

void DriveControl::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  bool flip = flipReverse && moveVoltage < MOTOR_REVERSE_FLIP_THRESHOLD;

  moveVoltage *= moveSensitivity;
  turnVoltage *= turnSensitivity;

  int leftVoltage = util::limit127(!flip ? moveVoltage + turnVoltage : moveVoltage - turnVoltage);
  int rightVoltage = util::limit127(!flip ? moveVoltage - turnVoltage : moveVoltage + turnVoltage);

  if (lock->take(MUTEX_WAIT_TIME)) {
    setLeftBrake(leftBrake ? BRAKE_BRAKE : BRAKE_COAST);
    setRightBrake(rightBrake ? BRAKE_BRAKE : BRAKE_COAST);
    runLeftMotors(leftBrake ? 0 : leftVoltage);
    runRightMotors(rightBrake ? 0 : rightVoltage);
    lock->give();
  }
}

DriveFunction::DriveFunction(DriveControl * driveControl) {
  DriveFunction::driveControl = driveControl;
}

DriveControl & DriveFunction::getDriveControl() {
  return *(DriveFunction::driveControl);
}

void DriveFunction::setTurnValues(int pt, int kt) {
  DriveFunction::pt = pt;
  DriveFunction::kt = kt;
}

std::pair<int, int> DriveFunction::getTurnValues() {
  return std::pair<int, int>(pt, kt);
}

void DriveFunction::setGearRatio(double in, double out, double wheelDiameter) {
  DriveFunction::gearRatio = (360 * out) / (wheelDiameter * PI * in);
}

double DriveFunction::getGearRatio() {
  return DriveFunction::gearRatio;
}

void DriveFunction::turn(int degrees) {
  DriveFunction::turn(false, degrees);
}

void DriveFunction::turn(bool backward, int degrees) {
  if (backward)
    if (degrees > 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -((degrees / 90 * pt) + kt));
    else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, ((degrees / 90 * pt) - kt), 0, 0);
    else;
  else
    if (degrees > 0) DriveFunction::driveControl->moveRelative(0, (degrees / 90 * pt) + kt, 0, 0);
    else if (degrees < 0) DriveFunction::driveControl->moveRelative(0, 0, 0, -((degrees / 90 * pt) - kt));
}

void DriveFunction::pivot(int degrees) {
  if (degrees > 0)
    DriveFunction::driveControl->moveRelative(0, 0.5 * (((degrees / 90.0) * pt) + kt), 0, -0.5 * ((degrees / 90.0 * pt) + kt));
  else
    DriveFunction::driveControl->moveRelative(0, 0.5 * (((degrees / 90.0) * pt) - kt), 0, -0.5 * ((degrees / 90.0 * pt) - kt));
}

void DriveFunction::move(double inches) {
    DriveFunction::driveControl->moveRelative(0, inches * gearRatio, 0, inches * gearRatio);
}

void DriveFunction::moveDegrees(int degrees) {
  DriveFunction::driveControl->moveRelative(0, degrees, 0, degrees);
}

void DriveFunction::moveRevolutions(double revolutions, int degrees) {
  DriveFunction::driveControl->moveRelative(revolutions, degrees, revolutions, degrees);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse) {
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse);
}

void DriveFunction::run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  DriveFunction::driveControl->run(moveVoltage, turnVoltage, leftBrake, rightBrake, flipReverse, moveSensitivity, turnSensitivity);
}
