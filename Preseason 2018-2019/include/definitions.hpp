#ifndef _DEFINITIONS_HPP_
#define _DEFINITIONS_HPP

#include <cmath>
#include "main.h"

/* May be needed, pros documentation is unclear
// Controller enumereration definitions
#define E_CONTROLLER_MASTER controller_id_e_t::E_CONTROLLER_MASTER // Main controller
#define E_CONTROLLER_PARTER controller_id_e_t::E_CONTROLLER_PARTER // Partner controller
*/

// Motor enumeration definitions
#define GEARSET_600 pros::c::E_MOTOR_GEARSET_06 // 6:1, 600 RPM, Blue gear
#define GEARSET_200 pros::c::E_MOTOR_GEARSET_18 // 18:1, 200 RPM, Green gear set
#define GEARSET_100 pros::c::E_MOTOR_GEARSET_06 // 36:1, 100 RPM, Red gear set
#define FWD false
#define REV true
#define ENCODER_DEGREES pros::c::E_MOTOR_ENCODER_DEGREES
#define ENCODER_REVOLUTIONS pros::c::E_MOTOR_ENCODER_REVOLUTIONS
#define ENCODER_COUNTS pros::c::E_MOTOR_ENCODER_COUNTS

// Math definitions
#define PI 3.141592653589793238

#endif
