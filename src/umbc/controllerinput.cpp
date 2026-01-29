/**
 * @file umbc/controllerinput.cpp
 *
 * Contains the implementation of the ControllerInput. ControllerInput
 * stores controller values with a minimal memory footprint.
 */

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
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

    // return the value of the requested analog channel
    switch(channel) {

        // left horizontal analog thumbstick
        case E_CONTROLLER_ANALOG_LEFT_X: {
            return this->analog_left_x;
        }

        // left vertical analog thumbstick
        case E_CONTROLLER_ANALOG_LEFT_Y: {
            return this->analog_left_y;
        }
        
        // right horizontal analog thumbstick
        case E_CONTROLLER_ANALOG_RIGHT_X: {
            return this->analog_right_x;
        }
        
        // right vertical analog thumbstick
        case E_CONTROLLER_ANALOG_RIGHT_Y: {
            return this->analog_right_y;
        }

        // invalid channel
        default: {
            return 0;
        }
    }
}


std::int32_t umbc::ControllerInput::get_digital(controller_digital_e_t button) {
    
    // return the value of the requested digital button
    switch(button) {

        // L1 bumper digital button
        case E_CONTROLLER_DIGITAL_L1: {
            return this->digital_l1;
        }
        
        // L2 trigger digital button
        case E_CONTROLLER_DIGITAL_L2: {
            return this->digital_l2;
        }

        // R1 bumper digital button
        case E_CONTROLLER_DIGITAL_R1: {
            return this->digital_r1;
        }

        // R2 trigger digital button
        case E_CONTROLLER_DIGITAL_R2: {
            return this->digital_r2;
        }

        // Up digital button
        case E_CONTROLLER_DIGITAL_UP: {
            return this->digital_up;
        }

        // Down digital button
        case E_CONTROLLER_DIGITAL_DOWN: {
            return this->digital_down;
        }

        // Left digital button
        case E_CONTROLLER_DIGITAL_LEFT: {
            return this->digital_left;
        }
        
        // Right digital button
        case E_CONTROLLER_DIGITAL_RIGHT: {
            return this->digital_right;
        }
        
        // X action digital button
        case E_CONTROLLER_DIGITAL_X: {
            return this->digital_x;
        }
        
        // B action digital button
        case E_CONTROLLER_DIGITAL_B: {
            return this->digital_b;
        }
        
        // Y action digital button
        case E_CONTROLLER_DIGITAL_Y: {
            return this->digital_y;
        }
        
        // A action digital button
        case E_CONTROLLER_DIGITAL_A: {
            return this->digital_a;
        }

        // invalid button
        default: {
            return 0;
        }
    }
}


void umbc::ControllerInput::set_analog(controller_analog_e_t channel, std::int32_t value) {

    const int32_t analog_min = static_cast<int32_t>(ControllerAnalogLimit::MIN);    // minimum analog value
    const int32_t analog_max = static_cast<int32_t>(ControllerAnalogLimit::MAX);    // maximum analog value

    // cap value to minimum
    if (value < analog_min) {
        value = analog_min;
    }
    
    // cap value to maximum
    else if (value > analog_max) {
        value = analog_max;
    }

    // set the value of the requested analog channel
    switch(channel) {

        // left horizontal analog thumbstick
        case E_CONTROLLER_ANALOG_LEFT_X: {
            this->analog_left_x = value;
            return;
        }

        // left vertical analog thumbstick
        case E_CONTROLLER_ANALOG_LEFT_Y: {
            this->analog_left_y = value;
            return;
        }

        // right horizontal analog thumbstick
        case E_CONTROLLER_ANALOG_RIGHT_X: {
            this->analog_right_x = value;
            return;
        }

        // right vertical analog thumbstick
        case E_CONTROLLER_ANALOG_RIGHT_Y: {
            this->analog_right_y = value;
            return;
        }

        // invalid channel
        default: {
            return;
        }
    }
}


void umbc::ControllerInput::set_digital(controller_digital_e_t button, std::int32_t value) {

    // convert value to either 0 or 1
    value = value == 0 ? value : 1;

    // set the value of the requested digital button
    switch(button) {

        // L1 bumper digital button
        case E_CONTROLLER_DIGITAL_L1: {
            this->digital_l1 = value;
            return;
        }

        // L2 trigger digital button
        case E_CONTROLLER_DIGITAL_L2: {
            this->digital_l2 = value;
            return;
        }

        // R1 bumper digital button
        case E_CONTROLLER_DIGITAL_R1: {
            this->digital_r1 = value;
            return;
        }

        // R2 trigger digital button
        case E_CONTROLLER_DIGITAL_R2: {
            this->digital_r2 = value;
            return;
        }

        // Up digital button
        case E_CONTROLLER_DIGITAL_UP: {
            this->digital_up = value;
            return;
        }

        // Down digital button
        case E_CONTROLLER_DIGITAL_DOWN: {
            this->digital_down = value;
            return;
        }
        
        // Left digital button
        case E_CONTROLLER_DIGITAL_LEFT: {
            this->digital_left = value;
            return;
        }

        // Right digital button
        case E_CONTROLLER_DIGITAL_RIGHT: {
            this->digital_right = value;
            return;
        }

        // X action digital button
        case E_CONTROLLER_DIGITAL_X: {
            this->digital_x = value;
            return;
        }

        // B action digital button
        case E_CONTROLLER_DIGITAL_B: {
            this->digital_b = value;
            return;
        }

        // Y action digital button
        case E_CONTROLLER_DIGITAL_Y: {
            this->digital_y = value;
            return;
        }

        // A action digital button
        case E_CONTROLLER_DIGITAL_A: {
            this->digital_a = value;
            return;
        }

        // invalid button
        default: {
            return;
        }
    }
}
