#ifndef __XDRIVECONTROL_HPP_
#define __XDRIVECONTROL_HPP_

#include "main.h"
#include <vector>

class XDriveControl {
  private:
    std::vector<pros::Motor> frontLeftMotors;
    std::vector<pros::Motor> frontRightMotors;
    std::vector<pros::Motor> rearLeftMotors;
    std::vector<pros::Motor> rearRightMotors;
    pros::Mutex * lock;
    void runFrontLeftMotors(int voltage);

    void runFrontRightMotors(int voltage);

    void runRearLeftMotors(int voltage);

    void runRearRightMotors(int voltage);

    void setFrontLeftBrake(pros::motor_brake_mode_e_t mode);

    void setFrontRightBrake(pros::motor_brake_mode_e_t mode);

    void setRearLeftBrake(pros::motor_brake_mode_e_t mode);

    void setRearRightBrake(pros::motor_brake_mode_e_t mode);

  public:
    /*
     * Creates the X-Drive Control object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * frontLeftMotor: a motor on the front left side of the robot
     * rearLeftMotor: a motor on the rear left side of the robot
     * frontRightMotor: a motor on the front right side of the robot
     * rearRightMotor: a motor on the rear right side of the robot
     */
    explicit XDriveControl (pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor);

    // Adds a motor to the front left position list
    void addFrontLeftMotor(pros::Motor motor);

    // Adds a motor to the front right position list
    void addFrontRightMotor(pros::Motor motor);

    // Adds a motor to the rear left position list
    void addRearLeftMotor(pros::Motor motor);

    // Adds a motor to the rear right position list
    void addRearRightMotor(pros::Motor motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the front left position list
    bool removeFrontLeftMotor(pros::Motor motor);

    // Removes a motor from the front right position list
    bool removeFrontRightMotor(pros::Motor motor);

    // Removes a motor from the rear left position list
    bool removeRearLeftMotor(pros::Motor motor);

    // Removes a motor from the rear right position list
    bool removeRearRightMotor(pros::Motor motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the front left position list
    void clearFrontLeftMotors();

    // Clears all motors from the front right position list
    void clearFrontRightMotors();

    // Clears all motors from the rear left position list
    void clearRearLeftMotors();

    // Clears all motors from the rear right position list
    void clearRearRightMotors();

    // Runs the Drive Control with a 1.0 sensitivity. See below
    void run(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake);

    /**
     * Runs the Drive Control by calculating the values for all four motor positions
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * strafeVoltage: the movement voltage, right or left, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * brake: whether to brake the motors. Braking involves a physical brake while reducing movement 10 fold
     * moveSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges
     * strafeSensitivity: multiplier to make movement right or left move or less sensitive. Output voltages will still be in valid ranges
     * turnSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges, however, it is recommended to stay at 1.0
     */
    void run(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake, double moveSensitivity, double strafeSensitivity, double turnSensitivity);

};

#endif
