/**
 * \file umbc/menu.hpp
 *
 * Contains the prototype for the menu system. The Menu class contains
 * various sub-menus for putting the robot in different modes and 
 * configuring settings.
 */

#ifndef _UMBC_MENU_HPP_
#define _UMBC_MENU_HPP_

#include "api.h"
#include "robot.hpp"

#include <cstdint>

using namespace pros;
using namespace std;

namespace umbc {
typedef enum {
    MENU_COMPETITION = 0,
    MENU_MODE = 1,
    MENU_MAX
} sub_menu;

class Menu {

    private:
    umbc::Robot* robot = nullptr;

    /**
     * Menu to select the competition type using the LLEMU.
     * 
     * \returns always 1
     */
    std::int32_t menu_competition();

    /**
     * Menu to select the mode using the LLEMU.
     * 
     * \returns 1 if a selection was made, otherwise -1.
     */
    std::int32_t menu_mode();

    public:

    /**
     * Creates a menu object.
     * 
     * \param robot
     *          A reference to the robot object the menu will manipulate.
     */
    Menu(umbc::Robot* robot);

    /**
     * Menu for selecting mode, competition, alliance, and starting
     * position useing the LLEMU.
     */
    void robot_menu();
};
}

#endif // _UMBC_MENU_HPP_