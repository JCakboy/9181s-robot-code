#include "main.h"
#include <vector>

bool LCD::initLLEMU;
bool LCD::initCLCD;

std::string LCD::status = "";
std::vector<std::string> LCD::lines;
std::vector<pros::Controller*> LCD::controllers;
int LCD::cycles = 0;

void LCD::initializeLLEMU(pros::Controller * controllerMain, pros::Controller * controllerPartner) {
  // Initialize the brain LCD
  pros::lcd::initialize();

  // Register the brain LCD button handlers
  pros::lcd::register_btn0_cb(LCD::onLeftButton);
  pros::lcd::register_btn1_cb(LCD::onCenterButton);
  pros::lcd::register_btn2_cb(LCD::onRightButton);

  // Populate the internal line cache
  for (int i = 0; i < 20; i++)
    lines.push_back("");

  // Store the controllers
  LCD::controllers.push_back(controllerMain);
  LCD::controllers.push_back(controllerPartner);

  // Signal the completion of initialization and update the screen
  LCD::setStatus("LCD initialized");
  LCD::initLLEMU = true;
}

void LCD::onLeftButton() {
  // Decrements the selected autonomous and update the LCD
  selectedAutonomous--;
  updateScreen(true);
}

void LCD::onCenterButton() {
  // No action set
  updateScreen(true);
}

void LCD::onRightButton() {
  // Increments the selected autonomous and update the LCD
  selectedAutonomous++;
  updateScreen(true);
}

void LCD::updateScreen() {
  updateScreen(false);
}

void LCD::updateScreen(bool forceController) {
  // Updates the selected autonomous on the LCD
  setText(0, "Selected autonomous: " + LCD::getAutonomousName());

  // Check to see whether the controller should updates its LCD
  if (cycles == 2 || forceController)
    LCD::setControllerText("Auto: " + LCD::getAutonomousName());
  LCD::cycles++;
  if (LCD::cycles > 200 || forceController) LCD::cycles = 0;
}

void LCD::printDebugInformation() {
  // Print sensor information
  LCD::setText(2, std::to_string(ports::gyro->getValue()));
  LCD::setText(3, std::to_string(ports::pid->desiredHeading));
  // LCD::setText(3, std::to_string(ports::leftUltrasonic->get_value()));
  // LCD::setText(4, std::to_string(ports::rightUltrasonic->get_value()));
}

void LCD::setStatus(std::string status) {
  // Sets the status on the LCD
  pros::lcd::set_text(0, "Status: " + status);
  LCD::status = status;
  lines.at(0) = "Status: " + status;
}

void LCD::setControllerText(std::string text) {
  // Make sure the text is padded to clear the existing text
  while (text.size() < 18) text += " ";
  // Iterate through the controllers and, if connected, update their text
  for (const auto & controller : LCD::controllers)
    if (controller != NULL && controller->is_connected())
      controller->set_text(0, 0, text.c_str());
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

std::string LCD::getAutonomousName() {
  // Return the selected autonomous name
  switch (selectedAutonomous) {
    case 0:
      return "None";
    case 1:
      return "Blue Tall";
    case 2:
      return "Red Tall";
    case 3:
      return "Blue Flat";
    case 4:
      return "Red Flat";
    case 5:
      return "Skills";
    case 6:
      return "Drv. Skills";
    default:
      return (std::to_string(selectedAutonomous) + (isAutonomousRed() ? " (Red)" : " (Blue)"));
  }
}

bool LCD::isAutonomousBlue() {
  return (selectedAutonomous % 2 != 0) && selectedAutonomous != 5;
}

bool LCD::isAutonomousRed() {
  return (selectedAutonomous % 2 == 0 || selectedAutonomous == 5) && selectedAutonomous;
}

bool LCD::isAutonomousSkills() {
  return selectedAutonomous == 5 || selectedAutonomous == 6;
}
