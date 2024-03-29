/*
 * system.h
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stateMachine.h"
#include "timeTriggeredScheduler.h"
#include "myUART.h"
#include "utils.hpp"
#include "PCIFmsg.h"
#include "port.hpp"

#define SIZE_BUFFER_MSG_BUFFER 8

typedef struct
{
	stateMachine_t super;
	evBuffer *mEventQueue_;

	timeTriggeredScheduler_t  *mTTscheduler_;
	myUART_t *mMyUART_;
	gpio *mLEDFailSilent_;
	circ_buffer<PCIFmsg_t, SIZE_BUFFER_MSG_BUFFER> mTxBuffer_;
} system_t;

void    system_constructor    ( system_t*, evBuffer*, timeTriggeredScheduler_t* ,myUART_t*,gpio *);
state_t system_initial        ( system_t*, event_t const );
state_t system_idle           ( system_t*, event_t const );
state_t system_fail_safe_mode ( system_t*, event_t const );
state_t system_normal_mode    ( system_t*, event_t const );

uint16_t system_has_new_msg    ( system_t *);
//void    system_update_uart    ( system *, uint8_t*, uint32_t );

#endif /* INC_SYSTEM_H_ */
