#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
std::string post() {
	bool pass = true;
	int frontRightValue = ports::frontRightDrive->get_position();
	int frontLeftValue = ports::frontLeftDrive->get_position();
	int backRightValue = ports::backRightDrive->get_position();
	int backLeftValue = ports::backLeftDrive->get_position();
	int intakeMotorLeftValue = ports::intakeMotorLeft->get_position();
	int intakeMotorRightValue = ports::intakeMotorRight->get_position();
	int indexerValue = ports::indexer->get_position();
	int flywheelValue = ports::flywheel->get_position();
	ports::frontRightDrive->move(8);
	ports::frontLeftDrive->move(8);
	ports::backRightDrive->move(8);
	ports::backLeftDrive->move(8);
	ports::intakeMotorLeft->move(8);
	ports::intakeMotorRight->move(8);
	ports::indexer->move(25);
	ports::flywheel->move(8);
	int timer = 0;
	while (util::abs(frontRightValue - ports::frontRightDrive->get_position()) < 3 || frontRightValue > 5000) {
		timer++;
		if (timer == 20)
			ports::frontRightDrive->move(-8);
		else if (timer == 40)
			return "frontRightDrive";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(frontLeftValue - ports::frontLeftDrive->get_position()) < 3 || frontLeftValue > 5000) {
		timer++;
		if (timer == 20)
			ports::frontLeftDrive->move(-8);
		else if (timer == 40)
			return "frontLeftDrive";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(backRightValue - ports::backRightDrive->get_position()) < 3 || backRightValue > 5000) {
		timer++;
		if (timer == 20)
			ports::backRightDrive->move(-8);
		else if (timer == 40)
			return "backRightDrive";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(backLeftValue - ports::backLeftDrive->get_position()) < 3 || backLeftValue > 5000) {
		timer++;
		if (timer == 20)
			ports::backLeftDrive->move(-8);
		else if (timer == 40)
			return "backLeftDrive";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(intakeMotorRightValue - ports::intakeMotorRight->get_position()) < 3 || intakeMotorRightValue > 5000) {
		timer++;
		if (timer == 20)
			ports::intakeMotorRight->move(-8);
		else if (timer == 40)
			return "intakeMotorRight";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(intakeMotorLeftValue - ports::intakeMotorLeft->get_position()) < 3 || intakeMotorLeftValue > 5000) {
		timer++;
		if (timer == 20)
			ports::intakeMotorLeft->move(-8);
		else if (timer == 40)
			return "intakeMotorLeft";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(indexerValue - ports::indexer->get_position()) < 3 || indexerValue > 5000) {
		timer++;
		if (timer == 35)
			ports::indexer->move(-25);
		else if (timer == 75)
			return "indexer";
		pros::delay(15);
	}
	timer = 0;
	while (util::abs(flywheelValue - ports::flywheel->get_position()) > 5) {
		timer++;
		if (timer == 20)
			ports::flywheel->move(-8);
		else if (timer == 40)
			return "flywheel";
		pros::delay(15);
	}
	if (ports::gyro->getHeading() > 5000)
		return "inertial";
	return "Success";
}

void initialize() {
	// Initialize the LLEMU
	LCD::initializeLLEMU(ports::controllerMain, ports::controllerPartner);

	// Initializes the gyro with calibration
	LCD::setStatus("IMU Calibration");
	LCD::setText(2, "Do not touch the robot");

	ports::imu->reset();

	pros::delay(2250);
	LCD::setText(2, "");

	LCD::setStatus("Initilizing motors");
	// Brake the intake motors
	ports::intakeMotorLeft->set_brake_mode(BRAKE_BRAKE);
	ports::intakeMotorRight->set_brake_mode(BRAKE_BRAKE);
	// Brake the indexer
	// ports::indexer->set_brake_mode(BRAKE_BRAKE);

	// Brake the drive motors
	ports::frontLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::frontRightDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backLeftDrive->set_brake_mode(BRAKE_BRAKE);
	ports::backRightDrive->set_brake_mode(BRAKE_BRAKE);

	LCD::setStatus("Initializing PID");
	// Set the PID configuration
	ports::pid->setVelocityGyro(ports::gyro);
	ports::pid->setPowerLimits(110, 30);
	ports::pid->setMovePosPID(0.2078, 0.01345, 0.992);
	ports::pid->setMoveVelPID(4.35, 0.000, 1.38);
	ports::pid->setPivotPID(1.5, 0.01785, 6.32);

	ports::pid->setStrafePosPID(39, 0.775, 0.000, 0.05);
	ports::pid->setStrafeVelPID(7, 0.000, 0);
	ports::pid->setForwardAcceleration(1.031, 9, 75);
	ports::pid->setBackwardAcceleration(1.02, 8, 100);

	ports::pid->setNoStopDebug(false);
	ports::pid->setLoggingDebug(false);

	LCD::setStatus("Initializing: Tasks");
	// Start gyroscope tracking
	ports::gyroTask = new pros::Task(gyroTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Gyro");
	// Start message debugging if the debugger is attached
	ports::mhTask = new pros::Task(mhTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Message Handler");
	postPass = true;
	return;
	LCD::setStatus("Power on self-test...");
	std::string postResult = post();
	ports::frontRightDrive->move(0);
	ports::frontLeftDrive->move(0);
	ports::backRightDrive->move(0);
	ports::backLeftDrive->move(0);
	ports::intakeMotorLeft->move(0);
	ports::intakeMotorRight->move(0);
	ports::indexer->move(0);
	ports::flywheel->move(0);
	if (postResult == "Success")
		LCD::setStatus("Initialization complete");
	else {
		LCD::setStatus("POST FAIL - " + postResult);
		postPass = false;
		::postResult = postResult;
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	LCD::setStatus("Disabled");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	LCD::setStatus("Competition detected");
	
	// Run the autonomous selector
	while (true) {
		// Prints debug information to the LCD
		LCD::printDebugInformation();

		// Maps the left and right buttons on the controller to the left and right buttons on the Brain LCD
		if (ports::controllerMain->get_digital_new_press(BUTTON_LEFT)) LCD::onLeftButton();
		if (ports::controllerMain->get_digital_new_press(BUTTON_RIGHT)) LCD::onRightButton();

		// Update the LCD screen
		LCD::updateScreen();

		// Run every 20 ms
		pros::delay(20);
	}
}
