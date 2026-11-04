/**
 * @file umbc/menu.cpp
 *
 * Contains the implementation for the Menu class. The Menu class contains
 * various sub-menus for putting the robot into different modes.
 */

// standard libraries
#include <cstdint>
#include <string>
#include <vector>

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


umbc::Menu::Utility umbc::Menu::menu_utilities() {

    umbc::Menu::Utility utility = umbc::Menu::Utility::NONE;
    std::uint8_t btn_press = 0;

    // display utilities sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Utilities");
    pros::lcd::set_text(3, "1) Select Autonomous");
    pros::lcd::set_text(4, "2) Back");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for utility selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();

        // select autonomous utility selected
        if (LCD_BTN_LEFT == btn_press) {
            utility = umbc::Menu::Utility::SELECT_AUTONOMOUS;
            INFO("select autonomous selected for utility");
        }

        // go back to the previous menu
        else if (LCD_BTN_CENTER == btn_press) {
            utility = umbc::Menu::Utility::NONE;
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

    return utility;
}


umbc::Robot::MatchType umbc::Menu::menu_autonomous_category() {

    umbc::Robot::MatchType category = umbc::Robot::MatchType::NONE;
    std::uint8_t btn_press = 0;

    // display autonomous category sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, "Select Autonomous");
    pros::lcd::set_text(3, "1) Competition");
    pros::lcd::set_text(4, "2) Skills");
    pros::lcd::set_text(5, "3) Back");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for autonomous category selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();

        // competition category selected
        if (LCD_BTN_LEFT == btn_press) {
            category = umbc::Robot::MatchType::TOURNAMENT;
            INFO("competition selected for autonomous category");
        }

        // skills category selected
        else if (LCD_BTN_CENTER == btn_press) {
            category = umbc::Robot::MatchType::SKILLS;
            INFO("skills selected for autonomous category");
        }

        // no category selected
        else if (LCD_BTN_RIGHT == btn_press) {
            category = umbc::Robot::MatchType::NONE;
            INFO("back selected");
        }

        // no valid categories selected
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

    return category;
}


umbc::Robot::AutonomousType umbc::Menu::menu_autonomous_type(umbc::Robot::MatchType category) {

    umbc::Robot::AutonomousType autonomous_type = umbc::Robot::AutonomousType::NONE;
    std::uint8_t btn_press = 0;

    // title reflects the category being selected for
    std::string title = (umbc::Robot::MatchType::SKILLS == category)
        ? "Select Skills Type" : "Select Competition Type";

    // display autonomous type sub-menu on the LCD
    umbc::clear_lcd();
    pros::lcd::set_text(1, title);
    pros::lcd::set_text(3, "1) Recorded");
    pros::lcd::set_text(4, "2) Programmed");
    pros::lcd::set_text(5, "3) Back");

    // continue polling LCD buttons until one is pressed
    INFO("waiting for autonomous type selection...");
    while (0 == btn_press) {

        // check if any LCD buttons have been pressed
        btn_press = umbc::read_lcd_buttons();

        // recorded autonomous type selected
        if (LCD_BTN_LEFT == btn_press) {
            autonomous_type = umbc::Robot::AutonomousType::RECORDED;
            INFO("recorded selected for autonomous type");
        }

        // programmed autonomous type selected
        else if (LCD_BTN_CENTER == btn_press) {
            autonomous_type = umbc::Robot::AutonomousType::PROGRAMMED;
            INFO("programmed selected for autonomous type");
        }

        // no autonomous type selected
        else if (LCD_BTN_RIGHT == btn_press) {
            autonomous_type = umbc::Robot::AutonomousType::NONE;
            INFO("back selected");
        }

        // no valid autonomous types selected
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

    return autonomous_type;
}


std::string umbc::Menu::menu_autonomous_recording() {

    std::string recording_name = std::string();
    std::string file_extension = std::string(umbc::Robot::autonomous_file_extension);
    std::string partner_suffix = std::string(umbc::Robot::autonomous_partner_suffix);

    // check that an SD card is installed
    if (0 == pros::usd::is_installed()) {
        ERROR("cannot list autonomous recordings; no SD Card detected");
        umbc::clear_lcd();
        pros::lcd::set_text(1, "No SD Card Detected");
        pros::lcd::set_text(3, "1) Back");

        // wait for the back button to be pressed
        while (LCD_BTN_LEFT != umbc::read_lcd_buttons()) {
            pros::Task::delay(this->lcd_btn_polling_delay_ms);
        }

        // wait for the LCD button to be released
        while (0 != umbc::read_lcd_buttons()) {
            pros::Task::delay(this->lcd_btn_polling_delay_ms);
        }

        // clear the LCD
        umbc::clear_lcd();

        return recording_name;
    }

    // gather the recordings at the root of the SD card
    FileExplorer file_explorer = FileExplorer(umbc::Robot::autonomous_list_directory);
    std::vector<std::string> recordings;
    for (const std::string& file : file_explorer.get_files()) {

        // only list files with the autonomous recording extension
        if (file.size() <= file_extension.size()
            || 0 != file.compare(file.size() - file_extension.size(), file_extension.size(), file_extension)) {
            continue;
        }

        // exclude partner controller recordings
        std::string name = file.substr(0, file.size() - file_extension.size());
        if (name.size() >= partner_suffix.size()
            && 0 == name.compare(name.size() - partner_suffix.size(), partner_suffix.size(), partner_suffix)) {
            continue;
        }

        recordings.push_back(name);
    }

    // display message when no recordings exist
    if (recordings.empty()) {
        WARN("no autonomous recordings found at " + string(umbc::Robot::autonomous_directory));
        umbc::clear_lcd();
        pros::lcd::set_text(1, "No Recordings Found");
        pros::lcd::set_text(3, "1) Back");

        // wait for the back button to be pressed
        while (LCD_BTN_LEFT != umbc::read_lcd_buttons()) {
            pros::Task::delay(this->lcd_btn_polling_delay_ms);
        }

        // wait for the LCD button to be released
        while (0 != umbc::read_lcd_buttons()) {
            pros::Task::delay(this->lcd_btn_polling_delay_ms);
        }

        // clear the LCD
        umbc::clear_lcd();

        return recording_name;
    }

    std::size_t cursor = 0;
    bool selection_made = false;

    // continue until a recording is selected or back is pressed
    INFO("waiting for recording selection...");
    while (false == selection_made) {

        // display the page of recordings the cursor is on
        umbc::clear_lcd();
        pros::lcd::set_text(1, "Select Recording");
        std::size_t page_start = (cursor / this->recordings_per_page) * this->recordings_per_page;
        for (std::size_t i = 0;
            i < this->recordings_per_page && page_start + i < recordings.size(); i++) {

            // truncate recording names too long to display with an ellipsis
            std::string name = recordings[page_start + i];
            std::string truncation_suffix = std::string(this->recording_truncation_suffix);
            if (this->recording_display_length_max < name.size()) {
                name = name.substr(0, this->recording_display_length_max - truncation_suffix.size())
                    + truncation_suffix;
            }

            std::string line = ((page_start + i == cursor) ? "> " : "  ") + name;
            pros::lcd::set_text(this->recording_list_first_line + i, line);
        }
        pros::lcd::set_text(7, "1) Next 2) Select 3) Back");

        // continue polling LCD buttons until one is pressed
        std::uint8_t btn_press = 0;
        while (0 == btn_press) {

            // check if any LCD buttons have been pressed
            btn_press = umbc::read_lcd_buttons();

            // move the cursor to the next recording
            if (LCD_BTN_LEFT == btn_press) {
                cursor = (cursor + 1) % recordings.size();
            }

            // select the recording the cursor is on
            else if (LCD_BTN_CENTER == btn_press) {
                recording_name = recordings[cursor];
                selection_made = true;
                INFO(recording_name + " selected for recording");
            }

            // no recording selected
            else if (LCD_BTN_RIGHT == btn_press) {
                selection_made = true;
                INFO("back selected");
            }

            // no valid selections made
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
    }

    // clear the LCD
    umbc::clear_lcd();

    return recording_name;
}


umbc::Menu::Menu(umbc::Robot* robot) {
    this->robot = robot;
}


void umbc::Menu::robot_menu() {

    // set the current menu to select match type
    umbc::Menu::SubMenu current_sub_menu = umbc::Menu::SubMenu::MATCH;

    // category currently being selected for on the autonomous sub-menus
    umbc::Robot::MatchType autonomous_category = umbc::Robot::MatchType::NONE;

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

                // skip alliance color selection and end of sub-menus for skills matches
                else {
                    this->robot->set_alliance_color(umbc::Robot::AllianceColor::NONE);
                    current_sub_menu = umbc::Menu::SubMenu::NONE;
                }
                break;
            }

            // utilities sub-menu
            case umbc::Menu::SubMenu::UTILITIES: {

                // display robot utilities
                umbc::Menu::Utility utility = this->menu_utilities();

                // go to autonomous category sub-menu
                if (umbc::Menu::Utility::SELECT_AUTONOMOUS == utility) {
                    current_sub_menu = umbc::Menu::SubMenu::AUTONOMOUS_CATEGORY;
                    break;
                }

                // go back to match type sub-menu
                current_sub_menu = umbc::Menu::SubMenu::MATCH;
                break;
            }

            // autonomous category sub-menu
            case umbc::Menu::SubMenu::AUTONOMOUS_CATEGORY: {

                // select the category to set the autonomous selection for
                autonomous_category = this->menu_autonomous_category();

                // go back to utilities sub-menu
                if (umbc::Robot::MatchType::NONE == autonomous_category) {
                    current_sub_menu = umbc::Menu::SubMenu::UTILITIES;
                    break;
                }

                // continue to autonomous type sub-menu
                current_sub_menu = umbc::Menu::SubMenu::AUTONOMOUS_TYPE;
                break;
            }

            // autonomous type sub-menu
            case umbc::Menu::SubMenu::AUTONOMOUS_TYPE: {

                // select the autonomous type for the category
                umbc::Robot::AutonomousType autonomous_type = this->menu_autonomous_type(autonomous_category);

                // go back to autonomous category sub-menu
                if (umbc::Robot::AutonomousType::NONE == autonomous_type) {
                    current_sub_menu = umbc::Menu::SubMenu::AUTONOMOUS_CATEGORY;
                    break;
                }

                // continue to recording sub-menu for recorded routines
                if (umbc::Robot::AutonomousType::RECORDED == autonomous_type) {
                    current_sub_menu = umbc::Menu::SubMenu::AUTONOMOUS_RECORDING;
                    break;
                }

                // save the programmed selection for the category
                this->robot->set_autonomous(autonomous_category,
                    umbc::Robot::AutonomousType::PROGRAMMED, std::string());

                // display the selection has been saved on the LCD
                umbc::clear_lcd();
                pros::lcd::set_text(1, "Selection Saved");
                pros::Task::delay(MSG_DELAY_MS);
                umbc::clear_lcd();

                // go back to utilities sub-menu
                current_sub_menu = umbc::Menu::SubMenu::UTILITIES;
                break;
            }

            // autonomous recording sub-menu
            case umbc::Menu::SubMenu::AUTONOMOUS_RECORDING: {

                // select the recording for the category
                std::string recording_name = this->menu_autonomous_recording();

                // go back to autonomous type sub-menu
                if (recording_name.empty()) {
                    current_sub_menu = umbc::Menu::SubMenu::AUTONOMOUS_TYPE;
                    break;
                }

                // save the recorded selection for the category
                this->robot->set_autonomous(autonomous_category,
                    umbc::Robot::AutonomousType::RECORDED, recording_name);

                // display the selection has been saved on the LCD
                umbc::clear_lcd();
                pros::lcd::set_text(1, "Selection Saved");
                pros::Task::delay(MSG_DELAY_MS);
                umbc::clear_lcd();

                // go back to utilities sub-menu
                current_sub_menu = umbc::Menu::SubMenu::UTILITIES;
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
