/**
 * \file opcontrol.cpp
 *
 * Contains user implemented opcontrol. User must use the
 * parameters to the opcontrol function when referencing
 * the master V5 controller or partner V5 controller.
 */

#include "api.h"
#include "umbc.h"

#include <cstdint>

using namespace pros;
using namespace umbc;
using namespace std;

void umbc::Robot::opcontrol(pros::Controller controller_master, pros::Controller controller_partner) {

    // initialize motors and sensors

    while(1) {

        // implement opcontrols

        pros::Task::delay(this->opcontrol_delay_ms);
    }
}