#ifndef _DEFINITIONS_HPP_
#define _DEFINITIONS_HPP

#include "main.h"

/*
 * A header file simplifying enumeration values,
 * similar to #define PROS_USE_SIMPLE_NAMES
 */

// Controller enumereration definitions
#define CONTROLLER_MAIN E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER E_CONTROLLER_PARTNER
#define STICK_LEFT_X E_CONTROLLER_ANALOG_LEFT_X
#define STICK_LEFT_Y E_CONTROLLER_ANALOG_LEFT_Y
#define STICK_RIGHT_X E_CONTROLLER_ANALOG_RIGHT_X
#define STICK_RIGHT_Y E_CONTROLLER_ANALOG_RIGHT_Y
#define BUTTON_UP E_CONTROLLER_DIGITAL_UP
#define BUTTON_DOWN E_CONTROLLER_DIGITAL_DOWN
#define BUTTON_LEFT E_CONTROLLER_DIGITAL_LEFT
#define BUTTON_RIGHT E_CONTROLLER_DIGITAL_RIGHT
#define BUTTON_A E_CONTROLLER_DIGITAL_A
#define BUTTON_B E_CONTROLLER_DIGITAL_B
#define BUTTON_X E_CONTROLLER_DIGITAL_X
#define BUTTON_Y E_CONTROLLER_DIGITAL_Y
#define BUTTON_L1 E_CONTROLLER_DIGITAL_L1
#define BUTTON_L2 E_CONTROLLER_DIGITAL_L2
#define BUTTON_R1 E_CONTROLLER_DIGITAL_R1
#define BUTTON_R2 E_CONTROLLER_DIGITAL_R2

// Motor enumeration definitions
#define GEARSET_600 pros::c::E_MOTOR_GEARSET_06 // 6:1, 600 RPM, Blue gear
#define GEARSET_200 pros::c::E_MOTOR_GEARSET_18 // 18:1, 200 RPM, Green gear set
#define GEARSET_100 pros::c::E_MOTOR_GEARSET_36 // 36:1, 100 RPM, Red gear set
#define FWD false
#define REV true
#define ENCODER_DEGREES pros::c::E_MOTOR_ENCODER_DEGREES
#define ENCODER_REVOLUTIONS pros::c::E_MOTOR_ENCODER_REVOLUTIONS
#define ENCODER_COUNTS pros::c::E_MOTOR_ENCODER_COUNTS
#define BRAKE_COAST pros::c::E_MOTOR_BRAKE_COAST
#define BRAKE_BRAKE pros::c::E_MOTOR_BRAKE_BRAKE
#define BRAKE_HOLD pros::c::E_MOTOR_BRAKE_HOLD

// Math definitions
#define PI 3.141592653589793238

#endif
