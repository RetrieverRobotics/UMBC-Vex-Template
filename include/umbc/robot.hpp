/**
 * \file umbc/robot.hpp
 *
 * Contains the prototype for the Robot. The Robot holds various values used for
 * different modes, as well as holds the functions for the selection menu,
 * autonomous, training autonomous, and opcontrol.
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
typedef enum {
    COMPETITION_MATCH = 0,
    COMPETITION_SKILLS = 1
} competition;

typedef enum {
    MODE_COMPETITION = 0,
    MODE_TRAIN_AUTONOMOUS = 1
} mode;

typedef enum {
    MENU_COMPETITION = 0,
    MENU_MODE = 1,
    MENU_MAX
} sub_menu;

class Robot {
    
    private:
    static constexpr char* t_opcontrol_name =  (char*)"robot_opcontrol";

    static constexpr char* match_autonomous_file_master = (char*)"/usd/autonomous_match.bin";
    static constexpr char* match_autonomous_file_partner = (char*)"/usd/autonomous_match-partner.bin";
    static constexpr char* skills_autonomous_file_master = (char*)"/usd/autonomous_skills.bin";
    static constexpr char* skills_autonomous_file_partner = (char*)"/usd/autonomous_skills-partner.bin";

    static constexpr uint32_t match_autonomous_time_ms = 45000;
    static constexpr uint32_t skills_autonomous_time_ms = 60000;
    static constexpr uint32_t opcontrol_delay_ms = 10;

    umbc::competition competition;
    umbc::mode mode;

    umbc::PController pcontroller_master = umbc::PController(E_CONTROLLER_MASTER);
    umbc::PController pcontroller_partner = umbc::PController(E_CONTROLLER_PARTNER);

    umbc::VController vcontroller_master = umbc::VController();
    umbc::VController vcontroller_partner = umbc::VController();

    umbc::Controller* controller_master = &vcontroller_master;
    umbc::Controller* controller_partner = &pcontroller_partner;

    std::unique_ptr<Task> t_opcontrol;

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

    /**
     * Allows operator to manually control the robot via a controller. Used
     * for training autonomous.
     * 
     * \param robot
     *          The type for this parameter must be Robot*.
     *          Intended to be 'this' pointer.
     */
    static void robot_opcontrol(Robot* robot);

    public:
    
    /**
	 * Creates a robot object.
	 */
    Robot();

    /**
     * Sets the controllers to use physical controllers.
     */
    void set_controllers_to_physical();

    /**
     * Sets the controllers to use virtual controllers.
     */
    void set_controllers_to_virtual();

    /**
     * Retrieve the competition setting.
     * 
     * \return the competition setting
    */
    umbc::competition get_competition();
    
    /**
     * Retrieve the robot mode setting.
     * 
     * \return the robot mode setting
    */
    umbc::mode get_mode();

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
     * \param include_partner_controller - Set to true if partner controller
     *      input was previously recorded and should be used.
     */
    void autonomous(uint32_t include_partner_controller);

    /**
     * Allows operator to manually control the robot via a controller.
     */
    void opcontrol();

    /**
     * Trains an autonomous routine for either skills or a tournament
     * match through using opcontrol and the controller recorder.
     * 
     * \param record_partner_controller - Set to true if the partner controller should be recorded.
     */
    void train_autonomous(uint32_t record_partner_controller);

    /**
     * Starts opcontrol task.
     */
	void opcontrol_start(void);

    /**
     * Pauses opcontrol task if opcontrol task is currently
     * active.
     */
	void opcontrol_pause(void);

    /**
     * Resumes opcontrol task if opcontrol task is currently
     * paused.
     */
	void opcontrol_resume(void);

    /**
     * Stops opcontrol task.
     */
	void opcontrol_stop(void);

    /**
     * Checks if opcontrol task is on the ready, blocked, suspended or event lists.
     * 
     * \return 1 if opcontrol task is on the ready, blocked, suspended or event lists, otherwise 0
     */
    std::int32_t opcontrol_isListed();
};
}

#endif // _UMBC_ROBOT_HPP_