/*
 * log_thread.c
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */

#include <stdbool.h>
#include "logger.h"
#include "vcp_th.h"



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


static inline uint8_t process_number_hex(uint32_t number, char *output)
{
	char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int8_t i;

	for(i = 7; i > -1; i--)
	{
		output[i] = hexVals[number & 0x0F];
		number >>= 4;
	}

	i = 0;
	while(i < 8 && output[i] == 0x30)
		i++;

	if(i == 8)		// Case when all digits are 0 should print one digit ('0')
		return 1;
	else			// Return number of digits to print without leading zeroes
		return 8-i;
}


void _log_string(char *string, uint32_t length)
{
	vcp_send(string, length);
}


void log_uint_dec(uint32_t number)
{
	char output[10];
	uint8_t n_digits;

	n_digits = process_number_decimal(number, output);
	_log_string(&output[10 - n_digits], n_digits);
}


void log_uint_hex(uint32_t number)
{
	char output[8];
	uint8_t n_digits;

	n_digits = process_number_hex(number, output);
	_log_string(&output[8 - n_digits], n_digits);
}


void log_sint_dec(int32_t number)
{
	char output[11];
	bool is_negative = number < 0;
	uint8_t n_digits;

	if(is_negative)
		number = -number;

	n_digits = process_number_decimal(number, &output[1]);

	if(is_negative)
		output[10 - n_digits++] = '-';

	_log_string(&output[11 - n_digits], n_digits);
}


void log_sint_hex(int32_t number)
{
	log_uint_hex((uint32_t)number);
}


void log_th(ULONG thread_input)
{

	while(1)
	{
		//BSP_LED_Toggle(LED_GREEN);
		log_string("Hola mundo\n");
		log_uint_dec(123456789);
		log_string("\n");
		log_uint_dec(123456);
		log_string("\n");
		log_uint_dec(1234);
		log_string("\n");
		log_uint_dec(123);
		log_string("\n");
		log_uint_dec(0);
		log_string("\n");

		log_uint_hex(0xFEDCBA98);
		log_string("\n");
		log_uint_hex(0xFEDCBA);
		log_string("\n");
		log_uint_hex(0xFEDC);
		log_string("\n");
		log_uint_hex(0xFE);
		log_string("\n");
		log_uint_hex(0);
		log_string("\n");

		log_sint_dec(123456789);
		log_string("\n");
		log_sint_dec(123456);
		log_string("\n");
		log_sint_dec(1234);
		log_string("\n");
		log_sint_dec(123);
		log_string("\n");
		log_sint_dec(0);
		log_string("\n");

		log_sint_dec(-123456789);
		log_string("\n");
		log_sint_dec(-123456);
		log_string("\n");
		log_sint_dec(-1234);
		log_string("\n");
		log_sint_dec(-123);
		log_string("\n");
		log_sint_dec(0);
		log_string("\n");

		log_sint_dec(INT32_MIN);
		log_string("\n");

		log_sint_hex(0xFEDCBA98);
		log_string("\n");
		log_sint_hex(0xFEDCBA);
		log_string("\n");
		log_sint_hex(0xFEDC);
		log_string("\n");
		log_sint_hex(0xFE);
		log_string("\n");
		log_sint_hex(0);
		log_string("\n");

		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}
