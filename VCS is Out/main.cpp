#include <vector>

// Controller enumereration definitions
#define CONTROLLER_MAIN E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER E_CONTROLLER_PARTNER
#define STICK_LEFT_X E_CONTROLLER_ANALOG_LEFT_X
#define STICK_LEFT_Y E_CONTROLLER_ANALOG_LEFT_Y
#define STICK_RIGHT_X E_CONTROLLER_ANALOG_RIGHT_X
#define STICK_RIGHT_Y E_CONTROLLER_ANALOG_RIGHT_Y
#define BUTTON_UP E_CONTROLLER_DIGITAL_UP
#define BUTTON_DOWN E_CONTROLLER_DIGITAL_DOWN
#define BUTTON_LEFT E_CONTROLLER_DIGITAL_LEFT
#define BUTTON_RIGHT E_CONTROLLER_DIGITAL_RIGHT
#define BUTTON_A E_CONTROLLER_DIGITAL_A
#define BUTTON_B E_CONTROLLER_DIGITAL_B
#define BUTTON_X E_CONTROLLER_DIGITAL_X
#define BUTTON_Y E_CONTROLLER_DIGITAL_Y
#define BUTTON_L1 E_CONTROLLER_DIGITAL_L1
#define BUTTON_L2 E_CONTROLLER_DIGITAL_L2
#define BUTTON_R1 E_CONTROLLER_DIGITAL_R1
#define BUTTON_R2 E_CONTROLLER_DIGITAL_R2

// Motor enumeration definitions
#define GEARSET_600 vex::gearSetting::ratio6_1 // 6:1, 600 RPM, Blue gear
#define GEARSET_200 vex::gearSetting::ratio18_1 // 18:1, 200 RPM, Green gear set
#define GEARSET_100 vex::gearSetting::ratio36_1 // 36:1, 100 RPM, Red gear set
#define FWD false
#define REV true
#define ENCODER_DEGREES vex::rotationUnits::deg
#define ENCODER_REVOLUTIONS vex::rotationUnits::rev
#define BRAKE_COAST prosvex::brakeType:coast
#define BRAKE_BRAKE vex::brakeType::brake
#define BRAKE_HOLD vex::brakeType::hold

// Math definitions
#define PI 3.141592653589793238

// Threshhold for horizontally flipping while reversing
#define MOTOR_REVERSE_FLIP_THRESHOLD -42.3 // should be negative value

// How long to run the elastic launcher loading motor
#define ELASTIC_LAUNCHER_MOTOR_TIME 1000 // in ms

// Returns the lesser of the two values
double min(double a, double b) {
	return ((a < b) ? a : b);
}

// Returns the greater of the two values
double max(double a, double b) {
	return ((a > b) ? a : b);
}

// Converts the given degrees to radians
double degrees_to_radians(double degrees) {
	return degrees * PI / 180.0;
}

// Scales the amount by cos(45°) * 100
double tankScaleJoystick(double amt) {
	return amt / cos(degrees_to_radians(45));
}

// Returns ±127, whichever is closer, if the value has an absolute values greater than 127. Otherwise, it returns the given value
int limit127(int a) {
	return max(min(a, 127), -127);
}

// Returns whether the values has an absolute of equal to or less than 127
bool within127(int a) {
	return limit127(a) == a;
}

// Adds or subtracts 1 from the given value, whicher gets it closer to 0
int step0(int a) {
	if (a > 0)
		a--;
	else if (a < 0)
		a++;
	return a;
}

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
    std::vector<vex::motor> xMotors;
    std::vector<vex::motor> yMotors;

    void runXMotors(int voltage);

    void runYMotors(int voltage);


  public:
    // Creates the Arm Control object without an x motor, see below
    explicit ArmControl(vex::motor yMotor);

    /*
     * Creates the Arm Control object
     *
     * xMotor: the motor to move the arm left or right
     * yMotor: the motor to move the arm up or down
     */
    explicit ArmControl(vex::motor xMotor, vex::motor yMotor);

    // Adds a motor to the list to move the arm left or right
    void addXMotor(vex::motor & motor);

    // Adds a motor to the list to move the arm up or down
    void addYMotor(vex::motor & motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the list to move the arm left or right
    bool removeXMotor(vex::motor & motor);

    // Removes a motor from the list to move the arm up or down
    bool removeYMotor(vex::motor & motor);

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

/*
 * A base class meant to interface a ball launcher to the opcontrol task
 *
 * Meant to be inherited and have the following methods overriden:
 *    - _load(): prepares the cannon for a launch, and will be called on creation
 *    - _launch(): launches the cannon
 *
 * The following methods should be called from the opcontrol task
 *    - start(): starts the launcher
 *    - load(): safely and concurrently calls _load()
 *    - launch(): safely and concurrently calls _launch()
 */
/*
class BallLauncher {
  protected:
    vex::task* launchTask;
    int state;

    // The task to control the launcher. Should never be called
    void task();

    // Meant to be overriden to implement the loading a specific type of launcher
    virtual void _load();

    // Meant to be overriden to implement the launching a specific type of launcher
    virtual void _launch();

  public:
    // Starts the launcher and its launcher task
    void start();

    // Returns whether or not the launcher is ready to launch
    bool ready();

    // Loads the launcher
    void load();

    // Launches the launcher
    void launch();
};


 * An implementation of a ball launcher using pneumatics
 *
 * See the base class for documentation
 */
/*
class PneumaticLauncher : public BallLauncher {
  private:
    vex::digital_out* piston;

  protected:
    void _load();

    void _launch();

  public:
    explicit PneumaticLauncher(vex::digital_out & pneumaticPort);
};


 * An implementation of a ball launcher using tension stored by elastics
 *
 * See the base class for documentation
 */
/*
class ElasticLauncher : public BallLauncher {
  private:
    std::vector<vex::motor> motors;

  protected:
    void _load();

    void _launch();

  public:
    explicit ElasticLauncher(vex::motor & motor);

    // Adds a motor to the motor list
    void addMotor(vex::motor & motor);

     may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the motor list
    void removeMotor(vex::motor & motor);
    */
/*
    // Clears all motors from the motor list
    void clearMotors();

  };


 * A class that is an alias for pros::battery
 *
 * https://pros.purduesigbots.com/v5/api/cpp/misc.html#pros-battery
 */

struct Battery {
	double get_capacity();

	double get_current();

	double get_temperature();

	double get_voltage();
};

/*
 * A class meant to interface the claw to the opcontrol task
 *
 * The following methods should be called from the opcontrol task
 *    - clamp(): clamps the launcher
 *    - coast(): coasts the claw
 *    - launch(): releases the claw
 *    - flip(): flips the claw 180°
 */

class Claw {
  private:
    std::vector<vex::motor> activeMotors;
    std::vector<vex::motor> flipMotors;

    void runActiveMotors(int voltage);

  public:
    // Creates the claw object without an active claw motor. See below
    explicit Claw (vex::motor & flipMotor);

    /*
     * Creates the claw object
     *
     * flipMotor: the motor to use to flip the claw
     * activeMotor: the motor to use to clamp the claw
     */
    explicit Claw (vex::motor & flipMotor, vex::motor & activeClawMotor);

    // Adds a motor to the list to clamp the claw
    void addActiveMotor(vex::motor & motor);

    // Adds a motor to the list to flip the claw
    void addFlipMotor(vex::motor & motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the list to clamp the claw
    bool removeActiveMotor(vex::motor & motor);

    // Removes a motor from the list to flip the claw
    bool removeFlipMotor(vex::motor & motor);

*/

    // Clears all motors from the motor lists
    void clearMotors();

    // Clears all motors from the list to clamp the claw
    void clearActiveMotors();

    // Clears all motors from the list to flip the claw
    void clearFlipMotors();

    // Clamps the claw
    void clamp();

    // Coast the claw, motors will stop and remain in their position while
    void coast();

    // Releases the claw
    void release();

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
    // Creates the Claw Arm Control object without an arm X motor or a claw active motor, see below
    explicit ClawArmControl (vex::motor & armYMotor, vex::motor & clawFlipMotor);

    // Creates the Claw Arm Control object without a claw active motor, see below
    explicit ClawArmControl (vex::motor & armXMotor, vex::motor & armYMotor, vex::motor & clawFlipMotor);

    /*
     * Creates the Claw Arm Control object
     *
     * armXMotor: the motor to move the arm left or right
     * armYMotor: the motor to move the arm up or down
     * clawFlipMotor: the motor to use to flip the claw
     * clawActiveMotor: the motor to use to clamp the claw
     */
    explicit ClawArmControl (vex::motor & armXMotor, vex::motor & armYMotor, vex::motor & clawActiveMotor, vex::motor & clawFlipMotor);

    // Returns the claw object
    Claw getClaw();

    // Returns the arm control object
    ArmControl getArm();

    // Runs the Claw Arm Control with the claw at a neutral position, see below
    void run(double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

    /*
     * Runs Claw Arm Control by distributing the given values
     *
     * clawClamp: whether to clamp the claw
     * armXVoltage: the movement voltage to move the arm left or right, ranging from -127 to 127
     * armYVoltage: the movement voltage to move the arm up or down, ranging from -127 to 127
     * armYankScale: whether to scale arm voltages by cos(45°)*100, used for single joystick movement
     * armXSensitivity: the multiplier used to make arm movement left or right more or less sensitive. Output voltages will still be in valid ranges
     * armYSensitivity: the multiplier used to make arm movement up or down more or less sensitive. Output voltages will still be in valid ranges
     *
     */
    void run(bool clawClamp, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity);

};

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
    std::vector<vex::motor> leftMotors;
    std::vector<vex::motor> rightMotors;
    void runLeftMotors(int voltage);

    void runRightMotors(int voltage);

  public:
    // Creates the Drive Control object with one left and one right motor, see below
    explicit DriveControl(vex::motor & leftMotor, vex::motor & rightMotor);

    /*
     * Creates the Drive Control object
     *
     * frontLeftMotor: a motor on the left side of the robot
     * rearLeftMotor: a motor on the left side of the robot
     * frontRightMotor: a motor on the right side of the robot
     * rearRightMotor: a motor on the right side of the robot
     */
    explicit DriveControl(vex::motor & frontLeftMotor, vex::motor & rearLeftMotor, vex::motor & frontRightMotor, vex::motor & rearRightMotor);

    // Adds a motor to the left position list
    void addLeftMotor(vex::motor & motor);

    // Adds a motor to the right position list
    void addRightMotor(vex::motor & motor);

/* currently not working, may work when pros 3 is released

    // Removes a motor from the left position list
    bool removeLeftMotor(vex::motor & motor);

    // Removes a motor from the left position list
    bool removeRightMotor(vex::motor & motor);

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

class XDriveControl {
  private:
    std::vector<vex::motor> frontLeftMotors;
    std::vector<vex::motor> frontRightMotors;
    std::vector<vex::motor> rearLeftMotors;
    std::vector<vex::motor> rearRightMotors;
    void runFrontLeftMotors(int voltage);

    void runFrontRightMotors(int voltage);

    void runRearLeftMotors(int voltage);

    void runRearRightMotors(int voltage);

  public:
    /*
     * Creates the X-Drive Control object
     *
     * frontLeftMotor: a motor on the front left side of the robot
     * rearLeftMotor: a motor on the rear left side of the robot
     * frontRightMotor: a motor on the front right side of the robot
     * rearRightMotor: a motor on the rear right side of the robot
     */
    explicit XDriveControl (vex::motor & frontLeftMotor, vex::motor & rearLeftMotor, vex::motor & frontRightMotor, vex::motor & rearRightMotor);

    // Adds a motor to the front left position list
    void addFrontLeftMotor(vex::motor & motor);

    // Adds a motor to the front right position list
    void addFrontRightMotor(vex::motor & motor);

    // Adds a motor to the rear left position list
    void addRearLeftMotor(vex::motor & motor);

    // Adds a motor to the rear right position list
    void addRearRightMotor(vex::motor & motor);

/* may be implemented in the future in conjunction with DriveControl::remove*Motor()

    // Removes a motor from the front left position list
    bool removeFrontLeftMotor(vex::motor & motor);

    // Removes a motor from the front right position list
    bool removeFrontRightMotor(vex::motor & motor);

    // Removes a motor from the rear left position list
    bool removeRearLeftMotor(vex::motor & motor);

    // Removes a motor from the rear right position list
    bool removeRearRightMotor(vex::motor & motor);

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
    void run(double moveVoltage, double strafeVoltage, double turnVoltage);

    /**
     * Runs the Drive Control by calculating the values for all four motor positions
     *
     * moveVoltage: the movement voltage, forward or backward, ranging from -127 to 127
     * strafeVoltage: the movement voltage, right or left, ranging from -127 to 127
     * turnVoltage: the turning voltage ranging from -127 to 127
     * moveSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges
     * strafeSensitivity: multiplier to make movement right or left move or less sensitive. Output voltages will still be in valid ranges
     * turnSensitivity: multiplier to make movement forward or backward move or less sensitive. Output voltages will still be in valid ranges, however, it is recommended to stay at 1.0
     */
    void run(double moveVoltage, double strafeVoltage, double turnVoltage, double moveSensitivity, double strafeSensitivity, double turnSensitivity);

};

void ArmControl::runXMotors(int voltage) {
  for (auto & motor : ArmControl::xMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

void ArmControl::runYMotors(int voltage) {
  for (auto & motor : ArmControl::yMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

ArmControl::ArmControl(vex::motor yMotor) {
  ArmControl::yMotors.push_back(yMotor);
}

ArmControl::ArmControl(vex::motor xMotor, vex::motor yMotor) {
  ArmControl::xMotors.push_back(xMotor);
  ArmControl::yMotors.push_back(yMotor);
}

void ArmControl::addXMotor(vex::motor & motor) {
  ArmControl::xMotors.push_back(motor);
}

void ArmControl::addYMotor(vex::motor & motor) {
  ArmControl::yMotors.push_back(motor);
}

void ArmControl::clearMotors() {
  ArmControl::clearXMotors();
  ArmControl::clearYMotors();
}

void ArmControl::clearXMotors() {
  ArmControl::xMotors.clear();
}

void ArmControl::clearYMotors() {
  ArmControl::yMotors.clear();
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool ArmControl::removeXMotor(vex::motor & motor) {

}

bool ArmControl::removeYMotor(vex::motor & motor) {

}
*/
void ArmControl::run(double xVoltage, double yVoltage, bool tankScale, double xSensitivity, double ySensitivity) {
  if (tankScale) {
    xVoltage = tankScaleJoystick(xVoltage);
    yVoltage = tankScaleJoystick(yVoltage);
  }

  xVoltage = limit127(xVoltage * xSensitivity);
  yVoltage = limit127(yVoltage * ySensitivity);

    ArmControl::runXMotors(xVoltage);
    ArmControl::runYMotors(yVoltage);

}
/*
void BallLauncher::start() {
  //BallLauncher::launchTask = &pros::Task( &BallLauncher::task );
  BallLauncher::launchTask->resume();
  BallLauncher::load();
}

bool BallLauncher::ready() {
  return BallLauncher::state == 1;
}

void BallLauncher::load() {
  BallLauncher::launchTask->notify_ext(1, E_NOTIFY_ACTION_OWRITE, NULL);
}

void BallLauncher::launch() {
  if (BallLauncher::ready())
    BallLauncher::launchTask->notify_ext(2, E_NOTIFY_ACTION_OWRITE, NULL);
}

void BallLauncher::task() {
  while (true) {
    int tstate = BallLauncher::launchTask->notify_take(true, TIMEOUT_MAX);
    if (tstate == BallLauncher::state)
      continue;
    if (tstate == 1) {
      BallLauncher::_load();
    } else if (tstate == 2) {
      BallLauncher::_launch();
    }
    BallLauncher::state = tstate;
  }
}


PneumaticLauncher::PneumaticLauncher(vex::digital_out & piston) {
  PneumaticLauncher::piston = &piston;
}

void PneumaticLauncher::_launch() {
  PneumaticLauncher::piston->set(true);
}

void PneumaticLauncher::_load() {
  PneumaticLauncher::piston->set(false);
}

ElasticLauncher::ElasticLauncher(vex::motor & motor) {
  motor.setStopping(BRAKE_HOLD);
  ElasticLauncher::motors.push_back(motor);
}

void ElasticLauncher::_launch() {
  for (auto & motor : ElasticLauncher::motors)
    motor.setStopping(BRAKE_COAST);
}

void ElasticLauncher::_load() {
  for (auto & motor : ElasticLauncher::motors) {
    motor.setStopping(BRAKE_HOLD);
    motor.move(127);
  }
  vex::this_thread::sleep_for(ELASTIC_LAUNCHER_MOTOR_TIME);
  for (auto & motor : ElasticLauncher::motors)
    motor.move(0);
}
*/
double Battery::get_capacity() {
  vex::brain::battery battery;
	return battery.capacity();
}

double Battery::get_temperature() {
  vex::brain::battery battery;
	return battery.temperature();
}

void Claw::runActiveMotors(int voltage) {
  for (auto & motor : Claw::activeMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

Claw::Claw (vex::motor & flipMotor) {
  Claw::addFlipMotor(flipMotor);
}

Claw::Claw (vex::motor & flipMotor, vex::motor & activeMotor) {
  Claw::addFlipMotor(flipMotor);
  Claw::addActiveMotor(activeMotor);
}

void Claw::addActiveMotor(vex::motor & motor) {
  Claw::activeMotors.push_back(motor);
}

void Claw::addFlipMotor(vex::motor & motor) {
  motor.setStopping(BRAKE_BRAKE);
  Claw::flipMotors.push_back(motor);
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool Claw::removeActiveMotor(vex::motor & motor) {

}

bool Claw::removeFlipMotor(vex::motor & motor) {

}
*/
void Claw::clearMotors() {
  Claw::clearActiveMotors();
  Claw::clearFlipMotors();
}

void Claw::clearActiveMotors(){
  Claw::activeMotors.clear();
}

void Claw::clearFlipMotors() {
  Claw::flipMotors.clear();
}

void Claw::clamp() {
    Claw::runActiveMotors(127);

}

void Claw::coast() {
    Claw::runActiveMotors(0);
}

void Claw::release() {
    Claw::runActiveMotors(-127);
}

void Claw::flip() {
    for (auto & motor : Claw::flipMotors)
      motor.startRotateTo(180, ENCODER_DEGREES, 100, vex::velocityUnits::pct);
}

ClawArmControl::ClawArmControl (vex::motor & armYMotor, vex::motor & clawFlipMotor) {
  ArmControl ac (armYMotor);
  Claw c (clawFlipMotor);
  ClawArmControl::arm = &ac;
  ClawArmControl::claw = &c;
}

ClawArmControl::ClawArmControl (vex::motor & armXMotor, vex::motor & armYMotor, vex::motor & clawFlipMotor) {
  ArmControl ac (armXMotor, armYMotor);
  Claw c (clawFlipMotor);
  ClawArmControl::arm = &ac;
  ClawArmControl::claw = &c;
}

ClawArmControl::ClawArmControl (vex::motor & armXMotor, vex::motor & armYMotor, vex::motor & clawActiveMotor, vex::motor & clawFlipMotor) {
  ArmControl ac (armXMotor, armYMotor);
  Claw c ( clawActiveMotor, clawFlipMotor);
  ClawArmControl::arm = &ac;
  ClawArmControl::claw = &c;
}

Claw ClawArmControl::getClaw() {
  return *ClawArmControl::claw;
}

ArmControl ClawArmControl::getArm() {
  return *ClawArmControl::arm;
}

void ClawArmControl::run(bool clawClamp, double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity) {
  if (clawClamp)
    claw->clamp();
  else
    claw->release();
  arm->run(armXVoltage, armYVoltage, armTankScale, armXSensitivity, armYSensitivity);
}

void ClawArmControl::run(double armXVoltage, double armYVoltage, bool armTankScale, double armXSensitivity, double armYSensitivity) {
  claw->coast();
  arm->run(armXVoltage, armYVoltage, armTankScale, armXSensitivity, armYSensitivity);
}

void DriveControl::runLeftMotors(int voltage) {
  for (auto & motor : DriveControl::leftMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

void DriveControl::runRightMotors(int voltage) {
  for (auto & motor : DriveControl::rightMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

DriveControl::DriveControl(vex::motor & leftMotor, vex::motor & rightMotor) {
  DriveControl::leftMotors.push_back(leftMotor);
  DriveControl::rightMotors.push_back(rightMotor);
}

DriveControl::DriveControl(vex::motor & frontLeftMotor, vex::motor & rearLeftMotor, vex::motor & frontRightMotor, vex::motor & rearRightMotor) {
  DriveControl::addLeftMotor(frontLeftMotor);
  DriveControl::addLeftMotor(rearLeftMotor);
  DriveControl::addRightMotor(frontRightMotor);
  DriveControl::addRightMotor(rearRightMotor);
}

void DriveControl::addLeftMotor(vex::motor & motor) {
  DriveControl::leftMotors.push_back(motor);
}

void DriveControl::addRightMotor(vex::motor & motor) {
  DriveControl::rightMotors.push_back(motor);
}

void DriveControl::clearMotors() {
  DriveControl::clearLeftMotors();
  DriveControl::clearRightMotors();
}

void DriveControl::clearLeftMotors() {
  DriveControl::leftMotors.clear();
}

void DriveControl::clearRightMotors() {
  DriveControl::rightMotors.clear();
}
/* currently not working, may work when pros 3 is released
bool DriveControl::removeLeftMotor(vex::motor & motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::leftMotors.size(); i++)
    if (DriveControl::leftMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::leftMotors.erase(DriveControl::leftMotors.begin()+i);
  return found;
}

bool DriveControl::removeRightMotor(vex::motor & motor) {
  int i = 0;
  bool found = false;
  for (; i < DriveControl::rightMotors.size(); i++)
    if (DriveControl::rightMotors[i]._port == motor._port) {
      found = true;
      break;
    }
  if (found)
    DriveControl::rightMotors.erase(DriveControl::rightMotors.begin()+i);
  return found;
}
*/
void DriveControl::run(double moveVoltage, double turnVoltage, bool flipReverse) {
  DriveControl::run(moveVoltage, turnVoltage, flipReverse, 1.0, 1.0);
}


void DriveControl::run(double moveVoltage, double turnVoltage, bool flipReverse, double moveSensitivity, double turnSensitivity) {
  bool flip = flipReverse && moveVoltage < MOTOR_REVERSE_FLIP_THRESHOLD;

  moveVoltage *= moveSensitivity;
  turnVoltage *= turnSensitivity;

  int leftVoltage = limit127(!flip ? moveVoltage - turnVoltage : moveVoltage + turnVoltage);
  int rightVoltage = limit127(!flip ? moveVoltage + turnVoltage : moveVoltage - turnVoltage);

    DriveControl::runLeftMotors(leftVoltage);
    DriveControl::runRightMotors(rightVoltage);
}

void XDriveControl::runFrontLeftMotors(int voltage) {
  for (auto & motor : XDriveControl::frontLeftMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

void XDriveControl::runRearLeftMotors(int voltage) {
  for (auto & motor : XDriveControl::rearLeftMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

void XDriveControl::runFrontRightMotors(int voltage) {
  for (auto & motor : XDriveControl::frontRightMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

void XDriveControl::runRearRightMotors(int voltage) {
  for (auto & motor : XDriveControl::rearRightMotors)
    motor.spin(vex::directionType::fwd, voltage / 127 * 100, vex::velocityUnits::pct);
}

XDriveControl::XDriveControl(vex::motor & frontLeftMotor, vex::motor & rearLeftMotor, vex::motor & frontRightMotor, vex::motor & rearRightMotor) {
  XDriveControl::addFrontLeftMotor(frontLeftMotor);
  XDriveControl::addFrontRightMotor(frontRightMotor);
  XDriveControl::addRearLeftMotor(rearLeftMotor);
  XDriveControl::addRearRightMotor(rearRightMotor);
}

void XDriveControl::addFrontLeftMotor(vex::motor & motor) {
  XDriveControl::frontLeftMotors.push_back(motor);
}

void XDriveControl::addFrontRightMotor(vex::motor & motor) {
  XDriveControl::frontRightMotors.push_back(motor);
}

void XDriveControl::addRearLeftMotor(vex::motor & motor) {
  XDriveControl::rearLeftMotors.push_back(motor);
}

void XDriveControl::addRearRightMotor(vex::motor & motor) {
  XDriveControl::rearRightMotors.push_back(motor);
}
/* may be implemented in the future in conjunction with DriveControl::remove*Motor()
bool XDriveControl::removeFrontLeftMotor(vex::motor & motor) {

}

bool XDriveControl::removeFrontRightMotor(vex::motor & motor) {

}

bool XDriveControl::removeRearLeftMotor(vex::motor & motor) {

}

bool XDriveControl::removeRearRightMotor(vex::motor & motor) {

}
*/
void XDriveControl::clearMotors() {
  XDriveControl::clearFrontLeftMotors();
  XDriveControl::clearFrontRightMotors();
  XDriveControl::clearRearLeftMotors();
  XDriveControl::clearRearRightMotors();
}

void XDriveControl::clearFrontLeftMotors() {
  XDriveControl::frontLeftMotors.clear();
}

void XDriveControl::clearFrontRightMotors() {
  XDriveControl::frontRightMotors.clear();
}

void XDriveControl::clearRearLeftMotors() {
  XDriveControl::rearLeftMotors.clear();
}

void XDriveControl::clearRearRightMotors() {
  XDriveControl::rearRightMotors.clear();
}

void XDriveControl::run(double moveVoltage, double strafeVoltage, double turnVoltage) {
  XDriveControl::run(moveVoltage, strafeVoltage, turnVoltage, 1.0, 1.0, 1.0);
}

void XDriveControl::run(double moveVoltage, double strafeVoltage, double turnVoltage, double moveSensitivity, double strafeSensitivity, double turnSensitivity) {
  moveVoltage *= moveSensitivity;
  strafeVoltage *= strafeSensitivity;
  turnVoltage *= turnSensitivity;

  int frontLeftVoltage = limit127(moveVoltage + strafeVoltage);
  int frontRightVoltage = limit127(moveVoltage - strafeVoltage);
  int rearLeftVoltage = limit127(moveVoltage - strafeVoltage);
  int rearRightVoltage = limit127(moveVoltage + strafeVoltage);

  class nest {
    public:
      static int distribute(int & turn, int & frontVoltage, int & rearVoltage) {
        int overflow = 0;
        for (; turn != 0; turn = step0(turn)) {
          int ftemp = round(frontVoltage + turn - step0(turn));
          int rtemp = round(rearVoltage + turn - step0(turn));

          if (within127(ftemp))
            frontVoltage = ftemp;
          else {
            int rtt = rtemp + turn - step0(turn);
            if (within127(rtt))
              rtemp = rtt;
            else if (!within127(rtemp)) {
              overflow = overflow - turn + step0(turn);
              continue;
            }
          }

          if (within127(rtemp))
            rearVoltage = rtemp;
          else {
            int ftt = rtemp + turn - step0(turn);
            if (within127(ftt))
              frontVoltage = ftt;
          }
        }
        return overflow;
      }
  };

  int lt = round(turnVoltage / 2.0);
	int rt = round(turnVoltage / -2.0);

	double overflow = 0;

  overflow = nest::distribute(lt, frontLeftVoltage, rearLeftVoltage);

	rt += overflow;
	overflow = nest::distribute(rt, frontRightVoltage, rearRightVoltage);

	lt += overflow;
	overflow = nest::distribute(lt, frontLeftVoltage, rearLeftVoltage);

    runFrontLeftMotors(frontLeftVoltage);
    runFrontRightMotors(frontRightVoltage);
    runRearLeftMotors(rearLeftVoltage);
    runRearRightMotors(rearRightVoltage);

}
