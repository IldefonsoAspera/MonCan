/*
 * demo.c
 *
 *  Created on: 2 nov. 2021
 *      Author: Ilde
 */

#include "demo.h"
#include "logger.h"



void demo_th(ULONG thread_input)
{

	while(1)
	{
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

		//BSP_LED_Toggle(LED_GREEN);
		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}
