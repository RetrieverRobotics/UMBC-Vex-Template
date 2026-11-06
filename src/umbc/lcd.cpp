/**
 * @file umbc/lcd.cpp
 *
 * Contains the implementation for the LCD class. The LCD class contains helpers
 * for the LLEMU used by the robot and menu.
 */

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "umbc.h"
#include "liblvgl/lvgl.h"
#include "liblvgl/osal/lv_os.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


lv_style_t umbc::LCD::alliance_style;
bool umbc::LCD::is_alliance_style_initialized = false;


lv_obj_t* umbc::LCD::find_screen(lv_obj_t* object) {

    std::uint32_t child_count = lv_obj_get_child_count(object);
    bool is_lcd_screen = (umbc::LCD::line_count == child_count);

    // the LLEMU text panel contains a direct label child for each line
    for (std::uint32_t i = 0; i < child_count && is_lcd_screen; ++i) {
        is_lcd_screen = lv_obj_check_type(lv_obj_get_child(object, i), &lv_label_class);
    }

    if (true == is_lcd_screen) {
        return object;
    }

    for (std::uint32_t i = 0; i < child_count; ++i) {
        lv_obj_t* screen = umbc::LCD::find_screen(lv_obj_get_child(object, i));
        if (nullptr != screen) {
            return screen;
        }
    }

    return nullptr;
}


lv_obj_t* umbc::LCD::get_screen() {

    if (false == pros::lcd::is_initialized()) {
        ERROR("cannot update LCD colors; LCD is not initialized");
        return nullptr;
    }

    lv_obj_t* active_screen = lv_screen_active();
    lv_obj_t* lcd_screen = (nullptr == active_screen) ? nullptr : umbc::LCD::find_screen(active_screen);

    if (nullptr == lcd_screen) {
        ERROR("cannot update LCD colors; LCD text panel was not found");
    }

    return lcd_screen;
}


void umbc::LCD::set_colors(pros::Color background_color, pros::Color text_color) {

    lv_lock();
    lv_obj_t* lcd_screen = umbc::LCD::get_screen();

    if (nullptr != lcd_screen) {

        if (false == umbc::LCD::is_alliance_style_initialized) {
            lv_style_init(&umbc::LCD::alliance_style);
            umbc::LCD::is_alliance_style_initialized = true;
        }

        // preserve the LLEMU's original colors beneath the alliance style
        lv_obj_remove_style(lcd_screen, &umbc::LCD::alliance_style, umbc::LCD::alliance_style_selector);
        lv_style_set_bg_color(&umbc::LCD::alliance_style, lv_color_hex(static_cast<std::uint32_t>(background_color)));
        lv_style_set_text_color(&umbc::LCD::alliance_style, lv_color_hex(static_cast<std::uint32_t>(text_color)));
        lv_obj_add_style(lcd_screen, &umbc::LCD::alliance_style, umbc::LCD::alliance_style_selector);
        lv_obj_add_state(lcd_screen, LV_STATE_USER_1);
    }
    lv_unlock();
}


void umbc::LCD::reset_colors() {

    lv_lock();
    lv_obj_t* lcd_screen = umbc::LCD::get_screen();

    if (nullptr != lcd_screen && true == umbc::LCD::is_alliance_style_initialized) {
        lv_obj_remove_state(lcd_screen, LV_STATE_USER_1);
        lv_obj_remove_style(lcd_screen, &umbc::LCD::alliance_style, umbc::LCD::alliance_style_selector);
    }
    lv_unlock();
}


void umbc::LCD::clear() {

    for (std::int16_t line = 0; line < umbc::LCD::line_count; ++line) {
        if (!pros::lcd::clear_line(line)) {
            ERROR("failed to clear LCD line " << line);
            return;
        }
    }
}


std::uint8_t umbc::LCD::read_buttons() {
    return pros::lcd::read_buttons() & (LCD_BTN_LEFT | LCD_BTN_CENTER | LCD_BTN_RIGHT);
}
