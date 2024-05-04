/*
 * CNI.cpp
 *
 *  Created on: Feb 13, 2024
 *      Author: fede
 */


#include "CNI.h"
#include "myUtils.h"
#include "timeTriggeredSchedulerPublic.h"

/** Cantidad máxima de bytes de datos útiles para la trama CAN */
#define MAX_LEN_PAYLOAD_CAN 8
#define NUM_MSGS_TO_RX 3

#define SLAVE_START_FILTER_BANK 14

#define MASK_GET_NODE_ID 0x00000007
#define CAN_MSG_MAKE_STD_ID_TX(serviceID)((serviceID<<3)|MY_NODE_ID)
#define CAN_MSG_GET_SERVICE_ID_FROM_STD_ID(stdID)(stdID>>3)
#define CAN_MSG_GET_NODE_ID_FROM_STD_ID(stdID)(stdID & MASK_GET_NODE_ID)
#define CAN_MAKE_FILTER_HIGH(serviceID)(serviceID<<8)

enum serviceID : uint32_t
{
	sync = 0x01,           // STD = 00000001|XXX = 0x00000008
	IMUdata = 0x02,        // STD = 00000010|XXX = 0x00000010
	attitudeData = 0x03,   // STD = 00000011|XXX = 0x00000018
	compareAttitude = 0x04 // STD = 00000100|XXX = 0x00000020
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
	// Mensaje de sincronización
	{ sync        , 0        , {}     , 0 }, // 0

	// Mensaje con datos de la IMU enviada por el slave de nodeID = 1
	{ IMUdata     , 1        , {}     , 0 }, // 1
	{ IMUdata     , 1        , {}     , 0 },
	{ IMUdata     , 1        , {}     , 0 },

	// Mensaje con datos de la IMU enviada por el slave de nodeID = 2
	{ IMUdata     , 2        , {}     , 0 }, // 4
	{ IMUdata     , 2        , {}     , 0 },
	{ IMUdata     , 2        , {}     , 0 },

	// Mensaje con datos de la IMU enviado por el slave de nodeID = 3
	{ IMUdata     , 3        , {}     , 0 }, // 7
	{ IMUdata     , 3        , {}     , 0 },
	{ IMUdata     , 3        , {}     , 0 },

	// Mensaje con datos de la estimación de altitud, por el slave de nodeID = 1
	{ attitudeData, 1        , {}     , 0 }, // 10

	// Mensaje con datos de la estimación de altitud, por el slave de nodeID = 2
	{ attitudeData, 2        , {}     , 0 }, // 11

	// Mensaje con datos de la estimación de altitud, por el slave de nodeID = 3
	{ attitudeData, 3        , {}     , 0 }, // 12

	// Mensaje con residuos enviado por el slave de nodeID = 1
	{ compareAttitude, 1        , {}     , 0 }, // 13
	{ compareAttitude, 1        , {}     , 0 },
	{ compareAttitude, 1        , {}     , 0 },

	// Mensaje con residuos enviado por el slave de nodeID = 2
	{ compareAttitude, 2        , {}     , 0 }, // 16
	{ compareAttitude, 2        , {}     , 0 },
	{ compareAttitude, 2        , {}     , 0 },

	// Mensaje con residuos enviado por el slave de nodeID = 3
	{ compareAttitude, 3        , {}     , 0 }, // 19
	{ compareAttitude, 3        , {}     , 0 },
	{ compareAttitude, 3        , {}     , 0 },
};

static CAN_FilterTypeDef CANfiltersList[] =
{
#if !IS_MASTER
	// FIltro para recibir el mensaje de sincronización
	{ CAN_MAKE_FILTER_HIGH(sync)           , 0x0000, 0xFF00, 0x0000, CAN_RX_FIFO0, 0, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK },
#endif
	// FIltro para recibir datos crudos de IMUs
	{ CAN_MAKE_FILTER_HIGH(IMUdata)        , 0x0000, 0xFF00, 0x0000, CAN_RX_FIFO0, 1, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK },
	// FIltro para recibir datos de estimación de altitud
	{ CAN_MAKE_FILTER_HIGH(attitudeData)   , 0x0000, 0xFF00, 0x0000, CAN_RX_FIFO0, 2, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK },
	// FIltro para recibir datos de residuos de altitud
	{ CAN_MAKE_FILTER_HIGH(compareAttitude), 0x0000, 0xFF00, 0x0000, CAN_RX_FIFO0, 3, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK }
	//	{ 0x0000 , 0x0000, 0x0000, 0x0000, CAN_RX_FIFO0, 0, CAN_FILTERMODE_IDMASK, CAN_FILTERSCALE_32BIT, CAN_FILTER_ENABLE, SLAVE_START_FILTER_BANK }
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
#if !IS_MASTER
    HAL_CAN_ActivateNotification(_instance.mHcan_, CAN_IT_RX_FIFO0_MSG_PENDING);
#endif
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


CNI_status_t CNI_receive_msg(uint32_t msgHandle, uint32_t timeout)
{
	CAN_RxHeaderTypeDef auxCANrxHeader;
	serviceID rxMsgServiceID;
	uint32_t rxNodeID;
	uint32_t i;
	uint32_t startTime;
	uint32_t elapsedTime;
	bool msgReceived;

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

	elapsedTime = 0;
	msgReceived = false;
	startTime = timeTriggeredScheduler_get_time();

	while( (msgReceived == false) && ((timeout == 0) || (elapsedTime < timeout)) )
	{
		// ¿Llegó algún mensaje nuevo?
		if(HAL_CAN_GetRxFifoFillLevel(_instance.mHcan_, CAN_RX_FIFO0) > 0)
		{
			// Se lee el mensaje nuevo
			HAL_CAN_GetRxMessage(_instance.mHcan_, CAN_RX_FIFO0, &auxCANrxHeader, CANmsgList[i].mPayload_);

			// El STD_ID y nodeID son los correspondientes?
			if( (rxMsgServiceID == CAN_MSG_GET_SERVICE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) && (rxNodeID == CAN_MSG_GET_NODE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) )
			{
				CANmsgList[i].mLenPayload_ = auxCANrxHeader.DLC;
				i++;
				if( (i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == rxMsgServiceID) && (CANmsgList[i].mNodeID_ == rxNodeID) )
				{
					msgReceived = false;
				}
				else
				{
					msgReceived = true;
				}
			}
			else
			{
				CANmsgList[i].mLenPayload_ = 0;
			}
		}
		elapsedTime = timeTriggeredScheduler_get_time() - startTime;
	}

	if(msgReceived == true)
	{
		return CNI_OK;
	}

	return CNI_MSG_RX_TIMEOUT;


//	while((i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == rxMsgServiceID) && (CANmsgList[i].mNodeID_ == rxNodeID))
//	{
//		while( (HAL_CAN_GetRxFifoFillLevel(_instance.mHcan_, CAN_RX_FIFO0) == 0) );
//		HAL_CAN_GetRxMessage(_instance.mHcan_, CAN_RX_FIFO0, &auxCANrxHeader, CANmsgList[i].mPayload_);
//		if( (rxMsgServiceID == CAN_MSG_GET_SERVICE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) && (rxNodeID == CAN_MSG_GET_NODE_ID_FROM_STD_ID(auxCANrxHeader.StdId)) )
//		{
//			CANmsgList[i].mLenPayload_ = auxCANrxHeader.DLC;
//			i++;
//		}
//		else
//		{
//			CANmsgList[i].mLenPayload_ = 0;
//		}
//	}
//
//	return CNI_OK;
}

CNI_status_t CNI_update_msg_content(uint32_t msgHandle, uint8_t *payload, uint32_t lenPayload)
{
	uint32_t i;
	serviceID rxMsgServiceID;
	uint32_t rxNodeID;
	uint32_t auxLenPayload;

	// Chequear si el msgHandle es válido
	if(msgHandle >= lenCANmsgList)
	{
		return CNI_MSG_HANDLE_NOT_VALID;
	}

	if(CANmsgList[msgHandle].mNodeID_ != MY_NODE_ID)
	{
		return CNI_MSG_HANDLE_TX_NOT_ALLOWED;
	}

	i = msgHandle;
	rxMsgServiceID = CANmsgList[msgHandle].mServiceID_;
	rxNodeID = CANmsgList[msgHandle].mNodeID_;
	auxLenPayload = lenPayload;

	while((i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == rxMsgServiceID) && (CANmsgList[i].mNodeID_ == rxNodeID) && (auxLenPayload > 0))
	{
		if(auxLenPayload > MAX_LEN_PAYLOAD_CAN)
		{
			//memcpy(CANmsgList[i].mPayload_, &payload[lenPayload-auxLenPayload], MAX_LEN_PAYLOAD_CAN);
			my_copy_array_of_uint8(CANmsgList[i].mPayload_, &payload[lenPayload-auxLenPayload], MAX_LEN_PAYLOAD_CAN);
			CANmsgList[i].mLenPayload_ = MAX_LEN_PAYLOAD_CAN;
			auxLenPayload-=MAX_LEN_PAYLOAD_CAN;
		}
		else
		{
			//memcpy(CANmsgList[i].mPayload_, &payload[lenPayload-auxLenPayload], auxLenPayload);
			my_copy_array_of_uint8(CANmsgList[i].mPayload_, &payload[lenPayload-auxLenPayload], auxLenPayload);
			CANmsgList[i].mLenPayload_ = auxLenPayload;
			auxLenPayload = 0;
		}
		i++;
	}

	return CNI_OK;
}

CNI_status_t CNI_get_msg_content(uint32_t msgHandle, uint8_t *buffer, uint32_t lenBuffer, uint32_t *lenPayload)
{
	uint32_t i;
	serviceID msgServiceID;
	uint32_t nodeID;

	// Chequear si el msgHandle es válido
	if(msgHandle >= lenCANmsgList)
	{
		return CNI_MSG_HANDLE_NOT_VALID;
	}

	i = msgHandle;
	msgServiceID = CANmsgList[msgHandle].mServiceID_;
	nodeID = CANmsgList[msgHandle].mNodeID_;
	(*lenPayload) = 0;

	while((i < lenCANmsgList) && (CANmsgList[i].mServiceID_ == msgServiceID) && (CANmsgList[i].mNodeID_ == nodeID) && ( (*lenPayload) < lenBuffer) )
	{
		my_copy_array_of_uint8(&buffer[*lenPayload], CANmsgList[i].mPayload_, CANmsgList[i].mLenPayload_);
		(*lenPayload) += CANmsgList[i].mLenPayload_;
		i++;
	}

	return CNI_OK;
}
