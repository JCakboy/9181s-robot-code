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


// Amount to move the puncher back for priming
#define PUNCHER_PRIME_AMOUNT 260 // in degrees
// Whether to use the vision sensor for puncher alignment
#define PUNCHER_USE_VISION_ALIGNMENT true
// Whether to strafe for vision sensor alignment
#define PUNCHER_STRAFE_ALIGN true
// THe threshold for vision sensor alignemmt
#define PUNCHER_VISION_ALIGNMENT_THRESHOLD 5 // in pixels
// Whether to rumble the controller when complete
#define PUNCHER_VISION_ALIGNMENT_RUMBLE true
// The pattern to rumble the controller when failed
#define PUNCHER_VISION_ALIGNMENT_FAILURE_RUMBLE_PATTERN "-"
// The pattern to rumble the controller when complete
#define PUNCHER_VISION_ALIGNMENT_RUMBLE_PATTERN ". "

// Default logs path
#define LOGS_PATH "/usd/logs/"

// Default minimum logging level
#define LOGGING_DEFAULT_LEVEL E_LOGGING_INFO


// Minimum alert level
#define ALERT_LEVEL E_LOGGING_INFO

// Minimum level to vibrate controller on alert
#define VIBRATE_LEVEL E_LOGGING_WARNING

// Whether to mirror alerts to the partner controller
#define ALERT_MIRROR_PARTNER true

#endif
