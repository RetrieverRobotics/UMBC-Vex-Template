/**
<<<<<<< HEAD
 * @file umbc/vcontroller.hpp
=======
 * \file umbc/vcontroller.hpp
>>>>>>> ac56c5b00ef790b55443ed4abad5c3499d83ead7
 *
 * Contains the prototype for the VController. The VController emulates
 * user controller input through reading a formatted binary file of
 * controller inputs.
 */

#ifndef _UMBC_V_CONTROLLER_HPP_
#define _UMBC_V_CONTROLLER_HPP_

// standard libraries
#include <cstdint>
#include <queue>
#include <map>

// local header files
#include "controller.hpp"
#include "controllerinput.hpp"
#include "api.h"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

	/**
	 * Simulate a controller's behavior by handling both input events and output requests in a way
	 * that mimics a real controller, while internally using tasks and queues loaded from a file to
	 * manage state updates at regular intervals. It Extends the umbc::Controller class and provides
	 * various functionalities to emulate controller hardware.
	 */
	class VController : public umbc::Controller {

		private:

			// name of the task that updates the virtual controller
			static constexpr char* t_update_controller_input_name = (char*)"vcontroller";

			class Digital;

			// rate in milliseconds that the virtual controller updates its state
			std::uint16_t poll_rate_ms;

			// all digital channels of the virtual controller
			std::map<controller_digital_e_t, Digital> digitals;

			// queue of each controller state in chronological order
			std::queue<ControllerInput> controller_input;

			// task for updating the state of the virtual controller
			std::unique_ptr<Task> t_update_controller_input;


			/**
			 * @brief Pops off the front of the controller input queue at the set poll rate
			 * 
			 * @param VController: The virtual controller whose inputs will be updated.
			 * 		The type for this parameter must be VController. Intended to be
			 * 		'this' pointer.
			 * 	
			 * @note This function is intended to be used as a task, which is why it is
			 * 		static.
			 */
			static void update(void* VController);

			/**
			 * Represents the state of a controller's digital channel
			 */
			class Digital {
				
				private:

					std::uint8_t current : 1;	// current digital state
					std::uint8_t previous : 1;	// previous digital state
					std::uint8_t newPress : 1;	// flag for if a new press occured


				public:

					/**
					 * @brief Creates a new digital object and zeros all data members
					 */
					Digital();

					/**
					 * @brief Sets the current held value
					 * 
					 * @param value: value to set
					 * 
					 * @note None zero values will be converted to 1
					 */
					void set(std::int32_t value);

					/**
					 * @brief Sets all data members to 0
					 */
					void reset();

					/**
					 * @brief Gets the currently held value
					 * 
					 * @return current held value
					 */
					std::int32_t get();
					
					/**
					 * @brief Return's a rising edge case for the currently held value
					 * 
					 * @note This function is not thread-safe.
					 * @note Multiple tasks polling a single digital object may return different
					 * 		results under the same circumstances, so only one task should call this
					 * 		function for any given digital object.
					 */
					std::int32_t get_new_press();
			};

		public:

			/**
			 * @brief Creates a virtual controller object and initializes all data members
			 */
			VController();

			/**
			 * @brief Checks if the controller is connected
			 *
			 * @return 0 if the controller input queue is empty. Otherwise 1
			 * 
			 * @note Connected for the virtual controller means that the controller input
			 * 		queue is not empty.
			 */
			std::int32_t is_connected();

			/**
			 * @brief Gets the value of an analog channel (joystick) on a controller
			 *
			 * @param channel: Analog channel to get. Must be ANALOG_LEFT_X, ANALOG_LEFT_Y,
			 * 		ANALOG_RIGHT_X or ANALOG_RIGHT_Y
			 *
			 * @return Current reading of the analog channel: [-127, 127]
			 * 
			 * @note If the controller is not connected, then 0 is returned
			 * @note Sets errno to EACCES when an error state is reached
			 */
			std::int32_t get_analog(controller_analog_e_t channel);

			/**
			 * @brief Gets the battery capacity of the controller
			 *
			 * @return Always 100
			 * 
			 * @note This function always returns 100 since the controller is virtual and
			 * 		does not have a battery.
			 */
			std::int32_t get_battery_capacity();

			/**
			 * @brief Gets the battery level of the controller
			 *
			 * @return Always 100
			 * 
			 * @note This function always returns 100 since the controller is virtual and
			 * 		does not have a battery.
			 */
			std::int32_t get_battery_level();

			/**
			 * @brief Checks if a digital channel (button) on the controller is pressed
			 *
			 * @param button: Button to read. Must be one of
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed. Otherwise 0
			 */
			std::int32_t get_digital(pros::controller_digital_e_t button);

			/**
			 * @brief Returns a rising-edge case for a controller button press
			 *
			 * @param button: Button to read. Must be
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed and had not been
			 * 		pressed the last time this function was called, 0 otherwise
			 * 
			 * @note Not thread safe
			 */
			std::int32_t get_digital_new_press(pros::controller_digital_e_t button);

			/**
			 * @brief Sets text to the controller LCD screen
			 *
			 * @param line: Line number at which the text will be displayed [0-2]
			 * @param col: Column number at which the text will be displayed [0-14]
			 * @param fmt: Format string to print to the controller
			 * @param args: Argument list for the format string
			 *
			 * @return Always 1
			 * 
			 * @note This function does nothing since the controller is virtual and there
			 * 		is no text to set. This function will always return 1.
			 */
			template <typename... Params>
			std::int32_t print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args);

			/**
			 * @brief Sets text to the controller LCD screen
			 *
			 * @param line: Line number at which the text will be displayed [0-2]
			 * @param col: Column number at which the text will be displayed [0-14]
			 * @param str: Pre-formatted string to print to the controller
			 *
			 * @return Always 1
			 * 
			 * @note This function does nothing since the controller is virtual and there
			 * 		is no text to set. This function will always return 1.
			 */
			std::int32_t set_text(std::uint8_t line, std::uint8_t col, const char* str);
			std::int32_t set_text(std::uint8_t line, std::uint8_t col, const std::string& str);

			/**
			 * @brief Clears an individual line of the controller screen
			 * 
			 * @param line: Line number to clear [0-2]
			 *
			 * @return Always 1
			 * 
			 * @note This function does nothing since the controller is virtual and there
			 * 		is no line to clear. This function will always return 1.
			 */
			std::int32_t clear_line(std::uint8_t line);

			/**
			 * @brief Rumble the controller
			 *
			 * @param rumble_pattern: String consisting of the characters '.', '-', and ' ',
			 * 		where dots are short rumbles, dashes are long rumbles, and spaces are pauses.
			 *		Maximum supported length is 8 characters
			 *
			 * @return Always 1
			 * 
			 * @note This function does nothing since the controller is virtual and there
			 * 		is nothing to rumble. This function will always return 1.
			 */
			std::int32_t rumble(const char* rumble_pattern);

			/**
			 * @brief Clears all of the lines on the controller screen
			 * 
			 * @return Always 1
			 * 
			 * @note This function does nothing since the controller is virtual and there
			 * 		is nothing to clear. This function will always return 1.
			 */
			std::int32_t clear();

			/**
			 * @brief Reads a controller input file and loads controller inputs into a queue
			 * 
			 * Reads a controller input file, saves the poll rate, and loads the
			 * controller inputs from the file into a queue.
			 * 
			 * @param file_path: Path for the file to retrieve the poll rate and load
			 * 		the controller input from
			 * 
			 * @return 1 on success. 0 otherwise
			 * 
			 * @note If the poll rate in the file is zero, this function will fail
			 * 		since zero is an illegal poll rate value.
			 */
			std::int32_t load(const char* file_path);
			std::int32_t load(std::string& file_path);

			/**
			 * @brief Creates a seperate task that pops off the front of the controller input queue
			 * 
			 * Creates a seperate task that pops off the front of the controller
			 * input queue at the set poll rate.
			 */
			void start();

			/**
			 * @brief Pauses popping off the front of the controller input queue
			 * 
			 * Pauses popping off the front of the controller input queue at the
			 * set poll rate by suspending the update controller input task.
			 */
			void pause();

			/**
			 * @brief Resumes popping off the front of the controller input queue
			 * 
			 * Resumes popping off the front of the controller input queue at the
			 * set poll rate by resuming the update controller input task
			 */
			void resume();

			/**
			 * @brief Deletes the update controller input task and clears the controller input queue
			 */
			void stop();

			/**
			 * @brief Wait for the update controller input task to complete
			 */
			void wait_till_complete();
	};
}

#endif // _UMBC_V_CONTROLLER_HPP_