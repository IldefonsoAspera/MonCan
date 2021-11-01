/*
 * vcp_th.c
 *
 *  Created on: 1 nov. 2021
 *      Author: Ilde
 */

#include "vcp_th.h"


UART_HandleTypeDef *mp_huart = NULL;



void vcp_th(ULONG thread_input)
{
	while(1)
	{
	    BSP_LED_Toggle(LED_GREEN);
		HAL_UART_Transmit(mp_huart, "Hola\n", strlen("hola\n"), HAL_MAX_DELAY);
		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}



void vcp_init(UART_HandleTypeDef *p_huart)
{
	mp_huart = p_huart;
}
