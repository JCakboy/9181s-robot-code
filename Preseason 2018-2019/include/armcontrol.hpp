#ifndef _ARM_HPP_
#define _ARM_HPP_

#include "main.h"
#include <vector>

class ArmControl {
private:
    std::vector<pros::Motor> xMotors;
    std::vector<pros::Motor> yMotors;
    pros::Mutex lock;

    void runXMotors(int voltage);

    void runYMotors(int voltage);


  public:
    explicit ArmControl(pros::Mutex & motorLock, pros::Motor yMotor);

    explicit ArmControl(pros::Mutex & motorLock, pros::Motor xMotor, pros::Motor yMotor);

    void addXMotor(pros::Motor & motor);

    void addYMotor(pros::Motor & motor);
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
    bool removeXMotor(pros::Motor & motor);

    bool removeYMotor(pros::Motor & motor);
*/
    void clearMotors();

    void clearXMotors();

    void clearYMotors();

    void run(double xVoltage, double yVoltage, bool tankScale, double xSensitivity, double ySensitivity);
};

#endif
