/**
 * @file umbc/robot.cpp
 *
 * Contains the implementation for the Robot. The Robot holds various values used for
 * different modes, as well as holds the functions for the selection menu,
 * autonomous, training autonomous, and opcontrol.
 */

// standard libraries
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


umbc::Robot::Robot() {
    this->match_type = umbc::Robot::MatchType::TOURNAMENT;
    this->alliance_color = umbc::Robot::AllianceColor::NONE;
    this->mode = umbc::Robot::Mode::COMPETITION;
    this->tournament_autonomous_time_ms = this->default_tournament_autonomous_time_ms;
    this->skills_autonomous_time_ms = this->default_skills_autonomous_time_ms;
    this->competition_autonomous_type = umbc::Robot::AutonomousType::RECORDED;
    this->competition_autonomous_name = this->default_tournament_autonomous_name;
    this->skills_autonomous_type = umbc::Robot::AutonomousType::RECORDED;
    this->skills_autonomous_name = this->default_skills_autonomous_name;
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

    // clear alliance color when not selecting a tournament match
    if (umbc::Robot::MatchType::TOURNAMENT != match_type) {
        this->set_alliance_color(umbc::Robot::AllianceColor::NONE);
    }
}


umbc::Robot::AllianceColor umbc::Robot::get_alliance_color() {
    return this->alliance_color;
}


void umbc::Robot::set_alliance_color(umbc::Robot::AllianceColor alliance_color) {
    this->alliance_color = alliance_color;

    if (true == pros::lcd::is_initialized()) {
        this->update_lcd_colors();
    }
}


void umbc::Robot::update_lcd_colors() {

    switch (this->alliance_color) {

        case umbc::Robot::AllianceColor::RED: {
            umbc::LCD::set_colors(pros::Color::red, pros::Color::white);
            break;
        }

        case umbc::Robot::AllianceColor::BLUE: {
            umbc::LCD::set_colors(pros::Color::blue, pros::Color::white);
            break;
        }

        case umbc::Robot::AllianceColor::NONE: {
            umbc::LCD::reset_colors();
            break;
        }

        default: {
            ERROR("cannot update LCD colors; invalid alliance color");
            break;
        }
    }
}


umbc::Robot::Mode umbc::Robot::get_mode() {
    return this->mode;
}


void umbc::Robot::set_mode(umbc::Robot::Mode mode) {
    this->mode = mode;
}


umbc::Robot::AutonomousType umbc::Robot::get_autonomous_type(umbc::Robot::MatchType match_type) {

    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            return this->competition_autonomous_type;
        }

        case umbc::Robot::MatchType::SKILLS: {
            return this->skills_autonomous_type;
        }

        default: {
            ERROR("cannot retrieve autonomous type; invalid match type");
            return umbc::Robot::AutonomousType::NONE;
        }
    }
}


std::string umbc::Robot::get_autonomous_name(umbc::Robot::MatchType match_type) {

    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            return this->competition_autonomous_name;
        }

        case umbc::Robot::MatchType::SKILLS: {
            return this->skills_autonomous_name;
        }

        default: {
            ERROR("cannot retrieve autonomous recording name; invalid match type");
            return std::string();
        }
    }
}


void umbc::Robot::set_autonomous(umbc::Robot::MatchType match_type,
    umbc::Robot::AutonomousType autonomous_type, const std::string& recording_name) {

    // check that the autonomous type is valid
    if (umbc::Robot::AutonomousType::RECORDED != autonomous_type
        && umbc::Robot::AutonomousType::PROGRAMMED != autonomous_type) {
        ERROR("cannot set autonomous selection; invalid autonomous type");
        return;
    }

    // check that recorded routines have a recording name
    if (umbc::Robot::AutonomousType::RECORDED == autonomous_type && recording_name.empty()) {
        ERROR("cannot set autonomous selection; recorded routines require a recording name");
        return;
    }

    // replace the previous selection for the match type
    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            this->competition_autonomous_type = autonomous_type;
            if (umbc::Robot::AutonomousType::RECORDED == autonomous_type) {
                this->competition_autonomous_name = recording_name;
            }
            break;
        }

        case umbc::Robot::MatchType::SKILLS: {
            this->skills_autonomous_type = autonomous_type;
            if (umbc::Robot::AutonomousType::RECORDED == autonomous_type) {
                this->skills_autonomous_name = recording_name;
            }
            break;
        }

        default: {
            ERROR("cannot set autonomous selection; invalid match type");
            return;
        }
    }

    // save the selections so they persist between robot restarts
    this->save_autonomous_selections();
}


std::int32_t umbc::Robot::read_config_file(Json& config) {

    config = Json();

    // check that an SD card is installed
    if (0 == pros::usd::is_installed()) {
        WARN("cannot read " + string(this->config_file_path) + "; no SD card detected");
        return 0;
    }

    // open the configuration file for reading
    std::ifstream config_file(this->config_file_path);
    if (!config_file.good()) {
        WARN("could not open " + string(this->config_file_path));
        return 0;
    }

    // read in the entire configuration file
    std::stringstream config_text;
    config_text << config_file.rdbuf();
    config_file.close();

    // parse the configuration file
    if (false == Json::parse(config_text.str(), config)) {
        WARN("could not parse " + string(this->config_file_path));
        return 0;
    }

    return 1;
}


std::int32_t umbc::Robot::write_config_file(const Json& config) {

    // check that an SD card is installed
    if (0 == pros::usd::is_installed()) {
        ERROR("cannot write " + string(this->config_file_path) + "; no SD card detected");
        return 0;
    }

    // write the configuration file
    std::ofstream config_file(this->config_file_path, std::ofstream::trunc);
    if (!config_file.good()) {
        ERROR("could not open " + string(this->config_file_path) + " for writing");
        return 0;
    }
    config_file << config.serialize();
    config_file.close();

    return 1;
}


std::int32_t umbc::Robot::load_autonomous_selections() {

    // read in the configuration file
    Json config;
    if (0 == this->read_config_file(config)) {
        WARN("using default autonomous selections");
        return 0;
    }

    // retrieve the autonomous selections object
    const Json* autonomous = config.get(this->config_name_autonomous);
    if (nullptr == autonomous) {
        WARN("no autonomous selections in " + string(this->config_file_path)
            + "; using default autonomous selections");
        return 1;
    }

    // load the selection for each match type
    const umbc::Robot::MatchType selection_match_types[] =
        {umbc::Robot::MatchType::TOURNAMENT, umbc::Robot::MatchType::SKILLS};
    for (umbc::Robot::MatchType selection_match_type : selection_match_types) {

        const char* selection_name = (umbc::Robot::MatchType::SKILLS == selection_match_type)
            ? this->config_name_skills : this->config_name_competition;

        // retrieve the selection object for the match type
        const Json* selection = autonomous->get(selection_name);
        if (nullptr == selection) {
            WARN("no " + string(selection_name) + " autonomous selection in "
                + string(this->config_file_path) + "; keeping current selection");
            continue;
        }

        // retrieve the autonomous type of the selection
        const Json* type = selection->get(this->config_name_type);
        std::string type_value = (nullptr == type) ? std::string() : type->get_string(std::string());

        // apply a programmed selection
        if (this->config_type_programmed == type_value) {
            if (umbc::Robot::MatchType::SKILLS == selection_match_type) {
                this->skills_autonomous_type = umbc::Robot::AutonomousType::PROGRAMMED;
            }
            else {
                this->competition_autonomous_type = umbc::Robot::AutonomousType::PROGRAMMED;
            }
            INFO(string(selection_name) + " autonomous selection loaded as programmed");
        }

        // apply a recorded selection
        else if (this->config_type_recorded == type_value) {

            // retrieve the recording name of the selection
            const Json* name = selection->get(this->config_name_file);
            std::string name_value = (nullptr == name) ? std::string() : name->get_string(std::string());

            // keep the current selection if the recording name is missing
            if (name_value.empty()) {
                WARN("no recording name for " + string(selection_name) + " autonomous selection in "
                    + string(this->config_file_path) + "; keeping current selection");
                continue;
            }

            if (umbc::Robot::MatchType::SKILLS == selection_match_type) {
                this->skills_autonomous_type = umbc::Robot::AutonomousType::RECORDED;
                this->skills_autonomous_name = name_value;
            }
            else {
                this->competition_autonomous_type = umbc::Robot::AutonomousType::RECORDED;
                this->competition_autonomous_name = name_value;
            }
            INFO(string(selection_name) + " autonomous selection loaded as recorded (" + name_value + ")");
        }

        // keep the current selection for invalid autonomous types
        else {
            WARN("invalid autonomous type for " + string(selection_name) + " autonomous selection in "
                + string(this->config_file_path) + "; keeping current selection");
        }
    }

    return 1;
}


std::int32_t umbc::Robot::save_autonomous_selections() {

    // check that an SD card is installed
    if (0 == pros::usd::is_installed()) {
        ERROR("cannot save autonomous selections; no SD card detected");
        return 0;
    }

    // read in the current configuration file to preserve unrelated values
    Json config;
    if (0 == this->read_config_file(config)) {
        WARN("recreating " + string(this->config_file_path));
    }

    // update the selection for each match type
    Json& autonomous = config.insert(this->config_name_autonomous);
    const umbc::Robot::MatchType selection_match_types[] =
        {umbc::Robot::MatchType::TOURNAMENT, umbc::Robot::MatchType::SKILLS};
    for (umbc::Robot::MatchType selection_match_type : selection_match_types) {

        bool is_skills = (umbc::Robot::MatchType::SKILLS == selection_match_type);
        const char* selection_name = is_skills ? this->config_name_skills : this->config_name_competition;
        umbc::Robot::AutonomousType autonomous_type =
            is_skills ? this->skills_autonomous_type : this->competition_autonomous_type;
        std::string recording_name =
            is_skills ? this->skills_autonomous_name : this->competition_autonomous_name;

        Json& selection = autonomous.insert(selection_name);

        // save a programmed selection without a recording name
        if (umbc::Robot::AutonomousType::PROGRAMMED == autonomous_type) {
            selection.insert(this->config_name_type).set_string(this->config_type_programmed);
            selection.remove(this->config_name_file);
        }

        // save a recorded selection with its recording name
        else {
            selection.insert(this->config_name_type).set_string(this->config_type_recorded);
            selection.insert(this->config_name_file).set_string(recording_name);
        }
    }

    // write the configuration file
    if (0 == this->write_config_file(config)) {
        ERROR("could not save autonomous selections");
        return 0;
    }

    INFO("autonomous selections saved to " + string(this->config_file_path));
    return 1;
}


bool umbc::Robot::is_valid_match_time(std::uint32_t match_time_s) {
    return match_time_s >= umbc::Robot::match_time_min_s
        && match_time_s <= umbc::Robot::match_time_max_s
        && 0 == (match_time_s % umbc::Robot::match_time_increment_s);
}


std::uint32_t umbc::Robot::get_match_time(umbc::Robot::MatchType match_type) {

    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            return this->tournament_autonomous_time_ms / this->ms_per_s;
        }

        case umbc::Robot::MatchType::SKILLS: {
            return this->skills_autonomous_time_ms / this->ms_per_s;
        }

        default: {
            ERROR("cannot retrieve match time; invalid match type");
            return 0;
        }
    }
}


void umbc::Robot::set_match_time(umbc::Robot::MatchType match_type, std::uint32_t match_time_s) {

    // check that the match time is valid
    if (false == this->is_valid_match_time(match_time_s)) {
        ERROR("cannot set match time; invalid match time of " << match_time_s << " seconds");
        return;
    }

    // replace the previous match time for the match type
    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            this->tournament_autonomous_time_ms = match_time_s * this->ms_per_s;
            break;
        }

        case umbc::Robot::MatchType::SKILLS: {
            this->skills_autonomous_time_ms = match_time_s * this->ms_per_s;
            break;
        }

        default: {
            ERROR("cannot set match time; invalid match type");
            return;
        }
    }

    // save the match times so they persist between robot restarts
    this->save_match_times();
}


std::int32_t umbc::Robot::load_match_times() {

    // start from the default match times
    this->tournament_autonomous_time_ms = this->default_tournament_autonomous_time_ms;
    this->skills_autonomous_time_ms = this->default_skills_autonomous_time_ms;

    // read in the configuration file
    Json config;
    if (0 == this->read_config_file(config)) {
        WARN("using default match times");
        return 0;
    }

    // retrieve the match time object
    const Json* match_time = config.get(this->config_name_match_time);
    if (nullptr == match_time) {
        WARN("no match times in " + string(this->config_file_path) + "; using default match times");
        return 1;
    }

    // load the match time for each match type
    const umbc::Robot::MatchType match_time_match_types[] =
        {umbc::Robot::MatchType::TOURNAMENT, umbc::Robot::MatchType::SKILLS};
    for (umbc::Robot::MatchType match_time_match_type : match_time_match_types) {

        bool is_skills = (umbc::Robot::MatchType::SKILLS == match_time_match_type);
        const char* match_time_name = is_skills ? this->config_name_skills : this->config_name_competition;

        // retrieve the match time value for the match type
        const Json* match_time_value = match_time->get(match_time_name);
        if (nullptr == match_time_value || false == match_time_value->is_number()) {
            WARN("no " + string(match_time_name) + " match time in "
                + string(this->config_file_path) + "; using default match time");
            continue;
        }

        // check that the match time is a valid whole number of seconds
        double match_time_s = match_time_value->get_number(0.0);
        if (false == (0.0 <= match_time_s && (double)this->match_time_max_s >= match_time_s)
            || match_time_s != (double)((std::uint32_t)match_time_s)
            || false == this->is_valid_match_time((std::uint32_t)match_time_s)) {
            WARN("invalid " + string(match_time_name) + " match time in "
                + string(this->config_file_path) + "; using default match time");
            continue;
        }

        // apply the match time
        if (true == is_skills) {
            this->skills_autonomous_time_ms = (std::uint32_t)match_time_s * this->ms_per_s;
        }
        else {
            this->tournament_autonomous_time_ms = (std::uint32_t)match_time_s * this->ms_per_s;
        }
        INFO(string(match_time_name) + " match time loaded as " << (std::uint32_t)match_time_s << " seconds");
    }

    return 1;
}


std::int32_t umbc::Robot::save_match_times() {

    // check that an SD card is installed
    if (0 == pros::usd::is_installed()) {
        ERROR("cannot save match times; no SD card detected");
        return 0;
    }

    // read in the current configuration file to preserve unrelated values
    Json config;
    if (0 == this->read_config_file(config)) {
        WARN("recreating " + string(this->config_file_path));
    }

    // update the match time for each match type
    Json& match_time = config.insert(this->config_name_match_time);
    match_time.insert(this->config_name_competition).set_number(
        (double)(this->tournament_autonomous_time_ms / this->ms_per_s));
    match_time.insert(this->config_name_skills).set_number(
        (double)(this->skills_autonomous_time_ms / this->ms_per_s));

    // write the configuration file
    if (0 == this->write_config_file(config)) {
        ERROR("could not save match times");
        return 0;
    }

    INFO("match times saved to " + string(this->config_file_path));
    return 1;
}


void umbc::Robot::robot_opcontrol(Robot* robot) {
    robot->opcontrol();
}


std::string umbc::Robot::get_autonomous_directory(umbc::Robot::MatchType match_type) {

    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            return string(umbc::Robot::autonomous_directory) + "/" + umbc::Robot::autonomous_competition_directory;
        }

        case umbc::Robot::MatchType::SKILLS: {
            return string(umbc::Robot::autonomous_directory) + "/" + umbc::Robot::autonomous_skills_directory;
        }

        default: {
            ERROR("cannot retrieve autonomous directory; invalid match type");
            return std::string();
        }
    }
}


std::string umbc::Robot::get_autonomous_list_directory(umbc::Robot::MatchType match_type) {

    switch (match_type) {

        case umbc::Robot::MatchType::TOURNAMENT: {
            return string(umbc::Robot::autonomous_list_directory) + "/" + umbc::Robot::autonomous_competition_directory;
        }

        case umbc::Robot::MatchType::SKILLS: {
            return string(umbc::Robot::autonomous_list_directory) + "/" + umbc::Robot::autonomous_skills_directory;
        }

        default: {
            ERROR("cannot retrieve autonomous list directory; invalid match type");
            return std::string();
        }
    }
}


std::string umbc::Robot::autonomous_file_path(umbc::Robot::MatchType match_type,
    const std::string& recording_name, bool is_partner_controller) {

    // check that the match type has an autonomous directory
    std::string directory = this->get_autonomous_directory(match_type);
    if (directory.empty()) {
        return std::string();
    }

    std::string file_path = directory + "/" + recording_name;

    // append the partner suffix for partner controller recordings
    if (true == is_partner_controller) {
        file_path += this->autonomous_partner_suffix;
    }

    return file_path + this->autonomous_file_extension;
}


void umbc::Robot::autonomous(bool is_partner_controller) {

    INFO("autonomous active");

    // retrieve the selection for the current match type
    umbc::Robot::AutonomousType autonomous_type = this->get_autonomous_type(this->match_type);
    std::string recording_name = this->get_autonomous_name(this->match_type);

    // perform the user-programmed autonomous routine
    if (umbc::Robot::AutonomousType::PROGRAMMED == autonomous_type) {

        if (umbc::Robot::MatchType::SKILLS == this->match_type) {
            INFO("performing programmed skills autonomous routine...");
            this->autonomous_skills();
            INFO("programmed skills autonomous routine completed");
        }
        else {
            INFO("performing programmed competition autonomous routine...");
            this->autonomous_competition();
            INFO("programmed competition autonomous routine completed");
        }
    }

    // play back the recorded autonomous routine
    else if (umbc::Robot::AutonomousType::RECORDED == autonomous_type) {
        this->autonomous_recorded(recording_name, is_partner_controller);
    }

    // no valid autonomous selection
    else {
        ERROR("cannot perform autonomous; invalid autonomous selection");
        pros::lcd::set_text(3, "Invalid Autonomous Selection");
    }

    INFO("autonomous completed");
}


void umbc::Robot::autonomous_recorded(const std::string& recording_name, bool is_partner_controller) {

    std::string file_path_master = this->autonomous_file_path(this->match_type, recording_name, false);
    std::string file_path_partner = this->autonomous_file_path(this->match_type, recording_name, true);

    // check that the recording exists
    FileExplorer file_explorer = FileExplorer(this->get_autonomous_list_directory(this->match_type));
    if (false == file_explorer.is_file(recording_name + this->autonomous_file_extension)) {
        ERROR("cannot play back recorded autonomous; " + file_path_master + " does not exist");
        pros::lcd::set_text(3, "Recording Not Found");
        return;
    }

    // switch to virtual controllers for autonomous
    INFO("setting robot controllers to virtual controllers...");
	this->set_controllers_to_virtual();
	INFO("robot controllers set to virtual controllers");

    // load master controller input file
    INFO("loading input file for virtual master controller...");
    this->vcontroller_master.load(file_path_master);
    INFO("loaded " << file_path_master << " as input file for virtual master controller");

    // load partner controller input file if needed
    if (true == is_partner_controller) {
        INFO("loading input file for virtual partner controller...");
        this->vcontroller_partner.load(file_path_partner);
        INFO("loaded " << file_path_partner << " as input file for virtual partner controller");
    }

    // start opcontrol task
    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    // start virtual controller tasks
    INFO("starting task for virtual master controller...");
    this->vcontroller_master.start();
    INFO("virtual master controller task started");

    // start partner controller task if needed
    if (true ==  is_partner_controller) {
        INFO("starting task for virtual partner controller...");
        this->vcontroller_partner.start();
        INFO("virtual partner controller task started");
    }

    // wait for virtual controller inputs to complete
    INFO("waiting for virtual master controller input to complete...");
    this->vcontroller_master.wait_till_complete();
    INFO("virtual master controller input completed");

    // wait for partner controller input if needed
    if (true == is_partner_controller) {
        INFO("waiting for virtual partner controller input to complete...");
        this->vcontroller_partner.wait_till_complete();
        INFO("virtual partner controller input completed");
    }

    // terminate opcontrol task
    INFO("terminating opcontrol task...");
    this->opcontrol_stop();
    INFO("opcontrol task has been terminated");

    // switch back to physical controllers
    INFO("setting robot controllers to physical controllers...");
    this->set_controllers_to_physical();
	INFO("robot controllers set to physical controllers");
}


void umbc::Robot::train_autonomous(bool is_partner_controller) {

    INFO("autonomous training active");

    // initialize controller recorders for both controllers
    ControllerRecorder controller_recorder_master = ControllerRecorder(controller_master, opcontrol_delay_ms);
    ControllerRecorder controller_recorder_partner = ControllerRecorder(controller_partner, opcontrol_delay_ms);

    // start opcontrol task
    INFO("starting opcontrol task...");
    this->opcontrol_start();
    INFO("opcontrol task started");

    // start recording master controller inputs
    INFO("starting master controller recording...");
    controller_recorder_master.start();
    INFO("recording master controller has begun");
    
    // start recording partner controller inputs if needed
    if (true == is_partner_controller) {
        INFO("starting partner controller recording...");
        controller_recorder_partner.start();
        INFO("recording partner controller has begun");
    }

    // wait for autonomous skills time to elapse
    if (umbc::Robot::MatchType::SKILLS == this->match_type) {
        INFO("setting task delay for skills autonomous time...");
        pros::Task::delay(this->skills_autonomous_time_ms);
        INFO("task delay set to " << this->skills_autonomous_time_ms << " ms");
    }
    
    // wait for autonomous tournament time to elapse
    else {
        INFO("setting task delay for match autonomous time...");
        pros::Task::delay(this->tournament_autonomous_time_ms);
        INFO("task delay set to " << this->tournament_autonomous_time_ms << " ms");
    }

    // terminate opcontrol task
    INFO("terminating opcontrol task...");
    this->opcontrol_stop();
    INFO("opcontrol task has been terminated");

    // stop recording master controller inputs
    INFO("stopping master controller recording...");
    controller_recorder_master.stop();
    INFO("master controller recording stopped");

    // stop recording partner controller inputs if needed
    if (true == is_partner_controller) {
        INFO("stopping partner controller recording...");
        controller_recorder_partner.stop();
        INFO("partner controller recording stopped");
    }

    // determine the recording name to save the training as
    std::string recording_name = (umbc::Robot::AutonomousType::RECORDED
            == this->get_autonomous_type(this->match_type))
        ? this->get_autonomous_name(this->match_type)
        : ((umbc::Robot::MatchType::SKILLS == this->match_type)
            ? string(this->default_skills_autonomous_name)
            : string(this->default_tournament_autonomous_name));

    std::string file_path_master = this->autonomous_file_path(this->match_type, recording_name, false);
    std::string file_path_partner = this->autonomous_file_path(this->match_type, recording_name, true);

    // save master controller file
    INFO("saving master controller file...");
    if (0 > controller_recorder_master.save(file_path_master.c_str())) {
        ERROR("autonomous training failed; could not save " + file_path_master);
        return;
    }
    INFO("master controller file saved to " << file_path_master);

    // save partner controller file if needed
    if (true == is_partner_controller) {
        INFO("saving partner controller file...");
        controller_recorder_partner.save(file_path_partner.c_str());
        INFO("partner controller file saved to " << file_path_partner);
    }

    // select the new recording so it plays back during autonomous
    this->set_autonomous(this->match_type, umbc::Robot::AutonomousType::RECORDED, recording_name);

    INFO("autonomous training complete");
}


void umbc::Robot::opcontrol_start() {

    this->t_opcontrol.reset(new Task((task_fn_t)this->robot_opcontrol, (void*)this, this->t_opcontrol_name));
    INFO(string(t_opcontrol_name) + " has started");
}


void umbc::Robot::opcontrol_pause() {

    // get opcontrol task pointer
    Task* t_opcontrol = this->t_opcontrol.get();

    // suspend opcontrol task if it exists
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

    // get opcontrol task pointer
    Task* t_opcontrol = this->t_opcontrol.get();

    // resume opcontrol task if it exists
    if (nullptr != t_opcontrol) {
        
        try {
            t_opcontrol->resume();
            INFO(string(t_opcontrol_name) + " has resumed");
        }
        
        catch (...) {
            ERROR("failed to resume " +  string(t_opcontrol_name));
        }
    }
}


void umbc::Robot::opcontrol_stop() {

    // get opcontrol task pointer
    Task* t_opcontrol = this->t_opcontrol.get();
    
    // remove opcontrol task if it exists
    if (nullptr != t_opcontrol) {
        
        try {
            t_opcontrol->remove();
            INFO(string(t_opcontrol_name) + " is stopped");
        }
        
        catch (...) {
            ERROR("failed to stop " + string(t_opcontrol_name));
        }
    }
}


bool umbc::Robot::is_opcontrol_listed() {

    // get opcontrol task pointer
    Task* t_opcontrol = this->t_opcontrol.get();

    // determine if opcontrol task is listed
    return (nullptr == t_opcontrol) ? false
        : ((t_opcontrol->get_state() != E_TASK_STATE_INVALID) && (t_opcontrol->get_state() != E_TASK_STATE_DELETED));
}
