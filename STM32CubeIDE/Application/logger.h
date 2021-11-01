/*
 * log_thread.h
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#ifndef APPLICATION_USER_LOGGER_H_
#define APPLICATION_USER_LOGGER_H_

#include "string.h"
#include "app_threadx.h"


#define LOG_THREAD_PRIO                          12
#define LOG_THREAD_PREEMPTION_THRESHOLD          LOG_THREAD_PRIO



#define log_string(str)		_log_string(str, strlen(str))


void log_uint_dec(uint32_t number);
void log_uint_hex(uint32_t number);
void log_sint_dec(int32_t number);
void log_sint_hex(int32_t number);

void log_th(ULONG thread_input);


#endif /* APPLICATION_USER_LOGGER_H_ */
