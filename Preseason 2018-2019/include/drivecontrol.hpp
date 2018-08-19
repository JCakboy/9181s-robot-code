#ifndef _DRIVECONTROL_HPP_
#define _DRIVECONTROL_HPP_

#include "main.h"
#include <vector>

/*
 * Class meant to control robot H-drive
 *
 * The current implementation supportsmultiple motors for
 * each of the two axes and will respect the given mutex
 *
 * Meant to have its run() method called each pass of the opcontrol while loop
 * approx. every 20 ms
 */

class DriveControl {
  private:
    std::vector<pros::Motor> leftMotors;
    std::vector<pros::Motor> rightMotors;
    pros::Mutex lock;
    void runLeftMotors(int voltage);

    void runRightMotors(int voltage);

  public:
    // Creates the Drive Control object with one left and one right motor, see below
    explicit DriveControl(pros::Mutex & motorLock, pros::Motor & leftMotor, pros::Motor & rightMotor);

    /* Creates the Drive Control object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * frontLeftMotor: a motor on the left side of the robot
     * rearLeftMotor: a motor on the left side of the robot
     * frontRightMotor: a motor on the right side of the robot
     * rearRightMotor: a motor on the right side of the robot
     */
    explicit DriveControl(pros::Mutex & motorLock, pros::Motor & frontLeftMotor, pros::Motor & rearLeftMotor, pros::Motor & frontRightMotor, pros::Motor & rearRightMotor);

    // Adds a motor to the left position list
    void addLeftMotor(pros::Motor & motor);

    // Adds a motor to the right position list
    void addRightMotor(pros::Motor & motor);

/* currently not working, may work when pros 3 is released

    // Removes a motor from the left position list
    bool removeLeftMotor(pros::Motor & motor);

    // Removes a motor from the left position list
    bool removeRightMotor(pros::Motor & motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the left position list
    void clearLeftMotors();

    // Clears all motors from the right position list
    void clearRightMotors();

    // Runs the Drive Control with a 1.0 sensitivity. See below
    void run(double moveVoltage, double turnVoltage, bool flipReverse);

    /*
     * Runs the Drive Control by calculating the values for the left and right motors
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * flipReverse: whether to flip left or right movement when reversing, used to more closely mimic movement
     * moveSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges
     * turnSensitivity: multiplier to make turning move or less sensitive. Output voltages will still be in valid ranges.
     */
    void run(double moveVoltage, double turnVoltage, bool flipReverse, double moveSensitivity, double turnSensitivity);

};

#endif
