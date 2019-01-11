#include "main.h"
#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <list>
#include <string>
#include <vector>

// Uses some elements of this namespace. Dump it into the global namespace for ease of programming
using namespace ports;

// The static initializations of private Debugger fields. See the header file for documentation
bool Debugger::stopped = true;
pros::Task * Debugger::task = NULL;

std::string Debugger::currentCommand;
std::string Debugger::pending;
std::vector<std::string> Debugger::history;
int Debugger::activeIndex;

unsigned int Debugger::callCode;
std::string Debugger::parameter1;
std::string Debugger::parameter2;
std::string Debugger::parameter3;
std::string Debugger::completion;

void Debugger::start() {
  // If the debugger is stopped, start the debugger
  if (Debugger::stopped) {
    Debugger::task = new pros::Task(Debugger::_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "Debugger");
    Debugger::stopped = false;
  }
}

void Debugger::run() {
  // If the debugger is not running, ignore
  if (Debugger::stopped) return;
  // If no call is waiting, continue
  if (Debugger::callCode == 0x00) return;

  try { // Catch all exceptions, should never error here
    // Call codes starting from 0x01 to 0x0f refer to general robot commands
    if (Debugger::callCode == 0x01) // Run autonomous
      autonomous();

    // Call codes starting from 0x10 to 0x1f refer to drive
    if (Debugger::callCode == 0x10)  // DriveFunction::move
      ports::drive->move(std::stoi(parameter1));
    else if (Debugger::callCode == 0x11)  // DriveFunction::moveDegrees
      ports::drive->moveDegrees(std::stoi(parameter1));
    else if (Debugger::callCode == 0x12)  // DriveFunction::pivot
      ports::drive->pivot(std::stoi(parameter1));
    else if (Debugger::callCode == 0x13)  // DriveFunction::turn
      ports::drive->turn(std::stoi(parameter1));

  } catch (std::exception e) {}

  // Reset the call code and parameters
  Debugger::callCode = 0x00;
  Debugger::parameter1 = "";
  Debugger::parameter2 = "";
  Debugger::parameter3 = "";
  Logger::log(LOG_INFO, completion);
}

void Debugger::stop() {
  // If the debugger is not stopped, erase the debugger task from memory as well as from the PROS RTOS API
  if (!Debugger::stopped) {
    Debugger::task->remove();
    delete Debugger::task;
    Debugger::task = NULL;
    Debugger::stopped = false;
  }
}

void Debugger::_task(void * param) {
  // Serial input is encoded with UTF-8
  while (true) {

    unsigned char c = std::cin.get();

    if (c == 0xc3) {
      // Wait for continuation byte
      unsigned char c0 = std::cin.get();
      std::cout << "\b \b";
      if (c0 != 0xa0)
        continue;

      // Get next character
      unsigned char c1 = std::cin.get();
      std::cout << "\b \b";

      // Save the current command, if necessary
      if (activeIndex == -1)
        pending = currentCommand;

      int size = 0;
      if (c1 == 0x48) { // Arrow Up was pressed
        activeIndex++;
        size = currentCommand.size();
        if (activeIndex >= history.size()) activeIndex = history.size() - 1;
        currentCommand = history.at(history.size() - 1 - activeIndex);
      } else if (c1 == 0x50) { // Arrow Down was pressed
        activeIndex--;
        if (activeIndex < -1) activeIndex = -1;
        size = currentCommand.size();
        if (activeIndex == -1)
          currentCommand = pending;
        else
          currentCommand = history.at(history.size() - 1 - activeIndex);
      }
      // Home, end, insert or similar key was pressed.
      // Ignore these inputs

      // Erase the line
      for (int i = 0; i < (size + 5); i++)
        std::cout << "\b";
      for (int i = 0; i < (size + 3); i++)
        std::cout << " ";
      for (int i = 0; i < (size + 5); i++)
        std::cout << "\b";

      std::cout << currentCommand;
    } else if (isalnum(c) || c == '.' || c == ' ' || c == '_' || c == '-') { // Acceptable characters: alphanumeric, '.', ' ', '_' or '-'
      currentCommand += c;
    } else if (c == '\b') { // Backspace was pressed
      std::cout << " \b"; // PROS terminal doesn't parse /b properly, do it manually
      currentCommand.erase(currentCommand.end() - 1);
    } else if (c == 0x7f) { // CTRL+Backspace was pressed
      // Erase until "." or " "
      std::cout << "\b \b"; // PROS terminal doesn't parse 0x7f (CTRL+Backspace), do it manually
      int amt = 0;

      while (currentCommand.size() != 0 && (currentCommand.at(currentCommand.size() - 1) != '.' && currentCommand.at(currentCommand.size() - 1) != ' ')) {
        currentCommand.erase(currentCommand.end() - 1);
        amt++;
      }
      for (int i = 0; i < amt; i++)
        std::cout << "\b";
      for (int i = 0; i < amt; i++)
        std::cout << " ";
      for (int i = 0; i < amt; i++)
        std::cout << "\b";
    } else if (c == 0x1b) { // Escape was pressed
      // Erase the line
      for (int i = 0; i < (currentCommand.size() + 5); i++)
        std::cout << "\b";
      for (int i = 0; i < (currentCommand.size() + 3); i++)
        std::cout << " ";
      for (int i = 0; i < (currentCommand.size() + 5); i++)
        std::cout << "\b";
      currentCommand = "";
    } else if (c == '\n') {
      // Log the command to the log file
      Logger::log(LOG_INFO, "Command: " + currentCommand);

      // Execute the command
      for (std::string s : Debugger::command(currentCommand))
        Logger::log(LOG_INFO, s);

      // Reset the state of control variables and push the command to the history
      history.push_back(currentCommand);
      currentCommand = "";
      pending = "";
      activeIndex = -1;
    } else
      std::cout << "\b \b";
  }
}

std::vector<std::string> Debugger::command(std::string command) {
  // When running a command, set the priority higher than operator control so it does not interrupt
  task->set_priority(TASK_PRIORITY_DEFAULT + 1);

  // The list of messages to return to the serial input
  std::vector<std::string> ret;

  // Trim the command
  command.erase(std::find_if(command.rbegin(), command.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), command.end());
  command.erase(command.begin(), std::find_if(command.begin(), command.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

  // Handle the command
  if (command.rfind("echo ", 0) == 0) {
    ret.push_back(command.substr(5));
  } else if (command.rfind("hel", 0) == 0) {
    ret.push_back("This is the serial debugging terminal for the VEX v5 Brain.");
    ret.push_back("This can be used for controlling behavior on the brain as well as retrieving information about connected devices.");
    ret.push_back("Some commands that are available include \"echo\", \"battery\", \"motor\", \"freeze\".");
    ret.push_back("More may be added in the future.");
  } else if (command.rfind("freeze", 0) == 0) {

    if (runOperatorControlLoop) {
      runOperatorControlLoop = false;
      ret.push_back("Operator Control Task paused");
      LCD::setStatus("Paused");
    } else
      ret.push_back("Operator Control Task is already paused");
  } else if (command.rfind("unfreeze", 0) == 0) {
    if (!runOperatorControlLoop) {
      runOperatorControlLoop = true;
      ret.push_back("Operator Control Task resumed");
      LCD::setStatus("Operator Control");
    } else
      ret.push_back("Operator Control Task is already running");

  } else if (command.rfind("motor", 0) == 0) {
    command = command.substr(5);

    bool invalidport = false;;
    std::string portstr;
    int port;

    if (command.size() == 0) {
      std::string found;
      for (int i = 1; i <= 21; i++)
        if (pros::Motor(i).get_efficiency() < 1000)
          found = found + std::to_string(i) + " ";
      ret.push_back("Motors found on ports: " + found);
      ret.push_back("Please specify a port to retrive motor information");
      goto end;
    } else if (command.rfind(".sto", 0) == 0) {
      for (int i = 1; i <= 21; i++)
        pros::Motor(i).move(0);
      ret.push_back("Stopped all motors");
      goto end;
    }

    portstr = command.substr(0, command.find("."));
    command = command.substr(portstr.size());

    try {
      port = std::stoi(portstr);
    } catch (std::exception & e) {
      invalidport = true;
    }
    if (invalidport || port > 21){
      ret.push_back("\"" + portstr + "\" is not a valid port number!");
      goto end;
    }

    pros::Motor motor (port, GEARSET_200, FWD, ENCODER_DEGREES);

    if (motor.get_efficiency() > 1000) {
      ret.push_back("Motor at Port " + portstr + ": Not Connected");
      goto end;
    }

    std::string direction = ((motor.get_direction() == 0) ? "Stopped" : ((motor.get_direction() == 1) ? "Forward" : "Backward"));
    std::string brake = ((motor.get_brake_mode() == BRAKE_COAST) ? "Coasting" : ((motor.get_brake_mode() == BRAKE_BRAKE) ? "Braking" : "Holding"));

    if (command.size() == 0) {
      ret.push_back("Motor at Port " + portstr + ":");
      ret.push_back("    Move Command: " + std::to_string(std::lround(motor.get_voltage() / 12000.0 * 127)));
      ret.push_back("    Direction: " + direction);
      ret.push_back("    Position: " + std::to_string(motor.get_position()) + " degrees");
      ret.push_back("    Velocity:");
      ret.push_back("        Turbo Gearing: " + std::to_string(motor.get_actual_velocity() * 3.0) + " RPM");
      ret.push_back("        High Speed Gearing: " + std::to_string(motor.get_actual_velocity()) + " RPM");
      ret.push_back("        Torque Gearing: " + std::to_string(motor.get_actual_velocity() / 2.0) + " RPM");
      ret.push_back("    Temperature: " + std::to_string(motor.get_temperature()) + " ºC" + ((motor.get_temperature() >= 55) ? " (Overheating)" : ""));
      ret.push_back("    Voltage: " + std::to_string(motor.get_voltage() / 1000.0) + " V");
      ret.push_back("    Current: " + std::to_string(motor.get_current_draw() / 1000.0) + " A");
      ret.push_back("    Power: " + std::to_string(motor.get_power()) + " W");
      ret.push_back("    Torque: " + std::to_string(motor.get_torque()) + " Nm");
      ret.push_back("    Efficiency: " + std::to_string(motor.get_efficiency()) + "%");
      ret.push_back("    Brake Mode: " + brake);
    } else if (command.rfind(".com", 0) == 0)
      ret.push_back("Move Command of Motor at Port " + portstr + ": " + std::to_string(std::lround(motor.get_voltage() / 12000.0 * 127)));
    else if (command.rfind(".loc" , 0) == 0)
      ret.push_back("Position of Motor at Port " + portstr + ": " + std::to_string(motor.get_position()) + " degrees");
    else if (command.rfind(".dir" , 0) == 0)
      ret.push_back("Direction of Motor at Port " + portstr + ": " + direction);
    else if (command.rfind(".vel" , 0) == 0) {
      ret.push_back("Velocity of Motor at Port " + portstr + ": ");
      ret.push_back("    Turbo Gearing: " + std::to_string(motor.get_actual_velocity() * 3.0) + " RPM");
      ret.push_back("    High Speed Gearing: " + std::to_string(motor.get_actual_velocity()) + " RPM");
      ret.push_back("    Torque Gearing: " + std::to_string(motor.get_actual_velocity() / 2.0) + " RPM");
    } else if (command.rfind(".tem" , 0) == 0)
      ret.push_back("Temperature of Motor at Port " + portstr + ": " + std::to_string(motor.get_temperature()) + " ºC" + ((motor.get_temperature() >= 55) ? " (Overheating)" : ""));
    else if (command.rfind(".vol" , 0) == 0)
      ret.push_back("Voltage of Motor at Port " + portstr + ": " + std::to_string(motor.get_voltage() / 1000.0) + " V");
    else if (command.rfind(".cur" , 0) == 0)
      ret.push_back("Current of Motor at Port " + portstr + ": " + std::to_string(motor.get_current_draw() / 1000.0) + " A");
    else if (command.rfind(".pw" , 0) == 0)
      ret.push_back("Power of Motor at Port " + portstr + ": " + std::to_string(motor.get_power()) + " W");
    else if (command.rfind(".tor" , 0) == 0)
      ret.push_back("Torque of Motor at Port " + portstr + ": " + std::to_string(motor.get_torque()) + " Nm");
    else if (command.rfind(".eff" , 0) == 0)
      ret.push_back("Efficiency of Motor at Port " + portstr + ": " + std::to_string(motor.get_efficiency()) + "%");
    else if (command.rfind(".bra", 0) == 0)
      ret.push_back("Brake Mode of Motor at Port" + portstr + ": " + brake);
    else if (command.rfind(".move_absolute", 0) == 0) {
      command = command.substr(14);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);

      int degrees = 0;

      try {
        degrees = std::stoi(command);
      } catch (std::exception & e) {
        ret.push_back("\"" + command + "\" is not a valid target!");
        goto end;
      }

      if (runOperatorControlLoop) {
        ret.push_back("It is recommended that the operator control loop is paused when manually");
        ret.push_back("moving the motors because the loop may issue a command after the manual move.");
        ret.push_back("Use \"freeze\" to pause the operator control loop.");
      }

      motor.move_absolute(degrees, 127);
      ret.push_back("Move absolute command issued to the motor on port " + portstr);
    } else if (command.rfind(".move_relative", 0) == 0) {
      command = command.substr(14);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);

      int degrees = 0;

      try {
        degrees = std::stoi(command);
      } catch (std::exception & e) {
        ret.push_back("\"" + command + "\" is not a valid target!");
        goto end;
      }

      if (runOperatorControlLoop) {
        ret.push_back("It is recommended that the operator control loop is paused when manually");
        ret.push_back("moving the motors because the loop may issue a command after the manual move.");
        ret.push_back("Use \"freeze\" to pause the operator control loop.");
      }

      motor.move_relative(degrees, 127);
      ret.push_back("Move relative command issued to the motor on port " + portstr);
    } else if (command.rfind(".move ", 0) == 0) {
      command = command.substr(5);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);

      int power = 0;
      bool invalidpower = false;

      try {
        power = std::stoi(command);
      } catch (std::exception & e) {
        invalidpower = true;
      }
      if (invalidpower || util::abs(power) > 127) {
        ret.push_back("\"" + command + "\" is not a valid power!");
        goto end;
      }

      if (runOperatorControlLoop) {
        ret.push_back("It is recommended that the operator control loop is paused when manually");
        ret.push_back("moving the motors because the loop may issue a command after the manual move.");
        ret.push_back("Use \"freeze\" to pause the operator control loop.");
      }

      std::string type = ((power == 0) ? "Stop" : "Move");

      motor.move(power);
      ret.push_back(type + " command issued to the motor on port " + portstr);
    } else if (command.rfind(".sto", 0) == 0) {
      if (runOperatorControlLoop) {
        ret.push_back("It is recommended that the operator control loop is paused when manually");
        ret.push_back("moving the motors because the loop may issue a command after the manual move.");
        ret.push_back("Use \"freeze\" to pause the operator control loop.");
      }
      motor.move(0);
      ret.push_back("Stop command issued to the motor on port " + portstr);
    } else if (command.rfind(".setbrake", 0) == 0) {
      command = command.substr(9);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);

      if (command.rfind("coast", 0) == 0) {
        motor.set_brake_mode(BRAKE_COAST);
        ret.push_back("Set brake mode of the motor on port " + portstr + " to COAST");
      } else if (command.rfind("brake", 0) == 0) {
        motor.set_brake_mode(BRAKE_BRAKE);
        ret.push_back("Set brake mode of the motor on port " + portstr + " to BRAKE");
      } else if (command.rfind("hold", 0) == 0) {
        motor.set_brake_mode(BRAKE_HOLD);
        ret.push_back("Set brake mode of the motor on port " + portstr + " to HOLD");
      } else
        ret.push_back("Unknown brake mode. Acceptable modes are \"coast\", \"brake\", and \"hold\"");
    } else if (command.rfind(".tare", 0) == 0) {
      motor.tare_position();
      ret.push_back("Set position motor at port " + portstr + " to 0");
    } else
      ret.push_back("Unknown subcommand in \"motor" + portstr + "\". Access fields to narrow information or issue commands");

  } else if (command.rfind("battery", 0) == 0) {
    command = command.substr(7);
    if (command.size() == 0) {
      ret.push_back("Battery Information:");
      ret.push_back("    Brain Battery:");
      ret.push_back("        Capacity: " + std::to_string(pros::battery::get_capacity()) + "%");
      ret.push_back("        Temperature: " + std::to_string(pros::battery::get_temperature()) + " ºC");
      ret.push_back("        Voltage: " + std::to_string(pros::battery::get_voltage() / 1000.0) + " V");
      ret.push_back("        Current: " + std::to_string(pros::battery::get_current() / 1000.0) + " A");
      if (controllerMain->is_connected()) {
        ret.push_back("    Main Controller Battery:");
        ret.push_back("        Capacity: " + std::to_string(controllerMain->get_battery_capacity()) + "%");
        ret.push_back("        Voltage: " + std::to_string(controllerMain->get_battery_level() / 1000.0) + " V");
      } else
        ret.push_back("    Main Controller Battery: Not Connected");
      if (controllerPartner->is_connected()) {
        ret.push_back("    Partner Controller Battery:");
        ret.push_back("        Capacity: " + std::to_string(controllerPartner->get_battery_capacity()) + "%");
        ret.push_back("        Voltage: " + std::to_string(controllerPartner->get_battery_level() / 1000.0) + " V");
      } else
        ret.push_back("    Partner Controller Battery: Not Connected");

    } else if (command.rfind(".brain", 0) == 0) {

      command = command.substr(6);
      if (command.size() == 0) {
        ret.push_back("Brain Battery:");
        ret.push_back("    Capacity: " + std::to_string(pros::battery::get_capacity()) + "%");
        ret.push_back("    Temperature: " + std::to_string(pros::battery::get_temperature()) + " ºC");
        ret.push_back("    Voltage: " + std::to_string(pros::battery::get_voltage() / 1000.0) + " V");
        ret.push_back("    Current: " + std::to_string(pros::battery::get_current() / 1000.0) + " A");
      } else if (command.rfind(".ca", 0) == 0)
        ret.push_back("Brain Battery Capacity: " + std::to_string(pros::battery::get_capacity()) + "%");
      else if (command.rfind(".t", 0) == 0)
        ret.push_back("Brain Battery Temperature: " + std::to_string(pros::battery::get_temperature()) + " ºC");
      else if (command.rfind(".v", 0) == 0)
        ret.push_back("Brain Battery Voltage: " + std::to_string(pros::battery::get_voltage() / 1000.0) + " V");
      else if (command.rfind(".cu", 0) == 0)
        ret.push_back("Brain Battery Current: " + std::to_string(pros::battery::get_current() / 1000.0) + " A");
      else
        ret.push_back("Unknown command in \"battery.brain\". Access fields \"capacity\" or \"voltage\" to narrow information");

    } else if (command.rfind(".cmain", 0) == 0) {

      if (!controllerMain->is_connected()) {
        ret.push_back("Main Controller Battery: Not Connected");
      } else {
        command = command.substr(6);
        if (command.size() == 0) {
          ret.push_back("Main Controller Battery:");
          ret.push_back("    Capacity: " + std::to_string(controllerMain->get_battery_capacity()) + "%");
          ret.push_back("    Voltage: " + std::to_string(controllerMain->get_battery_level() / 1000.0) + " V");
        } else if (command.compare(".c") == 0)
          ret.push_back("Main Controller Capacity: " + std::to_string(controllerMain->get_battery_capacity()) + "%");
        else if (command.compare(".v") == 0)
          ret.push_back("Main Controller Voltage: " + std::to_string(controllerMain->get_battery_level() / 1000.0) + " V");
        else
          ret.push_back("Unknown command in \"battery.cmain\". Access fields \"capacity\" or \"voltage\" to narrow information");
      }

    } else if (command.rfind(".csecond", 0) == 0 || command.rfind(".cprtner", 0) == 0) {

      if (!controllerPartner->is_connected()) {
        ret.push_back("Partner Controller Battery: Not Connected");
      } else {
        command = command.substr(8);
        if (command.size() == 0) {
          ret.push_back("Partner Controller Battery:");
          ret.push_back("    Capacity: " + std::to_string(controllerPartner->get_battery_capacity()) + "%");
          ret.push_back("    Voltage: " + std::to_string(controllerPartner->get_battery_level() / 1000.0) + " V");
        } else if (command.compare(".c") == 0)
          ret.push_back("Partner Controller Capacity: " + std::to_string(controllerPartner->get_battery_capacity()) + "%");
        else if (command.compare(".v") == 0)
          ret.push_back("Partner Controller Voltage: " + std::to_string(controllerPartner->get_battery_level() / 1000.0) + " V");
        else
          ret.push_back("Unknown command in \"battery.csecond\". Access fields \"capacity\" or \"voltage\" to narrow information");
      }

    } else
      ret.push_back("Unknown subcommand in \"battery\". Access fields \"brain\" or \"cmain\" to narrow information");

  } else if (command.rfind("controller", 0) == 0) {
    command = command.substr(10);

    pros::Controller main (CONTROLLER_MAIN);
    pros::Controller partner (CONTROLLER_PARTNER);

    bool displaymain = true;
    bool displaypartner = true;

    if (command.size() != 0) {
      if (command.rfind(".main", 0) == 0) {
        command = command.substr(5);
        if (command.size() == 0 || !main.is_connected()) {
          displaypartner = false;
          goto display;
        } else if (command.rfind(".analog", 0) == 0) {
          command = command.substr(7);
          bool left = true;
          bool right = true;
          if (command.size() != 0) {
            if (command.rfind(".left", 0) == 0)
              right = false;
            else if (command.rfind(".right", 0) == 0)
              left = false;
            else {
                ret.push_back("Unkown subcommand in \"controller.main.analog\". Use \"right\" or \"left\" to narrow information");
                goto end;
            }
          }

          if (left) {
            int qc = 0;
            std::string lsq;
            if (main.get_analog(STICK_LEFT_Y) > 0) {
              lsq += "Upper ";
              qc++;
            } else if (main.get_analog(STICK_LEFT_Y) < 0) {
              lsq += "Lower ";
              qc++;
            }
            if (main.get_analog(STICK_LEFT_X) > 0) {
              lsq += "Right ";
              qc++;
            } else if (main.get_analog(STICK_LEFT_X) < 0) {
              lsq += "Left ";
              qc++;
            }
            switch (qc) {
              case 2:
                lsq += "Quadrant";
                break;
              case 1:
                lsq += "Half";
                break;
              case 0:
                lsq += "Center";
            };

            ret.push_back("Main Controller Left Analog: " + std::to_string(main.get_analog(STICK_LEFT_X)) + ", " + std::to_string(main.get_analog(STICK_LEFT_Y)) + " (" + lsq + ")");
          }
          if (right) {
            int qc = 0;
            std::string rsq;
            if (main.get_analog(STICK_RIGHT_Y) > 0) {
              rsq += "Upper ";
              qc++;
            } else if (main.get_analog(STICK_RIGHT_Y) < 0) {
              rsq += "Lower ";
              qc++;
            }
            if (main.get_analog(STICK_RIGHT_X) > 0) {
              rsq += "Right ";
              qc++;
            } else if (main.get_analog(STICK_RIGHT_X) < 0) {
              rsq += "Left ";
              qc++;
            }
            switch (qc) {
              case 2:
                rsq += "Quadrant";
                break;
              case 1:
                rsq += "Half";
                break;
              case 0:
                rsq += "Center";
            };

            ret.push_back("Main Controller Right Analog: " + std::to_string(main.get_analog(STICK_RIGHT_X)) + ", " + std::to_string(main.get_analog(STICK_RIGHT_Y)) + " (" + rsq + ")");
          }
          goto end;
        } else if (command.rfind(".digital", 0) == 0) {
          command = command.substr(8);

          if (command.size() == 0) {
            bool one = false;
            ret.push_back("Main Controller Pressed Buttons:");
            if (main.get_digital(BUTTON_A)) {
              one = true;
              ret.push_back("  - A");
            }
            if (main.get_digital(BUTTON_B)) {
              one = true;
              ret.push_back("  - B");
            }
            if (main.get_digital(BUTTON_X)) {
              one = true;
              ret.push_back("  - X");
            }
            if (main.get_digital(BUTTON_Y)) {
              one = true;
              ret.push_back("  - Y");
            }
            if (main.get_digital(BUTTON_UP)) {
              one = true;
              ret.push_back("  - UP");
            }
            if (main.get_digital(BUTTON_DOWN)) {
              one = true;
              ret.push_back("  - DOWN");
            }
            if (main.get_digital(BUTTON_LEFT)) {
              one = true;
              ret.push_back("  - LEFT");
            }
            if (main.get_digital(BUTTON_RIGHT)) {
              one = true;
              ret.push_back("  - RIGHT");
            }
            if (main.get_digital(BUTTON_L1)) {
              one = true;
              ret.push_back("  - L1");
            }
            if (main.get_digital(BUTTON_L2)) {
              one = true;
              ret.push_back("  - L2");
            }
            if (main.get_digital(BUTTON_R1)) {
              one = true;
              ret.push_back("  - R1");
            }
            if (main.get_digital(BUTTON_R2)) {
              one = true;
              ret.push_back("  - R2");
            }
            if (!one)
              ret.push_back("  - None");
            goto end;
          } else if (command.rfind(".a", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_A) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button A: " + pressed);
            goto end;
          } else if (command.rfind(".b", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_B) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button B: " + pressed);
            goto end;
          } else if (command.rfind(".x", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_X) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button X: " + pressed);
            goto end;
          } else if (command.rfind(".y", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_Y) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button Y: " + pressed);
            goto end;
          } else if (command.rfind(".up", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_UP) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button UP: " + pressed);
            goto end;
          } else if (command.rfind(".down", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_DOWN) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button DOWN: " + pressed);
            goto end;
          } else if (command.rfind(".left", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_LEFT) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button LEFT: " + pressed);
            goto end;
          } else if (command.rfind(".right", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_RIGHT) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button RIGHT: " + pressed);
            goto end;
          } else if (command.rfind(".l1", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_L1) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button L1: " + pressed);
            goto end;
          } else if (command.rfind(".l2", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_L2) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button K2: " + pressed);
            goto end;
          } else if (command.rfind(".r1", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_R1) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button R1: " + pressed);
            goto end;
          } else if (command.rfind(".r2", 0) == 0) {
            std::string pressed = (main.get_digital(BUTTON_R2) ? "pressed" : "not pressed");
            ret.push_back("Main Controller Button R2: " + pressed);
            goto end;
          } else {
            ret.push_back("Unknown subcommand in \"controller.main.digital\". Specify a button to narrow information");
            goto end;
          }
        } else {
          ret.push_back("Unknown subcommand in \"controller.main\". Use \"analog\" or \"digital\" to narrow information");
          goto end;
        }
      } else if (command.rfind(".second", 0) == 0 || command.rfind(".prtner", 0) == 0) {
        command = command.substr(7);
        if (command.size() == 0 || !partner.is_connected()) {
          displaypartner = false;
          goto display;
        } else if (command.rfind(".analog", 0) == 0) {
          command = command.substr(7);
          bool left = true;
          bool right = true;
          if (command.size() != 0) {
            if (command.rfind(".left", 0) == 0)
              right = false;
            else if (command.rfind(".right", 0) == 0)
              left = false;
            else {
                ret.push_back("Unkown subcommand in \"controller.second.analog\". Use \"right\" or \"left\" to narrow information");
                goto end;
            }
          }

          if (left) {
            int qc = 0;
            std::string lsq;
            if (partner.get_analog(STICK_LEFT_Y) > 0) {
              lsq += "Upper ";
              qc++;
            } else if (partner.get_analog(STICK_LEFT_Y) < 0) {
              lsq += "Lower ";
              qc++;
            }
            if (partner.get_analog(STICK_LEFT_X) > 0) {
              lsq += "Right ";
              qc++;
            } else if (partner.get_analog(STICK_LEFT_X) < 0) {
              lsq += "Left ";
              qc++;
            }
            switch (qc) {
              case 2:
                lsq += "Quadrant";
                break;
              case 1:
                lsq += "Half";
                break;
              case 0:
                lsq += "Center";
            };

            ret.push_back("Partner Controller Left Analog: " + std::to_string(partner.get_analog(STICK_LEFT_X)) + ", " + std::to_string(partner.get_analog(STICK_LEFT_Y)) + " (" + lsq + ")");
          }
          if (right) {
            int qc = 0;
            std::string rsq;
            if (partner.get_analog(STICK_RIGHT_Y) > 0) {
              rsq += "Upper ";
              qc++;
            } else if (partner.get_analog(STICK_RIGHT_Y) < 0) {
              rsq += "Lower ";
              qc++;
            }
            if (partner.get_analog(STICK_RIGHT_X) > 0) {
              rsq += "Right ";
              qc++;
            } else if (partner.get_analog(STICK_RIGHT_X) < 0) {
              rsq += "Left ";
              qc++;
            }
            switch (qc) {
              case 2:
                rsq += "Quadrant";
                break;
              case 1:
                rsq += "Half";
                break;
              case 0:
                rsq += "Center";
            };

            ret.push_back("Partner Controller Right Analog: " + std::to_string(partner.get_analog(STICK_RIGHT_X)) + ", " + std::to_string(partner.get_analog(STICK_RIGHT_Y)) + " (" + rsq + ")");
          }
          goto end;
        } else if (command.rfind(".digital", 0) == 0) {
          command = command.substr(8);

          if (command.size() == 0) {
            bool one = false;
            ret.push_back("Partner Controller Pressed Buttons:");
            if (partner.get_digital(BUTTON_A)) {
              one = true;
              ret.push_back("  - A");
            }
            if (partner.get_digital(BUTTON_B)) {
              one = true;
              ret.push_back("  - B");
            }
            if (partner.get_digital(BUTTON_X)) {
              one = true;
              ret.push_back("  - X");
            }
            if (partner.get_digital(BUTTON_Y)) {
              one = true;
              ret.push_back("  - Y");
            }
            if (partner.get_digital(BUTTON_UP)) {
              one = true;
              ret.push_back("  - UP");
            }
            if (partner.get_digital(BUTTON_DOWN)) {
              one = true;
              ret.push_back("  - DOWN");
            }
            if (partner.get_digital(BUTTON_LEFT)) {
              one = true;
              ret.push_back("  - LEFT");
            }
            if (partner.get_digital(BUTTON_RIGHT)) {
              one = true;
              ret.push_back("  - RIGHT");
            }
            if (partner.get_digital(BUTTON_L1)) {
              one = true;
              ret.push_back("  - L1");
            }
            if (partner.get_digital(BUTTON_L2)) {
              one = true;
              ret.push_back("  - L2");
            }
            if (partner.get_digital(BUTTON_R1)) {
              one = true;
              ret.push_back("  - R1");
            }
            if (partner.get_digital(BUTTON_R2)) {
              one = true;
              ret.push_back("  - R2");
            }
            if (!one)
              ret.push_back("  - None");
            goto end;
          } else if (command.rfind(".a", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_A) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button A: " + pressed);
            goto end;
          } else if (command.rfind(".b", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_B) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button B: " + pressed);
            goto end;
          } else if (command.rfind(".x", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_X) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button X: " + pressed);
            goto end;
          } else if (command.rfind(".y", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_Y) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button Y: " + pressed);
            goto end;
          } else if (command.rfind(".up", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_UP) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button UP: " + pressed);
            goto end;
          } else if (command.rfind(".down", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_DOWN) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button DOWN: " + pressed);
            goto end;
          } else if (command.rfind(".left", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_LEFT) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button LEFT: " + pressed);
            goto end;
          } else if (command.rfind(".right", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_RIGHT) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button RIGHT: " + pressed);
            goto end;
          } else if (command.rfind(".l1", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_L1) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button L1: " + pressed);
            goto end;
          } else if (command.rfind(".l2", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_L2) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button K2: " + pressed);
            goto end;
          } else if (command.rfind(".r1", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_R1) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button R1: " + pressed);
            goto end;
          } else if (command.rfind(".r2", 0) == 0) {
            std::string pressed = (partner.get_digital(BUTTON_R2) ? "pressed" : "not pressed");
            ret.push_back("Partner Controller Button R2: " + pressed);
            goto end;
          } else {
            ret.push_back("Unknown subcommand in \"controller.second.digital\". Specify a button to narrow information");
            goto end;
          }
        } else {
          ret.push_back("Unknown subcommand in \"controller.second\". Use \"analog\" or \"digital\" to narrow information");
          goto end;
        }

      } else {
        ret.push_back("Unknown subcommand in \"controller\". Use \"main\" or \"second\" to narrow information");
        goto end;
      }
    }

    display:
    if (displaymain) {
      if (!main.is_connected())
        ret.push_back("Main Controller: Not Connected");
      else {
        int qc = 0;
        std::string lsq;
        if (main.get_analog(STICK_LEFT_Y) > 0) {
          lsq += "Upper ";
          qc++;
        } else if (main.get_analog(STICK_LEFT_Y) < 0) {
          lsq += "Lower ";
          qc++;
        }
        if (main.get_analog(STICK_LEFT_X) > 0) {
          lsq += "Right ";
          qc++;
        } else if (main.get_analog(STICK_LEFT_X) < 0) {
          lsq += "Left ";
          qc++;
        }
        switch (qc) {
          case 2:
            lsq += "Quadrant";
            break;
          case 1:
            lsq += "Half";
            break;
          case 0:
            lsq += "Center";
        };
        qc = 0;
        std::string rsq;
        if (main.get_analog(STICK_RIGHT_Y) > 0) {
          rsq += "Upper ";
          qc++;
        } else if (main.get_analog(STICK_RIGHT_Y) < 0) {
          rsq += "Lower ";
          qc++;
        }
        if (main.get_analog(STICK_RIGHT_X) > 0) {
          rsq += "Right ";
          qc++;
        } else if (main.get_analog(STICK_RIGHT_X) < 0) {
          rsq += "Left ";
          qc++;
        }
        switch (qc) {
          case 2:
            rsq += "Quadrant";
            break;
          case 1:
            rsq += "Half";
            break;
          case 0:
            rsq += "Center";
        };
        ret.push_back("Main Controller:");
        ret.push_back("    Left Analog: " + std::to_string(main.get_analog(STICK_LEFT_X)) + ", " + std::to_string(main.get_analog(STICK_LEFT_Y)) + " (" + lsq + ")");
        ret.push_back("    Right Analog: " + std::to_string(main.get_analog(STICK_RIGHT_X)) + ", " + std::to_string(main.get_analog(STICK_RIGHT_Y)) + " (" + rsq + ")");
        ret.push_back("    Pressed Buttons:");
        bool one = false;
        if (main.get_digital(BUTTON_A)) {
          one = true;
          ret.push_back("      - A");
        }
        if (main.get_digital(BUTTON_B)) {
          one = true;
          ret.push_back("      - B");
        }
        if (main.get_digital(BUTTON_X)) {
          one = true;
          ret.push_back("      - X");
        }
        if (main.get_digital(BUTTON_Y)) {
          one = true;
          ret.push_back("      - Y");
        }
        if (main.get_digital(BUTTON_UP)) {
          one = true;
          ret.push_back("      - UP");
        }
        if (main.get_digital(BUTTON_DOWN)) {
          one = true;
          ret.push_back("      - DOWN");
        }
        if (main.get_digital(BUTTON_LEFT)) {
          one = true;
          ret.push_back("      - LEFT");
        }
        if (main.get_digital(BUTTON_RIGHT)) {
          one = true;
          ret.push_back("      - RIGHT");
        }
        if (main.get_digital(BUTTON_L1)) {
          one = true;
          ret.push_back("      - L1");
        }
        if (main.get_digital(BUTTON_L2)) {
          one = true;
          ret.push_back("      - L2");
        }
        if (main.get_digital(BUTTON_R1)) {
          one = true;
          ret.push_back("      - R1");
        }
        if (main.get_digital(BUTTON_R2)) {
          one = true;
          ret.push_back("      - R2");
        }
        if (!one)
          ret.push_back("      - None");
      }
    }

    if (displaypartner) {
      if (!partner.is_connected())
        ret.push_back("Partner Controller: Not Connected");
      else {
        int qc = 0;
        std::string lsq;
        if (main.get_analog(STICK_LEFT_Y) > 0) {
          lsq += "Upper ";
          qc++;
        } else if (main.get_analog(STICK_LEFT_Y) < 0) {
          lsq += "Lower ";
          qc++;
        }
        if (main.get_analog(STICK_LEFT_X) > 0) {
          lsq += "Right ";
          qc++;
        } else if (main.get_analog(STICK_LEFT_X) < 0) {
          lsq += "Left ";
          qc++;
        }
        switch (qc) {
          case 2:
            lsq += "Quadrant";
            break;
          case 1:
            lsq += "Half";
            break;
          case 0:
            lsq += "Center";
        };
        qc = 0;
        std::string rsq;
        if (main.get_analog(STICK_RIGHT_Y) > 0) {
          rsq += "Upper ";
          qc++;
        } else if (main.get_analog(STICK_RIGHT_Y) < 0) {
          rsq += "Lower ";
          qc++;
        }
        if (main.get_analog(STICK_RIGHT_X) > 0) {
          rsq += "Right ";
          qc++;
        } else if (main.get_analog(STICK_RIGHT_X) < 0) {
          rsq += "Left ";
          qc++;
        }
        switch (qc) {
          case 2:
            rsq += "Quadrant";
            break;
          case 1:
            rsq += "Half";
            break;
          case 0:
            rsq += "Center";
        };
        ret.push_back("Partner Controller:");
        ret.push_back("    Left Analog: " + std::to_string(main.get_analog(STICK_LEFT_X)) + ", " + std::to_string(main.get_analog(STICK_LEFT_Y)) + " (" + lsq + ")");
        ret.push_back("    Right Analog: " + std::to_string(main.get_analog(STICK_RIGHT_X)) + ", " + std::to_string(main.get_analog(STICK_RIGHT_Y)) + " (" + rsq + ")");
        ret.push_back("    Pressed Buttons:");
        bool one = false;
        if (main.get_digital(BUTTON_A)) {
          one = true;
          ret.push_back("      - A");
        }
        if (main.get_digital(BUTTON_B)) {
          one = true;
          ret.push_back("      - B");
        }
        if (main.get_digital(BUTTON_X)) {
          one = true;
          ret.push_back("      - X");
        }
        if (main.get_digital(BUTTON_Y)) {
          one = true;
          ret.push_back("      - Y");
        }
        if (main.get_digital(BUTTON_UP)) {
          one = true;
          ret.push_back("      - UP");
        }
        if (main.get_digital(BUTTON_DOWN)) {
          one = true;
          ret.push_back("      - DOWN");
        }
        if (main.get_digital(BUTTON_LEFT)) {
          one = true;
          ret.push_back("      - LEFT");
        }
        if (main.get_digital(BUTTON_RIGHT)) {
          one = true;
          ret.push_back("      - RIGHT");
        }
        if (main.get_digital(BUTTON_L1)) {
          one = true;
          ret.push_back("      - L1");
        }
        if (main.get_digital(BUTTON_L2)) {
          one = true;
          ret.push_back("      - L2");
        }
        if (main.get_digital(BUTTON_R1)) {
          one = true;
          ret.push_back("      - R1");
        }
        if (partner.get_digital(BUTTON_R2)) {
          one = true;
          ret.push_back("      - R2");
        }
        if (!one)
          ret.push_back("      - None");
      }
    }

  } else if (command.rfind("drive", 0) == 0) {
    command = command.substr(5);

    if (command.size() == 0) {
      std::pair<int, int> ptkt = drive->getTurnValues();
      PID * pid = driveControl->getPID();

      ret.push_back("Drive Management:");
      ret.push_back("    Sensitivity:");
      ret.push_back("        Driving: " + std::to_string(sensitivity));
      ret.push_back("        Adjusting: " + std::to_string(adjustingSensitivity));
      ret.push_back("    Gearing:");
      ret.push_back("        Input: " + std::to_string(drive->getInputRatio()));
      ret.push_back("        Ouput: " + std::to_string(drive->getOutputRatio()));
      ret.push_back("        Wheel Diameter: " + std::to_string(drive->getWheelDiameter()) + " inches");
      ret.push_back("    Turn Values:");
      ret.push_back("        Proportion: " + std::to_string(ptkt.first));
      ret.push_back("        Constant: " + std::to_string(ptkt.second));
      if (pid == NULL)
        ret.push_back("    PID Values: Not Set");
      else {
        std::string braking = (pid->brake) ? "true" : "false";
        ret.push_back("    PID Values:");
        ret.push_back("        dt: " + std::to_string(pid->dt));
        ret.push_back("        kp: " + std::to_string(pid->kp));
        ret.push_back("        ki: " + std::to_string(pid->ki));
        ret.push_back("        kd: " + std::to_string(pid->kd));
        ret.push_back("        brake: " + braking);
        ret.push_back("        tLimit: " + std::to_string(pid->tLimit));
        ret.push_back("        iLimit: " + std::to_string(pid->iLimit));
        ret.push_back("        iZone: " + std::to_string(pid->iZone));
        ret.push_back("        dThreshold: " + std::to_string(pid->dThreshold));
        ret.push_back("        tThreshold: " + std::to_string(pid->tThreshold));
        ret.push_back("        hangThreshold: " + std::to_string(pid->de0));
      }
    } else if (command.rfind(".sens", 0) == 0) {
      command = command.substr(5);
      if (command.rfind("itivity", 0) == 0) command = command.substr(7);

      if (command.size() == 0) {
        ret.push_back("Drive Sensitivity:");
        ret.push_back("    Driving: " + std::to_string(std::lround(sensitivity * 100)) + "%");
        ret.push_back("    Adjusting: " + std::to_string(std::lround(adjustingSensitivity * 100)) + "%");
      } else if (command.rfind(".dr", 0) == 0)
        ret.push_back("Driving Sensitivity: " + std::to_string(sensitivity));
      else if (command.rfind(".ad", 0) == 0)
        ret.push_back("Drive Adjusting Sensitivity: " + std::to_string(adjustingSensitivity));
      else if (command.rfind(".set", 0) == 0) {
        command = command.substr(4);
        if (command.rfind(" ", 0) == 0) command = command.substr(1);

        if (command.size() == 0)
          ret.push_back("Please specify a value to set, such as \"driving\" or \"adjusting\"");
        else if (command.rfind(".dr", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("iv", 0) == 0) command = command.substr(2);
          if (command.rfind("e", 0) == 0) command = command.substr(1);
          else if (command.rfind("ing", 0) == 0) command = command.substr(3);
          std::string selstr;
          double sel;

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size() + 1);

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          try {
            sel = std::stod(command);
          } catch (std::exception & e) {
            ret.push_back("\"" + command + "\" is not a valid value!");
            goto end;
          }

          sensitivity = sel;
          ret.push_back("Set Driving Sensitivity to: " + selstr);
        } else if (command.rfind(".ad", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("iv", 0) == 0) command = command.substr(2);
          if (command.rfind("e", 0) == 0) command = command.substr(1);
          else if (command.rfind("ing", 0) == 0) command = command.substr(3);
          std::string selstr;
          double sel;

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size() + 1);

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          try {
            sel = std::stod(command);
          } catch (std::exception & e) {
            ret.push_back("\"" + command + "\" is not a valid value!");
            goto end;
          }

          adjustingSensitivity = sel;
          ret.push_back("Set Drive Adjusting Sensitivity to: " + selstr);
        } else
          ret.push_back("\"" + command + "\" is not a known value! Acceptable values are \"driving\" or \"adjusting\"");
      } else
        ret.push_back("Unknown subcommand in \"drive.sensitivity\". Set values with \"set\" or retrieve values such as \"driving\"");

    } else if (command.rfind(".turning", 0) == 0) {
      command = command.substr(8);
      std::pair<int, int> ptkt = drive->getTurnValues();
      if (command.size() == 0) {
        ret.push_back("Drive Turn Values:");
        ret.push_back("    Proportion: " + std::to_string(ptkt.first));
        ret.push_back("    Constant: " + std::to_string(ptkt.second));
      } else if (command.rfind(".set", 0) == 0) {
        command = command.substr(4);

        if (command.size() == 0)
          ret.push_back("Please specify a value to set, such as \"kp\" or \"tlimit\"");
        else if (command.rfind(".p", 0) == 0) {
          std::string selstr;
          double sel;

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size() + 1);

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          try {
            sel = std::stod(command);
          } catch (std::exception & e) {
            ret.push_back("\"" + command + "\" is not a valid value!");
            goto end;
          }

          drive->setTurnValues(sel, ptkt.second);
          ret.push_back("Set Drive Turning Proportion to: " + command);
        } else if (command.rfind(".k", 0) == 0) {
          std::string selstr;
          double sel;

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size() + 1);

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          try {
            sel = std::stod(command);
          } catch (std::exception & e) {
            ret.push_back("\"" + command + "\" is not a valid value!");
            goto end;
          }

          drive->setTurnValues(ptkt.first, sel);
          ret.push_back("Set Drive Turning Constant to: " + command);
        } else
          ret.push_back("\"" + command + "\" is not a known value! Acceptable values include \"pt\" or \"kt\"");
      } else if (command.rfind(".p", 0) == 0)
        ret.push_back("Drive Turn Proportion: " + std::to_string(ptkt.first));
      else if (command.rfind(".k", 0) == 0)
        ret.push_back("Drive Turn Constant: " + std::to_string(ptkt.second));
      else
        ret.push_back("Unknown subcommand in \"drive.turning\". Set values with \"set\" or retrieve values such as \"kt\"");

    } else if (command.rfind(".move_degrees", 0) == 0) {
      command = command.substr(13);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      std::string selstr;
      int sel;

      if (command.size() == 0) {
        ret.push_back("Please specify an amount of degrees to move");
        goto end;
      }

      selstr = command.substr(0, command.find(" "));
      command = command.substr(selstr.size());

      try {
        sel = std::stoi(selstr);
      } catch (std::exception & e) {
        ret.push_back("\"" + selstr + "\" is not a valid amount!");
        goto end;
      }

      Logger::log(LOG_INFO, "Moving " + selstr + " degrees...");
      Debugger::parameter1 = selstr;
      Debugger::callCode = 0x11;
      Debugger::completion = "Movement complete";

    } else if (command.rfind(".move", 0) == 0) {
      command = command.substr(5);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      std::string selstr;
      int sel;

      if (command.size() == 0) {
        ret.push_back("Please specify an amount of inches to move");
        goto end;
      }

      selstr = command.substr(0, command.find(" "));
      command = command.substr(selstr.size());

      try {
        sel = std::stoi(selstr);
      } catch (std::exception & e) {
        ret.push_back("\"" + selstr + "\" is not a valid amount!");
        goto end;
      }

      Logger::log(LOG_INFO, "Moving " + selstr + " inches...");
      Debugger::parameter1 = selstr;
      Debugger::callCode = 0x10;
      Debugger::completion = "Movement complete";

    } else if (command.rfind(".turn", 0) == 0) {
      command = command.substr(5);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      std::string selstr;
      int sel;

      if (command.size() == 0) {
        ret.push_back("Please specify an amount of inches to turn");
        goto end;
      }

      selstr = command.substr(0, command.find(" "));
      command = command.substr(selstr.size());

      try {
        sel = std::stoi(selstr);
      } catch (std::exception & e) {
        ret.push_back("\"" + selstr + "\" is not a valid amount!");
        goto end;
      }

      Logger::log(LOG_INFO, "Turning " + selstr + " degrees...");
      Debugger::parameter1 = selstr;
      Debugger::callCode = 0x13;
      Debugger::completion = "Turning complete";
    } else if (command.rfind(".pvot", 0) == 0) {
      command = command.substr(5);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      std::string selstr;
      int sel;

      if (command.size() == 0) {
        ret.push_back("Please specify an amount of inches to pivot");
        goto end;
      }

      selstr = command.substr(0, command.find(" "));
      command = command.substr(selstr.size());

      try {
        sel = std::stoi(selstr);
      } catch (std::exception & e) {
        ret.push_back("\"" + selstr + "\" is not a valid amount!");
        goto end;
      }

      Logger::log(LOG_INFO, "Pivoting " + selstr + " degrees...");
      Debugger::parameter1 = selstr;
      Debugger::callCode = 0x12;
      Debugger::completion = "Pivoting complete";
    } else if (command.rfind(".gearing", 0) == 0) {
      command = command.substr(8);

      if (command.size() == 0) {
        ret.push_back("Drive Gearing:");
        ret.push_back("    Input: " + std::to_string(drive->getInputRatio()));
        ret.push_back("    Ouput: " + std::to_string(drive->getOutputRatio()));
        ret.push_back("    Wheel Diameter: " + std::to_string(drive->getWheelDiameter()) + " inches");
      } else if (command.rfind(".set", 0) == 0) {
        command = command.substr(4);

        if (command.rfind(".in", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("put", 0) == 0) command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          drive->setGearRatio(sel, drive->getOutputRatio(), drive->getWheelDiameter());
          ret.push_back("Set Drive Gearing input to: " + selstr);
        } else if (command.rfind(".out", 0) == 0){
          command = command.substr(4);
          if (command.rfind("put", 0) == 0) command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          drive->setGearRatio(drive->getInputRatio(), sel, drive->getWheelDiameter());
          ret.push_back("Set Drive Gearing output to: " + selstr);
        } else if (command.rfind(".wd", 0) == 0 || command.rfind(".wh", 0) == 0 || command.rfind(".dia", 0) == 0) {
          std::string selstr;
          double sel;

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size() + 1);

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          try {
            sel = std::stod(command);
          } catch (std::exception & e) {
            ret.push_back("\"" + command + "\" is not a valid value!");
            goto end;
          }

          drive->setGearRatio(drive->getInputRatio(), drive->getOutputRatio(), sel);
          ret.push_back("Set Drive Gearing wheelDiameter to: " + command);
        } else
          ret.push_back("Please specify a value to set, such as \"in\" or \"out\"");
      } else if (command.rfind(".in", 0) == 0)
        ret.push_back("Drive Gearing Input Ratio: " + std::to_string(drive->getInputRatio()));
      else if (command.rfind(".out", 0) == 0)
        ret.push_back("Drive Gearing Output Ratio: " + std::to_string(drive->getOutputRatio()));
      else if (command.rfind(".wd", 0) == 0 || command.rfind(".wh", 0) == 0 || command.rfind(".dia", 0) == 0)
        ret.push_back("Drive Gearing Wheel Diameter: " + std::to_string(drive->getWheelDiameter()) + " inches");
      else
        ret.push_back("Unknown subcommand in \"drive.gearing\". Set values with \"set\" or retrieve values such as \"input\" or \"output\"");
    } else if (command.rfind(".pd", 0) == 0) {
      PID * pid = driveControl->getPID();
      command = command.substr(3);

      if (command.size() == 0) {
        if (pid == NULL)
          ret.push_back("Drive PID Values: Not Set");
        else {
          std::string braking = (pid->brake) ? "true" : "false";
          ret.push_back("Drive PID Values:");
          ret.push_back("    dt: " + std::to_string(pid->dt));
          ret.push_back("    kp: " + std::to_string(pid->kp));
          ret.push_back("    ki: " + std::to_string(pid->ki));
          ret.push_back("    kd: " + std::to_string(pid->kd));
          ret.push_back("    brake: " + braking);
          ret.push_back("    tLimit: " + std::to_string(pid->tLimit));
          ret.push_back("    iLimit: " + std::to_string(pid->iLimit));
          ret.push_back("    iZone: " + std::to_string(pid->iZone));
          ret.push_back("    dThreshold: " + std::to_string(pid->dThreshold));
          ret.push_back("    tThreshold: " + std::to_string(pid->tThreshold));
          ret.push_back("    hangThreshold: " + std::to_string(pid->de0));
        }
        goto end;
      }

      if (command.rfind(".set", 0) == 0) {
        command = command.substr(4);

        if (command.size() == 0) {
          ret.push_back("Please specify a value to set, such as \"kp\" or \"tlimit\"");
          goto end;
        }

        if (pid == NULL) {
          driveControl->setPID(20, 0, 0, 0, false, 127, 0, 0, 10, 50, 50);
          pid = driveControl->getPID();
        }

        if (command.rfind(".dth", 0) == 0) {
          command = command.substr(4);
          if (command.rfind("reshold", 0) == 0) command = command.substr(7);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->dThreshold = sel;
          ret.push_back("Set Drive PID dThreshold to: " + selstr);
        } else if (command.rfind(".kp", 0) == 0) {
          command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->kp = sel;
          ret.push_back("Set Drive PID kp to: " + selstr);
        } else if (command.rfind(".ki", 0) == 0) {
          command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->ki = sel;
          ret.push_back("Set Drive PID ki to: " + selstr);
        } else if (command.rfind(".kd", 0) == 0) {
          command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->kd = sel;
          ret.push_back("Set Drive PID kd to: " + selstr);
        } else if (command.rfind(".brake", 0) == 0) {
          command = command.substr(6);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          bool sel = false;

          if (command.size() == 0) {
            ret.push_back("Please specify a boolean");
            goto end;
          } else if (command.compare("1") == 0)
            sel = true;
          else if (command.compare("0") == 0)
            sel = false;
          else if (command.compare("true") == 0)
            sel = true;
          else if (command.compare("false") == 0)
            sel = false;
          else {
            ret.push_back("\"" + command + "\" is not a valid boolean! Acceptable values are \"true\" or \"false\"");
            goto end;
          }

          pid->brake = sel;
          ret.push_back("Set Drive PID brake to: " + command);
        } else if (command.rfind(".tl", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("imit", 0) == 0) command = command.substr(4);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->tLimit = sel;
          ret.push_back("Set Drive PID tLimit to: " + selstr);
        } else if (command.rfind(".il", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("imit", 0) == 0) command = command.substr(4);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->iLimit = sel;
          ret.push_back("Set Drive PID iLimit to: " + selstr);
        } else if (command.rfind(".iz", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("one", 0) == 0) command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->iZone = sel;
          ret.push_back("Set Drive PID iZone to: " + selstr);
        } else if (command.rfind(".dt", 0) == 0) {
          command = command.substr(3);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->dt = sel;
          ret.push_back("Set Drive PID dt to: " + selstr);
        } else if (command.rfind(".tt", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("hreshold", 0) == 0) command = command.substr(8);
          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->tThreshold = sel;
          ret.push_back("Set Drive PID tThreshold to: " + selstr);
        } else if (command.rfind(".de", 0) == 0 || command.rfind("ht", 0) == 0) {
          command = command.substr(3);
          if (command.rfind("0", 0) == 0) command = command.substr(1);
          else if (command.rfind("hreshold", 0) == 0) command = command.substr(8);

          if (command.rfind(" ", 0) == 0) command = command.substr(1);
          std::string selstr;
          double sel;

          if (command.size() == 0) {
            ret.push_back("Please specify a value");
            goto end;
          }

          selstr = command.substr(0, command.find(" "));
          command = command.substr(selstr.size());

          try {
            sel = std::stod(selstr);
          } catch (std::exception & e) {
            ret.push_back("\"" + selstr + "\" is not a valid value!");
            goto end;
          }

          pid->de0 = sel;
          ret.push_back("Set Drive PID hangThreshold to: " + selstr);
        } else
          ret.push_back("\"" + command + "\" is not a known value! Acceptable values include \"kp\" or \"tlimit\"");
      } else if (command.rfind(".clear", 0) == 0) {
        if (pid == NULL)
          ret.push_back("Drive PID Values are not set!");
        else {
          driveControl->clearPID();
          ret.push_back("Drive PID Values have been cleared");
        }
      } else {
        if (pid == NULL)
          ret.push_back("Drive PID Values: Not Set");
        else if (command.rfind(".dth", 0) == 0)
          ret.push_back("Drive PID dThreshold: " + std::to_string(pid->dThreshold));
        else if (command.rfind(".kp", 0) == 0)
          ret.push_back("Drive PID kp: " + std::to_string(pid->kp));
        else if (command.rfind(".ki", 0) == 0)
          ret.push_back("Drive PID ki: " + std::to_string(pid->ki));
        else if (command.rfind(".kd", 0) == 0)
          ret.push_back("Drive PID kd: " + std::to_string(pid->kd));
        else if (command.rfind(".brake", 0) == 0) {
          std::string braking = (pid->brake) ? "true" : "false";
          ret.push_back("Drive PID brake: " + braking);
        } else if (command.rfind(".tlimit", 0) == 0)
          ret.push_back("Drive PID tLimit: " + std::to_string(pid->tLimit));
        else if (command.rfind(".il", 0) == 0)
          ret.push_back("Drive PID iLimit: " + std::to_string(pid->iLimit));
        else if (command.rfind(".iz", 0) == 0)
          ret.push_back("Drive PID iZone: " + std::to_string(pid->iZone));
        else if (command.rfind(".dt", 0) == 0)
          ret.push_back("Drive PID dt: " + std::to_string(pid->dt));
        else if (command.rfind(".tt", 0) == 0)
          ret.push_back("Drive PID tThreshold: " + std::to_string(pid->tThreshold));
        else if (command.rfind(".de", 0) == 0 || command.rfind("ht") == 0)
          ret.push_back("Drive PID hangThreshold: " + std::to_string(pid->de0));
         else
          ret.push_back("Unknown subcommand in \"drive.pid\". Set values with \"set\" or retrieve values such as \"kp\"");
      }
    } else
      ret.push_back("Unknown subcommand in \"drive\". Issue commands such as \"move\" or retrieve information with \"pid\"");
  } else if (command.rfind("auto", 0) == 0) {
      command = command.substr(4);
      if (command.rfind("nomous", 0) == 0) command.substr(6);

      if (command.size() == 0) {
        ret.push_back("Currently Selected Autonomous: " + std::to_string(selectedAutonomous));
      } else if (command.rfind(".run", 0) == 0) {
        Logger::log(LOG_INFO, "Forcibly running autonomous...");
        Debugger::callCode = 0x01;
        Debugger::completion = "Autonomous routine complete";
      } else if (command.rfind(".sel", 0) == 0 || command.rfind(".set", 0) == 0) {
        command = command.substr(4);
        if (command.rfind("ect", 0) == 0) command = command.substr(3);
        if (command.rfind(" ", 0) == 0) command = command.substr(1);
        std::string selstr;
        int sel;

        if (command.size() == 0) {
          ret.push_back("Please specify a number to select");
          goto end;
        }

        selstr = command.substr(0, command.find(" "));
        command = command.substr(selstr.size());

        try {
          sel = std::stoi(selstr);
        } catch (std::exception & e) {
          ret.push_back("\"" + selstr + "\" is not a valid selection!");
          goto end;
        }

        selectedAutonomous = sel;
        ret.push_back("Selected autonomous: " + sel);
      } else
        ret.push_back("Unknown subcommand in \"auto\". Use \"select\" to select an autonomous and \"run\" to run it");
  } else if (command.rfind("lcd", 0) == 0) {
    command = command.substr(3);
    if (command.size() == 0) {
      ret.push_back("Please specify an action like \"status\" or \"settext\"");
      goto end;
    } else if (command.rfind(".status", 0) == 0) {
      ret.push_back("Status: " + LCD::getStatus());
      goto end;
    } else if (command.rfind(".text", 0) == 0) {
      command = command.substr(5);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      bool invalidline = false;
      std::string linestr;
      int line;

      if (command.size() == 0) {
        ret.push_back("Please specify a line number");
        goto end;
      }

      linestr = command.substr(0, command.find(" "));
      command = command.substr(linestr.size());

      try {
        line = std::stoi(linestr);
      } catch (std::exception & e) {
        invalidline = true;
      }
      if (invalidline || line > 9 || line < 0){
        ret.push_back("\"" + linestr + "\" is not a valid line number!");
        goto end;
      }

      ret.push_back("Line " + linestr + ": " + LCD::getText(line));
    } else if (command.rfind(".setstatus", 0) == 0) {
      command = command.substr(10);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);

      LCD::setStatus(command);
      ret.push_back("Set LCD status to: " + command);
    } else if (command.rfind(".settext", 0) == 0) {
      command = command.substr(8);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      bool invalidline = false;
      std::string linestr;
      int line;

      if (command.size() == 0) {
        ret.push_back("Please specify a line number");
        goto end;
      }

      linestr = command.substr(0, command.find(" "));
      command = command.substr(linestr.size() + 1);

      try {
        line = std::stoi(linestr);
      } catch (std::exception & e) {
        invalidline = true;
      }
      if (invalidline || line > 9 || line < 0){
        ret.push_back("\"" + linestr + "\" is not a valid line number!");
        goto end;
      }
      if (command.size() == 0) {
        ret.push_back("Please specify some text");
        goto end;
      }

      LCD::setText(line, command);
      ret.push_back("Set text on line " + linestr + " to: " + command);
    } else if (command.rfind(".clearstatus", 0) == 0) {
      LCD::setStatus("");
      ret.push_back("Cleared LCD status");
    } else if (command.rfind(".cleartext", 0) == 0) {
      command = command.substr(10);
      if (command.rfind(" ", 0) == 0) command = command.substr(1);
      bool invalidline = false;
      std::string linestr;
      int line;

      if (command.size() == 0) {
        ret.push_back("Please specify a line number");
        goto end;
      }

      linestr = command.substr(0, command.find(" "));
      command = command.substr(linestr.size());

      try {
        line = std::stoi(linestr);
      } catch (std::exception & e) {
        invalidline = true;
      }
      if (invalidline || line > 9 || line < 0){
        ret.push_back("\"" + linestr + "\" is not a valid line number!");
        goto end;
      }
      LCD::setText(line, "");
      ret.push_back("Cleared text on line " + linestr);
    } else if (command.rfind(".clearall", 0) == 0) {
      for (int i = 0; i < 10; i++)
        LCD::setText(i, "");
      ret.push_back("Cleared all text on the LCD");
    } else if (command.rfind(".button", 0) == 0) {
      command = command.substr(7);
      if (command.size() == 0) {
        std::string pressed = "None";
        switch (pros::lcd::read_buttons()) {
          case 4:
            pressed = "Left";
            break;
          case 2:
            pressed = "Center";
            break;
          case 1:
            pressed = "Right";
            break;
        }
        ret.push_back("LCD Pressed Button: " + pressed);
      } else if (command.rfind(".pess", 0) == 0) {
        command = command.substr(5);
        if (command.rfind(" ", 0) == 0) command = command.substr(1);

        std::string name;
        if (command.compare("left") == 0) {
          name = "left";
          LCD::onLeftButton();
        } else if (command.compare("right") == 0) {
          name = "right";
          LCD::onRightButton();
        } else if (command.compare("center") == 0) {
          name = "center";
          LCD::onCenterButton();
        } else {
          ret.push_back("Unknown button \"" + command + "\". Acceptable buttons are \"left\", \"right\", or \"center\"");
          goto end;
        }
        ret.push_back("Successfully pressed the " + name + " button.");
      } else if (command.rfind(".left", 0) == 0) {
        std::string pressed = (pros::lcd::read_buttons() == 4) ? "pressed" : "not pressed";
        ret.push_back("LCD Left Button: " + pressed);
      } else if (command.rfind(".right", 0) == 0) {
        std::string pressed = (pros::lcd::read_buttons() == 1) ? "pressed" : "not pressed";
        ret.push_back("LCD Right Button: " + pressed);
      } else if (command.rfind(".center", 0) == 0) {
        std::string pressed = (pros::lcd::read_buttons() == 2) ? "pressed" : "not pressed";
        ret.push_back("LCD Center Button: " + pressed);
      } else
        ret.push_back("Unkown subcommand in \"lcd.button\". Use \"press\" to virtually press a button");
    } else
      ret.push_back("Unknown subcommand in \"lcd\". Use subcommands such as \"button\" or \"text\"");
  } else
    ret.push_back("Unknown command. Type \"help\" for help");

  end:

  // Set the priority lower than operator control when waiting for a new command
  task->set_priority(TASK_PRIORITY_DEFAULT - 1);
  return ret;
}
