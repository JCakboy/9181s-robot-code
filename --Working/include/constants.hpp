#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 */

// Whether or not a microSD card is inserted
#define SD_INSERTED true


// How long to wait for a mutex to be released
#define MUTEX_WAIT_TIME 1000 // in ms


// Threshold for horizontally flipping while reversing
#define MOTOR_REVERSE_FLIP_THRESHOLD -55 // should be negative value

// Default threshold for move relative
#define MOTOR_MOVE_RELATIVE_THRESHOLD 15

// Maximum speed for move relative
#define MOTOR_MOVE_RELATIVE_MAX_SPEED 100

// Proportional constant for move relative
#define MOTOR_MOVE_RELATIVE_KP .43

// Default minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO

// Minimum alert level
#define ALERT_LEVEL E_LOGGING_INFO

// Minimum level to vibrate controller on alert
#define VIBRATE_LEVEL E_LOGGING_WARNING

// Whether to mirror alerts to the partner controller
#define ALERT_MIRROR_PARTNER true

#endif
