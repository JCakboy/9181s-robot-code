#ifndef _LCD_HPP_
#define _LCD_HPP_

#include "main.h"
#include <vector>

class LCD {
  private:
    static std::string status;
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

    // Sets the status on the lCD
    static void setStatus(std::string status);

    // Sets a line of text on the LCD
    static void setText(int line, std::string text);

    // Returns the status set on the LCD
    static std::string getStatus();

    // Returns a line of text on the LCD
    static std::string getText(int line);

};

#endif
