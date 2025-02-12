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

umbc::Robot::Robot() {
    this->match_type = umbc::Robot::MatchType::TOURNAMENT;
    this->mode = umbc::Robot::Mode::COMPETITION;
}

void umbc::Robot::set_controllers_to_physical() {
    this->controller_master = &(this->pcontroller_master);
    this->controller_partner = &(this->pcontroller_partner);
}

void umbc::Robot::set_controllers_to_virtual() {
    this->controller_master = &(this->vcontroller_master);
    this->controller_partner = &(this->vcontroller_partner);
}

umbc::Robot::MatchType umbc::Robot::get_match_type() {
    return this->match_type;
}

void umbc::Robot::set_match_type(umbc::Robot::MatchType match_type) {
    this->match_type = match_type;
}
    
umbc::Robot::Mode umbc::Robot::get_mode() {
    return this->mode;
}

void umbc::Robot::set_mode(umbc::Robot::Mode mode) {
    this->mode = mode;
}

void umbc::Robot::robot_opcontrol(Robot* robot) {
    robot->opcontrol();
}

void umbc::Robot::autonomous(bool is_partner_controller) {

    INFO("autonomous active");

    INFO("setting robot controllers to virtual controllers...");
	this->set_controllers_to_virtual();
	INFO("robot controllers set to virtual controllers");

    if (umbc::Robot::MatchType::SKILLS == this->match_type) {
        INFO("loading skills input file for virtual master controller...");
        this->vcontroller_master.load(this->skills_autonomous_file_master);
        INFO("loaded " << skills_autonomous_file_master << " as input file for virtual master controller");
        if (is_partner_controller) {
            INFO("loading skills input file for virtual partner controller...");
            this->vcontroller_partner.load(this->skills_autonomous_file_partner);
            INFO("loaded " << skills_autonomous_file_partner << " as input file for virtual partner controller");
        }
    } else {
        INFO("loading match input file for virtual master controller...");
        this->vcontroller_master.load(this->match_autonomous_file_master);
        INFO("loaded " << match_autonomous_file_master << " as input file for virtual master controller");
        if (is_partner_controller) {
            INFO("loading match input file for virtual partner controller...");
            this->vcontroller_partner.load(this->match_autonomous_file_partner);
            INFO("loaded " << match_autonomous_file_partner << " as input file for virtual partner controller");
        }
    }

    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    INFO("starting task for virtual master controller...");
    this->vcontroller_master.start();
    INFO("virtual master controller task started");

    if (is_partner_controller) {
        INFO("starting task for virtual partner controller...");
        this->vcontroller_partner.start();
        INFO("virtual partner controller task started");
    }

    INFO("waiting for virtual master controller input to complete...");
    this->vcontroller_master.wait_till_complete();
    INFO("virtual master controller input completed");

    if (is_partner_controller) {
        INFO("waiting for virtual partner controller input to complete...");
        this->vcontroller_partner.wait_till_complete();
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

void umbc::Robot::train_autonomous(bool is_partner_controller) {

    INFO("autonomous training active");

    ControllerRecorder controller_recorder_master = ControllerRecorder(controller_master, opcontrol_delay_ms);
    ControllerRecorder controller_recorder_partner = ControllerRecorder(controller_partner, opcontrol_delay_ms);

    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    INFO("starting master controller recording...");
    controller_recorder_master.start();
    INFO("recording master controller has begun");
    if (is_partner_controller) {
        INFO("starting partner controller recording...");
        controller_recorder_partner.start();
        INFO("recording partner controller has begun");
    }

    if (umbc::Robot::MatchType::SKILLS == this->match_type) {
        INFO("setting task delay for skills autonomous time...");
        pros::Task::delay(this->skills_autonomous_time_ms);
        INFO("task delay set to " << this->skills_autonomous_time_ms << " ms");
    } else {
        INFO("setting task delay for match autonomous time...");
        pros::Task::delay(this->tournament_autonomous_time_ms);
        INFO("task delay set to " << this->tournament_autonomous_time_ms << " ms");
    }

    INFO("terminating opcontrol task...");
    this->opcontrol_stop();
    INFO("opcontrol task has been terminated");

    INFO("stopping master controller recording...");
    controller_recorder_master.stop();
    INFO("master controller recording stopped");

    if (is_partner_controller) {
        INFO("stopping partner controller recording...");
        controller_recorder_partner.stop();
        INFO("partner controller recording stopped");
    }

    INFO("saving master controller file...");
    if (umbc::Robot::MatchType::SKILLS == this->match_type) {
        controller_recorder_master.save(this->skills_autonomous_file_master);
        INFO("master controller file saved to " << this->skills_autonomous_file_master);
        if (is_partner_controller) {
            INFO("saving partner controller file...");
            controller_recorder_partner.save(this->skills_autonomous_file_partner);
            INFO("partner controller file saved to " << this->skills_autonomous_file_partner);
        }
    } else {
        controller_recorder_master.save(this->match_autonomous_file_master);
        INFO("master controller file saved to " << this->match_autonomous_file_master);
        if (is_partner_controller) {
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

bool umbc::Robot::is_opcontrol_listed() {

    Task* t_opcontrol = this->t_opcontrol.get();

    return (nullptr == t_opcontrol) ? false
        : ((t_opcontrol->get_state() != E_TASK_STATE_INVALID) && (t_opcontrol->get_state() != E_TASK_STATE_DELETED));
}
