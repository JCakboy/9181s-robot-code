#ifndef _LCD_HPP_
#define _LCD_HPP_

#include "main.h"
#include <vector>

class LCD {
  private:
    // Whether the LLEMU is initialized
    static bool initLLEMU;
    // Whether the custom LCD is initialied
    static bool initCLCD;

    // The current status
    static std::string status;
    // The current text on the LCD, set through this class
    static std::vector<std::string> lines;
    // The list of controllers
    static std::vector<pros::Controller*> controllers;
    // The amount of cycles since the last controller text update, used to schedule multiple lines
    static int cycles;
  public:
    // Initializes the LLEMU
    static void initializeLLEMU(pros::Controller * controllerMain, pros::Controller * controllerPartner);

    // The method called on a left LCD button press, only effective if the LLEMU is initialized
    static void onLeftButton();

    // The method called on a center LCD button press, only effective if the LLEMU is initialized
    static void onCenterButton();

    // The method called on a right LCD button press, only effective if the LLEMU is initialized
    static void onRightButton();


    // Updates the selected autonomous on the lCD
    static void updateScreen();

    // Updates the selected autonomous on the lCD, specifying whether to force an update onto the controllers' LCD
    static void updateScreen(bool forceController);

    // Prints debug information to the LCD
    static void printDebugInformation();

    // Sets the status on the lCD
    static void setStatus(std::string status);

    // Sets a line of text on the LCD
    static void setText(int line, std::string text);

    // Sets the line of text on the controller LCD
    static void setControllerText(std::string text);

    // Returns the current status set on the LCD
    static std::string getStatus();

    // Returns a line of text on the LCD
    static std::string getText(int line);

    // Returns the name of the selected autnomous
    static std::string getAutonomousName();

    // Whether the selected autonomous is set for blue side
    static bool isAutonomousBlue();

    // Whether the selected autonomous is set for red side
    static bool isAutonomousRed();

};

#endif
