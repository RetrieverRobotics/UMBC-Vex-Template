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

    this->controller_input = std::queue<ControllerInput>();
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