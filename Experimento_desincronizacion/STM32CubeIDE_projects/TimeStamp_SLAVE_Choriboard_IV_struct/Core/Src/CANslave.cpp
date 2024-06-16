/*
 * task_CANsender.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#include <cstring>
#include "CANslave.h"
#include "systemEvents.h"
#include "system.h"
#include "main.h"

#define MAX_AUX_CAN_MSGS 12



static localClock_t *p_localClock;

/** Este tipo de dato sirve para pasarle el mensaje recibido por CAN, desde la interrupción
 *  al main */
typedef struct
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[MAX_LEN_PAYLOAD_CAN];
	timestamp_t timestamp;
} auxCANmsg_t;

/** A medida que recibo un mensaje nuevo, los voy a ir copiando en la primer posición libre, acá */
static auxCANmsg_t auxCANmessages[MAX_AUX_CAN_MSGS];
static volatile bool messageIsInUse[MAX_AUX_CAN_MSGS];

extern volatile uint8_t flagNewCANdataRx0;

//static bool msgLost = false;
static STM32::gpio canMsgLostLED(LED5_GPIO_Port, LED5_Pin);

//static CANmsg_t msgTimeStampStartServiceTest = {{0x01,0x02,0x03,0x04},4,timeStampServiceData,0x00};
//static CANmsg_t msgTimeStampStopServiceTest = {{0x04,0x03,0x02,0x01},4,timeStampServiceData,0x00};

static CANmsg_t msgTimeStampStartServiceData = {{},0,timeStampServiceData,0x00};

void CANslave_constructor(CANslave_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID, localClock_t *localClock)
{
	uint32_t i;

	stateMachine_constructor(&me->super, (stateHandler_t)&CANslave_initial);

	me->m_canHandler = canHandler;
	me->m_nodeID = nodeID;

	me->m_canTxHeader.ExtId = 0;
	me->m_canTxHeader.IDE = CAN_ID_STD;
	me->m_canTxHeader.RTR = CAN_RTR_DATA;
	me->m_canTxHeader.TransmitGlobalTime = DISABLE;

	p_localClock = localClock;

	for(i = 0; i < MAX_AUX_CAN_MSGS; i++)
	{
		messageIsInUse[i] = false;
	}
}


state_t CANslave_initial(CANslave_t *me)
{
	CAN_FilterTypeDef canFilterConfigTimeMsgs;

	canFilterConfigTimeMsgs.FilterActivation = CAN_FILTER_ENABLE;
	canFilterConfigTimeMsgs.FilterBank = 0;
	canFilterConfigTimeMsgs.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfigTimeMsgs.FilterIdHigh = 0x0000;
	canFilterConfigTimeMsgs.FilterIdLow = 0x0000;
	canFilterConfigTimeMsgs.FilterMaskIdHigh = 0x00E0;
	canFilterConfigTimeMsgs.FilterMaskIdLow = 0x0000;
	canFilterConfigTimeMsgs.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfigTimeMsgs.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfigTimeMsgs.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(me->m_canHandler, &canFilterConfigTimeMsgs);

	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANslave_off);
}

state_t CANslave_off(CANslave_t *me, event_t const e)
{
	switch(e)
	{
		case EV_CAN_TURN_ON:
			HAL_CAN_Start(me->m_canHandler);
			HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_MSG_PENDING);
			HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_TX_MAILBOX_EMPTY);
			//HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_ERROR);
			//HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_BUSOFF);

			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANslave_on);
	}

	return IGNORED;
}

state_t CANslave_on(CANslave_t *me, event_t const e)
{
	CANmsg_t canMsg;
	uint32_t freeMailboxes;
	uint32_t i;
	uint32_t txMailbox;
	uint8_t  timeStampData[MAX_LEN_PAYLOAD_CAN];

	switch(e)
	{
		case EV_ENTRY:

			return HANDLED;

		case EV_EXIT:

			return HANDLED;

		case EV_CAN_TURN_OFF:
			HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_MSG_PENDING);
			//HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_ERROR);
			//HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_BUSOFF);
			HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_TX_MAILBOX_EMPTY);
			HAL_CAN_Stop(me->m_canHandler);

			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANslave_off);

		case EV_CAN_FETCH_NEW_MSGS:
			for(i = 0; i < MAX_AUX_CAN_MSGS; i++)
			{
				if(messageIsInUse[i] == true)
				{
					if( CANmsg_make_from_raw_data(&canMsg, auxCANmessages[i].rxData, auxCANmessages[i].rxHeader.DLC, auxCANmessages[i].rxHeader.StdId, auxCANmessages[i].timestamp) == true )
					{
						p_canRxBuffer->write(canMsg);
						messageIsInUse[i] = false;
					}
				}
			}

			return HANDLED;

		case EV_CAN_SEND_NEXT_MSG:
			freeMailboxes = HAL_CAN_GetTxMailboxesFreeLevel(me->m_canHandler);
			i = 0;
			while( (!(p_canTxBuffer->is_empty())) && (i < freeMailboxes))
			{
				p_canTxBuffer->read(&canMsg);
				me->m_canTxHeader.StdId = CAN_MSG_GET_STD_ID(canMsg.id, me->m_nodeID);
				me->m_canTxHeader.DLC = canMsg.lenPayload;
				HAL_CAN_AddTxMessage(me->m_canHandler, &me->m_canTxHeader, canMsg.payload, &txMailbox);
				i++;
			}

			return HANDLED;

		case EV_CAN_PROCESS_NEW_MSG:
			i = 0;
			while( (p_canRxBuffer->read(&canMsg) != 0) && (i < 3) )
			{
				switch(canMsg.id)
				{
					case timeStampServiceStart:
						localClock_start(p_localClock);
						break;

					case timeStampServiceStop:
						localClock_stop(p_localClock);
						for(i = 0; i < MAX_AUX_CAN_MSGS; i++)
						{
							messageIsInUse[i] = false;
						}
						break;

					case timeStampServiceData:
						memcpy(timeStampData, &(canMsg.timestamp.timeStampCounter), sizeof(canMsg.timestamp.timeStampCounter));
						CANmsg_set_payload(&msgTimeStampStartServiceData, timeStampData, sizeof(canMsg.timestamp.timeStampCounter));
						p_canTxBuffer->write(msgTimeStampStartServiceData);
						break;

					default:
						break;
				}
				i++;
			}

			return HANDLED;
	}

	return IGNORED;
}

/** Callback para la interrupción de CAN. Esta interrupción salta cuando hay
 *  nuevos datos en la FIFO 0 de CAN. */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint32_t i = 0;

	while((i < MAX_AUX_CAN_MSGS) && (messageIsInUse[i] == true))
	{
		i++;
	}
	if(i < MAX_AUX_CAN_MSGS)
	{
		auxCANmessages[i].timestamp = localClock_get_timestamp(p_localClock);
		if( HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &(auxCANmessages[i].rxHeader), auxCANmessages[i].rxData) == HAL_OK )
		{
			messageIsInUse[i] = true;
			flagNewCANdataRx0 = 1;
		}

	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	canMsgLostLED.toggle();
}
