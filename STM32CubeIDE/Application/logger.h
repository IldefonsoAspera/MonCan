/*
 * log_thread.h
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#ifndef APPLICATION_USER_LOGGER_H_
#define APPLICATION_USER_LOGGER_H_

#include <stdint.h>
#include "app_threadx.h"


#define LOG_THREAD_PRIO                          12
#define LOG_THREAD_PREEMPTION_THRESHOLD          LOG_THREAD_PRIO


enum log_data_type {
	LOG_STRING,
	LOG_UINT_DEC,
	LOG_INT_DEC,
	LOG_HEX_2,
	LOG_HEX_4,
	LOG_HEX_8,
};


#define log_str(str)	_log_const_string(str, strlen(str))


#define log_dec(number)	_log_var((uint32_t)number, _Generic((number),                  \
														unsigned char:  LOG_UINT_DEC,  \
														unsigned short: LOG_UINT_DEC,  \
														unsigned long:  LOG_UINT_DEC,  \
														unsigned int:   LOG_UINT_DEC,  \
														char:           LOG_INT_DEC,   \
														signed char:    LOG_INT_DEC,   \
														signed short:   LOG_INT_DEC,   \
														signed long:    LOG_INT_DEC,   \
														signed int:     LOG_INT_DEC))


#define log_hex(number) _log_var((uint32_t)(number), _Generic((number),             \
														unsigned char:  LOG_HEX_2,  \
														unsigned short: LOG_HEX_4,  \
														unsigned long:  LOG_HEX_8,  \
														unsigned int:   LOG_HEX_8,  \
														char:           LOG_HEX_2,  \
														signed char:    LOG_HEX_2,  \
														signed short:   LOG_HEX_4,  \
														signed long:    LOG_HEX_8,  \
														signed int:     LOG_HEX_8))



void _log_var_hex(uint32_t number, enum log_data_type type, uint8_t n_digits);


void _log_var(uint32_t number, enum log_data_type type);
void _log_const_string(const char *string, uint32_t length);

void log_th(ULONG thread_input);
UINT log_init(void);


#endif /* APPLICATION_USER_LOGGER_H_ */
