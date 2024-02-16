/*
 * CNI.cpp
 *
 *  Created on: Feb 13, 2024
 *      Author: fede
 */


#include "CNI.h"

/** Cantidad máxima de bytes de datos útiles para la trama CAN */
#define MAX_LEN_PAYLOAD_CAN 8
#define NUM_MSGS_TO_RX 3

#define SLAVE_START_FILTER_BANK 14

#define MASK_GET_NODE_ID 0x00000007
#define CAN_MSG_MAKE_STD_ID_TX(serviceID)((serviceID<<3)|MY_NODE_ID)
#define CAN_MSG_GET_SERVICE_ID_FROM_STD_ID(stdID)(stdID>>3)
#define CAN_MSG_GET_NODE_ID_FROM_STD_ID(stdID)(stdID & MASK_GET_NODE_ID)

enum serviceID : uint32_t
{
	IMUdata = 0x01
};

typedef struct
{
	serviceID mServiceID_;
	uint32_t  mNodeID_;
	uint8_t   mPayload_[MAX_LEN_PAYLOAD_CAN];
	uint32_t  mLenPayload_;
} CANmsg_t;

static CANmsg_t CANmsgList[] =
{
	// id    , nodeID   , payload, lenPayload
	{ IMUdata, NODE_ID_0, {}     , 0 },
	{ IMUdata, NODE_ID_0, {}     , 0 },
	{ IMUdata, NODE_ID_0, {}     , 0 }
};

static CAN_FilterTypeDef CANfiltersList[] =
{
	//{ .FilterActivation = CAN_FILTER_ENABLE, .FilterBank = 0, .FilterFIFOAssignment = CAN_RX_FIFO0, .FilterIdHigh = IMUdata, .FilterIdLow = 0x0000, .FilterMaskIdHigh = 0xFF00, .FilterMaskIdLow = 0x0000, .FilterMode = CAN_FILTERMODE_IDMASK, .FilterScale = CAN_FILTERSCALE_32BIT, .SlaveStartFilterBank = SLAVE_START_FILTER_BANK }
	{ IMUdata, 0x0000, 0xFF00, 0x0000, CAN_RX_FIFO0, 0, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK }
};

static uint32_t lenCANmsgList = sizeof(CANmsgList)/sizeof(CANmsgList[0]);
static uint32_t lenCANfilterList = sizeof(CANfiltersList)/sizeof(CANfiltersList[0]);

static CNI_t _instance;


void CNI_constructor(CAN_HandleTypeDef *hcan)
{
	_instance.mHcan_ = hcan;

	_instance.mCANtxHeader_.ExtId = 0;
	_instance.mCANtxHeader_.IDE = CAN_ID_STD;
	_instance.mCANtxHeader_.RTR = CAN_RTR_DATA;
	_instance.mCANtxHeader_.TransmitGlobalTime = DISABLE;
}


void CNI_init(void)
{
	uint32_t i;

	for(i = 0; i < lenCANfilterList ; i++)
	{
		HAL_CAN_ConfigFilter(_instance.mHcan_, &CANfiltersList[i]);
	}
}


void CNI_start(void)
{
	HAL_CAN_Start(_instance.mHcan_);
}


CNI_status_t CNI_send_msg(uint32_t msgHandle)
{
	uint32_t txMailbox;
	serviceID currentMsgServiceID;
	uint32_t i;

	if(msgHandle >= lenCANmsgList)
	{
		return CNI_MSG_HANDLE_NOT_VALID;
	}

	if(CANmsgList[msgHandle].mNodeID_ != MY_NODE_ID)
	{
		return CNI_MSG_HANDLE_TX_NOT_ALLOWED;
	}

	// Armar el mensaje
	currentMsgServiceID = CANmsgList[msgHandle].mServiceID_;
	_instance.mCANtxHeader_.StdId = CAN_MSG_MAKE_STD_ID_TX(currentMsgServiceID);

	// Pasarle el msj al periférico

	// Limpia todos los mensajes pendientes (no debería haber ninguno, pero por las dudas)
	HAL_CAN_AbortTxRequest(_instance.mHcan_,CAN_TX_MAILBOX0|CAN_TX_MAILBOX1|CAN_TX_MAILBOX2);

	// Comenzando por la posición "msgHandle" se recorre la lista de mensajes. Esto es debido
	// a que es posible que se requiera enviar más de un mensaje.
	i = msgHandle;
	while( (i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == currentMsgServiceID) && (CANmsgList[i].mNodeID_ == MY_NODE_ID) )
	{
		while(HAL_CAN_GetTxMailboxesFreeLevel(_instance.mHcan_) == 0);

		_instance.mCANtxHeader_.DLC = CANmsgList[i].mLenPayload_;
		HAL_CAN_AddTxMessage(_instance.mHcan_, &_instance.mCANtxHeader_, CANmsgList[i].mPayload_, &txMailbox);
		i++;
	}

	// Antes de retornar, se queda esperando a que se terminen de enviar todos los mensajes
	while(HAL_CAN_GetTxMailboxesFreeLevel(_instance.mHcan_) == 0);

	return CNI_OK;
}


CNI_status_t CNI_receive_msg(uint32_t msgHandle)
{
	CAN_RxHeaderTypeDef auxCANrxHeader;
	//uint8_t auxRxData[MAX_LEN_PAYLOAD_CAN];
	serviceID rxMsgServiceID;
	uint32_t rxNodeID;
	uint32_t i;

	// Chequear si el msgHandle es válido
	if(msgHandle >= lenCANmsgList)
	{
		return CNI_MSG_HANDLE_NOT_VALID;
	}

	if(CANmsgList[msgHandle].mNodeID_ == MY_NODE_ID)
	{
		return CNI_MSG_HANDLE_RX_NOT_ALLOWED;
	}

	i = msgHandle;
	rxMsgServiceID = CANmsgList[msgHandle].mServiceID_;
	rxNodeID = CANmsgList[msgHandle].mNodeID_;

	while((i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == rxMsgServiceID) && (CANmsgList[i].mNodeID_ == rxNodeID))
	{
		while( (HAL_CAN_GetRxFifoFillLevel(_instance.mHcan_, CAN_RX_FIFO0) == 0) );
		HAL_CAN_GetRxMessage(_instance.mHcan_, CAN_RX_FIFO0, &auxCANrxHeader, CANmsgList[i].mPayload_);
		if( (rxMsgServiceID == CAN_MSG_GET_SERVICE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) && (rxNodeID == CAN_MSG_GET_NODE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) )
		{
			CANmsgList[i].mLenPayload_ = auxCANrxHeader.DLC;
			i++;
		}
		else
		{
			CANmsgList[i].mLenPayload_ = 0;
		}
	}

	return CNI_OK;
}
