#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "main.h"
#include <fstream>
#include <vector>

/*
 * An enumeration specifying different logging levels
 *
 * The lower the assigned value, the more severe the logging level
 */

typedef enum logging_levels_e {
  E_LOGGING_DISABLE,
  E_LOGGING_SEVERE,
  E_LOGGING_ERROR,
  E_LOGGING_WARNING,
  E_LOGGING_INFO,
  E_LOGGING_DEBUG_MOTOR,
  E_LOGGING_DEBUG_LOGIC
} logging_levels;

/*
 * A class to log robot events
 *
 * Start a log output by calling init() withe specified path
 * Log to all files by using log()
 *
 * Log to PROS terminal by using the path "/ser/sout"
 * Log to a micro SD card by placing a "/usd/" prefix to your file name
 */

class Logger {
  private:
    static std::vector<Logger*> loggers;
    int minLevel;
    std::ofstream logfile;

    explicit Logger(logging_levels mLevel, std::string filename);


    static void addNew(Logger * log);

    void _log(logging_levels level, std::string message);

  public:
    // Initalize a log output stream using the default minimum log level. See below
    static void init(std::string filename);

    /*
     * Initlize a log output stream
     *
     * minLevel: the minimum logging level to log messages
     * filename: the path to initialize the log output stream
     */
    static void init(logging_levels minLevel, std::string filename);

    // Log the message to all log output streams with the specified logging level
    static void log(logging_levels level, std::string message);

};

#endif
