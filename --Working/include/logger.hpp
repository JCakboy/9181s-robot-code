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
    std::string fileName;
    FILE * logfile;

    explicit Logger(logging_levels mLevel, std::string filename, FILE * file);

    static void addNew(Logger * log);

    void _log(logging_levels level, std::string message);

    static bool fileExists(std::string name);

  public:
    // Returns the file the logger is associated to
    FILE * getFile();

    // Intializes the default loggers, include serial output and logging files on the microSD card
    static void initializeDefaultLoggers();

    // Initalize a log output stream to a file with the given name using the default minimum log level. See below
    static void init(std::string filename);

    // Initialize a log output stream to the given file stream using the default minimum log level. See below
    static void init(std::string fileName, FILE * file);

    // Initlize a log output stream using a given level to a file with the given name. See below
    static void init(logging_levels minLevel, std::string filename);

    /*
     * Initialize a log output stream
     *
     * minLevel: the minimum logging level to log messages
     * file: the file to initialize the log output stream
     */
    static void init(logging_levels minLevel, std::string fileName, FILE * file);

    // Returns the full list of active loggers
    static std::vector<Logger*> getLoggers();

    // Log the message to all log output streams with the specified logging level
    static void log(logging_levels level, std::string message);

    // Gets the plaintext name of the logging level
    static std::string getLoggingLevelName(logging_levels level);
};

#endif
