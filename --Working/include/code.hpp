#ifndef _CODE_HPP_
#define _CODE_HPP_

/*
 * A namespace for holding global flag variables
 */

namespace flags {

  // Whether or not the main code shoudl be alive; a master switch
  static bool generalAlive;

  // Whether or not the opcontrol task should be alive
  static bool opcontrolAlive;

  // Whether or not the watchdog task should be alive
  static bool watchdogAlive;

  // The selected autonomous
  static int autonomousSelected;

  // Whether or not autonomous successfully completed
  static bool autonomousComplete;

  // Sets all alive variables to true
  static void aliveAll();

  // Sets all alive variables to false
  static void killAll();

};

void initilize();

void competition_initialize();

void autonomous();

void autonomousSafe();

extern TaskWatcher * opcontrolTaskwatcher;

void operatorControl();

void operatorControlSafe();

void disabled();

#endif
