/**
 * \file umbc/controllerrecorder.hpp
 *
 * Contains the prototype for the Robot. The Robot holds various values used for
 * competition and practice, as well as holds the functions for the selection menu,
 * autonomous and opcontrol.
 */

#ifndef _UMBC_ROBOT_HPP_
#define _UMBC_ROBOT_HPP_

#include "api.h"

#include <cstdint>
#include <queue>

using namespace pros;
using namespace std;

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

namespace umbc {
class Robot {
    
    private:

    public:
    
};
}

#endif // _UMBC_ROBOT_HPP_