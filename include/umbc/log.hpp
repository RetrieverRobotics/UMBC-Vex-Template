/**
 * \file umbc/log.hpp
 *
 * Contains definitions for logging to the pros terminal. To disable
 * commenting, ensure LOG is not defined.
 */

#ifndef _UMBC_LOG_HPP_
#define _UMBC_LOG_HPP_

#include <iostream>

#define LOG // comment out this line to disable logging

#ifdef LOG
#define INFO(str) cout << "INFO: " << str << endl;
#define WARN(str) cout << "WARN: " << str << endl;
#define ERROR(str) cerr << "ERROR: " << str << endl;

#else
#define INFO(str)
#define WARN(str)
#define ERROR(str)
#endif

#endif // _UMBC_LOG_HPP_