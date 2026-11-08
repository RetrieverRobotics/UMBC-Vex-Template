/**
 * @file umbc/menu.hpp
 *
 * Contains the prototype for the Menu class. The Menu class contains
 * various sub-menus for putting the robot into different modes.
 */

#ifndef _UMBC_MENU_HPP_
#define _UMBC_MENU_HPP_

// standard libraries
#include <cstdint>
#include <string>

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
                NONE = -1,  // no sub-menu
                MATCH = 0,  // sub-menu for selecting match type
                MODE = 1,   // sub-menu for selecting robot mode
                ALLIANCE = 2,  // sub-menu for selecting alliance color
                UTILITIES = 3, // sub-menu for accessing robot utilities
                AUTONOMOUS_CATEGORY = 4,  // sub-menu for selecting the autonomous selection category
                AUTONOMOUS_TYPE = 5,      // sub-menu for selecting the autonomous type
                AUTONOMOUS_RECORDING = 6, // sub-menu for selecting an autonomous recording
                MATCH_TIME_CATEGORY = 7,  // sub-menu for selecting the match time category
                MATCH_TIME = 8            // sub-menu for changing the match time
            };

            /**
             * Utility identifiers
             */
            enum class Utility : std::int32_t {
                NONE = -1,              // no utility
                SELECT_AUTONOMOUS = 0,  // utility for selecting autonomous routines
                SET_MATCH_TIME = 1      // utility for setting match times
            };

            // delay for when continuously polling LCD buttons in milliseconds
            static constexpr std::uint32_t lcd_btn_polling_delay_ms = 10;

            // number of recordings displayed per page on the recording sub-menu
            static constexpr std::uint32_t recordings_per_page = 3;

            // maximum number of characters of a recording name to display
            static constexpr std::uint32_t recording_display_length_max = 35;

            // suffix appended to recording names truncated for display
            static constexpr char* recording_truncation_suffix = (char*)"...";

            // LCD line of the first recording displayed on the recording sub-menu
            static constexpr std::uint32_t recording_list_first_line = 3;

            // LCD line of the options displayed on the recording sub-menu
            static constexpr std::uint32_t options_line = 7;
        
            umbc::Robot* robot = nullptr;   // reference to robot being configured by the menu

            /**
             * @brief Menu to select the competition type using the LLEMU
             * 
             * @returns Match type that was selected. Otherwise umbc::MatchType::NONE
             */
            umbc::Robot::MatchType menu_match();

            /**
             * @brief Menu to select the alliance color using the LLEMU
             *
             * @returns Alliance color that was selected. Otherwise umbc::Robot::AllianceColor::NONE
             */
            umbc::Robot::AllianceColor menu_alliance();

            /**
             * @brief Menu to select the robot mode using the LLEMU
             * 
             * @returns Robot mode that was selected. Otherwise umbc::RobotMode::NONE
             */
            umbc::Robot::Mode menu_mode();

            /**
             * @brief Menu to access robot utilities using the LLEMU
             * 
             * @returns Utility that was selected. Otherwise umbc::Menu::Utility::NONE
             */
            umbc::Menu::Utility menu_utilities();

            /**
             * @brief Menu to select the autonomous selection category using the LLEMU
             * 
             * @returns Match type of the category that was selected. Otherwise
             *      umbc::Robot::MatchType::NONE
             */
            umbc::Robot::MatchType menu_autonomous_category();

            /**
             * @brief Menu to select the autonomous type using the LLEMU
             * 
             * @param category: Match type of the category being selected for
             * 
             * @returns Autonomous type that was selected. Otherwise
             *      umbc::Robot::AutonomousType::NONE
             */
            umbc::Robot::AutonomousType menu_autonomous_type(umbc::Robot::MatchType category);

            /**
             * @brief Menu to select an autonomous recording using the LLEMU
             * 
             * Lists the autonomous recordings found in the category's autonomous
             * directory on the SD card for selection.
             * 
             * @param category: Match type of the category being selected for
             * 
             * @returns Name of the recording that was selected. Otherwise an
             *      empty string
             */
            std::string menu_autonomous_recording(umbc::Robot::MatchType category);

            /**
             * @brief Menu to select the match time category using the LLEMU
             * 
             * @returns Match type of the category that was selected. Otherwise
             *      umbc::Robot::MatchType::NONE
             */
            umbc::Robot::MatchType menu_match_time_category();

            /**
             * @brief Menu to change the match time using the LLEMU
             * 
             * Displays the current match time for the category, which can be
             * decreased or increased by umbc::Robot::match_time_increment_s until
             * save is selected.
             * 
             * @param category: Match type of the category being changed
             * 
             * @returns Match time in seconds that was saved
             */
            std::uint32_t menu_match_time(umbc::Robot::MatchType category);


        public:

            /**
             * @brief Creates a menu object
             * 
             * @param robot: Reference to the robot object the menu will configure
             */
            Menu(umbc::Robot* robot);

            /**
             * @brief Menu for selecting match type, robot mode and tournament alliance color using the LLEMU
             */
            void robot_menu();
    };
}

#endif // _UMBC_MENU_HPP_