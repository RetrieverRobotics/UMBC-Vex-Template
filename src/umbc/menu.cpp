/**
 * @file umbc/menu.cpp
 *
 * Contains the implementation for the Menu class. The Menu class contains
 * various sub-menus for putting the robot into different modes.
 */

// standard libraries
#include <cstdint>
#include <string>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


umbc::Robot::MatchType umbc::Menu::menu_match() {

    umbc::Robot::MatchType match_type = umbc::Robot::MatchType::NONE;
    std::uint8_t btn_press = 0;

    // display match type menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Select Match Type");
    pros::lcd::set_text(3, "1) Tournament");
    pros::lcd::set_text(4, "2) Skills");
    pros::lcd::set_text(5, "3) Utilities");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for match type selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();
        
        // set robot's match type to tournament
        if (LCD_BTN_LEFT == btn_press) {
            match_type = umbc::Robot::MatchType::TOURNAMENT;
            INFO("tournament selected for match type");
        }
        
        // set robot's match type to skills
        else if (LCD_BTN_CENTER == btn_press) {
            match_type = umbc::Robot::MatchType::SKILLS;
            INFO("skills selected for competition mode");
        }
        
        // open the utilities sub-menu
        else if (LCD_BTN_RIGHT == btn_press) {
            match_type = umbc::Robot::MatchType::NONE;
            INFO("utilities selected");
        }
        
        // no match type selected
        else {
            btn_press = 0;
        }

        // polling delay to not hog processor
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // wait for the LCD button to be released
    while (0 != umbc::read_lcd_buttons()) {
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // clear the LCD
    umbc::clear_lcd();

    return match_type;
}


umbc::Robot::AllianceColor umbc::Menu::menu_alliance() {

    umbc::Robot::AllianceColor alliance_color = umbc::Robot::AllianceColor::NONE;
    std::uint8_t btn_press = 0;

    // display alliance color sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Select Alliance Color");
    pros::lcd::set_text(3, "1) Red");
    pros::lcd::set_text(4, "2) Blue");
    pros::lcd::set_text(5, "3) Back");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for alliance color selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();

        // red alliance selected
        if (LCD_BTN_LEFT == btn_press) {
            alliance_color = umbc::Robot::AllianceColor::RED;
            INFO("red selected for alliance color");
        }

        // blue alliance selected
        else if (LCD_BTN_CENTER == btn_press) {
            alliance_color = umbc::Robot::AllianceColor::BLUE;
            INFO("blue selected for alliance color");
        }

        // no alliance color selected
        else if (LCD_BTN_RIGHT == btn_press) {
            alliance_color = umbc::Robot::AllianceColor::NONE;
            INFO("no alliance color selected");
        }

        // no valid alliance colors selected
        else {
            btn_press = 0;
        }

        // polling delay to not hog processor
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // wait for the LCD button to be released
    while (0 != umbc::read_lcd_buttons()) {
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // clear the LCD
    umbc::clear_lcd();

    return alliance_color;
}


umbc::Robot::Mode umbc::Menu::menu_mode() {

    umbc::Robot::Mode mode = umbc::Robot::Mode::NONE;
    std::uint8_t btn_press = 0;

    // display robot mode sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Select Mode");
    pros::lcd::set_text(3, "1) Competition");
    pros::lcd::set_text(4, "2) Train Autonomous");
    pros::lcd::set_text(5, "3) Back");
    
    // continue polling LCD buttons until one is pressed
    INFO("waiting for mode selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();
        
        // competition robot mode selected
        if (LCD_BTN_LEFT == btn_press) {
            mode = umbc::Robot::Mode::COMPETITION;
            INFO("competition selected for mode");
        }
        
        // train autonomous robot mode selected
        else if (LCD_BTN_CENTER == btn_press) {
            mode = umbc::Robot::Mode::TRAIN_AUTONOMOUS;
            INFO("train autonomous selected for mode");
        }
        
        // no robot mode selected
        else if (LCD_BTN_RIGHT == btn_press) {
            mode = umbc::Robot::Mode::NONE;
            INFO("no mode selected");
        }
        
        // no valid robot modes selected
        else {
            btn_press = 0;
        }

        // polling delay to not hog processor
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // wait for the LCD button to be released
    while (0 != umbc::read_lcd_buttons()) {
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // clear the LCD
    umbc::clear_lcd();

    return mode;
}


void umbc::Menu::menu_utilities() {

    std::uint8_t btn_press = 0;

    // display utilities sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Utilities");
    pros::lcd::set_text(3, "1) Back");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for utility selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();

        // go back to the previous menu
        if (LCD_BTN_LEFT == btn_press) {
            INFO("back selected");
        }

        // no valid utility selected
        else {
            btn_press = 0;
        }

        // polling delay to not hog processor
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // wait for the LCD button to be released
    while (0 != umbc::read_lcd_buttons()) {
        pros::Task::delay(this->lcd_btn_polling_delay_ms);
    }

    // clear the LCD
    umbc::clear_lcd();
}


umbc::Menu::Menu(umbc::Robot* robot) {
    this->robot = robot;
}


void umbc::Menu::robot_menu() {

    // set the current menu to select match type
    umbc::Menu::SubMenu current_sub_menu = umbc::Menu::SubMenu::MATCH;

    // check to ensure the LCD has been initialized
    if (false == pros::lcd::is_initialized()) {
        ERROR("LCD is not initialized");
        return;
    }

    // clear the LCD
    umbc::clear_lcd();

    // continue until end of sub-menues
    while (umbc::Menu::SubMenu::NONE != current_sub_menu) {

        // display current menu
        switch (current_sub_menu) {

            // match type sub-menu
            case umbc::Menu::SubMenu::MATCH: {

                // set robot's match type
                umbc::Robot::MatchType match_type = this->menu_match();
                this->robot->set_match_type(match_type);

                // go to utilities sub-menu
                if (umbc::Robot::MatchType::NONE == match_type) {
                    current_sub_menu = umbc::Menu::SubMenu::UTILITIES;
                    break;
                }

                // continue to robot mode sub-menu
                current_sub_menu = umbc::Menu::SubMenu::MODE;
                break;
            }

            // alliance color sub-menu
            case umbc::Menu::SubMenu::ALLIANCE: {

                // set robot's alliance color
                umbc::Robot::AllianceColor alliance_color = this->menu_alliance();
                this->robot->set_alliance_color(alliance_color);

                // go back to robot mode sub-menu
                if (umbc::Robot::AllianceColor::NONE == alliance_color) {
                    current_sub_menu = umbc::Menu::SubMenu::MODE;
                    break;
                }

                // end of sub-menus
                current_sub_menu = umbc::Menu::SubMenu::NONE;
                break;
            }
            
            // robot mode sub-menu
            case umbc::Menu::SubMenu::MODE: {

                // set robot's mode
                umbc::Robot::Mode mode = this->menu_mode();
                this->robot->set_mode(mode);

                // go back to match type sub-menu
                if (umbc::Robot::Mode::NONE == mode) {
                    current_sub_menu = umbc::Menu::SubMenu::MATCH;
                    break;
                }

                // select alliance color for tournament matches
                if (umbc::Robot::MatchType::TOURNAMENT == this->robot->get_match_type()) {
                    current_sub_menu = umbc::Menu::SubMenu::ALLIANCE;
                }

                // end of sub-menus for skills matches
                else {
                    current_sub_menu = umbc::Menu::SubMenu::NONE;
                }
                break;
            }

            // utilities sub-menu
            case umbc::Menu::SubMenu::UTILITIES: {

                // display robot utilities
                this->menu_utilities();

                // go back to match type sub-menu
                current_sub_menu = umbc::Menu::SubMenu::MATCH;
                break;
            }
            
            // invalid sub-menu
            default: {
                ERROR("invalid sub-menu");
                break;
            }
        }
    }

    // display all required selections have been completed on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Selection Complete");
    pros::Task::delay(MSG_DELAY_MS);

    // clear LCD menu
    umbc::clear_lcd();

    INFO("menu selections completed");
}
