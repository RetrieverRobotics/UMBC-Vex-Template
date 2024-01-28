/**
 * \file umbc/pcontroller.cpp
 *
 * Contains the implementaion of the PController. The PController
 * is a wrapper class for pros::Controller.
 */

#include "api.h"
#include "umbc.h"

#include <cstdint>

using namespace pros;
using namespace umbc;
using namespace std;

umbc::PController::PController(controller_id_e_t id) : controller(id) {
    // intentionally blank
}

std::int32_t umbc::PController::is_connected() {
    return this->controller.is_connected();
}

std::int32_t umbc::PController::get_analog(controller_analog_e_t channel) {
    return this->controller.get_analog(channel);
}

std::int32_t umbc::PController::get_battery_capacity() {
    return this->controller.get_battery_capacity();
}

std::int32_t umbc::PController::get_battery_level() {
    return this->controller.get_battery_level();
}

std::int32_t umbc::PController::get_digital(controller_digital_e_t button) {
    return this->controller.get_digital(button);
}

std::int32_t umbc::PController::get_digital_new_press(controller_digital_e_t button) {
    return this->controller.get_digital_new_press(button);
}

template <typename... Params> std::int32_t umbc::PController::print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args) {
    return this-controller.print(line, col, fmt, args...);
}

std::int32_t umbc::PController::set_text(std::uint8_t line, std::uint8_t col, const char* str) {
    return this->controller.set_text(line, col, str);
}

std::int32_t umbc::PController::set_text(std::uint8_t line, std::uint8_t col, const std::string& str) {
    return this->controller.set_text(line, col, str);
}

std::int32_t umbc::PController::clear_line(std::uint8_t line) {
    return this->controller.clear_line(line);
}

std::int32_t umbc::PController::rumble(const char* rumble_pattern) {
    return this->controller.rumble(rumble_pattern);
}

std::int32_t umbc::PController::clear() {
    return this->controller.clear();
}