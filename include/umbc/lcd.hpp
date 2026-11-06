/**
 * @file umbc/lcd.hpp
 *
 * Contains the prototype for the LCD class. The LCD class contains helpers
 * for the LLEMU used by the robot and menu.
 */

#ifndef _UMBC_LCD_HPP_
#define _UMBC_LCD_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "pros/colors.hpp"
#include "liblvgl/lvgl.h"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

    /**
     * Contains helpers for clearing, coloring and reading the buttons of the LLEMU
     */
    class LCD {

        private:

            // number of text lines on the LLEMU
            static constexpr std::int16_t line_count = 8;

            // LVGL style selector for the alliance style
            static constexpr lv_style_selector_t alliance_style_selector =
                static_cast<lv_style_selector_t>(LV_PART_MAIN) | static_cast<lv_style_selector_t>(LV_STATE_USER_1);

            static lv_style_t alliance_style;               // style applied for the alliance colors
            static bool is_alliance_style_initialized;      // whether the alliance style has been initialized

            /**
             * @brief Recursively searches for the LLEMU text panel
             *
             * @param object: LVGL object to search from
             *
             * @return LLEMU text panel if found. Otherwise nullptr
             */
            static lv_obj_t* find_screen(lv_obj_t* object);

            /**
             * @brief Retrieves the LLEMU text panel from the active screen
             *
             * @return LLEMU text panel if the LCD is initialized and the panel was found.
             *      Otherwise nullptr
             */
            static lv_obj_t* get_screen();


        public:

            /**
             * @brief Clears LCD text without deleting its labels
             *
             * @note The bundled liblvgl 9.2.0 lcd::clear() deletes labels that
             *      subsequent set_text() calls still use.
             */
            static void clear();

            /**
             * @brief Sets the LCD text panel's background and text colors
             *
             * @param background_color: Background color
             * @param text_color: Text color
             */
            static void set_colors(pros::Color background_color, pros::Color text_color);

            /**
             * @brief Restores the LCD text panel's original background and text colors
             */
            static void reset_colors();

            /**
             * @brief Reads the three LCD buttons, masking unused state bits
             *
             * @return Bitmask of LCD_BTN_LEFT, LCD_BTN_CENTER and LCD_BTN_RIGHT.
             *      Zero if no button is pressed.
             *
             * @note The bundled LLEMU leaves unused button-state bits uninitialized.
             */
            static std::uint8_t read_buttons();
    };
}

#endif // _UMBC_LCD_HPP_
