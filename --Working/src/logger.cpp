#include "main.h"
#include "logger.hpp"

Logger::Logger (logging_levels mLevel, std::string filename) {
  logfile.open(filename);
}

void Logger::_log(logging_levels level, std::string message) {
  if (minLevel == 0 || level > minLevel)
    return;
  logfile << "[" << emath::timestamp() << "] " << message;
}

void Logger::init(std::string filename) {
  Logger::init(LOGGING_DEFAULT_LEVEL, filename);
}

void Logger::init(logging_levels minLevel, std::string filename) {
  Logger log (minLevel, filename);
  Logger::loggers.push_back(&log);
}

void Logger::log(logging_levels level, std::string message) {
  for (auto & l : Logger::loggers)
    l->_log(level, message);
}
