#include "main.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

bool Debugger::stopped = true;
pros::Task * Debugger::task = NULL;

void Debugger::start() {
  if (Debugger::stopped) {
    Debugger::task = new pros::Task(Debugger::_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "Debugger");
    Debugger::stopped = false;
  }
}

void Debugger::stop() {
  if (!Debugger::stopped) {
    Debugger::task->remove();
    delete Debugger::task;
    Debugger::task = NULL;
    Debugger::stopped = false;
  }
}

void Debugger::_task(void * param) {
  while (true) {
    std::string input;
    getline(std::cin, input);

    Logger::log(LOG_INFO, "Command: " + input);

    for (std::string s : Debugger::command(input))
      Logger::log(LOG_INFO, s);
  }
}

std::vector<std::string> Debugger::command(std::string command) {
  std::vector<std::string> ret;

  // Trim command
  command.erase(std::find_if(command.rbegin(), command.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), command.end());
  command.erase(command.begin(), std::find_if(command.begin(), command.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

  std::replace(command.begin(), command.end(), ',', '.');

  pros::Controller main (CONTROLLER_MAIN);
  pros::Controller partner (CONTROLLER_PARTNER);
  pros::Controller * controllerMain = &main;
  pros::Controller * controllerPartner = &partner;

  if (command.rfind("echo ", 0) == 0) {
    ret.push_back(command.substr(5));
  } else if (command.rfind("hel", 0) == 0) {
    ret.push_back("This is the serial debugging terminal for the VEX v5 Brain.");
    ret.push_back("This can be used for controlling behavior on the brain as well as retrieving information about connected devices.");
    ret.push_back("Some commands that are available include \"echo\", \"battery\", \"motor\", \"freeze\".");
    ret.push_back("More may be added in the future.");
  } else if (command.rfind("freeze", 0) == 0) {

    if (runOperaterControlLoop) {
      runOperaterControlLoop = false;
      ret.push_back("Operator Control Task paused");
      LCD::setStatus("Paused");
    } else
      ret.push_back("Operator Control Task is already paused");
  } else if (command.rfind("unfreeze", 0) == 0) {
    if (!runOperaterControlLoop) {
      runOperaterControlLoop = true;
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
      ret.push_back("Please specify a port");
      goto end;
    }

    portstr = command.substr(0, command.find("."));
    command = command.substr(portstr.size());

    try {
      port = std::stoi(portstr);
    } catch (std::exception & e) {
      invalidport = true;
    }
    if (invalidport || port > 20){
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
      ret.push_back("    Position: " + std::to_string(motor.get_position()) + " degrees");
      ret.push_back("    Direction: " + direction);
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
    } else if (command.rfind(".loc" , 0) == 0)
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

      if (runOperaterControlLoop) {
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

      if (runOperaterControlLoop) {
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

      if (runOperaterControlLoop) {
        ret.push_back("It is recommended that the operator control loop is paused when manually");
        ret.push_back("moving the motors because the loop may issue a command after the manual move.");
        ret.push_back("Use \"freeze\" to pause the operator control loop.");
      }

      std::string type = ((power == 0) ? "Stop" : "Move");

      motor.move(power);
      ret.push_back(type + " command issued to the motor on port " + portstr);
    } else if (command.rfind(".sto", 0) == 0) {
      if (runOperaterControlLoop) {
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
        ret.push_back("    Main Controller Battery: Not connected");
      if (controllerPartner->is_connected()) {
        ret.push_back("    Partner Controller Battery:");
        ret.push_back("        Capacity: " + std::to_string(controllerPartner->get_battery_capacity()) + "%");
        ret.push_back("        Voltage: " + std::to_string(controllerPartner->get_battery_level() / 1000.0) + " V");
      } else
        ret.push_back("    Partner Controller Battery: Not connected");

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

  } else if (command.rfind("lcd", 0) == 0) {

  } else
    ret.push_back("Unknown command. Type \"help\" for help");

  end:

  return ret;
}
