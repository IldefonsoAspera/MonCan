/*
 * log_thread.c
 *
 *  Created on: Oct 31, 2021
 *      Author: Ilde
 */


#include "logger.h"
#include "vcp_th.h"




void log_th(ULONG thread_input)
{

	while(1)
	{
		//BSP_LED_Toggle(LED_GREEN);
		vcp_send("Hola mundo\n");
		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}
