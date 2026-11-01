/**
 * @file umbc/lcd.hpp
 *
 * Contains the prototypes for LCD helpers used by the robot and menu.
 */

#ifndef _UMBC_LCD_HPP_
#define _UMBC_LCD_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "pros/colors.hpp"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

    /**
     * @brief Clears LCD text without deleting its labels
     *
     * @note The bundled liblvgl 9.2.0 lcd::clear() deletes labels that
     *      subsequent set_text() calls still use.
     */
    void clear_lcd();

    /**
     * @brief Sets the LCD text panel's background and text colors
     *
     * @param background_color: Background color
     * @param text_color: Text color
     */
    void set_lcd_colors(pros::Color background_color, pros::Color text_color);

    /**
     * @brief Restores the LCD text panel's original background and text colors
     */
    void reset_lcd_colors();

    /**
     * @brief Reads the three LCD buttons, masking unused state bits
     *
     * @return Bitmask of LCD_BTN_LEFT, LCD_BTN_CENTER and LCD_BTN_RIGHT.
     *      Zero if no button is pressed.
     *
     * @note The bundled LLEMU leaves unused button-state bits uninitialized.
     */
    std::uint8_t read_lcd_buttons();
}

#endif // _UMBC_LCD_HPP_
