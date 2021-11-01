/*
 * log_thread.h
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#ifndef APPLICATION_USER_LOGGER_H_
#define APPLICATION_USER_LOGGER_H_


#include "app_threadx.h"


#define LOG_THREAD_PRIO                          12
#define LOG_THREAD_PREEMPTION_THRESHOLD          LOG_THREAD_PRIO




void log_th(ULONG thread_input);


#endif /* APPLICATION_USER_LOGGER_H_ */
