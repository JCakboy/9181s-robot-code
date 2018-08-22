#ifndef _ARM_HPP_
#define _ARM_HPP_

#include "main.h"
#include <vector>

/*
 * Class meant to control a robot arm
 *
 * The current implementation supports horizontal and vertical movement,
 * multiple motors for each of the two axes and will respect the given mutex
 *
 * Meant to have its run() method called each pass of the opcontrol while loop
 * approx. every 20 ms
 */

class ArmControl {
  private:
    std::vector<pros::Motor> xMotors;
    std::vector<pros::Motor> yMotors;
    pros::Mutex lock;

    void runXMotors(int voltage);

    void runYMotors(int voltage);


  public:
    // Creates the Arm Control object without an x motor, see below
    explicit ArmControl(pros::Mutex & motorLock, pros::Motor yMotor);

    /*
     * Creates the Arm Control object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * xMotor: the motor to move the arm left or right
     * yMotor: the motor to move the arm up or down
     */
    explicit ArmControl(pros::Mutex & motorLock, pros::Motor xMotor, pros::Motor yMotor);

    // Adds a motor to the list to move the arm left or right
    void addXMotor(pros::Motor & motor);

    // Adds a motor to the list to move the arm up or down
    void addYMotor(pros::Motor & motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the list to move the arm left or right
    bool removeXMotor(pros::Motor & motor);

    // Removes a motor from the list to move the arm up or down
    bool removeYMotor(pros::Motor & motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the list to move the arm left or right
    void clearXMotors();

    // Clears all motors from the list to move the arm up or down
    void clearYMotors();

    /* Runs the Arm Control by calculating the values for the x and y motors
     *
     * xVoltage: the movement voltage to move the arm left or right, ranging from -127 to 127
     * yVoltage: the movement voltage to move the arm up or down, ranging from -127 to 127
     * tankScale: whether to scale voltages by cos(45°)*100, used for single joystick movement
     * xSensitivity: the multiplier used to make movement left or right more or less sensitive. Output voltages will still be in valid ranges
     * ySensitivity: the multiplier used to make movement up or down more or less sensitive. Output voltages will still be in valid ranges
     */
    void run(double xVoltage, double yVoltage, bool tankScale, double xSensitivity, double ySensitivity);
};

#endif
