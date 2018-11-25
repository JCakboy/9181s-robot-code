#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	/*
	The V5 screen is 480 pixels wide by 272 pixels high.
	We use 32 lines at the top for the menu bar, so the area available for user code is 480 x 240 pixels.
	*/

	// Create and load the screen
	lv_obj_t * screen = lv_obj_create(NULL, NULL);
	lv_scr_load(screen);

	lv_obj_t * statusText = lv_ta_create(screen, NULL);
	lv_ta_set_cursor_type(statusText, LV_CURSOR_NONE);
	lv_ta_set_one_line(statusText, true);
	lv_ta_set_style(statusText, LV_TA_STYLE_BG, &lv_style_transp);
	lv_ta_set_text(statusText, "Status: Initializing screen");

	lv_obj_t * statusLine = lv_line_create(screen, NULL);
	lv_point_t statusLinePoints[] = {{0, 32}, {480, 32}};
	lv_line_set_points(statusLine, statusLinePoints, 2);



}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
