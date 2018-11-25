#ifndef _DRIVE_HPP_
#define _DRIVE_HPP_

#include "main.h"
#include <vector>

/*
 * Class meant to control robot H-drive
 *
 * The current implementation supports multiple motors for
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
    PID * leftPID;
    PID * rightPID;

    int pidSe;
    int pidLastError;

    void runLeftMotors(int voltage);

    void runRightMotors(int voltage);

    void setLeftBrake(pros::motor_brake_mode_e_t mode);

    void setRightBrake(pros::motor_brake_mode_e_t mode);

    bool runLeftMotorsRelative(int target, int threshold);

    bool runRightMotorsRelative(int target, int threshold);

    bool runMotorsRelative(PID * pid, std::vector<pros::Motor> motors, int target, int threshold);

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

    // Returns a reference to left PID constants
    PID * getLeftPID();

    // Returns a reference to right PID constants
    PID * getRightPID();

    // Clears PID constants
    void clearPID();

    // Runs the Drive Control relative to the current position, setting the left and right targets to the same value, see below
    void moveRelative(double revolutions, int degrees, int threshold);

    /*
     * Runs the Drive Control relative to the current position
     * This method pauses execution until the desired sensor value is reached
     * Given both revolutions and degrees, this method will take into account both values
     * If PID consants have been set, PID will control this operation
     *
     * leftRevolutions: the amount of revolutions to move the left motors forward
     * leftDegrees: the amount of degrees to move the left motors forward
     * rightRevolutions: the amount of revolutions to move the right motors forward
     * rightDegrees: the amount of degrees to move the right motors forward
     * threshold: the maximum allowable difference between the desired and the actual motor position
     */
    void moveRelative(double leftRevolutions, int leftDegrees, double rightRevolutions, int rightDegrees, int threshold);

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

/*
 * Class meant to wrap Drive Control
 *
 * Interfaces turning and pivoting to better code autonomous
 * Passes through necessary functions to Drive Control
 * See Drive Control for documentation
 */

class DriveFunction {
  private:
    DriveControl * driveControl;

    const static int kt;

  public:
    // Creates a Drive Function object, wrapping the given Drive Control
    DriveFunction(DriveControl * driveControl);

    // Returns the Drive Control
    DriveControl & getDriveControl();

    // Turns the robot, specifying whether the robot turns forward or backward and how far to turn
    void turn(bool backward, int degrees);

    // Pivots the robot, specifying how for to turn
    void pivot(int degrees);

    // Moves the robot forward the given amount, see Drive Control
    void move(double revolutions, int degrees, int threshold);

    // See Drive Control
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse);

    // See Drive Control
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity);

};

#endif
