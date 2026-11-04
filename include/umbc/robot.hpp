/**
 * @file umbc/robot.hpp
 *
 * Contains the prototype for the Robot. The Robot holds various values used for
 * different modes, as well as holds the functions for autonomous, training autonomous,
 * and opcontrol.
 */

#ifndef _UMBC_ROBOT_HPP_
#define _UMBC_ROBOT_HPP_

#include "controller.hpp"
#include "pcontroller.hpp"
#include "vcontroller.hpp"
#include "api.h"

#include <cstdint>
#include <queue>
#include <string>

using namespace pros;
using namespace std;

namespace umbc {

    /**
     * Holds various values for determining different modes. Contains functions for autonomous,
     * training autonomous, and opcontrol. Allows for basic control of the opcontrol task
     */
    class Robot {

        public:

            /**
             * Match types
             */
            enum class MatchType : std::int32_t {
                NONE = -1,          // invalid match type
                TOURNAMENT = 0,     // practice, qualification or elimination match
                SKILLS = 1          // skills match
            };


            /**
             * Alliance colors
             */
            enum class AllianceColor : std::int32_t {
                NONE = -1,  // no alliance color
                RED = 0,    // red alliance
                BLUE = 1    // blue alliance
            };


            /**
             * Robot modes
             */    
            enum class Mode : std::int32_t {
                NONE = -1,              // invalid robot mode
                COMPETITION = 0,        // competition or practice mode
                TRAIN_AUTONOMOUS = 1    // train autonomous mode
            };


            /**
             * Autonomous routine types
             */
            enum class AutonomousType : std::int32_t {
                NONE = -1,      // invalid autonomous type
                RECORDED = 0,   // recorded autonomous routine played back from a file
                PROGRAMMED = 1  // user-programmed autonomous routine
            };

            // directory where autonomous recordings are stored
            static constexpr char* autonomous_directory = (char*)"/usd";

            // directory for listing autonomous recordings
            // (usd_list_files paths must not be prepended with /usd)
            static constexpr char* autonomous_list_directory = (char*)"/";

            // file extension for autonomous recordings
            static constexpr char* autonomous_file_extension = (char*)".auto";

            // suffix appended to a recording name for the partner controller recording
            static constexpr char* autonomous_partner_suffix = (char*)"-partner";

        
        private:

            // name of task that converts controller input into robot actions
            static constexpr char* t_opcontrol_name =  (char*)"robot_opcontrol";

            // default recording name for a practice, qualification and elimination match
            static constexpr char* default_tournament_autonomous_name = (char*)"autonomous_tournament";

            // default recording name for a skills match
            static constexpr char* default_skills_autonomous_name = (char*)"autonomous_skills";

            // file path for loading and saving the robot configuration
            static constexpr char* config_file_path = (char*)"/usd/config.json";

            // name of the autonomous object in the configuration file
            static constexpr char* config_name_autonomous = (char*)"autonomous";

            // name of the competition selection object in the configuration file
            static constexpr char* config_name_competition = (char*)"competition";

            // name of the skills selection object in the configuration file
            static constexpr char* config_name_skills = (char*)"skills";

            // name of the autonomous type value in the configuration file
            static constexpr char* config_name_type = (char*)"type";

            // name of the recording file value in the configuration file
            static constexpr char* config_name_file = (char*)"file";

            // autonomous type value for recorded routines in the configuration file
            static constexpr char* config_type_recorded = (char*)"recorded";

            // autonomous type value for programmed routines in the configuration file
            static constexpr char* config_type_programmed = (char*)"programmed";

            // amount of time in milliseconds for a practice, qualification and elimination match
            static constexpr std::uint32_t tournament_autonomous_time_ms = 30000;

            // amount of time in milliseconds for an autonomous skills match
            static constexpr std::uint32_t skills_autonomous_time_ms = 60000;

            // delay in milliseconds when continuously polling controller input
            static constexpr std::uint32_t opcontrol_delay_ms = 10; 

            umbc::Robot::MatchType match_type;          // current match type
            umbc::Robot::AllianceColor alliance_color;  // current alliance color
            umbc::Robot::Mode mode;                     // current mode

            // autonomous type selected for a tournament match
            umbc::Robot::AutonomousType competition_autonomous_type;

            // recording name selected for a tournament match
            std::string competition_autonomous_name;

            // autonomous type selected for a skills match
            umbc::Robot::AutonomousType skills_autonomous_type;

            // recording name selected for a skills match
            std::string skills_autonomous_name;

            // physical version of master controller
            umbc::PController pcontroller_master = umbc::PController(E_CONTROLLER_MASTER);

            // physical version of partner controller
            umbc::PController pcontroller_partner = umbc::PController(E_CONTROLLER_PARTNER);    

            umbc::VController vcontroller_master = umbc::VController();     // virtual version of master controller
            umbc::VController vcontroller_partner = umbc::VController();    // virtual version of partner controller

            umbc::Controller* controller_master = &vcontroller_master;      // current controller being used for master controller input
            umbc::Controller* controller_partner = &pcontroller_partner;    // current controller being used for partner controller input

            std::unique_ptr<Task> t_opcontrol;  // task for converting controller input into robot actions

            /**
             * @brief Allows operator to manually control the robot via a controller
             * 
             * @param robot: Robot being controlled. Type for this parameter must be Robot*.
             *      Intended to be 'this' pointer.
             * 
             * @note Used for training autonomous
             * @note This function is intended to be used as a task, which is why it is static.
             */
            static void robot_opcontrol(Robot* robot);

            /**
             * @brief Builds the file path for an autonomous recording
             * 
             * @param recording_name: Name of the recording
             * @param is_partner_controller: Set to true to build the path for the
             *      partner controller recording
             * 
             * @return File path of the autonomous recording
             */
            std::string autonomous_file_path(const std::string& recording_name, bool is_partner_controller);

            /**
             * @brief Performs robot actions by playing back a recorded routine
             * 
             * Plays back a recorded routine that was created by training the robot using the
             * controller recorder through the virtual controllers
             * 
             * @param recording_name: Name of the recording to play back
             * @param is_partner_controller: Set to true if partner controller input was
             *      previously recorded and should be used
             */
            void autonomous_recorded(const std::string& recording_name, bool is_partner_controller);


        public:
        
            /**
             * @brief Creates a robot object
             */
            Robot();

            /**
             * @brief Performs user program initialization
             * 
             * Performs user implemented program initialization, such as initializing
             * sensors, motors and subsystems. Intended to be called once at program
             * startup before competition or driver control execution.
             * 
             * @note Implemented by the user in initialize.cpp
             */
            void initialize();

            /**
             * @brief Sets controllers to use physical controllers
             */
            void set_controllers_to_physical();

            /**
             * @brief Sets controllers to use virtual controllers
             */
            void set_controllers_to_virtual();

            /**
             * @brief Retrieve current match type
             * 
             * @return Current match type
            */
            umbc::Robot::MatchType get_match_type();

            /**
             * @brief Set match type
             * 
             * @param match_type: match type
            */
            void set_match_type(umbc::Robot::MatchType match_type);

            /**
             * @brief Retrieve current alliance color
             *
             * @return Current alliance color
            */
            umbc::Robot::AllianceColor get_alliance_color();

            /**
             * @brief Set alliance color and update LCD colors if initialized
             *
             * @param alliance_color: Alliance color
            */
            void set_alliance_color(umbc::Robot::AllianceColor alliance_color);

            /**
             * @brief Updates LCD colors for the current alliance
             *
             * @note No alliance restores the bundled LLEMU's default green background and dark text.
            */
            void update_lcd_colors();

            /**
             * @brief Retrieve current robot mode
             * 
             * @return Current robot mode
            */
            umbc::Robot::Mode get_mode();

            /**
             * @brief Set robot mode
             * 
             * @param mode: Robot mode
            */
            void set_mode(umbc::Robot::Mode mode);

            /**
             * @brief Retrieve the selected autonomous type for a match type
             * 
             * @param match_type: Match type of the selection
             * 
             * @return Selected autonomous type for the match type. Otherwise
             *      umbc::Robot::AutonomousType::NONE for an invalid match type
            */
            umbc::Robot::AutonomousType get_autonomous_type(umbc::Robot::MatchType match_type);

            /**
             * @brief Retrieve the selected recording name for a match type
             * 
             * @param match_type: Match type of the selection
             * 
             * @return Selected recording name for the match type. Otherwise an
             *      empty string for an invalid match type
            */
            std::string get_autonomous_name(umbc::Robot::MatchType match_type);

            /**
             * @brief Set the autonomous selection for a match type and save it to the configuration file
             * 
             * Sets the autonomous selection for a match type, replacing the previous
             * selection, and saves all selections to the configuration file so they
             * persist between robot restarts.
             * 
             * @param match_type: Match type of the selection
             * @param autonomous_type: Autonomous type to select
             * @param recording_name: Name of the recording to select. Ignored for
             *      programmed autonomous routines
            */
            void set_autonomous(umbc::Robot::MatchType match_type,
                umbc::Robot::AutonomousType autonomous_type, const std::string& recording_name);

            /**
             * @brief Loads the autonomous selections from the configuration file
             * 
             * Loads the autonomous selections from the configuration file on the SD card.
             * The current selections are kept for any selection that is missing or invalid.
             * 
             * @return 1 if the configuration file was read successfully. Otherwise 0
            */
            std::int32_t load_autonomous_selections();

            /**
             * @brief Saves the autonomous selections to the configuration file
             * 
             * Saves the autonomous selections to the configuration file on the SD card.
             * Unrelated values in the configuration file are preserved.
             * 
             * @return 1 if the configuration file was saved successfully. Otherwise 0
            */
            std::int32_t save_autonomous_selections();

            /**
             * @brief Performs robot actions for the selected autonomous routine
             * 
             * Performs the autonomous routine selected for the current match type.
             * Recorded routines are played back from their recording using the
             * virtual controllers, while programmed routines run the user
             * implemented autonomous code.
             * 
             * @param is_partner_controller: Set to true if partner controller input was
             *      previously recorded and should be used for a recorded routine
             */
            void autonomous(bool is_partner_controller);

            /**
             * @brief Performs the user-programmed autonomous routine for a tournament match
             * 
             * @note Implemented by the user in autonomous.cpp
             */
            void autonomous_competition();

            /**
             * @brief Performs the user-programmed autonomous routine for a skills match
             * 
             * @note Implemented by the user in autonomous.cpp
             */
            void autonomous_skills();

            /**
             * @brief Allows operator to manually control the robot via a controller
             */
            void opcontrol();

            /**
             * @brief Trains an autonomous routine for either skills or a tournament match
             * 
             * Trains an autonomous routine for either skills or a tournament match through using opcontrol
             * and the controller recorder.
             * 
             * @param is_partner_controller: Set to true if the partner controller should be recorded
             */
            void train_autonomous(bool is_partner_controller);

            /**
             * @brief Starts opcontrol task
             */
            void opcontrol_start();

            /**
             * @brief Pauses opcontrol task if task is currently active
             */
            void opcontrol_pause();

            /**
             * @brief Resumes opcontrol task if task is currently paused
             */
            void opcontrol_resume();

            /**
             * @brief Stops opcontrol task
             */
            void opcontrol_stop();

            /**
             * @brief Checks if opcontrol task is on the ready, blocked, suspended or event lists
             * 
             * @return True if opcontrol task exists and not actively being deleted. Otherwise false
             */
            bool is_opcontrol_listed();
    };
}

#endif // _UMBC_ROBOT_HPP_
