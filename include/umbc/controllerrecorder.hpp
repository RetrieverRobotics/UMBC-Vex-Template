/**
 * \file umbc/controllerrecorder.hpp
 *
 * Contains the prototype for the ControllerRecorder. ControllerRecorder
 * saves controller input at set poll rate. The files created by the
 * ControllerRecorder are meant to be used as input for the VController.
 */

#ifndef _UMBC_CONTROLLER_RECORDER_HPP_
#define _UMBC_CONTROLLER_RECORDER_HPP_

#include "controllerinput.hpp"
#include "api.h"

#include <cstdint>
#include <queue>

using namespace pros;
using namespace std;

namespace umbc {
class ControllerRecorder {

    private:
    static constexpr char* t_record_controller_input_name = "controllerrecorder";

    std::uint16_t poll_rate_ms;
    controller_id_e_t controller_id;
    std::queue<ControllerInput> controller_input;
    std::unique_ptr<Task> t_record_controller_input;

    /**
	 * Pushes current controller input to the controller input queue at the
     * set poll rate.
	 * 
	 * This function is intended to be used as a task, which is why it is
	 * static.
	 * 
	 * \param ControllerRecorder
	 * 			The controller recorder who will be saving controller input.
     *          The type for this parameter must be ControllerRecorder.
     *          Intended to be 'this' pointer.
	 */
	static void record(void* ControllerRecorder);

    public:
    /**
	 * Creates a controller recorder object.
     * 
     * \param controller_id
	 *      The ID of the controller (e.g. the master or partner controller) that will
     *      be recorded. Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
     * 
     * \param poll_rate_ms
     *      The rate in milliseconds controller input will be polled at.
     *           
	 */
    ControllerRecorder(controller_id_e_t controller_id, std::uint16_t poll_rate_ms);

    /**
     * Saves the poll rate and recorded controller input into a binary file.
     * 
     * \param file_path
     *      The file path that the binary file will be created adn saved at. If
     *      a file already exists at this location, it will be overwritten. 
     */
    std::int32_t save(const char* file_path);

    /**
     * Starts recording controller input.
     */
	void start(void);

    /**
     * Pauses recording controller input if controller recording is currently
     * active.
     */
	void pause(void);

    /**
     * Resumes recording controller input if controller recording is currently
     * paused.
     */
	void resume(void);

    /**
     * Stops recording controller input.
     */
	void stop(void);

    /**
     * Clears the controller input buffer.
     */
    void reset(void);

    /**
     * Checks if currently recording.
     * 
     * \return 1 if currently recording, otherwise 0
     */
    std::int32_t isRecording();

    /**
     * Checks if there is any controller input that has been recorded.
     * 
     * \return 1 if there is controller input recorded, otherwise 0 
     */
    std::int32_t hasControllerInput();
};
}

#endif // _UMBC_CONTROLLER_RECORDER_HPP_