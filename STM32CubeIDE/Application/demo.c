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
				.DataLength = FDCAN_DLC_BYTES_8,
				.ErrorStateIndicator = FDCAN_ESI_PASSIVE,
				.BitRateSwitch = FDCAN_BRS_OFF,
				.FDFormat = FDCAN_CLASSIC_CAN,
				.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
				.MessageMarker = 0
		};

		log_str("Inserting TX packet in CAN module\n");
		txPayload[7] = i;
		HAL_FDCAN_AddMessageToTxFifoQ(mphcan, &txHeader, txPayload);
		tx_thread_sleep(1);			// Thread sleep for 10ms

		log_str("Checking received messages\n");
		if(HAL_FDCAN_GetRxFifoFillLevel(mphcan, FDCAN_RX_FIFO0))
		{
			HAL_FDCAN_GetRxMessage(mphcan, FDCAN_RX_FIFO0, &rxHeader, rxPayload);

			log_str("Packet received\nDest addr: 0x");
			log_hex(rxHeader.Identifier);
			log_str("\nData:\n0x");
			log_hex(rxPayload[0]);
			log_hex(rxPayload[1]);
			log_hex(rxPayload[2]);
			log_hex(rxPayload[3]);
			log_str(" ");
			log_hex(rxPayload[4]);
			log_hex(rxPayload[5]);
			log_hex(rxPayload[6]);
			log_hex(rxPayload[7]);
			log_str("\n");
		}
		else
		{
			log_str("No msgs received\n");
		}
		log_str("\n");
		tx_thread_sleep(100);		// Thread sleep for 1s
	}
}


void demo_init(FDCAN_HandleTypeDef *phcan)
{
	mphcan = phcan;
}
