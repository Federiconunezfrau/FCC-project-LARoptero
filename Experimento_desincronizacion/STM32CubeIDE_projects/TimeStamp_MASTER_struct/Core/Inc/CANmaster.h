/*
 * task_CANsender.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_CANMASTER_H_
#define INC_CANMASTER_H_

#include "stm32f7xx_hal.h"
#include "stateMachine.h"
#include "gpio_STM32.hpp"

typedef struct
{
	stateMachine_t super;
	CAN_HandleTypeDef *m_canHandler;
	CAN_TxHeaderTypeDef m_canTxHeader;
	//STM32::gpio *m_ledCanMsgReceived;
	//STM32::gpio *m_ledMsgRxNodeID1;
	//STM32::gpio *m_ledMsgRxNodeID2;
	//STM32::gpio *m_ledMsgRxNodeID3;
	uint8_t *m_flagNewCANdata;
	uint32_t m_nodeID;
	bool m_snifferOn;
} CANmaster_t;


void CANmaster_constructor(CANmaster_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID);
//void CANmaster_constructor(CANmaster_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID, STM32::gpio *ledMsgRxNodeID2, STM32::gpio *ledMsgRxNodeID3);
//void CANmaster_constructor(CANmaster_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID, STM32::gpio *ledMsgRxNodeID1, STM32::gpio *ledMsgRxNodeID2, STM32::gpio *ledMsgRxNodeID3);
state_t CANmaster_initial(CANmaster_t *me);
state_t CANmaster_off(CANmaster_t *me, event_t const e);
state_t CANmaster_on(CANmaster_t *me, event_t const e);


#endif /* INC_CANMASTER_H_ */
