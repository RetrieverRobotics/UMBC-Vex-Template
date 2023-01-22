/**
 * \file umbc/controllerinput.cpp
 *
 * Contains the implementation of the ControllerInput. ControllerInput
 * stores controller values with a minimal memory footprint.
 */

#include "api.h"
#include "umbc.h"

#include <cstdint>

using namespace pros;
using namespace umbc;

umbc::ControllerInput::ControllerInput() { 

    this->digital_l1 = 0;
    this->digital_l2 = 0;
    this->digital_r1 = 0;
    this->digital_r2 = 0;
    this->digital_up = 0;
    this->digital_down = 0;
    this->digital_left = 0;
    this->digital_right = 0;
    this->digital_x = 0;
    this->digital_b = 0;
    this->digital_y = 0;
    this->digital_a = 0;

    this->analog_left_x = 0;
    this->analog_left_y = 0;
    this->analog_right_x = 0;
    this->analog_right_y = 0;
}

std::int32_t umbc::ControllerInput::get_analog(controller_analog_e_t channel) {

    switch(channel){
        case E_CONTROLLER_ANALOG_LEFT_X:
            return this->analog_left_x;
        case E_CONTROLLER_ANALOG_LEFT_Y:
            return this->analog_left_y;
        case E_CONTROLLER_ANALOG_RIGHT_X:
            return this->analog_right_x;
        case E_CONTROLLER_ANALOG_RIGHT_Y:
            return this->analog_right_y;
        default:
            return 0;
    }
}

std::int32_t umbc::ControllerInput::get_digital(controller_digital_e_t button) {
    
    switch(button) {
        case E_CONTROLLER_DIGITAL_L1:
            return this->digital_l1;
        case E_CONTROLLER_DIGITAL_L2:
            return this->digital_l2;
        case E_CONTROLLER_DIGITAL_R1:
            return this->digital_r1;
        case E_CONTROLLER_DIGITAL_R2:
            return this->digital_r2;
        case E_CONTROLLER_DIGITAL_UP:
            return this->digital_up;
        case E_CONTROLLER_DIGITAL_DOWN:
            return this->digital_down;
        case E_CONTROLLER_DIGITAL_LEFT:
            return this->digital_left;
        case E_CONTROLLER_DIGITAL_RIGHT:
            return this->digital_right;
        case E_CONTROLLER_DIGITAL_X:
            return this->digital_x;
        case E_CONTROLLER_DIGITAL_B:
            return this->digital_b;
        case E_CONTROLLER_DIGITAL_Y:
            return this->digital_y;
        case E_CONTROLLER_DIGITAL_A:
            return this->digital_a;
        default:
            return 0;
    }
}

void umbc::ControllerInput::set_analog(controller_analog_e_t channel, std::int32_t value) {

    if (value < E_CONTROLLER_ANALOG_MIN) {
        value = E_CONTROLLER_ANALOG_MIN;
    } else if (value > E_CONTROLLER_ANALOG_MAX) {
        value = E_CONTROLLER_ANALOG_MAX;
    }

    switch(channel) {
        case E_CONTROLLER_ANALOG_LEFT_X:
            this->analog_left_x = value;
            return;
        case E_CONTROLLER_ANALOG_LEFT_Y:
            this->analog_left_y = value;
            return;
        case E_CONTROLLER_ANALOG_RIGHT_X:
            this->analog_right_x = value;
            return;
        case E_CONTROLLER_ANALOG_RIGHT_Y:
            this->analog_right_y = value;
            return;
        default:
            return;
    }
}

void umbc::ControllerInput::set_digital(controller_digital_e_t button, std::int32_t value) {

    value = value == 0 ? value : 1;

    switch(button) {
        case E_CONTROLLER_DIGITAL_L1:
            this->digital_l1 = value;
            return;
        case E_CONTROLLER_DIGITAL_L2:
            this->digital_l2 = value;
            return;
        case E_CONTROLLER_DIGITAL_R1:
            this->digital_r1 = value;
            return;
        case E_CONTROLLER_DIGITAL_R2:
            this->digital_r2 = value;
            return;
        case E_CONTROLLER_DIGITAL_UP:
            this->digital_up = value;
            return;
        case E_CONTROLLER_DIGITAL_DOWN:
            this->digital_down = value;
            return;
        case E_CONTROLLER_DIGITAL_LEFT:
            this->digital_left = value;
            return;
        case E_CONTROLLER_DIGITAL_RIGHT:
            this->digital_right = value;
            return;
        case E_CONTROLLER_DIGITAL_X:
            this->digital_x = value;
            return;
        case E_CONTROLLER_DIGITAL_B:
            this->digital_b = value;
            return;
        case E_CONTROLLER_DIGITAL_Y:
            this->digital_y = value;
            return;
        case E_CONTROLLER_DIGITAL_A:
            this->digital_a = value;
            return;
        default:
            return;
    }
}