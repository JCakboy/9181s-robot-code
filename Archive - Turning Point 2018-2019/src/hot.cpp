#include "main.h"

/*
 * This file includes all definitions that require linking to the hot image
 *
 * This is a general-purpose file and code from this file can
 * refer to any part of robot control
 */
/*
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
*/
