#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "main.h"
#include <fstream>
#include <unordered_map>
#include <vector>

/*
 * A wrapper class for pros::Controller to provide support for recording and playback
 *
 * See https://pros.cs.purdue.edu/v5/api/cpp/misc.html#pros-controller for more documentation
 */

class RecordedController {
  private:
    pros::Controller * controller;
    int state; // 0: standard, 1: recording, 2: playback

    std::unordered_map<int, int> digitalLastKnown;

    std::string recfile;
    std::unordered_map<int, std::vector<int>> analogStack;
    std::unordered_map<int, std::vector<int>> digitalStack;

    void (*playbackComplete)();

    static void pbf();

    static int popFront(std::vector<int> & stack);

    bool readFile(std::string file);

    void writeFile(std::string file);

  public:
    // Wraps the specified controller
    explicit RecordedController(pros::Controller & controller);

    // Creates and wraps the specified controller
    explicit RecordedController(pros::controller_id_e_t id);

    int clear();

    int clear_line(int line);

    int is_connected();

    int get_analog(pros::controller_analog_e_t channel);

    int get_battery_capacity();

    int get_battery_level();

    int get_digital(pros::controller_digital_e_t button);

    int get_digital_new_press(pros::controller_digital_e_t button);

    int set_text(std::uint8_t line, std::uint8_t col, const char* str);

    // Begins the recording or stops and saves the recorded inputs
    void record(bool record);

    // Plays back the given file, calling the given function when completed
    void playback(std::string file, void (*f)());

    // Aborts the current playback
    void playback();

};

#endif
