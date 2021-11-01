/*
 * vcp_th.c
 *
 *  Created on: 1 nov. 2021
 *      Author: Ilde
 */

#include <stdint.h>
#include "vcp_th.h"

#define BUFFER_READY_FLAG	1


uint8_t  			 txBuffer[2][VCP_TX_BUFFER_SIZE];	// Ping pong buffers with outgoing data (TX)
uint32_t 			 nBytes = 0;						// Number of bytes written in buffer not being TX'd
uint8_t* 			 p_txBuffer = txBuffer[0];			// Pointer to current buffer being filled
UART_HandleTypeDef*  p_huart = NULL;
TX_EVENT_FLAGS_GROUP vcpTxFlag;



void vcp_th(ULONG thread_input)
{
	while(1)
	{
		uint32_t dummy;
		if(tx_event_flags_get(&vcpTxFlag, BUFFER_READY_FLAG, TX_OR_CLEAR, &dummy, TX_WAIT_FOREVER) == TX_SUCCESS)
		{
			uint8_t *outgoing_buffer;
			uint32_t outgoing_nBytes;

			__disable_irq();
			outgoing_buffer = p_txBuffer;
			outgoing_nBytes = nBytes;
			p_txBuffer      = (p_txBuffer == txBuffer[0]) ? txBuffer[1] : txBuffer[0];
			nBytes          = 0;
			__enable_irq();

			BSP_LED_Toggle(LED_GREEN);
			HAL_UART_Transmit(p_huart, outgoing_buffer, outgoing_nBytes, HAL_MAX_DELAY);
		}
	}
}


// TODO investigate if calling this function inside while interrupts are disabled would cause them to reactivate prematurely
void vcp_send(void* p_data, uint32_t length)
{
	__disable_irq();
	if(length <= sizeof(txBuffer[0]) - nBytes)
	{
		memcpy(&p_txBuffer[nBytes], p_data, length);
		nBytes += length;
	}
	__enable_irq();

	tx_event_flags_set(&vcpTxFlag, BUFFER_READY_FLAG, TX_OR);
}


UINT vcp_init(UART_HandleTypeDef *phuart)
{
	p_huart = phuart;
	return tx_event_flags_create(&vcpTxFlag, "vcpTxFlag");
}
