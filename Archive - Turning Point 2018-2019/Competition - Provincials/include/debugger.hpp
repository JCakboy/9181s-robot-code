#ifndef _DEBUGGER_HPP_
#define _DEBUGGER_HPP_

#include "main.h"
#include <vector>

/*
 * A class to handle serial debugging through USB with a computer.
 * Interfaces with 'pros terminal' and queries motors, controllers, and batteries
 */

class Debugger {
  private:
    // Whether the debugger is stopped
    static bool stopped;
    // The debugger task
    static pros::Task * task;

    // The current command buffer
    static std::string currentCommand;
    // The pending command buffer (the string acting as if it were passed the end of history)
    static std::string pending;
    // The history of commands
    static std::vector<std::string> history;
    // The active index of history, changed with the arrow keys
    static int activeIndex;

    // The code for the command to run during operator control
    static unsigned int callCode;
    // The parameters for the call
    static std::string parameter1;
    static std::string parameter2;
    static std::string parameter3;
    // The completion message
    static std::string completion;

    // Handles a command
    static std::vector<std::string> command(std::string command);

    // Properly cooks the PROS terminal and is responsible for waiting for, getting and parsing a command for command()
    static void _task(void * param);

    // The variables from the ports namespace required to function properly
    static DriveControl * driveControl;
    static DriveFunction * drive;

  public:
    // Starts the debugging task
    static void start(DriveControl * driveControl, DriveFunction * drive);

    // Runs the debugger, meant to be called in the operator control loop to give direct access to methods
    static void run();

    // Stops the debugging task
    static void stop();

};

#endif
