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


class Robot {
    
    private:
    umbc::competition competition;
    umbc::mode mode;
    umbc::alliance alliance;
    umbc::position position;

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

    public:
    
    /**
	 * Creates a robot object.
	 */
    Robot();

    /**
     * Menu for selecting mode, competition, alliance, and starting
     * position useing the LLEMU.
     */
    void menu();

    /**
     * Robot performs a preset routine that was created by training
     * the robot using the controller recorder and playing back the
     * controller inputs using the virtual controller.
     */
    void autonomous();

    /**
     * Allows operator to manually control the robot via a controller.
     */
    void opcontrol();

    /**
     * Trains an autonomous routine for either skills or a tournament
     * match through using opcontrol and the controller recorder.
     */
    void train_autonomous();
};
}

#endif // _UMBC_ROBOT_HPP_