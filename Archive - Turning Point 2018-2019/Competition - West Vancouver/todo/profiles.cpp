#include "main.h"

using namespace ports;

int Profiles::selectedProfile = 1;
std::vector<std::pair<std::string, profileFunction>> Profiles::profiles;
volatile bool Profiles::unchangedProfile = false;

void profile1() {
  liftMotor->set_brake_mode(BRAKE_BRAKE);

  bool controllerDC = false;

	while (Profiles::unchangedProfile) {

    if(controllerMain->get_digital(BUTTON_X))
    {
      autonomous();
      LCD::setStatus("Returning to Operator Control");
    }

    drive->run(controllerMain->get_analog(STICK_LEFT_Y), controllerMain->get_analog(STICK_LEFT_X), false, false, true);

    if (controllerMain->get_digital(BUTTON_R2)) {
      frontLauncherMotor->move(-127);
      backLauncherMotor->move(-127);
      intakeMotor->move(-127);
    } else {
  		if (controllerMain->get_digital(BUTTON_R1))
        intakeMotor->move(127);
      else
        intakeMotor->move(0);

      if (controllerMain->get_digital(BUTTON_L2)) {
        frontLauncherMotor->move(127);
        backLauncherMotor->move(127);
      } else if (controllerMain->get_digital(BUTTON_L1)) {
        frontLauncherMotor->move(110);
        backLauncherMotor->move(110);
      } else {
        frontLauncherMotor->move(0);
        backLauncherMotor->move(0);
      }
    }
    liftMotor->move(controllerMain->get_analog(ANALOG_RIGHT_Y));

    if (!controllerMain->is_connected() && !controllerDC) {
      LCD::setStatus("Operator Controller Disconnected");
      controllerDC = true;
    } else if (controllerMain->is_connected() && controllerDC) {
      LCD::setStatus("Operator Controller Reconnected");
      controllerDC = false;
    }

    LCD::setText(3, "Left avg: " + std::to_string((frontLeftDrive->get_position() + backLeftDrive->get_position()) / 2));
    LCD::setText(4, "Right avg: " + std::to_string((frontRightDrive->get_position() + backRightDrive->get_position()) / 2));

    /* pid dynamic
    if (controllerMain->get_digital(BUTTON_B)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifykd(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifykd(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifykd(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifykd(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_X)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifykp(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifykp(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifykp(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifykp(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_Y)) {
      if (controllerMain->get_digital(BUTTON_UP))
        driveControl->getPID().modifyki(0.01);
      if (controllerMain->get_digital(BUTTON_DOWN))
        driveControl->getPID().modifyki(-0.01);
      if (controllerMain->get_digital(BUTTON_LEFT))
        driveControl->getPID().modifyki(0.1);
      if (controllerMain->get_digital(BUTTON_RIGHT))
        driveControl->getPID().modifyki(-0.1);
    }

    if (controllerMain->get_digital(BUTTON_A)) {
      driveControl->moveRelative(0, 1000, 10, true, true);
    }

    pros::lcd::set_text(1, "kp = " + std::to_string(driveControl->getPID().getkp()));
    pros::lcd::set_text(2, "ki = " + std::to_string(driveControl->getPID().getki()));
    pros::lcd::set_text(3, "kd = " + std::to_string(driveControl->getPID().getkd()));
    */

    // flywheel dynamic
    //if (controllerMain->get_digital(BUTTON_UP)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity + 1);
    //if (controllerMain->get_digital(BUTTON_DOWN)) currentFlywheelVelocity = util::limit127 (currentFlywheelVelocity - 1);

    /* Overheat lcd
    std::string hiheat = "";
    std::string overheat = "";

    if (backLeftDrive->get_temperature() > 55) overheat += "bldrive ";
    else if (backLeftDrive->get_temperature() > 45) hiheat += "bldrive ";

    if (backRightDrive->get_temperature() > 55) overheat += "brdrive ";
    else if (backLeftDrive->get_temperature() > 45) hiheat += "brdrive ";

    if (frontLeftDrive->get_temperature() > 55) overheat += "fldrive ";
    else if (frontLeftDrive->get_temperature() > 45) hiheat += "fldrive ";

    if (frontRightDrive->get_temperature() > 55) overheat += "frdrive ";
    else if (frontRightDrive->get_temperature() > 45) hiheat += "frdrive ";

    if (liftMotor->get_temperature() > 55) overheat += "lift ";
    else if (liftMotor->get_temperature() > 45) hiheat += "lift ";

    if (intakeMotor->get_temperature() > 55) overheat += "intake ";
    else if (intakeMotor->get_temperature() > 45) hiheat += "intake ";

    if (frontLauncherMotor->get_temperature() > 55) overheat += "frontfw ";
    else if (frontLauncherMotor->get_temperature() > 45) hiheat += "frontfw ";

    if (backLauncherMotor->get_temperature() > 55) overheat += "backfw ";
    else if (backLauncherMotor->get_temperature() > 45) hiheat += "backfw ";

    if (overheat != "") pros::lcd::set_text(5, overheat + " ovrht");
    if (hiheat != "") pros::lcd::set_text(6, hiheat + " hiheat");
    */
    if (controllerMain->get_digital(BUTTON_LEFT)) Profiles::prev();
    if (controllerMain->get_digital(BUTTON_RIGHT)) Profiles::next();


		pros::delay(20);
	}
}

void Profiles::empty() {
  while (Profiles::unchangedProfile) {
    if (controllerMain->get_digital(BUTTON_LEFT)) Profiles::prev();
    if (controllerMain->get_digital(BUTTON_RIGHT)) Profiles::next();
    pros::delay(20);
  }
}

void Profiles::run() {
  while (true) {
    while (Profiles::profiles.size() == 0) {
      LCD::setText(5, "size 0, waiting");
      pros::delay(20);
    }
    Profiles::unchangedProfile = true;
    void (*call)(void) = Profiles::profiles.at(Profiles::selectedProfile).second;
    LCD::setText(5, "starting call");
    (*call)();
    pros::delay(20);
  }
}

void Profiles::registerAll() {
  pros::delay(2000);
  LCD::setText(3, "Registering disabled");
  Profiles::registerProfile("Disabled", &Profiles::empty);
  pros::delay(2000);
  LCD::setText(3, "Registering p1");
  Profiles::registerProfile("Profile 1", &profile1);
}

int Profiles::registerProfile(std::string name, profileFunction function) {
  std::pair<std::string, profileFunction> pair(name, function);
  int index = Profiles::profiles.size();
  Profiles::profiles.push_back(pair);
  return index;
}

void Profiles::next() {
  Profiles::selectedProfile++;
  if (Profiles::selectedProfile >= Profiles::profiles.size())
    Profiles::selectedProfile = 0;
  Profiles::unchangedProfile = false;
  controllerMain->set_text(0, 0, ("Profile: " + Profiles::getSelectedName()).c_str());
}

void Profiles::prev() {
  Profiles::selectedProfile--;
  if (Profiles::selectedProfile < 0)
    Profiles::selectedProfile = (Profiles::profiles.size() - 1);
  Profiles::unchangedProfile = false;
  controllerMain->set_text(0, 0, ("Profile: " + Profiles::getSelectedName()).c_str());
}

void Profiles::select(int index) {
  Profiles::selectedProfile = index;
  if (Profiles::selectedProfile < 0)
    Profiles::selectedProfile = (Profiles::profiles.size() - 1);
  if (Profiles::selectedProfile >= Profiles::profiles.size())
    Profiles::selectedProfile = 0;
  Profiles::unchangedProfile = false;
}

std::string Profiles::getSelectedName() {
  return Profiles::getProfileName(Profiles::selectedProfile);
}

std::string Profiles::getProfileName(int index) {
  return Profiles::profiles.at(Profiles::selectedProfile).first;
}
