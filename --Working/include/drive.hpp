#ifndef _DRIVE_HPP_
#define _DRIVE_HPP_

#include "main.h"
#include <utility>
#include <vector>

/*
 * Class meant to control robot H-drive
 *
 * The current implementation supports multiple motors for
 * each of the two sides and will respect the given mutex
 *
 * Meant to have its run() method called each pass of the opcontrol while loop
 * approx. every 20 ms
 */

class DriveControl {
  friend void ::autonomous();
  private:
    // Lists of motors for right and left sides
    std::vector<pros::Motor> leftMotors;
    std::vector<pros::Motor> rightMotors;
    // The mutex to take before attempting to move the motors
    pros::Mutex * lock;

    // PID constant and calculation variables
    bool usePID;
    PID * pid;
    PIDCalc * leftPIDCalc;
    PIDCalc * rightPIDCalc;

    int pidSe;
    int pidLastError;

    // Iterates and runs left motors at a given voltage
    void runLeftMotors(int voltage);

    // Iterates and runs right motors at a given voltage
    void runRightMotors(int voltage);

    // Iterates and sets the brake mode of all left motors to a given mode
    void setLeftBrake(pros::motor_brake_mode_e_t mode);

    // Iterates and sets the brake mode of all right motors to a given mode
    void setRightBrake(pros::motor_brake_mode_e_t mode);

    // The middleman to facilitate choosing between a PID calculation or a simple move_relative() motor command
    PIDCommand runMotorsRelative(PIDCalc * calc, std::vector<pros::Motor> motors, int target);

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

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the left position list
    void clearLeftMotors();

    // Clears all motors from the right position list
    void clearRightMotors();

    // Sets PID constants, see PID documentation
    void setPID(int dt, double kp, double ki, double kd, bool brake, int tLimit, int iLimit, int iZone, int dThreshold, int tThreshold, int de0);

    // Gets PID constants
    PID * getPID();

    // Clears PID constants
    void clearPID();

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
     */
    void moveRelative(double leftRevolutions, int leftDegrees, double rightRevolutions, int rightDegrees);

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
    // The DriveControl object to wrap and/or call
    DriveControl * driveControl;

    // Values influencing gear ratio
    double in;
    double out;
    double wheelDiameter;

    // The values to use to calculate turning targets
    int pt;
    int kt;

  public:
    // Creates a Drive Function object, wrapping the given Drive Control
    DriveFunction(DriveControl * driveControl);

    // Returns the Drive Control
    DriveControl * getDriveControl();

    // Sets the turn porpotional value and the turn constant, used to calculate the amount to move the motors to turn the robot
    void setTurnValues(int pt, int kt);

    // Returns the turn porpotional value and the turn constant
    std::pair<int, int> getTurnValues();

    // Sets the gear ratio for the drive motors, allowing for movement given in inches
    void setGearRatio(double in, double out, double wheelDiameter);

    // Returns the full gear ratio calculated for the motors, factoring in input, output and wheelDiameter;
    double getFullRatio();

    // Returns the input gear ratio set with setGearRatio();
    double getInputRatio();

    // Returns the output gear ratio set with setGearRatio();
    double getOutputRatio();

    // Returns the wheel diameter set with setGearRatio();
    double getWheelDiameter();

    // Turns the robot forward, specifying how far to turn
    void turn(int degrees);

    // Turns the robot, specifying whether the robot turns forward or backward and how far to turn
    void turn(bool backward, int degrees);

    // Pivots the robot, specifying how for to turn
    void pivot(int degrees);

    // Moves the robot forward the given amount of inches, calculated using the given gear ratio
    void move(double inches);

    // Moves the robot forward the given amount of degrees
    void moveDegrees(int degrees);

    // Moves the robot forward the given amount of revolutions and degrees
    void moveRevolutions(double revolutions, int degrees);

    // See Drive Control
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse);

    // See Drive Control
    void run(double moveVoltage, double turnVoltage, bool leftBrake, bool rightBrake, bool flipReverse, double moveSensitivity, double turnSensitivity);

};

#endif
