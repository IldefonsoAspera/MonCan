/*
 * log_thread.h
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#ifndef APPLICATION_USER_LOGGER_H_
#define APPLICATION_USER_LOGGER_H_

//#include <string.h>
#include "app_threadx.h"


#define LOG_THREAD_PRIO                          12
#define LOG_THREAD_PREEMPTION_THRESHOLD          LOG_THREAD_PRIO


enum log_data_type {
	LOG_STRING,
	LOG_UINT_DEC,
	LOG_UINT_HEX,
	LOG_INT_DEC,
	LOG_INT_HEX
};


#define log_string(str)		 _log_const_string(str, strlen(str))
#define log_uint_dec(number) _log_var((uint32_t)number, LOG_UINT_DEC)
#define log_uint_hex(number) _log_var((uint32_t)number, LOG_UINT_HEX)
#define log_sint_dec(number) _log_var((uint32_t)number, LOG_INT_DEC)
#define log_sint_hex(number) _log_var((uint32_t)number, LOG_INT_HEX)


void _log_var(uint32_t number, enum log_data_type type);
void _log_const_string(const char *string, uint32_t length);

void log_th(ULONG thread_input);
UINT log_init(void);


#endif /* APPLICATION_USER_LOGGER_H_ */
