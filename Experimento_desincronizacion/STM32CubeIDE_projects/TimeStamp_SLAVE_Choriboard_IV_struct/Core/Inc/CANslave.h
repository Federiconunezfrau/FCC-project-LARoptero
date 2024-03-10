/*
 * task_CANsender.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_CANSLAVE_H_
#define INC_CANSLAVE_H_

#include "stm32f7xx_hal.h"
#include "localClock.h"
#include "stateMachine.h"

typedef struct
{
	stateMachine_t super;
	CAN_HandleTypeDef *m_canHandler;
	CAN_TxHeaderTypeDef m_canTxHeader;
	uint32_t m_nodeID;
} CANslave_t;


void CANslave_constructor(CANslave_t *me, CAN_HandleTypeDef *canHandler, uint32_t nodeID, localClock_t *localClock);
state_t CANslave_initial(CANslave_t *me);
state_t CANslave_off(CANslave_t *me, event_t const e);
state_t CANslave_on(CANslave_t *me, event_t const e);


#endif /* INC_CANSLAVE_H_ */
