/**
 * \file umbc/robot.hpp
 *
 * Contains the prototype for the Robot. The Robot holds various values used for
 * competition and practice, as well as holds the functions for the selection menu,
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
    ALLIANCE_RED = 0,
    ALLIANCE_BLUE = 1
} alliance;

typedef enum {
    POSITION_ONE = 0,
    POSITION_TWO = 1
} position;

typedef enum {
    COMPETITION_MATCH = 0,
    COMPETITION_SKILLS = 1
} competition;

typedef enum {
    MODE_COMPETITION = 0,
    MODE_TRAIN_AUTONOMOUS = 1,
    MODE_PRACTICE_AUTONOMOUS = 2,
    MODE_PRACTICE_OPCONTROL = 3
} mode;

class Robot {
    
    private:
    static constexpr char* t_current_name = "robot";
    
    std::int32_t mode;
    std::int32_t competition;
    std::int32_t alliance;
    std::int32_t position;

    /**
     * Menu to select the mode using the LLEMU.
     */
    std::int32_t menu_mode();

    /**
     * Menu to select the competition type using the LLEMU.
     */
    std::int32_t menu_competition();

    /**
     * Menu to select the alliance using the LLEMU.
     */
    std::int32_t menu_alliance();

    /**
     * Menu to select the starting position using the LLEMU.
     */
    std::int32_t menu_position();

    public:
    
    /**
	 * Creates a robot object.
	 */
    Robot();

    /**
     * Menu for selecting mode, competition, alliance, and starting
     * position usuing the LLEMU.
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