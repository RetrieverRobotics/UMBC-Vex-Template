/**
 * \file umbc/controllerrecorder.cpp
 *
 * Contains the implementatino of the ControllerRecorder. ControllerRecorder
 * saves controller input at set poll rate. The files created by the
 * ControllerRecorder are meant to be used as input for the VController.
 */

#include "api.h"
#include "umbc.h"

#include <fstream>
#include <queue>
#include <cstdint>

using namespace pros;
using namespace umbc;
using namespace std;

umbc::ControllerRecorder::ControllerRecorder(pros::Controller* controller, std::uint16_t poll_rate_ms) {

    this->controller = controller;
    this->poll_rate_ms = 0;
    this->controller_input = std::queue<ControllerInput>();
    this->t_record_controller_input.reset(nullptr);
}

void umbc::ControllerRecorder::record(void* ControllerRecorder) {

    umbc::ControllerRecorder* controller_recorder = (umbc::ControllerRecorder*)ControllerRecorder;

    if (0 == controller_recorder->poll_rate_ms) {
        return;
    }

    std::uint32_t now = pros::millis();

    while (INT32_MAX > controller_recorder->controller_input.empty())
    {
        ControllerInput controller_input;

        controller_input.set_digital(E_CONTROLLER_DIGITAL_L1,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_L1));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_L2,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_L2));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_R1,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_R1));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_R2,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_R2));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_UP,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_UP));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_DOWN,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_DOWN));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_LEFT,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_LEFT));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_RIGHT,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_RIGHT));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_X,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_X));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_B,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_B));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_Y,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_Y));
        controller_input.set_digital(E_CONTROLLER_DIGITAL_A,
            controller_recorder->controller->get_digital(E_CONTROLLER_DIGITAL_A));

        controller_input.set_analog(E_CONTROLLER_ANALOG_LEFT_X,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_LEFT_X));
        controller_input.set_analog(E_CONTROLLER_ANALOG_LEFT_Y,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        controller_input.set_analog(E_CONTROLLER_ANALOG_LEFT_X,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_RIGHT_X));
        controller_input.set_analog(E_CONTROLLER_ANALOG_RIGHT_Y,
            controller_recorder->controller->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

        controller_recorder->controller_input.push(controller_input);

        pros::Task::delay_until(&now, controller_recorder->poll_rate_ms);
    }

    return;
}

std::int32_t umbc::ControllerRecorder::save(const char* file_path) {

    std::int32_t number_of_controller_inputs = this->controller_input.size();

    if (0 == this->poll_rate_ms || this->controller_input.empty()) {
        return -1;
    }

    std::ofstream file(file_path, std::ifstream::binary);
    if (!file.good()) {
        file.close();
        return -1;
    }

    file.write((char*)(&(this->poll_rate_ms)), sizeof(this->poll_rate_ms));
    if (!file.good()) {
        file.close();
        return -1;
    }

    while (!this->controller_input.empty()) {
        file.write((char*)(&(this->controller_input.front())), sizeof(umbc::ControllerInput));
        if (!file.good()) {
            file.close();
            this->reset();
            return -1;
        }
        this->controller_input.pop();
    }
    file.close();

    return number_of_controller_inputs;
}

void umbc::ControllerRecorder::start() {

    this->t_record_controller_input.reset(
        new Task((task_fn_t)this->record, (void*)this, this->t_record_controller_input_name));
}

void umbc::ControllerRecorder::pause() {

    Task* t_record = this->t_record_controller_input.get();

    if (nullptr != t_record) {
        t_record->suspend();
    }
}

void umbc::ControllerRecorder::resume() {

    Task* t_record = this->t_record_controller_input.get();

    if (nullptr != t_record) {
        t_record->resume();
    }
}

void umbc::ControllerRecorder::stop() {

    Task* t_record = this->t_record_controller_input.get();

    if (nullptr != t_record) {
        t_record->remove();
    }
}

void umbc::ControllerRecorder::reset() {
    this->controller_input = queue<ControllerInput>();
}

std::int32_t umbc::ControllerRecorder::isRecording() {

    Task* t_record = this->t_record_controller_input.get();

    if (nullptr == t_record) {
        return 0;
    }

    return this->t_record_controller_input.get()->get_state() == E_TASK_STATE_RUNNING;
}

std::int32_t umbc::ControllerRecorder::hasControllerInput() {
    return !this->controller_input.empty();
}