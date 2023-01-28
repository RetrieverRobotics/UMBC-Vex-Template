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

using namespace pros;
using namespace umbc;
using namespace std;

umbc::VController::VController():Controller(E_CONTROLLER_MASTER) {

    this->poll_rate_ms = 0;
    this->controller_input.reset(new std::queue<ControllerInput>());
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
    std::uint32_t now = pros::millis();

    while (!controller->controller_input.get()->empty()) {
        pros::Task::delay_until(&now, controller->get_poll_rate_ms());
        controller->controller_input.get()->pop();

        // TODO: update buttons.
    }

    // TODO: zero buttons through destruction.
}

std::int32_t umbc::VController::is_connected() {

    std::queue<ControllerInput>* controller_input = this->controller_input.get();

    if (controller_input == nullptr || controller_input->empty()) {
        return 0;
    }
    return 1;
}

std::int32_t umbc::VController::get_analog(controller_analog_e_t channel) {

    std::queue<ControllerInput>* controller_input = this->controller_input.get();

    if (controller_input == nullptr || controller_input->empty()) {
        return 0;
    }
    return controller_input->front().get_analog(channel);
}

std::int32_t umbc::VController::get_battery_capacity() {
    return 100;
}

std::int32_t umbc::VController::get_battery_level() {
    return 100;
}

std::int32_t umbc::VController::get_digital(controller_digital_e_t button) {

    std::queue<ControllerInput>* controller_input = this->controller_input.get();

    if (controller_input == nullptr || controller_input->empty()) {
        return 0;
    }
    return controller_input->front().get_digital(button);
}

std::int32_t umbc::VController::get_digital_new_press(controller_digital_e_t button) {

    std::map<controller_digital_e_t, Digital>::iterator digital = this->digitals.find(button);

    if (digital == this->digitals.end()) {
        return 0;
    }
    return digital->second.get_new_press();
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

std::int32_t umbc::VController::get_poll_rate_ms() {
    return this->poll_rate_ms;
}

std::int32_t umbc::VController::load(const char* file_path) {

    this->controller_input.reset(new std::queue<ControllerInput>());

    std::ifstream file(file_path, std::ifstream::binary);
    if (!file.good()) {
        file.close();
        return 0;
    }

    file.read((char*)(&(this->poll_rate_ms)), sizeof(this->poll_rate_ms));
    if (!file.good() || 0 == this->poll_rate_ms) {
        this->poll_rate_ms = 0;
        file.close();
        return 0;
    }

    while(1) {
        ControllerInput controller_input;
        file.read((char*)(&controller_input), sizeof(controller_input)); 

        if (file.eof()) {
            break;
        } else if (!file.good()) {
            this->poll_rate_ms = 0;
            this->controller_input.reset(new std::queue<ControllerInput>());
            file.close();
            return 0;
        }
        this->controller_input.get()->push(controller_input);
    } 

    file.close();
    return 1;
}

std::int32_t umbc::VController::load(std::string& file_path) {
    return this->load(file_path.c_str());
}

void umbc::VController::start() {

    this->t_update_controller_input.reset(
        new Task((task_fn_t)this->update, (void*)this, this->t_update_controller_input_name));
}

void umbc::VController::pause() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        t_update->suspend();
    }
}

void umbc::VController::resume() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        t_update->resume();
    }
}

void umbc::VController::stop() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        t_update->remove();
    }

    this->controller_input.reset(new std::queue<ControllerInput>());
}

void umbc::VController::wait_till_complete() {

    Task* t_update = this->t_update_controller_input.get();

    if (nullptr != t_update) {
        t_update->join();
    }
}