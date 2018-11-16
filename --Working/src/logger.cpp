#include "main.h"
#include "logger.hpp"

std::vector<Logger*> Logger::loggers;

Logger::Logger (logging_levels mLevel, std::string filename) {
  logfile = fopen(filename.c_str(), "w");
}

void Logger::_log(logging_levels level, std::string message) {
  if (logfile == NULL)     pros::lcd::set_text(6, "not logging cause null" + message);
  if (minLevel == 0)pros::lcd::set_text(6, "not logging cause disable" + message);
  if (level > minLevel)pros::lcd::set_text(6, "not logging cause level" + message);
  if (minLevel == 0 || level >  minLevel || logfile == NULL) {
    return;
  }
  fputs(("[" + util::timestamp() + "] " + util::getLoggingLevelName(level) + ": " + message).c_str(), logfile);
  pros::lcd::set_text(6, "logged " + message);
}

void Logger::init(std::string filename) {
  Logger::init(LOGGING_DEFAULT_LEVEL, filename);
}

void Logger::init(logging_levels minLevel, std::string filename) {
  Logger * log = new Logger(minLevel, filename);
  Logger::loggers.push_back(log);
}

void Logger::log(logging_levels level, std::string message) {
  for (const auto & l : Logger::loggers)
    l->_log(level, message);
}
