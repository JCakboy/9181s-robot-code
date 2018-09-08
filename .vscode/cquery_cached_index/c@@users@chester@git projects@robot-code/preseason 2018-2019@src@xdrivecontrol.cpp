#include "main.h"
#include "xdrivecontrol.hpp"
#include <cmath>

void XDriveControl::runFrontLeftMotors(int voltage) {
  for (const auto & motor : XDriveControl::frontLeftMotors)
    motor.move(voltage);
}

void XDriveControl::runRearLeftMotors(int voltage) {
  for (const auto & motor : XDriveControl::rearLeftMotors)
    motor.move(voltage);
}

void XDriveControl::runFrontRightMotors(int voltage) {
  for (const auto & motor : XDriveControl::frontRightMotors)
    motor.move(voltage);
}

void XDriveControl::runRearRightMotors(int voltage) {
  for (const auto & motor : XDriveControl::rearRightMotors)
    motor.move(voltage);
}

XDriveControl::XDriveControl(pros::Mutex & motorLock, pros::Motor & frontLeftMotor, pros::Motor & rearLeftMotor, pros::Motor & frontRightMotor, pros::Motor & rearRightMotor) {
  XDriveControl::lock = motorLock;
  XDriveControl::addFrontLeftMotor(frontLeftMotor);
  XDriveControl::addFrontRightMotor(frontRightMotor);
  XDriveControl::addRearLeftMotor(rearLeftMotor);
  XDriveControl::addRearRightMotor(rearRightMotor);
}

void XDriveControl::addFrontLeftMotor(pros::Motor & motor) {
  XDriveControl::frontLeftMotors.push_back(motor);
}

void XDriveControl::addFrontRightMotor(pros::Motor & motor) {
  XDriveControl::frontRightMotors.push_back(motor);
}

void XDriveControl::addRearLeftMotor(pros::Motor & motor) {
  XDriveControl::rearLeftMotors.push_back(motor);
}

void XDriveControl::addRearRightMotor(pros::Motor & motor) {
  XDriveControl::rearRightMotors.push_back(motor);
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool XDriveControl::removeFrontLeftMotor(pros::Motor & motor) {

}

bool XDriveControl::removeFrontRightMotor(pros::Motor & motor) {

}

bool XDriveControl::removeRearLeftMotor(pros::Motor & motor) {

}

bool XDriveControl::removeRearRightMotor(pros::Motor & motor) {

}
*/
void XDriveControl::clearMotors() {
  XDriveControl::clearFrontLeftMotors();
  XDriveControl::clearFrontRightMotors();
  XDriveControl::clearRearLeftMotors();
  XDriveControl::clearRearRightMotors();
}

void XDriveControl::clearFrontLeftMotors() {
  XDriveControl::frontLeftMotors.clear();
}

void XDriveControl::clearFrontRightMotors() {
  XDriveControl::frontRightMotors.clear();
}

void XDriveControl::clearRearLeftMotors() {
  XDriveControl::rearLeftMotors.clear();
}

void XDriveControl::clearRearRightMotors() {
  XDriveControl::rearRightMotors.clear();
}

void XDriveControl::run(double moveVoltage, double strafeVoltage, double turnVoltage) {
  XDriveControl::run(moveVoltage, strafeVoltage, turnVoltage, 1.0, 1.0, 1.0);
}

void XDriveControl::run(double moveVoltage, double strafeVoltage, double turnVoltage, double moveSensitivity, double strafeSensitivity, double turnSensitivity) {
  moveVoltage *= moveSensitivity;
  strafeVoltage *= strafeSensitivity;
  turnVoltage *= turnSensitivity;

  int frontLeftVoltage = emath::limit127(moveVoltage + strafeVoltage);
  int frontRightVoltage = emath::limit127(moveVoltage - strafeVoltage);
  int rearLeftVoltage = emath::limit127(moveVoltage - strafeVoltage);
  int rearRightVoltage = emath::limit127(moveVoltage + strafeVoltage);

  class nest {
    public:
      static int distribute(int & turn, int & frontVoltage, int & rearVoltage) {
        int overflow = 0;
        for (; turn != 0; turn = emath::step0(turn)) {
          int ftemp = std::round(frontVoltage + turn - emath::step0(turn));
          int rtemp = std::round(rearVoltage + turn - emath::step0(turn));

          if (emath::within127(ftemp))
            frontVoltage = ftemp;
          else {
            int rtt = rtemp + turn - emath::step0(turn);
            if (emath::within127(rtt))
              rtemp = rtt;
            else if (!emath::within127(rtemp)) {
              overflow = overflow - turn + emath::step0(turn);
              continue;
            }
          }

          if (emath::within127(rtemp))
            rearVoltage = rtemp;
          else {
            int ftt = rtemp + turn - emath::step0(turn);
            if (emath::within127(ftt))
              frontVoltage = ftt;
          }
        }
        return overflow;
      }
  };

  int lt = std::round(turnVoltage / 2.0);
	int rt = std::round(turnVoltage / -2.0);

	double overflow = 0;

  overflow = nest::distribute(lt, frontLeftVoltage, rearLeftVoltage);

	rt += overflow;
	overflow = nest::distribute(rt, frontRightVoltage, rearRightVoltage);

	lt += overflow;
	overflow = nest::distribute(lt, frontLeftVoltage, rearLeftVoltage);

  if (lock.take(MUTEX_WAIT_TIME)) {
    runFrontLeftMotors(frontLeftVoltage);
    runFrontRightMotors(frontRightVoltage);
    runRearLeftMotors(rearLeftVoltage);
    runRearRightMotors(rearRightVoltage);
    lock.give();
  }

}
