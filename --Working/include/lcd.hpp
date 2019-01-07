#ifndef _LCD_HPP_
#define _LCD_HPP_

#include "main.h"
#include <vector>

class LCD {
  private:
    // The current status
    static std::string status;
    // The current text on the LCD, set through this class
    static std::vector<std::string> lines;

  public:
    // Initializes the LCD
    static void initialize();

    // The method called on a left LCD button press
    static void onLeftButton();

    // The method called on a center LCD button press
    static void onCenterButton();

    // The method called on a right LCD button press
    static void onRightButton();

    // Updates the selected autonomous on the lCD
    static void updateSelectedAutonomous();

    // Sets the status on the lCD
    static void setStatus(std::string status);

    // Sets a line of text on the LCD
    static void setText(int line, std::string text);

    // Returns the current status set on the LCD
    static std::string getStatus();

    // Returns a line of text on the LCD
    static std::string getText(int line);

};

#endif
