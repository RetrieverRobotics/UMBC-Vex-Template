/**
 * \file umbc/robot.hpp
 *
 * Contains the prototype for the Robot. The Robot holds various values used for
 * different modes, as well as holds the functions for the selection menu,
 * autonomous, training autonomous, and opcontrol.
 */

#ifndef _UMBC_ROBOT_HPP_
#define _UMBC_ROBOT_HPP_

#include "api.h"

#include <cstdint>
#include <queue>

using namespace pros;
using namespace std;

namespace umbc {
typedef enum {
    COMPETITION_MATCH = 0,
    COMPETITION_SKILLS = 1
} competition;

typedef enum {
    MODE_COMPETITION = 0,
    MODE_TRAIN_AUTONOMOUS = 1
} mode;

typedef enum {
    ALLIANCE_RED = 0,
    ALLIANCE_BLUE = 1
} alliance;

typedef enum {
    POSITION_ALPHA = 0,
    POSITION_BRAVO = 1
} position;

typedef enum {
    MENU_COMPETITION = 0,
    MENU_MODE = 1,
    MENU_ALLIANCE = 2,
    MENU_POSITION = 3
} sub_menu;

class Robot {
    
    private:
    static constexpr char* match_autonomous_file_master = "/usd/autonomous_match.bin";
    static constexpr char* match_autonomous_file_partner = "/usd/autonomous_match-partner.bin";
    static constexpr char* skills_autonomous_file_master = "/usd/autonomous_skills.bin";
    static constexpr char* skills_autonomous_file_partner = "/usd/autonomous_skills-partner.bin";
    static constexpr uint32_t match_autonomous_time_ms = 45000;
    static constexpr uint32_t skills_autonomous_time_ms = 75000;
    static constexpr uint32_t autonomous_train_poll_rate_ms = 10;

    umbc::competition competition;
    umbc::mode mode;
    umbc::alliance alliance;
    umbc::position position;

    pros::Controller controller_master = pros::Controller(E_CONTROLLER_MASTER);
    pros::Controller controller_partner = pros::Controller(E_CONTROLLER_PARTNER);

    /**
     * Menu to select the competition type using the LLEMU.
     * 
     * @returns always 1
     */
    std::int32_t menu_competition();

    /**
     * Menu to select the mode using the LLEMU.
     * 
     * @returns 1 if a selection was made, otherwise -1.
     */
    std::int32_t menu_mode();

    /**
     * Menu to select the alliance using the LLEMU.
     * 
     * @returns 1 if a selection was made, otherwise -1.
     */
    std::int32_t menu_alliance();

    /**
     * Menu to select the starting position using the LLEMU.
     * 
     * @returns 1 if a selection was made, otherwise -1.
     */
    std::int32_t menu_position();

    /**
     * Allows operator to manually control the robot via a controller. Used
     * for training autonomous.
     */
    static void opcontrol_static(Robot robot);

    public:
    
    /**
	 * Creates a robot object.
	 */
    Robot();

    /**
     * Retrieve the master controller.
     * 
     * @return The master controller.
     */
    pros::Controller get_controller_master();

    /**
     * Retrieve the partner controller.
     * 
     * @return the partner controller.
     */
    pros::Controller get_controller_partner();

    /**
     * Menu for selecting mode, competition, alliance, and starting
     * position useing the LLEMU.
     */
    void menu();

    /**
     * Robot performs a preset routine that was created by training
     * the robot using the controller recorder and playing back the
     * controller inputs using the virtual controller.
     * 
     * @param include_partner_controller - Set to true if partner controller
     *      input was previously recorded and should be used.
     */
    void autonomous(uint32_t include_partner_controller);

    /**
     * Allows operator to manually control the robot via a controller.
     * 
     * @param controller_master - Vex V5 master controller.
     * @param controlelr_partner - Vex V5 partner controller.
     */
    void opcontrol(pros::Controller controller_master, pros::Controller controller_partner);

    /**
     * Trains an autonomous routine for either skills or a tournament
     * match through using opcontrol and the controller recorder.
     * 
     * @param record_partner_controller - Set to true if the partner controller should be recorded.
     */
    void train_autonomous(uint32_t record_partner_controller);
};
}

#endif // _UMBC_ROBOT_HPP_