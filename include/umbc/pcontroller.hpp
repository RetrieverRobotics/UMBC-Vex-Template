/**
 * @file umbc/pcontroller.hpp
 *
 * Contains the prototype wrapper class that represents physical controllers. This class is
 * meant to wrap around the pros::Controller class.
 */

#ifndef _UMBC_P_CONTROLLER_HPP_
#define _UMBC_P_CONTROLLER_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "controller.hpp"
#include "api.h"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {
	
	/**
	 * Wrapper for the pros::Controller class which is designed for physical controller interaction
	 * within the umbc namespace. It extends the umbc::Controller class and provides various
	 * functionalities to interact with the controller hardware.
	 */
	class PController : public umbc::Controller {

		private:

			pros::Controller controller; // used to interact with the physical controller


		public:

			/**
			 * @brief Creates a controller object for the given controller id
			 *
			 * @param id: The ID of the controller (e.g. the master or partner controller).
			 * 		Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
			 */
			PController(controller_id_e_t id);

			/**
			 * @brief Checks if the controller is connected
			 *
			 * @return 1 if the controller is connected. Otherwise 0
			 * 
			 * @note Sets errno to EACCES when an error state is reached
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
			 * @return Controller's battery capacity
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 */
			std::int32_t get_battery_capacity();

			/**
			 * @brief Gets the battery level of the controller
			 *
			 * @return Controller's battery level
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 */
			std::int32_t get_battery_level();

			/**
			 * @brief Checks if a digital channel (button) on the controller is pressed
			 *
			 * @param button: Button to read. Must be one of
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed. Otherwise 0
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note 0 is returned if the controller is not connected
			 */
			std::int32_t get_digital(controller_digital_e_t button);

			/**
			 * @brief Returns a rising-edge case for a controller button press
			 *
			 * @param button: Button to read. Must be
			 * 		DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
			 *
			 * @return 1 if the button on the controller is pressed and had not been
			 * 		pressed the last time this function was called, 0 otherwise
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Not thread safe
			 */
			std::int32_t get_digital_new_press(controller_digital_e_t button);

			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wunused-function"
				template <typename T>
				T convert_args(T arg) {
					return arg;
				}
				const char* convert_args(const std::string& arg) {
					return arg.c_str();
				}
			#pragma GCC diagnostic pop

			/**
			 * @brief Sets text to the controller LCD screen
			 *
			 * @param line: Line number at which the text will be displayed [0-2]
			 * @param col: Column number at which the text will be displayed [0-14]
			 * @param fmt: Format string to print to the controller
			 * @param args: Argument list for the format string
			 *
			 * @return 1 if the operation was successful or PROS_ERR if the operation
			 * 		failed, setting errno
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Controller text setting is currently in beta, so continuous, fast
			 * 		updates will not work well
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
			 * @return 1 if the operation was successful or PROS_ERR if the operation
			 * 		failed, setting errno
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Controller text setting is currently in beta, so continuous, fast
			 * 		updates will not work well.
			 */
			std::int32_t set_text(std::uint8_t line, std::uint8_t col, const char* str);
			std::int32_t set_text(std::uint8_t line, std::uint8_t col, const std::string& str);

			/**
			 * @brief Clears an individual line of the controller screen
			 *
			 * @param line: Line number to clear [0-2]
			 *
			 * @return 1 if the operation was successful or PROS_ERR if the operation
			 * 		failed, setting errno
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Controller text setting is currently in beta, so continuous, fast
			 * 		updates will not work well
			 */
			std::int32_t clear_line(std::uint8_t line);

			/**
			 * @brief Rumble the controller
			 *
			 * @param rumble_pattern: String consisting of the characters '.', '-', and ' ',
			 * 		where dots are short rumbles, dashes are long rumbles, and spaces are pauses.
			 *		Maximum supported length is 8 characters
			 *
			 * @return 1 if the operation was successful or PROS_ERR if the operation
			 * 		failed, setting errno
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Controller rumble activation is currently in beta, so continuous, fast
			 * 		updates will not work well
			 */
			std::int32_t rumble(const char* rumble_pattern);

			/**
			 * @brief Clears all of the lines on the controller screen
			 *
			 * @return 1 if the operation was successful or PROS_ERR if the operation
			 * 		failed, setting errno
			 * 
			 * @note Sets errno to EACCES when an error state is reached
			 * @note Controller text setting is currently in beta, so continuous, fast
			 * 		updates will not work well. On vexOS version 1.0.0 this function will
			 * 		block for 110ms
			 */
			std::int32_t clear();
	};
}

#endif  // _UMBC_P_CONTROLLER_HPP_
