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

    INFO("waiting for competition mode selection...");
    while(0 == btn_press) {

        btn_press = pros::lcd::read_buttons();
        
        if (LCD_BTN_LEFT == btn_press) {
            this->competition = COMPETITION_MATCH;
            INFO("match selected for competition mode");
        } else if (LCD_BTN_CENTER == btn_press) {
            this->competition = COMPETITION_SKILLS;
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

    return 1;
}

std::int32_t umbc::Robot::menu_mode() {

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
            this->mode = MODE_COMPETITION;
            INFO("competition selected for mode");
        } else if (LCD_BTN_CENTER == btn_press) {
            this->mode = MODE_TRAIN_AUTONOMOUS;
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
    return menu_direction;
}

umbc::Robot::Robot() {

    this->competition = COMPETITION_MATCH;
    this->mode = MODE_COMPETITION;
}

void umbc::Robot::set_controllers_to_physical() {
    this->controller_master = &(this->pcontroller_master);
    this->controller_partner = &(this->pcontroller_partner);
}

void umbc::Robot::set_controllers_to_virtual() {
    this->controller_master = &(this->vcontroller_master);
    this->controller_partner = &(this->vcontroller_partner);
}

umbc::competition umbc::Robot::get_competition() {
    return this->competition;
}
    
umbc::mode umbc::Robot::get_mode() {
    return this->mode;
}

void umbc::Robot::menu() {

    uint8_t current_sub_menu = MENU_COMPETITION;

    if (!pros::lcd::is_initialized()) {
        ERROR("failed to initialize LCD menu");
        return;
    }

    pros::lcd::clear();

    while (MENU_MAX > current_sub_menu) {

        switch (current_sub_menu) {
            case MENU_COMPETITION:
                current_sub_menu += menu_competition();
            break;
            case MENU_MODE:
                current_sub_menu += menu_mode();
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

void umbc::Robot::robot_opcontrol(Robot* robot) {
    robot->opcontrol();
}

void umbc::Robot::autonomous(uint32_t include_partner_controller) {

    INFO("autonomous active");

    INFO("setting robot controllers to virtual controllers...");
	this->set_controllers_to_virtual();
	INFO("robot controllers set to virtual controllers");

    if (COMPETITION_SKILLS == this->competition) {
        INFO("loading skills input file for virtual master controller...");
        this->vcontroller_master.load(this->skills_autonomous_file_master);
        INFO("loaded " << skills_autonomous_file_master << " as input file for virtual master controller");
        if (include_partner_controller) {
            INFO("loading skills input file for virtual partner controller...");
            vcontroller_partner.load(this->skills_autonomous_file_partner);
            INFO("loaded " << skills_autonomous_file_partner << " as input file for virtual partner controller");
        }
    } else {
        INFO("loading match input file for virtual master controller...");
        this->vcontroller_master.load(this->match_autonomous_file_master);
        INFO("loaded " << match_autonomous_file_master << " as input file for virtual master controller");
        if (include_partner_controller) {
            INFO("loading match input file for virtual partner controller...");
            vcontroller_partner.load(this->match_autonomous_file_partner);
            INFO("loaded " << match_autonomous_file_partner << " as input file for virtual partner controller");
        }
    }

    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    INFO("starting task for virtual master controller...");
    this->vcontroller_master.start();
    INFO("virtual master controller task started");

    if (include_partner_controller) {
        INFO("starting task for virtual partner controller...");
        vcontroller_partner.start();
        INFO("virtual partner controller task started");
    }

    INFO("waiting for virtual master controller input to complete...");
    this->vcontroller_master.wait_till_complete();
    INFO("virtual master controller input completed");

    if (include_partner_controller) {
        INFO("waiting for virtual partner controller input to complete...");
        vcontroller_partner.wait_till_complete();
        INFO("virtual partner controller input completed");
    }

    INFO("terminating opcontrol task...");
    this->opcontrol_stop();
    INFO("opcontrol task has been terminated");

    INFO("setting robot controllers to physical controllers...");
    this->set_controllers_to_physical();
	INFO("robot controllers set to physical controllers");

    INFO("autonomous completed");
}

void umbc::Robot::train_autonomous(uint32_t record_partner_controller) {

    INFO("autonomous training active");

    ControllerRecorder controller_recorder_master = ControllerRecorder(controller_master, opcontrol_delay_ms);
    ControllerRecorder controller_recorder_partner = ControllerRecorder(controller_partner, opcontrol_delay_ms);

    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    INFO("starting master controller recording...");
    controller_recorder_master.start();
    INFO("recording master controller has begun");
    if (record_partner_controller) {
        INFO("starting partner controller recording...");
        controller_recorder_partner.start();
        INFO("recording partner controller has begun");
    }

    if (COMPETITION_SKILLS == this->competition) {
        INFO("setting task delay for skills autonomous time...");
        pros::Task::delay(this->skills_autonomous_time_ms);
        INFO("task delay set to " << this->skills_autonomous_time_ms << " ms");
    } else {
        INFO("setting task delay for match autonomous time...");
        pros::Task::delay(this->match_autonomous_time_ms);
        INFO("task delay set to " << this->match_autonomous_time_ms << " ms");
    }

    INFO("terminating opcontrol task...");
    this->opcontrol_stop();
    INFO("opcontrol task has been terminated");

    INFO("stopping master controller recording...");
    controller_recorder_master.stop();
    INFO("master controller recording stopped");

    if (record_partner_controller) {
        INFO("stopping partner controller recording...");
        controller_recorder_partner.stop();
        INFO("partner controller recording stopped");
    }

    INFO("saving master controller file...");
    if (COMPETITION_SKILLS == this->competition) {
        controller_recorder_master.save(this->skills_autonomous_file_master);
        INFO("master controller file saved to " << this->skills_autonomous_file_master);
        if (record_partner_controller) {
            INFO("saving partner controller file...");
            controller_recorder_partner.save(this->skills_autonomous_file_partner);
            INFO("partner controller file saved to " << this->skills_autonomous_file_partner);
        }
    } else {
        controller_recorder_master.save(this->match_autonomous_file_master);
        INFO("master controller file saved to " << this->match_autonomous_file_master);
        if (record_partner_controller) {
            INFO("saving partner controller file...");
            controller_recorder_partner.save(this->match_autonomous_file_partner);
            INFO("partner controller file saved to " << this->match_autonomous_file_partner);
        }
    }

    INFO("autonomous training complete");
}

void umbc::Robot::opcontrol_start() {

    this->t_opcontrol.reset(
        new Task((task_fn_t)this->robot_opcontrol, (void*)this, this->t_opcontrol_name));
    INFO(string(t_opcontrol_name) + " has started");
}

void umbc::Robot::opcontrol_pause() {

    Task* t_opcontrol = this->t_opcontrol.get();

    if (nullptr != t_opcontrol) {
        try {
            t_opcontrol->suspend();
            INFO(string(t_opcontrol_name) + " is paused");
        }
        catch (...) {
            ERROR("failed to suspend " + string(t_opcontrol_name));
        }
    }
}

void umbc::Robot::opcontrol_resume() {

    Task* t_opcontrol = this->t_opcontrol.get();

    if (nullptr != t_opcontrol) {
        try {
            t_opcontrol->resume();
            INFO(string(t_opcontrol_name) + " has resumed");
        } catch (...) {
            ERROR("failed to resume " +  string(t_opcontrol_name));
        }
    }
}

void umbc::Robot::opcontrol_stop() {

    Task* t_opcontrol = this->t_opcontrol.get();
    
    if (nullptr != t_opcontrol) {
        try {
            t_opcontrol->remove();
            INFO(string(t_opcontrol_name) + " is stopped");
        } catch (...) {
            ERROR("failed to stop " + string(t_opcontrol_name));
        }
    }
}

std::int32_t umbc::Robot::opcontrol_isListed() {

    Task* t_opcontrol = this->t_opcontrol.get();

    return (nullptr == t_opcontrol) ? 0 
        : ((t_opcontrol->get_state() != E_TASK_STATE_INVALID) && (t_opcontrol->get_state() != E_TASK_STATE_DELETED));
}