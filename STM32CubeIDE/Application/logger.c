/*
 * log_thread.c
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#include <stdbool.h>
#include "logger.h"
#include "vcp_th.h"



#define LOG_IN_QUEUE_SIZE		2048
#define CEILING_MULT_OF_4(x)	(((x)>>2) + ((x & 0x03) > 0))


struct queue_item
{
	uint32_t 		   data;
	uint16_t           str_len;
	enum log_data_type type;
};


uint8_t qLogBuffer[LOG_IN_QUEUE_SIZE];
TX_QUEUE qLog;



static inline uint8_t process_number_decimal(uint32_t number, char *output)
{
	uint32_t divider = 1000000000UL;
	uint8_t i;

	for(i=0; i<10; i++)
	{
		output[i] = 0x30 + number/divider;
		number %= divider;
		divider /= 10;
	}

	i = 0;
	while(i < 10 && output[i] == 0x30)
		i++;

	if(i == 10)		// Case when all digits are 0 should print one digit ('0')
		return 1;
	else			// Return number of digits to print without leading zeroes
		return 10-i;
}


static inline void process_number_hex(uint32_t number, char *output, uint8_t n_digits)
{
	char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int8_t i;

	for(i = sizeof(number)*2 - 1; i > -1; i--)
	{
		output[i] = hexVals[number & 0x0F];
		number >>= 4;
	}
}


void _log_const_string(const char *string, uint32_t length)
{
	struct queue_item item = {.type = LOG_STRING, .data = (uint32_t)string, .str_len = length};
	tx_queue_send(&qLog, &item, TX_NO_WAIT);
}


static void proc_string(char *string, uint32_t length)
{
	vcp_send(string, length);
}


static void proc_uint_dec(uint32_t number)
{
	char output[10];
	uint8_t n_digits;

	n_digits = process_number_decimal(number, output);
	proc_string(&output[10 - n_digits], n_digits);
}


static void proc_hex(uint32_t number, uint8_t n_digits)
{
	char output[sizeof(number)*2];

	process_number_hex(number, output, n_digits);
	proc_string(&output[8 - n_digits], n_digits);
}


static void proc_sint_dec(int32_t number)
{
	char output[11];
	bool is_negative = number < 0;
	uint8_t n_digits;

	if(is_negative)
		number = -number;

	n_digits = process_number_decimal(number, &output[1]);

	if(is_negative)
		output[10 - n_digits++] = '-';

	proc_string(&output[11 - n_digits], n_digits);
}


void _log_var(uint32_t number, enum log_data_type type)
{
	struct queue_item item = {.type = type, .data = number};
	tx_queue_send(&qLog, &item, TX_NO_WAIT);
}


void log_th(ULONG thread_input)
{
	while(1)
	{
		struct queue_item item;

		if(tx_queue_receive(&qLog, &item, TX_WAIT_FOREVER) == TX_SUCCESS)
		{
			switch(item.type)
			{
			case LOG_STRING:
				proc_string((char*)item.data, item.str_len);
				break;
			case LOG_UINT_DEC:
				proc_uint_dec(item.data);
				break;
			case LOG_INT_DEC:
				proc_sint_dec((int32_t)item.data);
				break;
			case LOG_HEX_2:
				proc_hex(item.data, 2);
				break;
			case LOG_HEX_4:
				proc_hex(item.data, 4);
				break;
			case LOG_HEX_8:
				proc_hex(item.data, 8);
				break;
			}
		}
	}
}




UINT log_init(void)
{
	return tx_queue_create(&qLog, "log in queue", CEILING_MULT_OF_4(sizeof(struct queue_item)), qLogBuffer, sizeof(qLogBuffer));
}



