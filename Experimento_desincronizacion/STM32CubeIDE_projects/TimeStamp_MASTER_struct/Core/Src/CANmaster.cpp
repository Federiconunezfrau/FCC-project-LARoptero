/*
 * task_CANsender.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#include <cstring>
#include "CANmaster.h"
#include "systemEvents.h"
#include "system.h"
#include "main.h"

extern volatile uint8_t flagNewCANdataRx0;

/** Este filtro solo se usa para hacer pruebas de recepción de mensajes. Se reserva para
 *  usar la funcionalidad de forwardear mensajes que provienen de CAN hacia la interfaz USB
 *  hacia la PC. */
static CAN_FilterTypeDef canFilterConfigTestMsgs;

static bool msgLost = false;
static STM32::gpio canMsgLostLED(LD2_GPIO_Port, LD2_Pin);

static PCIFmsg_t msgConfirmationCANenabled = {{"Bus CAN habilitado"},sizeof("Bus CAN habilitado")-1,enableCANbus};
static PCIFmsg_t msgConfirmationCANdisabled = {{"Bus CAN deshabilitado"}, sizeof("Bus CAN deshabilitado")-1, disableCANbus};
static PCIFmsg_t msgConfirmationCANsnifferActivated = {{"Modo sniffer activado"}, sizeof("Activado el modo sniffer")-1, activateCANsniffer};
static PCIFmsg_t msgConfirmationCANsnifferDeactivated = {{"Modo sniffer desactivado"}, sizeof("Modo sniffer desactivado")-1, deactivateCANsniffer};

void CANmaster_constructor(CANmaster_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID)
//void CANmaster_constructor(CANmaster_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID, STM32::gpio *ledMsgRxNodeID1, STM32::gpio *ledMsgRxNodeID2, STM32::gpio *ledMsgRxNodeID3)
{
	stateMachine_constructor(&me->super, (stateHandler_t)&CANmaster_initial);

	me->m_canHandler = canHandler;
	me->m_nodeID = nodeID;
	//me->m_ledCanMsgReceived = ledCanMsgReceived;
//	me->m_ledMsgRxNodeID1 = ledMsgRxNodeID1;
//	me->m_ledMsgRxNodeID2 = ledMsgRxNodeID2;
//	me->m_ledMsgRxNodeID3 = ledMsgRxNodeID3;

	me->m_canTxHeader.ExtId = 0;
	me->m_canTxHeader.IDE = CAN_ID_STD;
	me->m_canTxHeader.RTR = CAN_RTR_DATA;
	me->m_canTxHeader.TransmitGlobalTime = DISABLE;

	me->m_snifferOn = false;
}


state_t CANmaster_initial(CANmaster_t *me)
{
	CAN_FilterTypeDef canFilterConfigTimeMsgs;

	canFilterConfigTimeMsgs.FilterActivation = CAN_FILTER_ENABLE;
	canFilterConfigTimeMsgs.FilterBank = 0;
	canFilterConfigTimeMsgs.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfigTimeMsgs.FilterIdHigh = 0x0700;
	canFilterConfigTimeMsgs.FilterIdLow = 0x0000;
	canFilterConfigTimeMsgs.FilterMaskIdHigh = 0xFF00;
	canFilterConfigTimeMsgs.FilterMaskIdLow = 0x0000;
	canFilterConfigTimeMsgs.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfigTimeMsgs.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfigTimeMsgs.SlaveStartFilterBank = 14;

	// Se configura el filtro de mensajes de prueba. Se deja deshabilitado
	canFilterConfigTestMsgs.FilterActivation = CAN_FILTER_DISABLE;
	canFilterConfigTestMsgs.FilterBank = 1;
	canFilterConfigTestMsgs.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfigTestMsgs.FilterIdHigh = 0x0000;
	canFilterConfigTestMsgs.FilterIdLow = 0x0000;
	canFilterConfigTestMsgs.FilterMaskIdHigh = 0x0000;
	canFilterConfigTestMsgs.FilterMaskIdLow = 0x0000;
	canFilterConfigTestMsgs.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfigTestMsgs.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfigTestMsgs.SlaveStartFilterBank = 14;


	HAL_CAN_ConfigFilter(me->m_canHandler, &canFilterConfigTimeMsgs);
	HAL_CAN_ConfigFilter(me->m_canHandler, &canFilterConfigTestMsgs);
	//me->m_ledCanMsgReceived->write(GPIO_ST::LOW);

	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANmaster_off);
}

state_t CANmaster_off(CANmaster_t *me, event_t const e)
{
	switch(e)
	{
		case EV_CAN_TURN_ON:
			HAL_CAN_Start(me->m_canHandler);
			HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_MSG_PENDING);
			//HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_FULL);
			// HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_RX_FIFO1_MSG_PENDING);

			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANmaster_on);

		case EV_CAN_ACTIVATE_SNIFFER:
			canFilterConfigTestMsgs.FilterActivation = CAN_FILTER_ENABLE;
			HAL_CAN_ConfigFilter(me->m_canHandler, &canFilterConfigTestMsgs);
			me->m_snifferOn = true;
			p_PCIFTxBuffer->write(msgConfirmationCANsnifferActivated);

			return HANDLED;

		case EV_CAN_DEACTIVATE_SNIFFER:
			canFilterConfigTestMsgs.FilterActivation = CAN_FILTER_DISABLE;
			HAL_CAN_ConfigFilter(me->m_canHandler, &canFilterConfigTestMsgs);
			me->m_snifferOn = false;
			p_PCIFTxBuffer->write(msgConfirmationCANsnifferDeactivated);

			return HANDLED;
	}

	return IGNORED;
}

state_t CANmaster_on(CANmaster_t *me, event_t const e)
{
	CANmsg_t canMsg;
	uint32_t freeMailboxes;
	uint32_t i;
	uint32_t txMailbox;
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[MAX_LEN_PAYLOAD_CAN];
	uint32_t fifoFillLevel;
	PCIFmsg_t pcifMsg;

	switch(e)
	{

		case EV_ENTRY:
			p_PCIFTxBuffer->write(msgConfirmationCANenabled);
			return HANDLED;

		case EV_EXIT:
			p_PCIFTxBuffer->write(msgConfirmationCANdisabled);
			return HANDLED;

		case EV_CAN_TURN_OFF:
			HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_MSG_PENDING);
			//HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_FULL);
			//HAL_CAN_DeactivateNotification(me->m_canHandler, CAN_IT_RX_FIFO1_MSG_PENDING);
			HAL_CAN_Stop(me->m_canHandler);

			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) CANmaster_off);

		case EV_CAN_FETCH_NEW_MSGS:
			i = 0;
			fifoFillLevel = HAL_CAN_GetRxFifoFillLevel(me->m_canHandler, CAN_RX_FIFO0);
			while(i < fifoFillLevel)
			{
				HAL_CAN_GetRxMessage(me->m_canHandler, CAN_RX_FIFO0, &rxHeader, rxData);
				if( CANmsg_make_from_raw_data(&canMsg, rxData, rxHeader.DLC, rxHeader.StdId) == true )
				{
					p_canRxBuffer->write(canMsg);

//					if(p_canRxBuffer->is_full())
//					{
//						me->m_ledMsgRxNodeID3->write(GPIO_ST::HIGH);
//					}
				}
				i++;
			}
			HAL_CAN_ActivateNotification(me->m_canHandler, CAN_IT_RX_FIFO0_MSG_PENDING);

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
			//me->m_ledCanMsgReceived->write(GPIO_ST::HIGH);
			while( (p_canRxBuffer->read(&canMsg) != 0) && (i < 3) )
			{
				switch(canMsg.id)
				{
					case timeStampServiceData:

//						if(canMsg.nodeID == 1)
//						{
//							me->m_ledMsgRxNodeID1->toggle();
//						}
//						else if(canMsg.nodeID == 2)
//						{
//							me->m_ledMsgRxNodeID2->toggle();
//						}
//						else if(canMsg.nodeID == 3)
//						{
//							me->m_ledMsgRxNodeID3->toggle();
//						}

						pcifMsg.cmd = dataTimeStampService;
						memcpy(pcifMsg.payload, &(canMsg.nodeID), sizeof(canMsg.nodeID));
						memcpy(&(pcifMsg.payload[sizeof(canMsg.nodeID)]), canMsg.payload, canMsg.lenPayload);
						pcifMsg.lenPayload = canMsg.lenPayload + sizeof(canMsg.nodeID);

//						if( p_PCIFTxBuffer->is_full() )
//						{
//							me->m_ledMsgRxNodeID2->write(GPIO_ST::HIGH);
//						}

						p_PCIFTxBuffer->write(pcifMsg);
						break;

					default:
						if(me->m_snifferOn)
						{
							pcifMsg.cmd = forwardCANmsg;
							memcpy(pcifMsg.payload, &(canMsg.nodeID), sizeof(canMsg.nodeID));
							memcpy(&(pcifMsg.payload[sizeof(canMsg.nodeID)]), &(canMsg.id), sizeof(canMsg.id));
							memcpy(&(pcifMsg.payload[sizeof(canMsg.nodeID) + sizeof(canMsg.id)]), canMsg.payload, canMsg.lenPayload);
							pcifMsg.lenPayload = canMsg.lenPayload + sizeof(canMsg.nodeID) + sizeof(canMsg.id);
							p_PCIFTxBuffer->write(pcifMsg);
						}
						break;
				}
				i++;
			}

			//me->m_ledCanMsgReceived->write(GPIO_ST::LOW);
			return HANDLED;

	}
	return IGNORED;
}

/** Callback para la interrupción de CAN. Esta interrupción salta cuando hay
 *  nuevos datos en la FIFO 0 de CAN. */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	flagNewCANdataRx0 = 1;
	HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
	if(msgLost == false)
	{
		canMsgLostLED.write(GPIO_ST::HIGH);
		msgLost = true;
	}
}
