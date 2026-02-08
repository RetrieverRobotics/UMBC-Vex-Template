/**
 * @file umbc/controllerrecorder.hpp
 *
 * Contains the prototype for the ControllerRecorder. ControllerRecorder
 * saves controller input at set poll rate. The files created by the
 * ControllerRecorder are meant to be used as input for the VController.
 */

#ifndef _UMBC_CONTROLLER_RECORDER_HPP_
#define _UMBC_CONTROLLER_RECORDER_HPP_

// standard libraries
#include <cstdint>
#include <queue>

// local header files
#include "controller.hpp"
#include "controllerinput.hpp"
#include "api.h"

// namespaces used
using namespace pros;
using namespace std;

namespace umbc {

    /**
     * Records controller input at a set poll rate and saves it to a binary file
     */
    class ControllerRecorder {

        private:

            // name of the task that records controller input
            static constexpr char* t_record_controller_input_name =  (char*)"controllerrecorder";

            std::uint16_t poll_rate_ms;                         // rate in milliseconds the controller is polled
            umbc::Controller* controller;                       // controller being recorded
            std::queue<ControllerInput> controller_input;       // queue to save controller input
            std::unique_ptr<Task> t_record_controller_input;    // task for polling controller input at the poll rate

            /**
             * @brief Pushes current controller input to a queue at the set poll rate
             * 
             * @param ControllerRecorder: The controller recorder that will be saving controller
             *      input. The type for this parameter must be ControllerRecorder. Intended to be
             *      'this' pointer.
             * 
             * @note This function is intended to be used as a task, which is why it is static.
             * @note This will continue to record until stop is called or the number of recorded
             *      inputs reached INT32_MAX.
             */
            static void record(void* ControllerRecorder);


        public:

            /**
             * @brief Creates a controller recorder object
             * 
             * @param controller: controller to record
             * 
             * @param poll_rate_ms: Rate in milliseconds controller input will be polled at       
             */
            ControllerRecorder(umbc::Controller* controller, std::uint16_t poll_rate_ms);

            /**
             * @brief Saves the poll rate and recorded controller input into a binary file
             * 
             * @param file_path: File path that the binary file will be created and saved at. 
             * 
             * @return Number of controller inputs written to the file. Otherwise -1 on
             *      failure
             * 
             * @note This method is destructive and will clear all recorded controller
             *      input.
             * @note If a file already exists at the file path location, it will be overwritten.
             */
            std::int32_t save(const char* file_path);

            /**
             * @brief Starts recording controller input
             */
            void start();

            /**
             * @brief Pauses recording controller input if recording is currently active
             */
            void pause();

            /**
             * @brief Resumes recording controller input if recording is currently paused
             */
            void resume();

            /**
             * @brief Stops recording controller input
             */
            void stop();

            /**
             * @brief Clears the controller input buffer
             */
            void reset();

            /**
             * @brief Checks if currently recording
             * 
             * @return True if currently recording. Otherwise false
             */
            bool isRecording();

            /**
             * @brief Checks if there is any controller input that has been recorded
             * 
             * @return True if there is controller input recorded. Otherwise false 
             */
            bool isControllerInput();
    };
}

#endif // _UMBC_CONTROLLER_RECORDER_HPP_