#include "main.h"

// Create the default constructor
CompetitionTimer::CompetitionTimer() = default;

// Starts the autonomous timer
void CompetitionTimer::autonomousStartTimer() {
  autonomousStart = util::sign(pros::millis());
  autonomousEnd = 0;
}

// Stops the autonomous timer
void CompetitionTimer::autonomousEndTimer() {
  autonomousEnd = util::sign(pros::millis());
}

// Returns the time autonomous took
int CompetitionTimer::autonomousTime() {
  if (autonomousStart == 0)
    return 0;
  if (autonomousEnd == 0)
    return util::sign(pros::millis()) - autonomousStart;
  return autonomousEnd - autonomousStart;
}

// Waits until a certain time after autonomous start
void CompetitionTimer::autonomousWaitUntil(int ms) {
  while (autonomousStart == 0)
    pros::delay(100);
  pros::delay(ms + autonomousStart - util::sign(pros::millis()));
}

// CLears the autonomous timer
void CompetitionTimer::autonomousClearTimer() {
  autonomousStart = 0;
  autonomousEnd = 0;
}


// Starts the operator control timer
void CompetitionTimer::opcontrolStartTimer() {
  opcontrolStart = util::sign(pros::millis());
  opcontrolEnd = 0;
}

// Stops the operator control timer
void CompetitionTimer::opcontrolEndTimer() {
  opcontrolEnd = util::sign(pros::millis());
}

// Returns the time operator control took
int CompetitionTimer::opcontrolTime() {
  if (opcontrolStart == 0)
    return 0;
  else if (opcontrolEnd == 0)
    return util::sign(pros::millis()) - opcontrolStart;
  else
    return opcontrolEnd - opcontrolStart;
}

// Waits until a certain time after operator control start
void CompetitionTimer::opcontrolWaitUntil(int ms) {
  while (opcontrolStart == 0)
    pros::delay(100);
  pros::delay(ms + opcontrolStart - util::sign(pros::millis()));
}

// CLears the opcontrol timer
void CompetitionTimer::opcontrolClearTimer() {
  opcontrolStart = 0;
  opcontrolEnd = 0;
}

// Create the default constructor
MessageHolder::MessageHolder() = default;

#if ATTACH_DEBUGGING
void MessageHolder::task() {
  while (true) {
    std::cin.get();
    std::cout << MessageHolder::popMessage();
  }
}

// Appends a message
void MessageHolder::append(std::string message) {
  MessageHolder::message += message;
}

// Appends a message to a new line
void MessageHolder::appendLine(std::string message){
  MessageHolder::message += message + "\n";
}

// Returns and erases the message
std::string MessageHolder::popMessage() {
  std::string message = MessageHolder::message;
  MessageHolder::message = "";
  return message;
}

// Returns the message
std::string MessageHolder::getMessage() {
  return MessageHolder::message;
}

void mhTask(void * param) {
  ports::messageHolder->task();
}
#else
// Ignore all calls to these methods if debugger is not attached
void MessageHolder::task() {}
void MessageHolder::append(std::string message) {}
void MessageHolder::appendLine(std::string message){}
std::string MessageHolder::popMessage() {
  return "";
}
std::string MessageHolder::getMessage() {
  return "";
}
void mhTask(void * param) {}
#endif
