#ifndef _DRIVE_HPP_
#define _DRIVE_HPP_

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
    pros::Mutex * lock;

    bool usePID;
    PID * pid;

    void runLeftMotors(int voltage);

    void runRightMotors(int voltage);

    void setLeftBrake(pros::motor_brake_mode_e_t mode);

    void setRightBrake(pros::motor_brake_mode_e_t mode);

  public:
    // Creates the Drive Control object with one left and one right motor, see below
    explicit DriveControl(pros::Mutex & motorLock, pros::Motor leftMotor, pros::Motor rightMotor);

    /*
     * Creates the Drive Control object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * frontLeftMotor: a motor on the left side of the robot
     * rearLeftMotor: a motor on the left side of the robot
     * frontRightMotor: a motor on the right side of the robot
     * rearRightMotor: a motor on the right side of the robot
     */
    explicit DriveControl(pros::Mutex & motorLock, pros::Motor frontLeftMotor, pros::Motor rearLeftMotor, pros::Motor frontRightMotor, pros::Motor rearRightMotor);

    // Adds a motor to the left position list
    void addLeftMotor(pros::Motor motor);

    // Adds a motor to the right position list
    void addRightMotor(pros::Motor motor);

/* currently not working, may work when pros 3 is released

    // Removes a motor from the left position list
    bool removeLeftMotor(pros::Motor motor);

    // Removes a motor from the left position list
    bool removeRightMotor(pros::Motor motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the left position list
    void clearLeftMotors();

    // Clears all motors from the right position list
    void clearRightMotors();

    // Sets PID constants
    void setPID(int dt, double kp, double ki, double kd, int limit);

    // Returns a reference to PID constants
    PID & getPID();

    // Clears PID constants
    void clearPID();

    /*
     * Runs the Drive Control relative to the current position
     * This method pauses execution until the desired sensor value is reached
     * Given both revolutions and degrees, this method will take into account both values
     * If PID consants have been set, PID will control this operation
     *
     * revolutions: the amount of revolutions to move the motor forward
     * degrees: the amount of degrees to move the motor forward
     * threshold: the maximum allowable difference between the desired and the actual motor position
     * moveLeft: whether to move the left motors during this operation
     * moveRight: whether to move the right motors during this operation
     */
    void moveRelative(double revolutions, int degrees, int threshold, bool moveLeft, bool moveRight);

    // Runs the Drive Control with a 1.0 sensitivity. See below
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse);

    /*
     * Runs the Drive Control by calculating the values for the left and right motors
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * leftBrake: whether to brake the left motors. Braking involves a physical brake while reducing movement 10 fold
     * rightBrake: whether to brake the right motors. Braking involves a physical brake while reducing movement 10 fold
     * flipReverse: whether to flip left or right movement when reversing, used to more closely mimic movement
     * moveSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges
     * turnSensitivity: multiplier to make turning move or less sensitive. Output voltages will still be in valid ranges.
     */
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity);

};

#endif