/*
 * demo.c
 *
 *  Created on: 2 nov. 2021
 *      Author: Ilde
 */

#include "demo.h"
#include "logger.h"
#include "main.h"

FDCAN_HandleTypeDef *mphcan = NULL;

void demo_th(ULONG thread_input)
{
	uint16_t i = 0;

	HAL_FDCAN_Start(mphcan);
	while(1)
	{
		uint8_t rxPayload[8];
		uint8_t txPayload[8];
		FDCAN_RxHeaderTypeDef rxHeader;
		FDCAN_TxHeaderTypeDef txHeader = {
				.IdType = FDCAN_STANDARD_ID,
				.Identifier = i++,
				.TxFrameType = FDCAN_DATA_FRAME,
				.DataLength = sizeof(txPayload),
				.ErrorStateIndicator = FDCAN_ESI_PASSIVE,
				.BitRateSwitch = FDCAN_BRS_OFF,
				.FDFormat = FDCAN_CLASSIC_CAN,
				.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
				.MessageMarker = 0
		};


		log_string("Inserting TX packet in CAN module\n");
		txPayload[7] = i;
		HAL_FDCAN_AddMessageToTxFifoQ(mphcan, &txHeader, txPayload);
		tx_thread_sleep(1);			// Thread sleep for 10ms

		log_string("Checking received messages\n");
		if(HAL_FDCAN_GetRxFifoFillLevel(mphcan, FDCAN_RX_FIFO0))
		{
			// Extraer paquete de buffer
			HAL_FDCAN_GetRxMessage(mphcan, FDCAN_RX_FIFO0, &rxHeader, rxPayload);

			// Descomponer paquete:
			log_string("Packet received\nDest addr: 0x");
			log_uint_hex(rxHeader.Identifier);
			log_string("\nData:\n0x");
			log_uint_hex(rxPayload[0]);
			log_uint_hex(rxPayload[1]);
			log_uint_hex(rxPayload[2]);
			log_uint_hex(rxPayload[3]);
			log_string(" ");
			log_uint_hex(rxPayload[4]);
			log_uint_hex(rxPayload[5]);
			log_uint_hex(rxPayload[6]);
			log_uint_hex(rxPayload[7]);
			log_string("\n");
			// Imprimir dirección de destino
			// Imprimir payload

			//BSP_LED_Toggle(LED_GREEN);
		}
		else
		{
			log_string("No msgs received\n");
		}
		log_string("\n");
		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}


void demo_init(FDCAN_HandleTypeDef *phcan)
{
	mphcan = phcan;
}
