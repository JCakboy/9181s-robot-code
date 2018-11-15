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

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor leftMotor, pros::Motor rightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::leftMotors.push_back(leftMotor);
  DriveControl::rightMotors.push_back(rightMotor);

  DriveControl::usePID = false;
  DriveControl::pid = NULL;
}

DriveControl::DriveControl(pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor) {
  DriveControl::lock = &motorLock;
  DriveControl::addLeftMotor(frontLeftMotor);
  DriveControl::addLeftMotor(rearLeftMotor);
  DriveControl::addRightMotor(frontRightMotor);
  DriveControl::addRightMotor(rearRightMotor);

  DriveControl::usePID = false;
  DriveControl::pid = NULL;
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
/* currently not working, may work when pros 3 is released
bool DriveControl::removeLeftMotor(pros::Motor motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::leftMotors.size(); i++)
    if (DriveControl::leftMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::leftMotors.erase(DriveControl::leftMotors.begin()+i);
  return found;
}

bool DriveControl::removeRightMotor(pros::Motor motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::rightMotors.size(); i++)
    if (DriveControl::rightMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::rightMotors.erase(DriveControl::rightMotors.begin()+i);
  return found;
}
*/

void DriveControl::setPID(int dt, double kp, double ki, double kd, int limit) {
  if (usePID) clearPID();
  DriveControl::usePID = true;
  DriveControl::pid = new PID(dt, kp, ki, kd, limit);
}

PID & DriveControl::getPID() {
  return *(DriveControl::pid);
}

void DriveControl::clearPID() {
  if (!usePID) return;
  DriveControl::usePID = false;
  delete DriveControl::pid;
}

void DriveControl::moveRelative(double revolutions, int degrees, int threshold, bool moveLeft, bool moveRight) {
  if (lock->take(MUTEX_WAIT_TIME)) {
    for (const auto & motor : DriveControl::leftMotors) {
      motor.set_encoder_units(ENCODER_DEGREES);
      motor.tare_position();
    }
    for (const auto & motor : DriveControl::rightMotors) {
      motor.set_encoder_units(ENCODER_DEGREES);
      motor.tare_position();
    }
    lock->give();
  } else return;

  long target = std::lround(revolutions * 360) + degrees;

  if (!usePID) if (lock->take(MUTEX_WAIT_TIME)) {
    if (moveLeft)
      for (const auto & motor : DriveControl::leftMotors)
        motor.move_relative(target, 127);
    if (moveRight)
      for (const auto & motor : DriveControl::rightMotors)
        motor.move_relative(target, 127);
    while (true) {
      bool done = true;
      if (moveLeft)
        for (const auto & motor : DriveControl::leftMotors)
          if (std::abs(target - motor.get_position()) > threshold)
            done = false;
      if (moveRight)
        for (const auto & motor : DriveControl::rightMotors)
          if (std::abs(target - motor.get_position()) > threshold)
            done = false;
      if (done) break;
    }
    lock->give();
  } else; else if (moveLeft || moveRight) {

    class RelevantMotors {
      public:
        std::vector<pros::Motor> relevant;

        explicit RelevantMotors(bool useLeft, bool useRight, std::vector<pros::Motor> left, std::vector<pros::Motor> right) {
          if (useLeft)
            for (const auto & motor : left)
              relevant.push_back(motor);
          if (useRight)
            for (const auto & motor : right)
              relevant.push_back(motor);
        }

        int amount() {
          return relevant.size();
        }

        double sum() {
          double sum = 0;
          for (const auto & motor : relevant)
            sum += motor.get_position();
          return sum;
        }

    };

    RelevantMotors motors (moveLeft, moveRight, leftMotors, rightMotors);

    int error = target;
    int lastError = 0;
    int se = 0;
    while (true) {
      error = (motors.sum() / motors.amount()) - target; // Calculate the current error
      se += error; // Add the current error to the sum of all errors
      int de = (error - lastError) / pid->dt; // Calculate the change in error

      if (error <= threshold) break;

      int p = error * pid->kp;
      int i = util::limitX(pid->limit, se * pid->ki);
      int d = de * pid->kd;

      if (lock->take(MUTEX_WAIT_TIME)) {
        runLeftMotors(p + i + d);
        runRightMotors(p + i + d);
        lock->give();
      }

      lastError = error;
      pros::delay(pid->dt);
    }

    if (lock->take(MUTEX_WAIT_TIME)) {
      runLeftMotors(0);
      runRightMotors(0);
      lock->give();
    }
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
