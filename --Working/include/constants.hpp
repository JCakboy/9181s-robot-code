#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 */

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

// How long to run the elastic launcher loading motor
#define ELASTIC_LAUNCHER_MOTOR_TIME 1000 // in ms


// Default minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO

// Minimum alert level
#define ALERT_LEVEL E_LOGGING_WARNING


// Threshold for low battery alert
#define LOW_BATTERY_THRESHOLD 0

// Threshold for very low battery alert
#define VERY_LOW_BATTERY_THRESHOLD 0

// Threshold for slow task alert
#define TASK_SLOW_THRESHOLD 0.10

// Threshold for hanging task alert
#define TASK_HANG_THRESHOLD 0


#endif
