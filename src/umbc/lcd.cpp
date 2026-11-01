/**
 * @file umbc/lcd.cpp
 *
 * Contains the implementation for LCD helpers used by the robot and menu.
 */

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


void umbc::clear_lcd() {

    for (std::int16_t line = 0; line < 8; ++line) {
        if (!pros::lcd::clear_line(line)) {
            ERROR("failed to clear LCD line " << line);
            return;
        }
    }
}


std::uint8_t umbc::read_lcd_buttons() {
    return pros::lcd::read_buttons() & (LCD_BTN_LEFT | LCD_BTN_CENTER | LCD_BTN_RIGHT);
}
