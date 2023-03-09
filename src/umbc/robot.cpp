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

using namespace pros;
using namespace umbc;
using namespace std;

std::int32_t umbc::Robot::menu_competition() {

    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Competition Mode");
    pros::lcd::set_text(3, "1) Match");
    pros::lcd::set_text(4, "2) Skills");

    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->competition = COMPETITION_MATCH;
        } else if (LCD_BTN_CENTER == btn_press) {
            this->competition = COMPETITION_SKILLS;
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();
}

std::int32_t umbc::Robot::menu_mode() {

    std::uint8_t menu_direction = 1;
    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Mode");
    pros::lcd::set_text(3, "1) Competition");
    pros::lcd::set_text(4, "2) Train Autonomous");
    pros::lcd::set_text(5, "3) Back");
    
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->mode = MODE_COMPETITION;
        } else if (LCD_BTN_CENTER == btn_press) {
            this->mode = MODE_TRAIN_AUTONOMOUS;
        } else if (LCD_BTN_RIGHT == btn_press) {
            menu_direction = -1;
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();
    return menu_direction;
}

std::int32_t umbc::Robot::menu_alliance() {

    std::uint8_t menu_direction = 1;
    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Alliance");
    pros::lcd::set_text(3, "1) Red");
    pros::lcd::set_text(4, "2) Blue");
    pros::lcd::set_text(5, "3) Back");
    
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->alliance = ALLIANCE_RED;
        } else if (LCD_BTN_CENTER == btn_press) {
            this->alliance = ALLIANCE_BLUE;
        } else if (LCD_BTN_RIGHT == btn_press) {
            menu_direction = -1;
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();
    return menu_direction;
}

std::int32_t umbc::Robot::menu_position() {

    std::uint8_t menu_direction = 1;
    std::uint8_t btn_press = 0;

    pros::lcd::clear();
    pros::lcd::set_text(1, "Select Starting Position");
    pros::lcd::set_text(3, "1) Alpha");
    pros::lcd::set_text(4, "2) Bravo");
    pros::lcd::set_text(5, "3) Back");
    
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->position = POSITION_ALPHA;
        } else if (LCD_BTN_CENTER == btn_press) {
            this->position = POSITION_BRAVO;
        } else if (LCD_BTN_RIGHT == btn_press) {
            menu_direction = -1;
        } else {
            btn_press = 0;
        }

        pros::Task::delay(10);
    }

    while (0 != pros::lcd::read_buttons()) {
        pros::Task::delay(10);
    }

    pros::lcd::clear();
    return menu_direction;
}

void umbc::Robot::menu() {

}

void umbc::Robot::autonomous() {

}

void umbc::Robot::opcontrol() {

}

void umbc::Robot::train_autonomous() {

}