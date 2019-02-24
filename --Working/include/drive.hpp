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
  private:
    // The mutex to take before attempting to move the motors
    pros::Mutex * lock;

    // Lists of motors for the left sides
    std::vector<pros::Motor*> otherLeftMotors;
    std::vector<pros::Motor*> frontLeftMotors;
    std::vector<pros::Motor*> backLeftMotors;

    // Lists of motors for the right sides
    std::vector<pros::Motor*> otherRightMotors;
    std::vector<pros::Motor*> frontRightMotors;
    std::vector<pros::Motor*> backRightMotors;

    // Whether to use PID for the motor movement
    bool usePID;

    // PID constants
    PID * frontLeftPID;
    PID * backLeftPID;
    PID * frontRightPID;
    PID * backRightPID;

    // Iterates and runs left motors at a given power
    void runLeftMotors(int power);

    // Iterates and runs right motors at a given power
    void runRightMotors(int power);

    // Iterates and runs the front left motors at a given power
    void runFrontLeftMotors(int power);

    // Iterates and runs the back left motors at a given power
    void runBackLeftMotors(int power);

    // Iterates and runs the front right motors at a given power
    void runFrontRightMotors(int power);

    // Iterates and runs the back right motors at a given power
    void runBackRightMotors(int power);

    // Iterates and sets the brake mode of all left motors to a given mode
    void setLeftBrake(pros::motor_brake_mode_e_t mode);

    // Iterates and sets the brake mode of all right motors to a given mode
    void setRightBrake(pros::motor_brake_mode_e_t mode);

    // The middleman to facilitate choosing between a PID calculation or a simple move_relative() motor command
    PIDCommand runMotorsRelative(PID * pid, PIDCalc * calc, std::vector<pros::Motor*> motors, int target);

  public:
    // Creates the Drive Control object with one left and one right motor, see below
    explicit DriveControl(pros::Mutex * motorLock, pros::Motor * leftMotor, pros::Motor * rightMotor);

    /*
     * Creates the Drive Control object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * frontLeftMotor: a motor on the left side of the robot
     * rearLeftMotor: a motor on the left side of the robot
     * frontRightMotor: a motor on the right side of the robot
     * rearRightMotor: a motor on the right side of the robot
     */
    explicit DriveControl(pros::Mutex * motorLock, pros::Motor * frontLeftMotor, pros::Motor * rearLeftMotor, pros::Motor * frontRightMotor, pros::Motor * rearRightMotor);

    // Reinitializes the motors of the Drive Control object with 2 motors
    void reinitialize(pros::Motor * leftMotor, pros::Motor * rightMotor);

    // Reinitializes the motors of the Drive Control object with 4 motors
    void reinitialize(pros::Motor * frontLeftMotor, pros::Motor * rearLeftMotor, pros::Motor * frontRightMotor, pros::Motor * rearRightMotor);

    // Adds a motor to the general left motors list
    void addLeftMotor(pros::Motor * motor);

    // Adds a motor to the front left motors list
    void addFrontLeftMotor(pros::Motor * motor);

    // Adds a motor to the back left motors list
    void addBackLeftMotor(pros::Motor * motor);

    // Adds a motor to the general right motors list
    void addRightMotor(pros::Motor * motor);

    // Adds a motor to the front right motors list
    void addFrontRightMotor(pros::Motor * motor);

    // Adds a motor to the back right motors list
    void addBackRightMotor(pros::Motor * motor);

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the general left motors list
    void clearLeftMotors();

    // Clears all motors from the front left motors list
    void clearFrontLeftMotors();

    // Clears all motors from the back left motors list
    void clearBackLeftMotors();

    // Clears all motors from the general right motors list
    void clearRightMotors();

    // Clears all motors from the front right motors list
    void clearFrontRightMotors();

    // Clears all motors from the back right motors list
    void clearBackRightMotors();

    // Sets left and right PID constants to the same values, see PID documentation
    void setPID(int dt, double kp, double ki, double kd, bool brake, int tLimit, double aLimit, int iLimit, int iZone, int dThreshold, int tThreshold, int de0);

    // Sets all PID constants to the same values, see PID documentation
    void setPID(PID pid);

    // Sets left and right PID constants, see PID documentation
    void setPID(PID leftPID, PID rightPID);

    // Sets all PID constants, see PID documentation
    void setPID(PID * frontLeftPID, PID * backLeftPID, PID * frontRightPID, PID * backRightPID);

    // Returns whether or not it is using PID control
    bool usingPID();

    // Gets front left PID constants, see PID documentation
    PID * getFrontLeftPID();

    // Gets back left PID constants, see PID documentation
    PID * getBackLeftPID();

    // Gets front right PID constants, see PID documentation
    PID * getFrontRightPID();

    // Gets back right PID constants, see PID documentation
    PID * getBackRightPID();

    // Clears PID constants
    void clearPID();

    /*
     * Runs the Drive Control relative to the current position
     * This method pauses execution until the desired sensor value is reached
     * If PID consants have been set, PID will control this operation
     *
     * frontLeftDegrees: the amount of degrees to move the front left motors forward
     * backLeftDegrees: the amount of degrees to move the back left motors forward
     * frontRightDegrees: the amount of degrees to move the front right motors forward
     * backRightDegrees: the amount of degrees to move the back right motors forward
     */
    void moveRelative(int frontLeftDegrees, int backLeftDegrees, int frontRightDegrees, int backRightDegrees);

    // Runs the Robot H-Drive Control with a 1.0 sensitivity. See below
    void run(double moveVoltage, double turnVoltage, bool brake, bool flipReverse);

    /*
     * Runs the Robot H-Drive Control by calculating the values for the left and right motors
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * brake: whether to physically brake the motors when stopped
     * flipReverse: whether to flip left or right movement when reversing, used to more closely mimic joystick movement
     * moveSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges
     * turnSensitivity: multiplier to make turning move or less sensitive. Output voltages will still be in valid ranges.
     */
    void runH(double moveVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double turnSensitivity);

    /*
     * Runs the Robot Mecanum H-Drive Control by calculating the values for the all drive motors
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * turnStrafeVoltage: the strafing or turning voltage ranging from -127 to 127. This value will be interpreted using the value set in strafe
     * strafe: whether to interpret turnStrafeVoltage as a strafe voltage, otherwise, will be turn voltage
     * brake: whether to physically brake the motors when stopped
     * flipReverse: whether to flip left or right movement when reversing, used to more closely mimic joystick movement
     * moveSensitivity: multiplier to make movement forward or backward more or less sensitive. Output voltages will still be in valid ranges
     * turnStrafeSensitivity: multiplier to make turning or strafing more or less sensitive. Output voltages will still be in valid ranges.
     */
    void runStrafe(double moveVoltage, double turnStrafeVoltage, bool strafe, bool brake, bool flipReverse, double moveSensitivity, double turnStrafeSensitivity);

    /*
     * Runs the Robot Mecanum H-Drive Control or X-Drive Control by calculating the values for the all drive motors
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * strafeVoltage: the movement voltage, left or right, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * brake: whether to physically brake the motors when stopped
     * flipReverse: whether to flip left or right movement when reversing, used to more closely mimic joystick movement
     * moveSensitivity: multiplier to make movement forward or backward more or less sensitive. Output voltages will still be in valid ranges
     * strafeSensitivity: multiplier to make movement left or right more or less sensitive. Output voltages will still be in valud ranges
     * turnSensitivity: multiplier to make turning more or less sensitive. Output voltages will still be in valid ranges.
     */
    void runX(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double strafeSensitivity, double turnSensitivity);

    // Stops all drive motors, braking if requested
    void stop(bool brake);
};

/*
 * Class meant to wrap Drive Control
 *
 * Interfaces turning, pivoting and strafing (if applicable) to better code autonomous
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

    // The amount to move the motors to strafe one inch
    int ks;

  public:
    // Creates a Drive Function object, wrapping the given Drive Control
    DriveFunction(DriveControl * driveControl);

    // Returns the Drive Control
    DriveControl * getDriveControl();

    // Sets the strafe constant, the amount to move the motors to strafe one inch
    void setStrafeValue(int ks);

    // Returns the strafe constant
    int getStrafeValue();

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

    // Stafes the robot, given the amount of inches
    void strafe(double inches);

    // Moves the robot forward the given amount of inches, calculated using the given gear ratio
    void move(double inches);

    // Moves the robot forward the given amount of degrees
    void moveDegrees(int degrees);

    // Moves the robot forward the given amount of revolutions and degrees
    void moveRevolutions(double revolutions, int degrees);

    // Pass through functions to DriveControl. See above for documentation
    void run(double moveVoltage, double turnVoltage, bool brake, bool flipReverse);
    void runH(double moveVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double turnSensitivity);
    void runStrafe(double moveVoltage, double turnStrafeVoltage, bool strafe, bool brake, bool flipReverse, double moveSensitivity, double turnStrafeSensitivity);
    void runX(double moveVoltage, double strafeVoltage, double turnVoltage, bool brake, bool flipReverse, double moveSensitivity, double strafeSensitivity, double turnSensitivity);

    // Stops the robot
    void stop();

    // Stops the robot, braking if requested
    void stop(bool brake);
};

#endif
