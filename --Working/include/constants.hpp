#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 */

// Whether or not a microSD card is inserted
#define SD_INSERTED true


// Refresh rate for ball launcher
#define TASK_BALLLAUNCHER_HZ 2

// Refresh rate for opcontrol
#define TASK_OPCONTROL_HZ 50

// Refresh rate for watchdog
#define TASK_WATCHDOG_HZ 1


// How long to wait for a mutex to be released
#define MUTEX_WAIT_TIME 1000 // in ms


// Threshold for horizontally flipping while reversing
#define MOTOR_REVERSE_FLIP_THRESHOLD -42.3 // should be negative value


// Whether the ball launcher should automatically load after a launch
#define BALL_LAUNCHER_AUTO_LOAD true

// How long to wait between pneumatic updates
#define PNEUMATIC_LAUNCHER_WAIT_TIME 250 // in ms

// How long to run the elastic launcher loading motor
#define ELASTIC_LAUNCHER_MOTOR_TIME 1000 // in ms


// Default minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO

// Minimum alert level
#define ALERT_LEVEL E_LOGGING_INFO

// Minimum level to vibrate controller on alert
#define VIBRATE_LEVEL E_LOGGING_WARNING


// For the following constants, use -1 if alerts should be disabled for that category

// Threshold for low battery alert
#define LOW_BATTERY_THRESHOLD 0 // unknown units

// Threshold for very low battery alert
#define VERY_LOW_BATTERY_THRESHOLD 0 // unknown units

// Threshold for slow task alert
#define TASK_SLOW_THRESHOLD 0.10 // in pct

// Threshold for hanging task alert
#define TASK_HANG_THRESHOLD 0 // in pct

// How long to wait before a motor watcher cycle is deemed frozen
#define MOTORWATCHER_MUTEX_WAIT_TIME (1000 / TASK_OPCONTROL_HZ) // in ms

// Threshold for locked up motor alert
#define MOTOR_LOCKED_UP_THRESHOLD 3 // in seconds

// Threshold for hot motor alert
#define MOTOR_HOT_THRESHOLD 45 // in °C

// Threshold for overheating motor alert
#define MOTOR_OVERHEATING_THRESHOLD 55 // in °C

// Threshold for low motor load alert
#define MOTOR_LOW_LOAD_THRESHOLD 75 // in efficiency pct

// Threshold for high motor load alert
#define MOTOR_HIGH_LOAD_THRESHOLD 20 // in efficiency pct

// Threshold for motor overload alert
#define MOTOR_OVERLOAD_THRESHOLD 10 // in efficiency pct

#endif
