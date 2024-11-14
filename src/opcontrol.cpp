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

#define MOTOR_RED_GEAR_MULTIPLIER    100
#define MOTOR_GREEN_GEAR_MULTIPLIER  200
#define MOTOR_BLUE_GEAR_MULTIPLIER   600
#define MOTOR_REVERSE                true

#define LEFT_MOTOR_PORT 17
#define RIGHT_MOTOR_PORT 18

void umbc::Robot::opcontrol() {

    // nice names for controllers (do not edit)
    umbc::Controller* controller_master = this->controller_master;
    umbc::Controller* controller_partner = this->controller_partner;

    // initialize motors and sensors
    pros::Motor drive_left_motor = pros::Motor(LEFT_MOTOR_PORT);
    pros::MotorGroup drive_left = pros::MotorGroup(vector<pros::Motor>{drive_left_motor});

    pros::Motor drive_right_motor = pros::Motor(RIGHT_MOTOR_PORT);
    pros::MotorGroup drive_right = pros::MotorGroup(vector<pros::Motor>{drive_right_motor});

    while(1) {
        int32_t arcade_y = controller_master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
        int32_t arcade_x = controller_master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
        // implement opcontrols
        int32_t drive_left_velocity = (int32_t)(((double)(arcade_y + arcade_x) / (double)E_CONTROLLER_ANALOG_MAX)
                                        * MOTOR_GREEN_GEAR_MULTIPLIER);

        int32_t drive_right_velocity = (int32_t)(((double)(arcade_y - arcade_x) / (double)E_CONTROLLER_ANALOG_MAX)
                                        * MOTOR_GREEN_GEAR_MULTIPLIER);

        drive_left.move_velocity(drive_left_velocity);
        drive_right.move_velocity(drive_right_velocity);
        // required loop delay (do not edit)
        pros::Task::delay(this->opcontrol_delay_ms);
    }
}