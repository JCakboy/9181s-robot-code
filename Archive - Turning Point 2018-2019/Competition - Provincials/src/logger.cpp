#include "main.h"
#include "logger.hpp"
#include <cstdio>

std::vector<Logger*> Logger::loggers;

Logger::Logger(logging_levels mLevel, std::string fileName, FILE * file) {
  // Store the minimum level, file name, and file
  Logger::minLevel = mLevel;
  Logger::fileName = fileName;
  Logger::logfile = file;
}

void Logger::addNew(Logger * log) {
  // Adds the logger to the list of active loggers
  Logger::loggers.push_back(log);
}

void Logger::_log(logging_levels level, std::string message) {
  if (logfile == NULL) {
    // Log file is null, attempt to open a new file with filename
    FILE * logfile = fopen(fileName.c_str(), "a");
    if (logfile == NULL)
      // Log file is still null, abort
      pros::lcd::set_text(6, "not logging to " + fileName + "cause null" + message);

    if (minLevel == 0 || level > minLevel || logfile == NULL)
      // Ignore if the logging is disabled, log file is null, or the log level is not significant
      return;

    // Write the formatted log with the file
    fputs(("[" + util::timestamp() + "] " + Logger::getLoggingLevelName(level) + ": " + message + "\n").c_str(), logfile);

    // Close the file to force the flush and write
    fclose(logfile);
  }

  if (minLevel == 0 || level > minLevel || logfile == NULL)
    // Ignore if the logging is disabled, log file is null, or the log level is not significant
    return;

  // Write the formatted log with the file
  fputs(("[" + util::timestamp() + "] " + Logger::getLoggingLevelName(level) + ": " + message + "\n").c_str(), logfile);

  // Close the file to force the flush and write
  fflush(logfile);
}

bool Logger::fileExists(std::string name) {
  // Check whether the file has a read input stream
  if (FILE *file = fopen(name.c_str(), "r")) {
      fclose(file);
      return true;
  } else
      return false;
}

FILE * Logger::getFile() {
  // Return the log file
  return Logger::logfile;
}

void Logger::initializeDefaultLoggers() {
  // Initialize a log pointing to the USB output
  Logger::init("stdout", stdout);

  // Initialize a logger to a numbered log file
  int i = 0;
	while (fileExists(LOGS_PATH + util::ensureDigits(3, i) + ".log")) i++;
	Logger::init(LOGS_PATH + util::ensureDigits(3, i) + ".log");

  // Initialize a logger to "/usd/latest.log"
  Logger::init("/usd/latest.log");
}

void Logger::init(std::string fileName) {
  // Initialize a logger to the filename with the defaut logging level
  Logger::init(LOGGING_DEFAULT_LEVEL, fileName);
}

void Logger::init(std::string fileName, FILE * file) {
  // Initialize a logger to the file with the defaut logging level
  Logger::init(LOGGING_DEFAULT_LEVEL, fileName, file);
}

void Logger::init(logging_levels minLevel, std::string fileName) {
  // Initialize a logger to the filename with the given logging level
  FILE * file = fopen(fileName.c_str(), "w");
  if (file == NULL) {
    Logger::log(LOG_ERROR, "Could not open logger to a null file: " + fileName);
    return;
  }
  Logger * log = new Logger(minLevel, fileName, NULL);
  Logger::addNew(log);
}

void Logger::init(logging_levels minLevel, std::string fileName, FILE * file) {
  // Initialize a logger to the file with the given logging level
  if (file == NULL) {
    Logger::log(LOG_ERROR, "Could not open logger to a null file: " + fileName);
    return;
  }
  Logger * log = new Logger(minLevel, fileName, file);
  Logger::addNew(log);
}

std::vector<Logger*> Logger::getLoggers() {
  // Returns the list of active loggers
  return loggers;
}

void Logger::log(logging_levels level, std::string message) {
  // Passes the logging message to all active loggers
  for (const auto & l : Logger::loggers)
    l->_log(level, message);
}

std::string Logger::getLoggingLevelName(logging_levels level) {
  // Returns the plaintext name of a logging level
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
