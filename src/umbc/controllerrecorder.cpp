/**
 * @file umbc/controllerrecorder.cpp
 *
 * Contains the implementatino of the ControllerRecorder. ControllerRecorder
 * saves controller input at set poll rate. The files created by the
 * ControllerRecorder are meant to be used as input for the VController.
 */

// standard libraries
#include <fstream>
#include <queue>
#include <cstdint>
#include <string>

// local header files
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;


umbc::ControllerRecorder::ControllerRecorder(umbc::Controller* controller, std::uint16_t poll_rate_ms) {

    this->controller = controller;
    this->poll_rate_ms = poll_rate_ms;
    this->controller_input = std::queue<ControllerInput>();
    this->t_record_controller_input.reset(nullptr);
}


void umbc::ControllerRecorder::record(void* ControllerRecorder) {

    // cast the controller recorder from a void pointer to a ControllerRecorder pointer
    umbc::ControllerRecorder* controller_recorder = (umbc::ControllerRecorder*)ControllerRecorder;

    // invalid poll rate
    if (0 == controller_recorder->poll_rate_ms) {
        ERROR("invalid poll rate");
        return;
    }

    // get the current time in milliseconds
    std::uint32_t now = pros::millis();

    INFO("recording controller input...");

    // continuously poll controller input at the poll rate and push it to a queue until the number of recorded
    while (INT32_MAX > controller_recorder->controller_input.size()) {

        ControllerInput controller_input;

        // L1 bumper digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_L1,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_L1));

        // L2 trigger digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_L2,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_L2));

        // R1 bumper digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_R1,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_R1));

        // R2 trigger digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_R2,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_R2));

        // Up digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_UP,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_UP));

        // Down digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_DOWN,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_DOWN));

        // Left digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_LEFT,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_LEFT));

        // Right digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_RIGHT,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_RIGHT));

        // X action digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_X,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_X));

        // B action digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_B,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_B));

        // Y action digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_Y,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_Y));

        // A action digital controller button
        controller_input.set_digital(E_CONTROLLER_DIGITAL_A,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_A));

        // left horizontal analog controller thumbstick
        controller_input.set_analog(E_CONTROLLER_ANALOG_LEFT_X,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_LEFT_X));

        // left vertical analog controller thumbstick
        controller_input.set_analog(E_CONTROLLER_ANALOG_LEFT_Y,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_LEFT_Y));

        // right horizontal analog controller thumbstick
        controller_input.set_analog(E_CONTROLLER_ANALOG_RIGHT_X,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_RIGHT_X));

        // right vertical analog controller thumbstick
        controller_input.set_analog(E_CONTROLLER_ANALOG_RIGHT_Y,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

        // save the current controller input to the recorder input queue
        controller_recorder->controller_input.push(controller_input);

        // delay until the next poll time
        pros::Task::delay_until(&now, controller_recorder->poll_rate_ms);
    }

    INFO("max controller input recorded reached");

    return;
}


std::int32_t umbc::ControllerRecorder::save(const char* file_path) {

    // get the number of controller inputs recorded
    std::int32_t number_of_controller_inputs = this->controller_input.size();
    
    // convert the file path to a string for easier concatenation in error messages
    string file_path_str = string(file_path);

    // check if there is any controller input to save
    if (0 == this->poll_rate_ms || this->controller_input.empty()) {
        WARN("nothing to save to " + file_path_str);
        return -1;
    }

    // create and open a binary file to save the poll rate and controller input
    std::ofstream file(file_path, std::ifstream::binary);

    // check if the file was successfully opened
    if (!file.good()) {
        file.close();
        ERROR("could not open " + file_path_str);
        return -1;
    }

    INFO("writing poll rate to " + file_path_str + "...");

    // write the poll rate to the file
    file.write((char*)(&(this->poll_rate_ms)), sizeof(this->poll_rate_ms));
    
    // check if the poll rate was successfully written to the file
    if (!file.good()) {
        file.close();
        ERROR("failed to write poll rate to " + file_path_str);
        return -1;
    }

    INFO("poll rate written to " + file_path_str);
    INFO("writing controller input to " + file_path_str + "...");

    // write the controller input to the file
    while (!this->controller_input.empty()) {

        // write the front controller input in the queue to the file
        file.write((char*)(&(this->controller_input.front())), sizeof(umbc::ControllerInput));
        
        // check if the controller input was successfully written to the file
        if (!file.good()) {
            file.close();
            this->reset();
            ERROR("failed to write controller input to " + file_path_str);
            return -1;
        }
        this->controller_input.pop();
    }

    INFO("controller input written to " + file_path_str);
    
    // close the file
    file.close();

    return number_of_controller_inputs;
}


void umbc::ControllerRecorder::start() {

    // check if already recording
    this->t_record_controller_input.reset(
        new Task((task_fn_t)this->record, (void*)this, this->t_record_controller_input_name));

    INFO(string(t_record_controller_input_name) + " has started");
}


void umbc::ControllerRecorder::pause() {

    // get the task for recording controller input
    Task* t_record = this->t_record_controller_input.get();

    // check if the task exists before trying to suspend it
    if (nullptr != t_record) {
        try {
            t_record->suspend();
            INFO(string(t_record_controller_input_name) + " is paused");
        }
        catch (...) {
            ERROR("failed to suspend " + string(t_record_controller_input_name));
        }
    }
}

void umbc::ControllerRecorder::resume() {

    // get the task for recording controller input
    Task* t_record = this->t_record_controller_input.get();

    // check if the task exists before trying to resume it
    if (nullptr != t_record) {
        
        try {
            t_record->resume();
            INFO(string(t_record_controller_input_name) + " has resumed");
        }
        
        catch (...) {
            ERROR("failed to resume " +  string(t_record_controller_input_name));
        }
    }
}


void umbc::ControllerRecorder::stop() {

    // get the task for recording controller input
    Task* t_record = this->t_record_controller_input.get();

    // check if the task exists before trying to remove it
    if (nullptr != t_record) {
        
        try {
            t_record->remove();
            INFO(string(t_record_controller_input_name) + " is stopped");
        }
        
        catch (...) {
            ERROR("failed to stop " + string(t_record_controller_input_name));
        }
    }
}


void umbc::ControllerRecorder::reset() {
    this->controller_input = queue<ControllerInput>();
}


bool umbc::ControllerRecorder::isRecording() {

    // get the task for recording controller input
    Task* t_record = this->t_record_controller_input.get();

    // check if the task exists and is running
    return (nullptr == t_record) ? false : (t_record->get_state() == E_TASK_STATE_RUNNING);
}


bool umbc::ControllerRecorder::isControllerInput() {
    return !this->controller_input.empty();
}
