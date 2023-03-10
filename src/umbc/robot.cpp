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

umbc::Robot::Robot() {

    this->competition = COMPETITION_MATCH;
    this->mode = MODE_COMPETITION;
    this->alliance = ALLIANCE_RED;
    this->position = POSITION_ALPHA;
}

void umbc::Robot::set_controller_master(pros::Controller controller_master) {
    this->controller_master = controller_master;
}

void umbc::Robot::set_controller_partner(pros::Controller controller_partner) {
    this->controller_partner = controller_partner;
}

pros::Controller umbc::Robot::get_controller_master() {
    return this->controller_master;
}

pros::Controller umbc::Robot::get_controller_partner() {
    return this->controller_partner;
}

umbc::competition umbc::Robot::get_competition() {
    return this->competition;
}
    

umbc::mode umbc::Robot::get_mode() {
    return this->mode;
}
    

umbc::alliance umbc::Robot::get_alliance() {
    return this->alliance;
}
    

umbc::position umbc::Robot::get_position() {
    return this->position;
}

void umbc::Robot::menu() {

    const uint8_t last_sub_menu = MENU_POSITION;
    uint8_t current_sub_menu = MENU_COMPETITION;

    if (!pros::lcd::is_initialized()) {
        return;
    }

    pros::lcd::clear();

    while (MENU_POSITION >= current_sub_menu) {

        switch (current_sub_menu) {
            case MENU_COMPETITION:
                current_sub_menu += menu_competition();
            break;
            case MENU_MODE:
                current_sub_menu += menu_mode();
            break;
            case MENU_ALLIANCE:
                current_sub_menu += menu_alliance();
            break;
            case MENU_POSITION:
                current_sub_menu += menu_position();
            default:
                break;
        }
    }

    pros::lcd::clear();
    pros::lcd::set_text(1, "Selection Complete");
    pros::Task::delay(3000);
    pros::lcd::clear();
}

void umbc::Robot::robot_opcontrol(Robot robot) {

    robot.opcontrol(robot.get_controller_master(), robot.get_controller_partner());
}

void umbc::Robot::autonomous(uint32_t include_partner_controller) {

    char* t_autonomous_name = "autonomous";

    pros::Controller controller_master_prev = this->get_controller_master();
    pros::Controller controller_partner_prev = this->get_controller_partner();

    VController vcontroller_master = VController();
    VController vcontroller_partner = VController();

    this->controller_master = vcontroller_master;
    this->controller_partner = vcontroller_partner;

    if (COMPETITION_SKILLS == this->competition) {

        vcontroller_master.load(this->skills_autonomous_file_master);
        if (include_partner_controller) {
            vcontroller_partner.load(this->skills_autonomous_file_partner);
        }
    } else {

        vcontroller_master.load(this->match_autonomous_file_master);
        if (include_partner_controller) {
            vcontroller_partner.load(this->match_autonomous_file_partner);
        }
    }

    Task opcontrol = Task((task_fn_t)this->robot_opcontrol, (void*)this, t_autonomous_name);
    vcontroller_master.start();
    if (include_partner_controller) {
        vcontroller_partner.start();
    }

    vcontroller_master.wait_till_complete();
    if (include_partner_controller) {
        vcontroller_partner.wait_till_complete();
    }
    opcontrol.remove();

    this->controller_master = controller_master_prev;
    this->controller_partner = controller_partner_prev;
}

void umbc::Robot::train_autonomous(uint32_t record_partner_controller) {

    char* t_train_autonomous_name = "trainautonomous";

    ControllerRecorder controller_recorder_master = ControllerRecorder(&controller_master, autonomous_train_poll_rate_ms);
    ControllerRecorder controller_recorder_partner = ControllerRecorder(&controller_partner, autonomous_train_poll_rate_ms);

    Task opcontrol = Task((task_fn_t)this->robot_opcontrol, (void*)this, t_train_autonomous_name);
    controller_recorder_master.start();
    if (record_partner_controller) {
        controller_recorder_partner.start();
    }

    if (COMPETITION_SKILLS == this->competition) {
        pros::Task::delay(this->match_autonomous_time_ms);
    } else {
        pros::Task::delay(this->match_autonomous_time_ms);
    }

    opcontrol.remove();
    controller_recorder_master.stop();
    if (record_partner_controller) {
        controller_recorder_partner.stop();
    }

    if (COMPETITION_SKILLS == this->competition) {
        controller_recorder_master.save(this->skills_autonomous_file_master);
        if (record_partner_controller) {
            controller_recorder_partner.save(this->skills_autonomous_file_partner);
        }
    } else {
        controller_recorder_master.save(this->match_autonomous_file_master);
        if (record_partner_controller) {
            controller_recorder_partner.save(this->match_autonomous_file_partner);
        }
    }
}