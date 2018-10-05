#include "main.h"
#include "logger.hpp"

std::vector<Logger*> Logger::loggers;

Logger::Logger (logging_levels mLevel, std::string filename) {
  logfile.open(filename);
}

void Logger::_log(logging_levels level, std::string message) {
  if (minLevel == 0 || level > minLevel)
    return;
  std::string levelmsg = "";
  switch (level) {
    case LOG_SEVERE:
      levelmsg = "SEVERE: ";
      break;
    case LOG_ERROR:
      levelmsg = "ERROR: ";
      break;
    case LOG_WARNING:
      levelmsg = "WARNING: ";
      break;
    case LOG_INFO:
      levelmsg = "INFO: ";
      break;
    default:
      levelmsg = "DEBUG: ";
      break;
  }
  logfile << "[" << emath::timestamp() << "] " << levelmsg << message;
}

void Logger::init(std::string filename) {
  Logger::init(LOGGING_DEFAULT_LEVEL, filename);
}

void Logger::init(logging_levels minLevel, std::string filename) {
  Logger * log = new Logger(minLevel, filename);
  Logger::loggers.push_back(log);
}

void Logger::log(logging_levels level, std::string message) {
  for (auto & l : Logger::loggers)
    l->_log(level, message);
}
