/*
 * vcp_th.c
 *
 *  Created on: 1 nov. 2021
 *      Author: Ilde
 */

#include <stdint.h>
#include "vcp_th.h"



#define BUFFER_READY_FLAG	1


struct vcp_data
{
	uint8_t 			 txBuffer[2][VCP_TX_BUFFER_SIZE];		// Ping pong buffers with outgoing data (TX)
	uint32_t 			 nBytesFilled;							// Number of bytes written in buffer not being TX'd
	uint8_t* 			 p_toFill;								// Pointer to current buffer being filled
	UART_HandleTypeDef*  p_huart;
	TX_EVENT_FLAGS_GROUP vcpTxFlag;
	TX_MUTEX 			 buf_mutex;
};


struct vcp_data m_data = {.p_huart = NULL, .nBytesFilled = 0};



void vcp_th(ULONG thread_input)
{
	UINT status;
	while(1)
	{
		uint32_t dummy;

		status = tx_event_flags_get(&m_data.vcpTxFlag, BUFFER_READY_FLAG, TX_OR_CLEAR, &dummy, TX_WAIT_FOREVER);
		if(status == TX_SUCCESS && (m_data.vcpTxFlag.tx_event_flags_group_current & BUFFER_READY_FLAG))
		{
			uint8_t *outgoing_buffer;
			uint32_t outgoing_nBytes;

			if(tx_mutex_get(&m_data.buf_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
			{
				outgoing_buffer   = m_data.p_toFill;
				outgoing_nBytes   = m_data.nBytesFilled;
				m_data.p_toFill     = (m_data.p_toFill == m_data.txBuffer[0]) ? m_data.txBuffer[1] : m_data.txBuffer[0];
				m_data.nBytesFilled = 0;
				tx_mutex_put(&m_data.buf_mutex);
			}

			BSP_LED_Toggle(LED_GREEN);
			HAL_UART_Transmit(m_data.p_huart, outgoing_buffer, outgoing_nBytes, HAL_MAX_DELAY);
		}
	}
}


void vcp_send(void* p_data, uint32_t length)
{
	if(tx_mutex_get(&m_data.buf_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
	{
		if(length <= sizeof(m_data.txBuffer[0]) - m_data.nBytesFilled)
		{
			memcpy(&m_data.p_toFill[m_data.nBytesFilled], p_data, length);
			m_data.nBytesFilled += length;
		}
		tx_mutex_put(&m_data.buf_mutex);
		tx_event_flags_set(&m_data.vcpTxFlag, BUFFER_READY_FLAG, TX_OR);
	}
	return;
}


UINT vcp_init(UART_HandleTypeDef *p_huart)
{
	UINT status;

	m_data.p_huart = p_huart;
	status = tx_mutex_create(&m_data.buf_mutex,"vcp TxBuf mutex", TX_NO_INHERIT);
	if(status == TX_SUCCESS)
		status = tx_event_flags_create(&m_data.vcpTxFlag, "vcpTxFlag");

	return status;
}
