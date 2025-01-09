/**
 * @file umbc/controller.hpp
 *
 * Contains the abstract class Controller. This abstract class is meant to be used as a
 * base class for VController and PController.
 */

#ifndef _UMBC_CONTROLLER_HPP_
#define _UMBC_CONTROLLER_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "api.h"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

	/**
	 * Abstract base class that provides a set of pure virtual functions that define the essential
	 * operations needed to interact with a controller, ensuring compatibility and functional
	 * parity with the pros::Controller class
	 */
	class Controller {

		public:
		
			/**
			 * @brief Checks if the controller is connected
			 *
			 * @return 1 if the controller is connected. Otherwise 0
			 */
			virtual std::int32_t is_connected(void) = 0;

			/**
			 * @brief Gets the value of an analog channel (joystick) on a controller
			 *
			 * @param channel: Analog channel to get. Must be ANALOG_LEFT_X,
			 * 		ANALOG_LEFT_Y, ANALOG_RIGHT_X or ANALOG_RIGHT_Y
			 *
			 * @return Current reading of the analog channel: [-127, 127]. If the
			 * 		controller is not connected, then 0 is returned
			 */
			virtual std::int32_t get_analog(pros::controller_analog_e_t channel) = 0;

			/**
			 * @brief Gets the battery capacity of the controller
			 *
			 * @return Controller's battery capacity
			 */
			virtual std::int32_t get_battery_capacity(void) = 0;

			/**
			 * @brief Gets the battery level of the controller
			 *
			 * @return Controller's battery level
			 */
			virtual std::int32_t get_battery_level(void) = 0;

			/**
			 * @brief Checks if a digital channel (button) on the controller is pressed
			 *
			 * @param button: Button to read. Must be
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed. Otherwise 0
			 */
			virtual std::int32_t get_digital(pros::controller_digital_e_t button) = 0;

			/**
			 * @brief Returns a rising-edge case for a controller button press
			 *
			 * @param button: Button to read. Must be
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed and had not been
			 * 		pressed the last time this function was called. Otherwise 0
			 */
			virtual std::int32_t get_digital_new_press(pros::controller_digital_e_t button) = 0;

			/**
			 * @brief Sets text to the controller LCD screen
			 *
			 * @param line: Line number at which the text will be displayed [0-2]
			 * @param col: Column number at which the text will be displayed [0-14]
			 * @param fmt: Format string to print to the controller
			 * @param args: Argument list for the format string
			 *
			 * @return 1 if the operation was successful. Otherwise PROS_ERR
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
			 * @return 1 if the operation was successful. Otherwise PROS_ERR
			 */
			virtual std::int32_t set_text(std::uint8_t line, std::uint8_t col, const char* str) = 0;
			virtual std::int32_t set_text(std::uint8_t line, std::uint8_t col, const std::string& str) = 0;

			/**
			 * @brief Clears an individual line of the controller screen
			 *
			 * @param line: Line number to clear [0-2]
			 *
			 * @return 1 if the operation was successful. Otherwise PROS_ERR
			 */
			virtual std::int32_t clear_line(std::uint8_t line) = 0;

			/**
			 * @brief Rumble the controller
			 * 
			 * @param rumble_pattern: String consisting of the characters '.', '-', and ' ',
			 * 		where dots are short rumbles, dashes are long rumbles, and spaces are
			 * 		pauses. Maximum supported length is 8 characters.
			 * 
			 * @return 1 if the operation was successful. Otherwise PROS_ERR
			 */
			virtual std::int32_t rumble(const char* rumble_pattern) = 0;

			/**
			 * @brief Clears all of the lines on the controller screen
			 *
			 * @return 1 if the operation was successful. Otherwise PROS_ERR
			 */
			virtual std::int32_t clear(void) = 0;
	};
}

#endif  // _UMBC_CONTROLLER_HPP_
