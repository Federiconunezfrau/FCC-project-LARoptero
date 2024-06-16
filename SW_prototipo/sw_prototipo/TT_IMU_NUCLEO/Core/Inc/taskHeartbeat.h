/*
 * taskHeartbeat.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TASKHEARTBEAT_H_
#define INC_TASKHEARTBEAT_H_

#include "timeTriggeredTask.h"
#include "gpio_STM32.hpp"

typedef struct
{
	timeTriggeredTask_t super;
	STM32::gpio *mLED_;
} taskHeartbeat_t;

void taskHeartbeat_constructor ( taskHeartbeat_t *, uint32_t, uint32_t, uint32_t, uint32_t, STM32::gpio* );
void taskHeartbeat_destructor  ( taskHeartbeat_t * );
void taskHeartbeat_start       ( taskHeartbeat_t * );
void taskHeartbeat_update      ( taskHeartbeat_t * );

#endif /* INC_TASKHEARTBEAT_H_ */
