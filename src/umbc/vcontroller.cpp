/**
 * \file umbc/vcontroller.cpp
 *
 * Contains the implementaion of the VController. The VController
 * emulates user controller input through reading a formatted binary
 * file of controller inputs.
 */

#include "api.h"
#include "umbc.h"

#include <cstdint>

using namespace pros;
using namespace umbc;

umbc::VController::VController():Controller(E_CONTROLLER_MASTER) {

}

std::int32_t umbc::VController::update() {

}

std::int32_t umbc::VController::is_connected() {

}

std::int32_t umbc::VController::get_analog(controller_analog_e_t channel) {

}

std::int32_t umbc::VController::get_battery_capacity() {
    return 100;
}

std::int32_t umbc::VController::get_battery_level() {
    return 100;
}

std::int32_t umbc::VController::get_digital(controller_digital_e_t button) {

}

std::int32_t umbc::VController::get_digital_new_press(controller_digital_e_t button) {

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

}

std::int32_t umbc::VController::load(std::string& file_path) {

}

std::int32_t umbc::VController::start() {

}

void umbc::VController::pause() {

}

void umbc::VController::resume() {

}

void umbc::VController::stop() {

}

void umbc::VController::wait_till_complete() {

}