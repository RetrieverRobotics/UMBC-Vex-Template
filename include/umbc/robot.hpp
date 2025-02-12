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
             * Robot modes
             */    
            enum class Mode : std::int32_t {
                NONE = -1,              // invalid robot mode
                COMPETITION = 0,        // competition or practice mode
                TRAIN_AUTONOMOUS = 1    // train autonomous mode
            };

        
        private:

            // name of task that converts controller input into robot actions
            static constexpr char* t_opcontrol_name =  (char*)"robot_opcontrol";

            // file path for loading master virtual controller input for a practice, qualification and elimination match
            static constexpr char* match_autonomous_file_master = (char*)"/usd/autonomous_tournament.bin";

            // file path for loading partner virtual controller input for a practice, qualification and elimination match
            static constexpr char* match_autonomous_file_partner = (char*)"/usd/autonomous_tournament-partner.bin";

            // file path for loading master virtual controller input for a skills match
            static constexpr char* skills_autonomous_file_master = (char*)"/usd/autonomous_skills.bin";

            // file path for loading partner virtual controller input for a skills match
            static constexpr char* skills_autonomous_file_partner = (char*)"/usd/autonomous_skills-partner.bin";

            // amount of time in milliseconds for a practice, qualification and elimination match
            static constexpr std::uint32_t tournament_autonomous_time_ms = 30000;

            // amount of time in milliseconds for an autonomous skills match
            static constexpr std::uint32_t skills_autonomous_time_ms = 60000;

            // delay in milliseconds when continuously polling controller input
            static constexpr std::uint32_t opcontrol_delay_ms = 10; 

            umbc::Robot::MatchType match_type;  // current match type
            umbc::Robot::Mode mode;             // current mode

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


        public:
        
            /**
             * @brief Creates a robot object
             */
            Robot();

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
             * @brief Performs robot actions via a preset routine
             * 
             * Performs a preset routine that was created by training the robot using the
             * controller recorder and playing back the controller inputs using the virtual
             * controller
             * 
             * @param is_partner_controller - Set to true if partner controller input was
             *      previously recorded and should be used
             */
            void autonomous(bool is_partner_controller);

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
             * @param is_partner_controller - Set to true if the partner controller should be recorded
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
