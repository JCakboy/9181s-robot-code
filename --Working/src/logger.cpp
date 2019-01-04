#include "main.h"
#include "logger.hpp"
#include <cstdio>

std::vector<Logger*> Logger::loggers;

Logger::Logger(logging_levels mLevel, std::string fileName, FILE * file) {
  Logger::minLevel = mLevel;
  Logger::fileName = fileName;
  Logger::logfile = file;
}

void Logger::addNew(Logger * log) {
  Logger::loggers.push_back(log);
}

void Logger::_log(logging_levels level, std::string message) {
  if (logfile == NULL) {
    FILE * logfile = fopen(fileName.c_str(), "a");
    if (logfile == NULL)
      pros::lcd::set_text(6, "not logging to " + fileName + "cause null" + message);

    if (minLevel == 0 || level >  minLevel || logfile == NULL) {
      return;
    }

    fputs(("[" + util::timestamp() + "] " + Logger::getLoggingLevelName(level) + ": " + message + "\n").c_str(), logfile);

    fclose(logfile);
  }

  if (minLevel == 0 || level > minLevel || logfile == NULL) {
    return;
  }

  fputs(("[" + util::timestamp() + "] " + Logger::getLoggingLevelName(level) + ": " + message + "\n").c_str(), logfile);
  fflush(logfile);
}

bool Logger::fileExists(std::string name) {
  if (FILE *file = fopen(name.c_str(), "r")) {
      fclose(file);
      return true;
  } else {
      return false;
	}
}

FILE * Logger::getFile() {
  return Logger::logfile;
}

void Logger::initializeDefaultLoggers() {
  Logger::init("stdout", stdout);

  int i = 0;
	while (fileExists(LOGS_PATH + util::ensureDigits(3, i) + ".log")) i++;
	Logger::init(LOGS_PATH + util::ensureDigits(3, i) + ".log");

  Logger::init("/usd/latest.log");
}

void Logger::init(std::string fileName) {
  Logger::init(LOGGING_DEFAULT_LEVEL, fileName);
}

void Logger::init(std::string fileName, FILE * file) {
  Logger::init(LOGGING_DEFAULT_LEVEL, fileName, file);
}

void Logger::init(logging_levels minLevel, std::string fileName) {
  FILE * file = fopen(fileName.c_str(), "w");
  if (file == NULL) {
    Logger::log(LOG_ERROR, "Could not open logger to a null file: " + fileName);
    return;
  }
  Logger * log = new Logger(minLevel, fileName, NULL);
  Logger::addNew(log);
}

void Logger::init(logging_levels minLevel, std::string fileName, FILE * file) {
  if (file == NULL) {
    Logger::log(LOG_ERROR, "Could not open logger to a null file: " + fileName);
    return;
  }
  Logger * log = new Logger(minLevel, fileName, file);
  Logger::addNew(log);
}

std::vector<Logger*> Logger::getLoggers() {
  return loggers;
}

void Logger::log(logging_levels level, std::string message) {
  for (const auto & l : Logger::loggers)
    l->_log(level, message);
}

std::string Logger::getLoggingLevelName(logging_levels level) {
		std::string levelmsg = "";
	  switch (level) {
	    case LOG_SEVERE:
	      levelmsg = "SEVERE";
	      break;
	    case LOG_ERROR:
	      levelmsg = "ERROR";
	      break;
	    case LOG_WARNING:
	      levelmsg = "WARNING";
	      break;
	    case LOG_INFO:
	      levelmsg = "INFO";
	      break;
	    default:
	      levelmsg = "DEBUG";
	      break;
	  }
		return levelmsg;
	}
