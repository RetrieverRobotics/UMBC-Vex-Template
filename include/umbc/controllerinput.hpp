/**
 * @file umbc/controllerinput.hpp
 *
 * Contains the prototype for the ControllerInput. ControllerInput stores controller
 * values with a minimal memory footprint.
 */

#ifndef _UMBC_CONTROLLER_INPUT_HPP_
#define _UMBC_CONTROLLER_INPUT_HPP_

// standard libraries
#include <cstdint>

// local header files
#include "api.h"

// namespaces used
using namespace pros;
using namespace std;


namespace umbc {

	/**
	 * Lower and upper value limits for controller analog channels
	 */
	enum class ControllerAnalogLimit : std::int32_t {
		MIN = -127,
		MAX = 127
	};


	/**
	 * Represents all digital channels (buttons) and analog channels (thumbsticks)
	 * of a controller
	 */
	class ControllerInput {

		private:

			// digital channels
			std::uint8_t digital_l1 : 1;	// L! bumper digital controller button
			std::uint8_t digital_l2 : 1;	// L2 trigger digital controller button
			std::uint8_t digital_r1 : 1;	// R1 bumper digital controller button
			std::uint8_t digital_r2 : 1;	// R2 trigger digital controller button
			std::uint8_t digital_up : 1;	// up directional pad digital controller button
			std::uint8_t digital_down : 1;	// down directional pad digital controller button
			std::uint8_t digital_left : 1;	// left directional pad digital controller button
			std::uint8_t digital_right : 1;	// right directional pad digital controller button
			std::uint8_t digital_x : 1;		// X action digital controller button
			std::uint8_t digital_b : 1;		// B action digital controller button
			std::uint8_t digital_y : 1;		// Y action digital controller button
			std::uint8_t digital_a : 1;		// A action digital controller button

			// analog channels
			std::int8_t analog_left_x;	// left horizontal analog controller thumbstick
			std::int8_t analog_left_y;	// left vertical analog controller thumbstick
			std::int8_t analog_right_x;	// right horizontal analog controller thumbstick
			std::int8_t analog_right_y;	// right vertical analog controller thumbstick


		public:

			/**
			 * @brief Creates a controller input object and initializes all inputs to zero
			 */
			ControllerInput();

			/**
			 * @brief Gets the value of an analog input (thumbstick)
			 *
			 * @param channel: Analog input value to get. Must be one of
			 * 		ANALOG_{LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y}
			 *
			 * @return Value of the analog input: [-127, 127]
			 */
			std::int32_t get_analog(pros::controller_analog_e_t channel);

			/**
			 * @brief Gets the value of a digital input (button)
			 *
			 * @param button: Digital input value to get. Must be one of
			 * 		DIGITAL_{RIGHT, DOWN, LEFT, UP, A, B, Y, X, R1, R2, L1, L2}
			 *
			 * @return Value of the digital input: 1 or 0
			 */
			std::int32_t get_digital(pros::controller_digital_e_t button);

			/**
			 * @brief Sets the value of an analog input (thumbstick)
			 *
			 * @param channel: Analog input value to get. Must be one of
			 * 		ANALOG_{LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y}
			 *
			 * @param value: Value for the analog input. Must be [-127, 127]
			 * 
			 * @note Values lower than -127 will be converted to -127, and values higher
			 * 		than 127 will be converted to 127 for the value parameter.
			 */
			void set_analog(pros::controller_analog_e_t channel, std::int32_t value);

			/**
			 * @brief Sets the value of a digital input (button)
			 *
			 * @param button: Digital input value to get. Must be one of
			 * 		DIGITAL_{RIGHT, DOWN, LEFT, UP, A, B, Y, X, R1, R2, L1, L2}
			 *
			 * @param value: Value for the digital input. Either 0 or 1
			 * 
			 * @note Non-zero numbers will be converted to 1 for the value parameter.
			 */
			void set_digital(pros::controller_digital_e_t button, std::int32_t value);
	};
}

#endif // _UMBC_CONTROLLER_INPUT_HPP_