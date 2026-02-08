/**
 * @file umbc.h
 *
 * Contains all the header files of the various definitions, classes and functions
 * in the umbc namespace
 */

#ifndef _UMBC_H_
#define _UMBC_H_

// amount of time in milliseconds to wait to display a message
#define MSG_DELAY_MS 1000

#ifdef __cplusplus
#include "umbc/controller.hpp"
#include "umbc/controllerinput.hpp"
#include "umbc/controllerrecorder.hpp"
#include "umbc/pcontroller.hpp"
#include "umbc/robot.hpp"
#include "umbc/vcontroller.hpp"
#include "umbc/log.hpp"
#include "umbc/menu.hpp"
#endif

#endif // _UMBC_H_