#ifndef _CLAW_HPP_
#define _CLAW_HPP_

#include "main.h"
#include <vector>

class Claw {
  private:
    std::vector<pros::Motor> activeMotors;
    std::vector<pros::Motor> flipMotors;
    pros::Mutex lock;

    void runActiveMotors(int voltage);

  public:
    explicit Claw (pros::Mutex motorLock, pros::Motor & flipMotor);

    explicit Claw (pros::Mutex motorLock, pros::Motor & flipMotor, pros::Motor & activeClawMotor);

    void addActiveMotor(pros::Motor & motor);

    void addFlipMotor(pros::Motor & motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    bool removeActiveMotor(pros::Motor & motor);

    bool removeFlipMotor(pros::Motor & motor);

*/

    void clearActiveMotors();

    void clearFlipMotors();

    void clamp();

    void coast();

    void release();

    void flip();

};

class ClawArmControl {
  private:
    Claw* claw;
    ArmControl* arm;

  public:
    explicit ClawArmControl (pros::Mutex & armLock, pros::Motor & armYMotor, pros::Mutex & clawLock, pros::Motor & clawFlipMotor);

    explicit ClawArmControl (pros::Mutex & armLock, pros::Motor & armXMotor, pros::Motor & armYMotor, pros::Mutex & clawLock, pros::Motor & clawFlipMotor);

    explicit ClawArmControl (pros::Mutex & armLock, pros::Motor & armYMotor, pros::Mutex & clawLock, pros::Motor & clawActiveMotor, pros::Motor & clawFlipMotor);

    explicit ClawArmControl (pros::Mutex & armLock, pros::Motor & armXMotor, pros::Motor & armYMotor, pros::Mutex & clawLock, pros::Motor & clawActiveMotor, pros::Motor & clawFlipMotor);

    Claw getClaw();

    ArmControl getArm();

    void run(bool clawClamp, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

    void run(double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

};

#endif
