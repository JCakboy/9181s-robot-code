#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "main.h"

/*
 * A header file listing constant settings
 */

// How long to wait for a mutex to be released
 #define MUTEX_WAIT_TIME 1000 // in ms

// Threshhold for horizontally flipping while reversing
 #define MOTOR_REVERSE_FLIP_THRESHOLD -42.3 // should be negative value

// How long to run the elastic launcher loading motor
#define ELASTIC_LAUNCHER_MOTOR_TIME 1000 // in ms

#endif
