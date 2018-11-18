#ifndef _LCD_HPP_
#define _LCD_HPP_

#include "main.h"

class LCD {
  public:
    static void initialize();

    static void onLeftButton();
    static void onCenterButton();
    static void onRightButton();

    static void setStatus(std::string status);
    static void setText(int line, std::string text);

};

#endif
