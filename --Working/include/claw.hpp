#ifndef _CLAW_HPP_
#define _CLAW_HPP_

#include "main.h"
#include <vector>

/*
 * A class meant to interface the claw to the opcontrol task
 *
 * The following methods should be called from the opcontrol task
 *    - clamp(): clamps the launcher
 *    - coast(): coasts the claw
 *    - release(): releases the claw
 *    - pull(): pulls the claw push_back
 *    - push(): returns the claw to its un-pulled position
 *    - flip(): flips the claw 180°
 */

class Claw {
  private:
    std::vector<pros::Motor> activeMotors;
    std::vector<pros::Motor> flipMotors;
    std::vector<pros::Motor> pullMotors;
    pros::Mutex * lock;

    void runActiveMotors(int voltage);

  public:
    // Creates the claw object without an active claw motor. See below
    explicit Claw (pros::Mutex & motorLock, pros::Motor flipMotor);

    // Creates the claw object without a claw flip motor. See below
    explicit Claw (pros::Mutex & motorLock, pros::Motor pullMotor, pros::Motor activeMotor);

    /*
     * Creates the claw object
     *
     * motorLock: the mutex it will attempt to take when accessing the motors
     * flipMotor: the motor to use to flip the claw
     * pullMotor: the motor to pull the claw back
     * activeMotor: the motor to use to clamp the claw
     */
    explicit Claw (pros::Mutex & motorLock, pros::Motor flipMotor, pros::Motor pullMotor, pros::Motor activeMotor);

    // Adds a motor to the list to clamp the claw
    void addActiveMotor(pros::Motor motor);

    // Adds a motor to the list to flip the claw
    void addFlipMotor(pros::Motor motor);

    // Adds a motor to the list to pull back the clawLock
    void addPullMotor(pros::Motor motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the list to clamp the claw
    bool removeActiveMotor(pros::Motor motor);

    // Removes a motor from the list to flip the claw
    bool removeFlipMotor(pros::Motor motor);

    // Removes a motor from the list to pull back the clawLock
    bool removePullMotor(pros::Motor motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the list to clamp the claw
    void clearActiveMotors();

    // Clears all motors from the list to flip the claw
    void clearFlipMotors();

    // Clears all motors from the list to pull back the claw
    void clearPullMotors();

    // Clamps the claw
    void clamp();

    // Coast the claw, motors will stop and remain in their position while
    void coast();

    // Releases the claw
    void release();

    // Pulls the claw back
    void pull();

    // Returns the claw to its un-pulled position
    void push();

    // Flips the claw 180°
    void flip();

};

/*
 * Class meant to control a robot claw arm
 *
 * See Claw and ArmControl for documentation
 *
 * Meant to have its run() method caleld each pass of the opcontrol while loop
 * approx. every 20 ms
 */

class ClawArmControl {
  private:
    Claw* claw;
    ArmControl* arm;

  public:
    // Creates the Claw ARm Control object with the spcified arm control and claw
    explicit ClawArmControl (ArmControl & ac, Claw & c);

    // Returns the claw object
    Claw getClaw();

    // Returns the arm control object
    ArmControl getArm();

    // Runs the Claw Arm Control with the claw at a neutral position, see below
    void run(bool clawPull, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

    /*
     * Runs Claw Arm Control by distributing the given values
     *
     * clawClamp: whether to clamp the claw
     * clawPull: whether to pull the claw back
     * armXVoltage: the movement voltage to move the arm left or right, ranging from -127 to 127
     * armYVoltage: the movement voltage to move the arm up or down, ranging from -127 to 127
     * armYankScale: whether to scale arm voltages by cos(45°)*100, used for single joystick movement
     * armXSensitivity: the multiplier used to make arm movement left or right more or less sensitive. Output voltages will still be in valid ranges
     * armYSensitivity: the multiplier used to make arm movement up or down more or less sensitive. Output voltages will still be in valid ranges
     *
     */
    void run(bool clawClamp, bool clawPull, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

};

#endif
