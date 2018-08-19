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

  int lt = std::round(turnVoltage / 2.0);
	int rt = std::round(turnVoltage / -2.0);

	double overflow = 0;

	for (; lt != 0; lt = emath::step0(lt)) {
		int ftemp = std::round(frontLeftVoltage + lt - emath::step0(lt));
		int rtemp = std::round(rearLeftVoltage + lt - emath::step0(lt));

		if (emath::within127(ftemp))
			frontLeftVoltage = ftemp;
		else {
			int rtt = rtemp + lt - emath::step0(lt);
			if (emath::within127(rtt))
				rtemp = rtt;
			else if (!emath::within127(rtemp)) {
				overflow = overflow - lt + emath::step0(lt);
				continue;
			}
		}

		if (emath::within127(rtemp))
			rearLeftVoltage = rtemp;
		else {
			int ftt = rtemp + lt - emath::step0(lt);
			if (emath::within127(ftt))
				frontLeftVoltage = ftt;
		}
	}

	rt += overflow;
	overflow = 0;

	for (; rt != 0; rt = emath::step0(rt)) {
		int ftemp = std::round(frontRightVoltage + rt - emath::step0(rt));
		int rtemp = std::round(rearRightVoltage + rt - emath::step0(rt));

		if (emath::within127(ftemp))
			frontRightVoltage = ftemp;
		else {
			int rtt = rtemp + rt - emath::step0(rt);
			if (emath::within127(rtt))
				rtemp = rtt;
			else if (!emath::within127(rtemp)) {
				overflow = overflow + rt - emath::step0(rt);
				continue;
			}
		}

		if (emath::within127(rtemp))
			rearRightVoltage = rtemp;
		else {
			int ftt = rtemp + rt - emath::step0(rt);
			if (emath::within127(ftt))
				frontRightVoltage = ftt;
		}
	}

	lt += overflow;
	overflow = 0;

	for (; lt != 0; lt = emath::step0(lt)) {
		int ftemp = std::round(frontLeftVoltage + lt - emath::step0(lt));
		int rtemp = std::round(rearLeftVoltage + lt - emath::step0(lt));

		if (emath::within127(ftemp))
			frontLeftVoltage = ftemp;
		else {
			int rtt = rtemp + lt - emath::step0(lt);
			if (emath::within127(rtt))
				rtemp = rtt;
			else if (!emath::within127(rtemp)) {
				overflow = overflow - lt + emath::step0(lt);
				continue;
			}
		}

		if (emath::within127(rtemp))
			rearLeftVoltage = rtemp;
		else {
			int ftt = rtemp + lt - emath::step0(lt);
			if (emath::within127(ftt))
				frontLeftVoltage = ftt;
		}
	}

  if (lock.take(MUTEX_WAIT_TIME)) {
    runFrontLeftMotors(frontLeftVoltage);
    runFrontRightMotors(frontRightVoltage);
    runRearLeftMotors(rearLeftVoltage);
    runRearRightMotors(rearRightVoltage);
    lock.give();
  }

}
