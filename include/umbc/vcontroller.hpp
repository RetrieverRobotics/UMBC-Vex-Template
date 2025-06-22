/**
 * \file umbc/vcontroller.hpp
 *
 * Contains the prototype for the VController. The VController emulates
 * user controller input through reading a formatted binary file of
 * controller inputs.
 */

#ifndef _UMBC_V_CONTROLLER_HPP_
#define _UMBC_V_CONTROLLER_HPP_

#include "controller.hpp"
#include "controllerinput.hpp"
#include "api.h"

#include <cstdint>
#include <queue>
#include <map>
using namespace pros;
using namespace std;

namespace umbc {
class VController : public umbc::Controller {

	private:
	static constexpr char* t_update_controller_input_name = (char*)"vcontroller";

	class Digital;

	std::uint16_t poll_rate_ms;
	std::map<controller_digital_e_t, Digital> digitals;
	std::queue<ControllerInput> controller_input;
	std::unique_ptr<Task> t_update_controller_input;

	/**
	 * Pops off the front of the controller input queue at the set poll rate.
	 * 
	 * This function is intended to be used as a task, which is why it is
	 * static.
	 * 
	 * \param VController
	 * 			The virtual controller whose inputs will be updated. The type for
	 * 			this parameter must be VController. Intended to be 'this' pointer.
	 */
	static void update(void* VController);

	/**
	 * Represents the state of a controller's digital channel. 
	 */
	class Digital {
		
		private:
		std::uint8_t current : 1;
		std::uint8_t previous : 1;
		std::uint8_t newPress : 1;

		public:

		/**
		 * Creates a new digital object and zeros all data members.
		 */
		Digital();

		/**
		 * Sets the current held value.
		 * 
		 * None zero values will be converted to 1.
		 * 
		 * \param 
		 */
		void set(std::int32_t value);

		/**
		 * Sets all data members to 0.
		 */
		void reset();

		/**
		 * Gets the currently held value.
		 * 
		 * \return The current held value.
		 */
		std::int32_t get();
		
		/**
		 * Return's a rising edge case for the currently held value.
		 * 
		 * This function is not thread-safe.
	 	 * Multiple tasks polling a single digital object may return different results under
	 	 * the same circumstances, so only one task should call this function for any
	 	 * given digital object.
		 */
		std::int32_t get_new_press();
	};

    public:

	/**
	 * Creates a virtual controller object and initializes all data members.
	 */
	VController();

    /**
	 * Checks if the controller is connected.
	 *
	 * Connected for the virtual controller means that the controller input queue
	 * is not empty.
	 *
	 * \return 0 if the controller input queue is empty, otherwise 1
	 */
	std::int32_t is_connected(void);

	/**
	 * Gets the value of an analog channel (joystick) on a controller.
	 * 
	 * Multiple tasks polling a single analog channel may return different results
	 * under the same circumstances, so only one task should call this function for
	 * any given analog channel. E.g., Task A calls this function for analog channels
	 * left x and left y. Task B may call this function for analog channel right x,
	 * but should not for analog channels left 1 or left 2.
	 *
	 * \param channel
	 * 			  The analog channel to get.
	 * 			  Must be one of ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X,
	 *        ANALOG_RIGHT_Y
	 *
	 * \return The current reading of the analog channel: [-127, 127]
	 */
	std::int32_t get_analog(controller_analog_e_t channel);

	/**
	 * Gets the battery capacity of the controller.
	 *
	 * This function always returns 100 since the controller is virtual and
     * does not have a battery.
	 *
	 * \return Always 100
	 */
	std::int32_t get_battery_capacity(void);

	/**
	 * Gets the battery level of the controller.
	 *
	 * This function always returns 100 since the controller is virtual and
     * does not have a battery.
	 *
	 * \return Always 100
	 */
	std::int32_t get_battery_level(void);

	/**
	 * Checks if a digital channel (button) on the controller is currently
	 * pressed.
	 * 
	 * This function is not thread safe.
	 * Multiple tasks polling a single button may return different results under
	 * the same circumstances, so only one task should call this function for any
	 * given button. E.g., Task A calls this function for buttons 1 and 2.
	 * Task B may call this function for button 3, but should not for buttons
	 * 1 or 2.
	 *
	 * \param button
	 * 			  The button to read. Must be one of
	 *        DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed, otherwise 0.
	 */
	std::int32_t get_digital(controller_digital_e_t button);

	/**
	 * Returns a rising-edge case for a controller button press.
	 *
	 * This function is not thread-safe.
	 * Multiple tasks polling a single button may return different results under
	 * the same circumstances, so only one task should call this function for any
	 * given button. E.g., Task A calls this function for buttons 1 and 2.
	 * Task B may call this function for button 3, but should not for buttons
	 * 1 or 2. A typical use-case for this function is to call inside opcontrol
	 * to detect new button presses, and not in any other tasks.
	 *
	 * \param button
	 * 			  The button to read. Must be one of
	 *        DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed and had not been
	 * pressed the last time this function was called, 0 otherwise.
	 */
	std::int32_t get_digital_new_press(controller_digital_e_t button);

	/**
	 * Sets text to the controller LCD screen.
	 *
	 * This function does nothing since the controller is virtual and there
     * is no display to print anything. This function will always return 1.
	 *
	 * \param line
	 *        The line number at which the text will be displayed [0-2]
	 * \param col
	 *        The column number at which the text will be displayed [0-14]
	 * \param fmt
	 *        The format string to print to the controller
	 * \param ...
	 *        The argument list for the format string
	 *
	 * \return Always 1
	 */
	template <typename... Params>
	std::int32_t print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args);

	/**
	 * Sets text to the controller LCD screen.
	 *
	 * This function does nothing since the controller is virtual and there
     * is no text to set. This function will always return 1.
	 *
	 * \param line
	 *        The line number at which the text will be displayed [0-2]
	 * \param col
	 *        The column number at which the text will be displayed [0-14]
	 * \param str
	 *        The pre-formatted string to print to the controller
	 *
	 * \return Always 1
	 */
	std::int32_t set_text(std::uint8_t line, std::uint8_t col, const char* str);
	std::int32_t set_text(std::uint8_t line, std::uint8_t col, const std::string& str);

	/**
	 * Clears an individual line of the controller screen.
	 *
	 * This function does nothing since the controller is virtual and there
     * is no line to clear. This function will always return 1.
	 *
	 * \param line
	 *        The line number to clear [0-2]
	 *
	 * \return Always 1
	 */
	std::int32_t clear_line(std::uint8_t line);

	/**
	 * Rumble the controller.
	 *
	 * This function does nothing since the controller is virtual and there
     * is nothing to rumble. This function will always return 1.
	 *
	 * \param rumble_pattern
	 *				A string consisting of the characters '.', '-', and ' ', where dots
	 *				are short rumbles, dashes are long rumbles, and spaces are pauses.
	 *				Maximum supported length is 8 characters.
	 *
	 * \return Always 1
	 */
	std::int32_t rumble(const char* rumble_pattern);

	/**
	 * Clears all of the lines on the controller screen.
	 *
	 * This function does nothing since the controller is virtual and there
     * is nothing to clear. This function will always return 1.
	 *
	 * \return Always 1
	 */
	std::int32_t clear(void);

	/**
	 * Reads a controller input file, saves the poll rate, and loads the
	 * controller inputs from the file into a queue.
	 * 
	 * If the poll rate in the file is zero, this function will fail
	 * since zero is an illegal poll rate value.
	 * 
	 * \param file_path
	 * 			The path for the file to retrieve the poll rate and load
	 * 			the controller input from.
	 * 
	 * \return 1 on success, 0 otherwise.
	 */
	std::int32_t load(const char* file_path);
	std::int32_t load(std::string& file_path);

	/**
	 * Creates a seperate task that pops off the front of the controller
	 * input queue at the set poll rate.
	 */
	void start(void);

	/**
	 * Pauses popping off the front of the controller input queue at the
	 * set poll rate by suspending the update controller input task.
	 */
	void pause(void);

	/**
	 * Resumes popping off the front of the controller input queue at the
	 * set poll rate by resuming the update controller input task.
	 */
	void resume(void);

	/**
	 * Deletes the update controller input task and clears the the controller
	 * input queue.
	 */
	void stop(void);

	/**
	 * Wait for the update controller input task to complete.
	 */
	void wait_till_complete(void);
};
}

#endif // _UMBC_V_CONTROLLER_HPP_