#include "main.h"

void on_center_button() {

}

void initialize() {
  pros::c::lcd_initialize();

  pros::c::lcd_register_btn1_cb(on_center_button);
}

// the following functions don't work presently because comp. control
// hasn't been fully implemented
void disabled() {}
void competition_initialize() {}
