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


void start() {

}


void pause() {

}

void resume() {

}

void stop() {

}

void reset() {

}

std::int32_t isRecording() {

}