/**
 * @file main.cpp
 *
 * Contains the main functions for the robot for the Vex Robotics
 * Competition.
 */

// standard libraries
#include <iostream>

// local header files
#include "main.h"
#include "api.h"
#include "umbc.h"

// namespaces used
using namespace pros;
using namespace umbc;
using namespace std;

// 1 for if the partner controller is being used or 0 if not
#define PARTNER_CONTROLLER 0

// global robot object
umbc::Robot robot = Robot();


/**
 * @brief Runs initialization code. This occurs as soon as the program is started
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	INFO("initializing robot...");

	// initialize the LLEMU
	pros::lcd::initialize();

	INFO("robot initialized");
}


/**
 * @brief Runs while the robot is in the disabled state
 * 
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

	INFO("disabling robot...");

	// display disabled message on the LLEMU
	pros::lcd::clear();
	pros::lcd::set_text(1, "Robot Disabled");

	// stop the opcontrol task if it is currently active
	if (true == robot.is_opcontrol_listed()) {
		robot.opcontrol_stop();
	}

	INFO("robot disabled");
}


/**
 * @brief Runs after the robot is enabled, and autonomous is selected
 * 
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

	// display robot menu on the LLEMU for selecting match type and mode
	pros::lcd::clear();
	Menu menu = Menu(&robot);
	menu.robot_menu();

	INFO("competition initialization completed");
}


/**
 * @brief Runs the user autonomous code
 * 
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

	// perform trained autonomous routine
	pros::lcd::clear();
	pros::lcd::set_text(1, "Autonomous Active");
	robot.autonomous(PARTNER_CONTROLLER);
	pros::lcd::clear();
	
	INFO("autonomous routine completed");
}


/**
 * @brief Runs the operator control code
 * 
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

	// stop the opcontrol task if it is currently active
	if (true == robot.is_opcontrol_listed()) {
		robot.opcontrol_stop();
	}

	// main opcontrol loop
	while (true) {

		pros::lcd::clear();

		// set robot controllers to physical controllers
		INFO("setting robot controllers to physical controllers...");
		robot.set_controllers_to_physical();
		INFO("robot controllers set to physical controllers");

		// autonomous training mode
		if (umbc::Robot::Mode::TRAIN_AUTONOMOUS == robot.get_mode()) {

			INFO("autonomous training starting...");
			
			// check that an SD card is installed
			if (0 == pros::usd::is_installed()) {
				ERROR("autonomous training failed; no SD Card detected");
				pros::lcd::clear();
				pros::lcd::set_text(1, "No SD Card Detected!");
				pros::lcd::set_text(3, "Power off V5 Brain and insert");
				pros::lcd::set_text(4, "SD Card.");
				return;
			}

			// train autonomous routine
			pros::lcd::set_text(1, "Autonomous Training Active");
			robot.train_autonomous(PARTNER_CONTROLLER);
			pros::lcd::clear();
			pros::lcd::set_text(1, "Autonomous Training Complete");
			INFO("autonomous training complete");
			pros::Task::delay(MSG_DELAY_MS);
			
			// reinitialize robot settings
			competition_initialize(); 
		}
		
		// normal opcontrol mode
		else {
			pros::lcd::set_text(1, "Opcontrol Active");
			INFO("opcontrol active");
			robot.opcontrol();
		}
	}
}
