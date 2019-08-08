#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

#include "main.h"

/*
 * Class to hold time values for debugging
 */
class CompetitionTimer {
private:
  // Start and end times
  int autonomousStart = 0;
  int autonomousEnd = 0;
  int opcontrolStart = 0;
  int opcontrolEnd = 0;
public:
  // Constructs the CompetitionTimer object
  CompetitionTimer();

  // Starts the autonomous timer
  void autonomousStartTimer();
  // Stops the autonomous timer
  void autonomousEndTimer();
  // Returns the time autonomous took
  int autonomousTime();
  // Waits until a certain time after autonomous start
  void autonomousWaitUntil(int ms);
  // CLears the autnomous timer
  void autonomousClearTimer();

  // Starts the operator control timer
  void opcontrolStartTimer();
  // Stops the operator control timer
  void opcontrolEndTimer();
  // Returns the time operator control took
  int opcontrolTime();
  // Waits until a certain time after operator control start
  void opcontrolWaitUntil(int ms);
  // CLears the opcontrol timer
  void opcontrolClearTimer();

};


/*
 * Class to hold messages for logging to a USB port
 */
class MessageHolder {
friend void mhTask(void * param);
private:
  // Message to hold
  std::string message = "";
  // Task to wait for USB input to log output
  void task();
public:
  // Constructs the MessageHolder object
  MessageHolder();

  // Appends a message
  void append(std::string message);
  // Appends a message to a new line
  void appendLine(std::string message);

  // Returns and erases the message
  std::string popMessage();
  // Returns the message
  std::string getMessage();
};

// Task to be given to the global MessageHolder object
extern void mhTask(void * param);

#endif
