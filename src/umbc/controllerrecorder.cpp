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

umbc::ControllerRecorder::ControllerRecorder(controller_id_e_t controller_id, std::uint16_t poll_rate_ms) {

    this->poll_rate_ms = 0;
    this->controller_input = std::queue<ControllerInput>();
    this->t_record_controller_input.reset(nullptr);
}

void umbc::ControllerRecorder::record(void* ControllerRecorder) {

}

std::int32_t save(const char* file_path) {

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

}

std::int32_t umbc::ControllerRecorder::hasControllerInput() {

}