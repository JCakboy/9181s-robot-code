#include "main.h"

void LCD::initialize() {
  pros::lcd::initialize();

  pros::lcd::register_btn0_cb(LCD::onLeftButton);
  pros::lcd::register_btn1_cb(LCD::onCenterButton);
  pros::lcd::register_btn2_cb(LCD::onRightButton);

  LCD::setStatus("LCD initialized");
  setText(0, "Selected autonomous: " + std::to_string(selectedAutonomous));
}

void LCD::onLeftButton() {
  selectedAutonomous -= 1;
  setText(0, "Selected autonomous: " + std::to_string(selectedAutonomous));
}

void LCD::onCenterButton() {}

void LCD::onRightButton() {
  selectedAutonomous += 1;
  setText(0, "Selected autonomous: " + std::to_string(selectedAutonomous));
}

void LCD::setStatus(std::string status) {
  pros::lcd::set_text(0, "Status: " + status);
}

void LCD::setText(int line, std::string text) {
  pros::lcd::set_text(line + 1, text);
}
