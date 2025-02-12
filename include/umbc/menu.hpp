/**
 * @file umbc/menu.hpp
 *
 * Contains the prototype for the menu system. The Menu class contains
 * various sub-menus for putting the robot into different modes.
 */

#ifndef _UMBC_MENU_HPP_
#define _UMBC_MENU_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "api.h"
#include "robot.hpp"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

    /**
     * Contains various sub-menus for setting the robot's different modes
     */
    class Menu {

        private:

            /**
             * Sub-menu identifiers
             */
            enum class SubMenu : std::int32_t {
                MATCH = 0,  // sub-menu for selecting match type
                MODE = 1,   // sub-menu for selecting robot mode
                MAX         // delete when refactoring
            };
        
            umbc::Robot* robot = nullptr;   // reference to robot being configured by the menu

            /**
             * @brief Menu to select the competition type using the LLEMU
             * 
             * @returns Match type that was selected. Otherwise umbc::MatchType::NONE
             */
            umbc::Robot::MatchType menu_match();

            /**
             * @brief Menu to select the robot mode using the LLEMU
             * 
             * @returns Robot mode that was selected. Otherwise umbc::RobotMode::NONE
             */
            umbc::Robot::Mode menu_mode();


        public:

            /**
             * @brief Creates a menu object
             * 
             * @param robot: Reference to the robot object the menu will configure
             */
            Menu(umbc::Robot* robot);

            /**
             * @brief Menu for selecting robot mode and match type useing the LLEMU
             */
            void robot_menu();
    };
}

#endif // _UMBC_MENU_HPP_