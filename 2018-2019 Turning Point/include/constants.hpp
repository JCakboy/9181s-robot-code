#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 *
 * These values are constant each compilation, but may vary between compilations
 */

// Whether or not a microSD card is inserted
#define SD_INSERTED true


// How long to wait for a mutex to be released
#define MUTEX_WAIT_TIME 1000 // in ms

// Whether, by default, to brake the motors
#define MOTOR_DEFAULT_BRAKE true

// Threshold for horizontally flipping while reversing
#define MOTOR_REVERSE_FLIP_THRESHOLD -55 // should be negative value

// Default threshold for move relative
#define MOTOR_MOVE_RELATIVE_THRESHOLD 8 // in degrees

// Maximum speed for move relative
#define MOTOR_MOVE_RELATIVE_MAX_SPEED 100

// Proportional constant for move relative
#define MOTOR_MOVE_RELATIVE_KP .43


// Default logs path
#define LOGS_PATH "/usd/logs/"

// Default minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO


#endif
