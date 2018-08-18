#ifndef _DRIVECONTROL_HPP_
#define _DRIVECONTROL_HPP_

#include "main.h"
#include <vector>

class DriveControl {
  private:
    std::vector<pros::Motor> leftMotors; // Holds POINTERS to the motors, be sure to DEREFERENCE them before calling functions
    std::vector<pros::Motor> rightMotors;
    pros::Mutex lock;
    void runLeftMotors(int voltage);

    void runRightMotors(int voltage);


  public:
    explicit DriveControl (pros::Mutex & motorLock, pros::Motor & leftMotor, pros::Motor & rightMotor);

    explicit DriveControl (pros::Mutex & motorLock, pros::Motor & leftFrontMotor, pros::Motor & leftRearMotor, pros::Motor & rightFrontMotor, pros::Motor & rightRearMotor);

    void addLeftMotor(pros::Motor & motor);

    void addRightMotor(pros::Motor & motor);
/* currently not working, may work when pros 3 is released
    bool removeLeftMotor(pros::Motor & motor);

    bool removeRightMotor(pros::Motor & motor);
*/
    void clearLeftMotors();

    void clearRightMotors();

    void run(double moveVoltage, double turnVoltage, bool tankScale, double moveSensitivity, double turnSensitivity);

};

#endif
