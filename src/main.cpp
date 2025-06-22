/**
 * \file main.cpp
 *
 * Contains the main functions for the robot for the Vex Robotics
 * Competition.
 */

 #include <iostream>

#include "main.h"
#include "api.h"
#include "umbc.h"

#define PARTNER_CONTROLLER 0

using namespace pros;
using namespace umbc;
using namespace std;

umbc::Robot robot = Robot();

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	INFO("initializing robot...");

	pros::lcd::initialize();

	INFO("robot initialized");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

	INFO("disabling robot...");

	pros::lcd::clear();
	pros::lcd::set_text(1, "Robot Disabled");

	if (robot.opcontrol_isListed()) {
		robot.opcontrol_stop();
	}

	INFO("robot disabled");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	
	INFO("performing competition initialization...");

	pros::lcd::clear();
	robot.menu();

	INFO("competition initialization completed");
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	INFO("performing autonomous routine...");

	pros::lcd::clear();
	pros::lcd::set_text(1, "Autonomous Active");
	robot.autonomous(PARTNER_CONTROLLER);
	pros::lcd::clear();
	
	INFO("autonomous routine completed");
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	if (robot.opcontrol_isListed()) {
		robot.opcontrol_stop();
	}

	while (1) {
		pros::lcd::clear();

		INFO("setting robot controllers to physical controllers...");
		robot.set_controllers_to_physical();
		INFO("robot controllers set to physical controllers");

		if (MODE_TRAIN_AUTONOMOUS == robot.get_mode()) {
			INFO("autonomous training starting...");
			if (pros::usd::is_installed()) {
				pros::lcd::set_text(1, "Autonomous Training Active");
				robot.train_autonomous(PARTNER_CONTROLLER);
				pros::lcd::clear();
    			pros::lcd::set_text(1, "Autonomous Training Complete");
    			INFO("autonomous training complete");
				pros::Task::delay(MSG_DELAY_MS);
				competition_initialize();
			} else {
				ERROR("autonomous training failed; no SD Card detected");
				pros::lcd::clear();
				pros::lcd::set_text(1, "No SD Card Detected!");
				pros::lcd::set_text(3, "Power off V5 Brain and insert");
				pros::lcd::set_text(4, "SD Card.");
				return;
			}
		} else {
			pros::lcd::set_text(1, "Opcontrol Active");
			INFO("opcontrol active");
			robot.opcontrol();
		}
	}
}
