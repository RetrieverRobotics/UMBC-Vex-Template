/**
 * \file umbc/vcontroller.cpp
 *
 * Contains the implementaion of the VController. The VController
 * emulates user controller input through reading a formatted binary
 * file of controller inputs.
 */

#include "api.h"
#include "umbc.h"

#include <fstream>
#include <map>
#include <cstdint>
#include <string>

using namespace pros;
using namespace umbc;
using namespace std;

umbc::VController::VController() {

    this->poll_rate_ms = 0;
    this->controller_input = std::queue<ControllerInput>();
    this->t_update_controller_input.reset(nullptr);

    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_L1, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_L2, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_R1, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_R2, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_UP, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_DOWN, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_LEFT, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_RIGHT, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_X, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_B, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_Y, Digital()));
    this->digitals.insert(std::pair<controller_digital_e_t, Digital>(E_CONTROLLER_DIGITAL_A, Digital()));  
}

void umbc::VController::update(void* vcontroller) {

    umbc::VController* controller = (umbc::VController*)vcontroller;

    if (0 == controller->poll_rate_ms) {
        ERROR("invalid poll rate");
        return;
    }

    std::uint32_t now = pros::millis();

    while (!controller->controller_input.empty()) {

        pros::Task::delay_until(&now, controller->poll_rate_ms);
        controller->controller_input.pop();

        for (auto it = controller->digitals.begin(); it != controller->digitals.end(); it++) {
            it->second.set(controller->controller_input.front().get_digital(it->first));
        }
    }

    for (auto it = controller->digitals.begin(); it != controller->digitals.end(); it++) {
        it->second.reset();
    }
}

std::int32_t umbc::VController::is_connected() {
    return !this->controller_input.empty();
}

std::int32_t umbc::VController::get_analog(controller_analog_e_t channel) {
    return this->controller_input.empty() ? 0 : this->controller_input.front().get_analog(channel);
}

std::int32_t umbc::VController::get_battery_capacity() {
    return 100;
}

std::int32_t umbc::VController::get_battery_level() {
    return 100;
}

std::int32_t umbc::VController::get_digital(controller_digital_e_t button) {
    return this->controller_input.empty() ? 0 : this->controller_input.front().get_digital(button);
}

std::int32_t umbc::VController::get_digital_new_press(controller_digital_e_t button) {

    std::map<controller_digital_e_t, Digital>::iterator digital = this->digitals.find(button);
    return (digital == this->digitals.end()) ? 0 : digital->second.get_new_press();
}

template <typename... Params> std::int32_t umbc::VController::print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args) {
    return 1;
}

std::int32_t umbc::VController::set_text(std::uint8_t line, std::uint8_t col, const char* str) {
    return 1;
}

std::int32_t umbc::VController::set_text(std::uint8_t line, std::uint8_t col, const std::string& str) {
    return 1;
}

std::int32_t umbc::VController::clear_line(std::uint8_t line) {
    return 1;
}

std::int32_t umbc::VController::rumble(const char* rumble_pattern) {
    return 1;
}

std::int32_t umbc::VController::clear() {
    return 1;
}

std::int32_t umbc::VController::load(const char* file_path) {

    this->controller_input = std::queue<ControllerInput>();

    string file_path_str = string(file_path);

    std::ifstream file(file_path, std::ifstream::binary);
    if (!file.good()) {
        file.close();
        ERROR("could not open " + file_path_str);
        return 0;
    }

    INFO("reading poll rate from " + file_path_str + "...");
    file.read((char*)(&(this->poll_rate_ms)), sizeof(this->poll_rate_ms));
    if (!file.good() || 0 == this->poll_rate_ms) {
        this->poll_rate_ms = 0;
        file.close();
        ERROR("failed to read poll rate from " + file_path_str);
        return 0;
    }
    INFO("poll rate is " + std::to_string(this->poll_rate_ms) + "ms");

    INFO("loading in controller data from " + file_path_str + "...");
    while(1) {

        ControllerInput controller_input;
        file.read((char*)(&controller_input), sizeof(controller_input)); 

        if (file.eof()) {
            break;
        } else if (!file.good()) {
            this->poll_rate_ms = 0;
            this->controller_input = std::queue<ControllerInput>();
            file.close();
            ERROR("failed to read controller data from " + file_path_str);
            return 0;
        }
        this->controller_input.push(controller_input);
    }
    INFO("controller data from " + file_path_str + " loaded successfully");

    file.close();
    return 1;
}

std::int32_t umbc::VController::load(std::string& file_path) {
    return this->load(file_path.c_str());
}

void umbc::VController::start() {

    this->t_update_controller_input.reset(
        new Task((task_fn_t)this->update, (void*)this, this->t_update_controller_input_name));
    INFO(string(t_update_controller_input_name) + " has started");
}

void umbc::VController::pause() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        try {
            t_update->suspend();
            INFO(string(t_update_controller_input_name) + " is paused");
        } catch (...) {
            ERROR("failed to pause " + string(t_update_controller_input_name));
        }
    }
}

void umbc::VController::resume() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        try {
            t_update->resume();
            INFO(string(t_update_controller_input_name) + " has resumed");
        } catch (...) {
            ERROR("failed to resume " + string(t_update_controller_input_name));
        }
    }
}

void umbc::VController::stop() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        try {
            t_update->remove();
            INFO(string(t_update_controller_input_name) + " is stopped");
        } catch (...) {
            ERROR("failed to stop " + string(t_update_controller_input_name));
        }
    }

    this->controller_input = std::queue<ControllerInput>();
    INFO("virtual controller input queue is cleared");
}

void umbc::VController::wait_till_complete() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        try {
            t_update->join();
            INFO(string(t_update_controller_input_name) + " has completed");
        } catch (...) {
            ERROR("failed to complete " + string(t_update_controller_input_name));
        }
    }
}

umbc::VController::Digital::Digital(){
    this->reset();
}

void umbc::VController::Digital::set(std::int32_t value) {

    this->previous = this->current;
    this->current = value;
    this->newPress = this->newPress || (this->current && !this->previous);
}

void umbc::VController::Digital::reset() {

    this->current = 0;
    this->previous = 0;
    this->newPress = 0;
}

std::int32_t umbc::VController::Digital::get() {
    return this->current;
}

std::int32_t umbc::VController::Digital::get_new_press() {

    int32_t newPress = this->newPress;
    this->newPress = 0;
    return newPress;
}