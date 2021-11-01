/*
 * vcp_th.c
 *
 *  Created on: 1 nov. 2021
 *      Author: Ilde
 */

#include <stdint.h>
#include "vcp_th.h"




uint8_t				vcpTxq_array[500];
TX_QUEUE           	vcpTx_queue;
UART_HandleTypeDef 	*mp_huart = NULL;




void vcp_th(ULONG thread_input)
{
	uint8_t* qItem;

	if (tx_queue_create(&vcpTx_queue, "vcpTx queue",TX_1_ULONG, vcpTxq_array, sizeof(vcpTxq_array)) != TX_SUCCESS)
	{
		return;
	}


	while(1)
	{

		if(tx_queue_receive(&vcpTx_queue, &qItem, TX_WAIT_FOREVER) == TX_SUCCESS)
		{
			BSP_LED_Toggle(LED_GREEN);
			HAL_UART_Transmit(mp_huart, qItem, strlen((const char*)qItem), HAL_MAX_DELAY);
		}
	}
}


void vcp_send(char* string)
{
	tx_queue_send(&vcpTx_queue, &string, TX_NO_WAIT);
}


void vcp_init(UART_HandleTypeDef *p_huart)
{
	mp_huart = p_huart;
}
