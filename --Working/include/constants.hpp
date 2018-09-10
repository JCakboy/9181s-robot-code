#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 */

// Refresh rate for opcontrol
#define TASK_OPCONTROL_HZ 50

// How long to wait for a mutex to be released
#define MUTEX_WAIT_TIME 1000 // in ms

// Threshhold for horizontally flipping while reversing
#define MOTOR_REVERSE_FLIP_THRESHOLD -42.3 // should be negative value

// Whether the ball launcher should automatically load after a launch
#define BALL_LAUNCHER_AUTO_LOAD true

// How long to run the elastic launcher loading motor
#define ELASTIC_LAUNCHER_MOTOR_TIME 1000 // in ms

// Minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO

#endif
