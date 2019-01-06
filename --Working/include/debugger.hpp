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
    static bool stopped;
    static pros::Task * task;

    static std::vector<std::string> command(std::string command);

    static void _task(void * param);

  public:
    // Starts the debugging task
    static void start();

    // Stops the debugging task
    static void stop();

};

#endif
