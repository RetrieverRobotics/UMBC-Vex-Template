/**
 * \file umbc/robot.cpp
 *
 * Contains the implementation for the Robot. The Robot holds various values used for
 * different modes, as well as holds the functions for the selection menu,
 * autonomous, training autonomous, and opcontrol.
 */

#include "api.h"
#include "umbc.h"

#include <cstdint>
#include <string>

using namespace pros;
using namespace umbc;
using namespace std;

umbc::Robot::MatchType umbc::Menu::menu_match() {

    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Competition Mode");
    pros::lcd::set_text(3, "1) Match");
    pros::lcd::set_text(4, "2) Skills");

    INFO("waiting for competition mode selection...");
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->robot->set_match_type(umbc::Robot::MatchType::TOURNAMENT);
            INFO("match selected for competition mode");
        } else if (LCD_BTN_CENTER == btn_press) {
            this->robot->set_match_type(umbc::Robot::MatchType::SKILLS);
            INFO("skills selected for competition mode");
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();

    return umbc::Robot::MatchType::NONE;   // change to actually returned
}

umbc::Robot::Mode umbc::Menu::menu_mode() {

    std::int32_t menu_direction = 1;
    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Mode");
    pros::lcd::set_text(3, "1) Competition");
    pros::lcd::set_text(4, "2) Train Autonomous");
    pros::lcd::set_text(5, "3) Back");
    
    INFO("waiting for mode selection...");
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->robot->set_mode(umbc::Robot::Mode::COMPETITION);
            INFO("competition selected for mode");
        } else if (LCD_BTN_CENTER == btn_press) {
            this->robot->set_mode(umbc::Robot::Mode::TRAIN_AUTONOMOUS);
            INFO("train autonomous selected for mode");
        } else if (LCD_BTN_RIGHT == btn_press) {
            menu_direction = -1;
            INFO("going back to prevoius menu");
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();
    return umbc::Robot::Mode::NONE; // change to actually returned
}

umbc::Menu::Menu(umbc::Robot* robot) {
    this->robot = robot;
}

void umbc::Menu::robot_menu() {

    umbc::Menu::SubMenu current_sub_menu = umbc::Menu::SubMenu::MATCH;

    if (!pros::lcd::is_initialized()) {
        ERROR("failed to initialize LCD menu");
        return;
    }

    pros::lcd::clear();

    while (umbc::Menu::SubMenu::MAX > current_sub_menu) {

        switch (current_sub_menu) {
            case umbc::Menu::SubMenu::MATCH:
                current_sub_menu = umbc::Robot::MatchType::NONE == menu_match() 
                    ? current_sub_menu : umbc::Menu::SubMenu::MODE;
            break;
            case umbc::Menu::SubMenu::MODE:
                current_sub_menu = umbc::Robot::Mode::NONE == menu_mode()
                    ? umbc::Menu::SubMenu::MATCH : umbc::Menu::SubMenu::MAX;
            break;
            default:
                break;
        }
    }

    pros::lcd::clear();
    pros::lcd::set_text(1, "Selection Complete");
    pros::Task::delay(MSG_DELAY_MS);
    pros::lcd::clear();
    INFO("menu selections completed");
}
