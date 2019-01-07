#include "main.h"
#include <vector>

std::string LCD::status = "";
std::vector<std::string> LCD::lines;

void LCD::initialize() {
  pros::lcd::initialize();

  pros::lcd::register_btn0_cb(LCD::onLeftButton);
  pros::lcd::register_btn1_cb(LCD::onCenterButton);
  pros::lcd::register_btn2_cb(LCD::onRightButton);

  for (int i = 0; i < 20; i++)
    lines.push_back("");

  LCD::setStatus("LCD initialized");
  updateSelectedAutonomous();
}

void LCD::onLeftButton() {
  // Decrements the selected autonomous and update the LCD
  selectedAutonomous++;
  updateSelectedAutonomous();
}

void LCD::onCenterButton() {
  // No action set
}

void LCD::onRightButton() {
  // Increments the selected autonomous and update the LCD
  selectedAutonomous--;
  updateSelectedAutonomous();
}

void LCD::updateSelectedAutonomous() {
  // Updates the selected autonomous on the LCD
  setText(0, "Selected autonomous: " + std::to_string(selectedAutonomous));
}

void LCD::setStatus(std::string status) {
  // Sets the status on the LCD
  pros::lcd::set_text(0, "Status: " + status);
  LCD::status = status;
  lines.at(0) = "Status: " + status;
}

void LCD::setText(int line, std::string text) {
  // Sets the text at a given line on the LCD
  if (line > 9 || line < 0)
    return;
  pros::lcd::set_text(line + 1, text);
  lines.at(line + 1) = text;
}

std::string LCD::getStatus() {
  // Returns the status on the LCD
  return LCD::status;
}

std::string LCD::getText(int line) {
  // Returns the text at a given line on the LCD
  if (line > 9 || line < 0)
    return "";
  return lines.at(line + 1);
}
