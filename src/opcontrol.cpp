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

void umbc::Robot::opcontrol() {

    // nice names for controllers (do not edit)
    umbc::Controller* controller_master = this->controller_master;
    umbc::Controller* controller_partner = this->controller_partner;

    // initialize motors and sensors


    while(1) {

        // implement opcontrols


        // required loop delay (do not edit)
        pros::Task::delay(this->opcontrol_delay_ms);
    }
}