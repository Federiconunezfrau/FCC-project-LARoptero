/*
 * taskWatchdog.h
 *
 *  Created on: Feb 11, 2024
 *      Author: fede
 */

#ifndef INC_TASKWATCHDOG_H_
#define INC_TASKWATCHDOG_H_

#include "timeTriggeredTask.h"
#include "stm32f7xx_hal.h"

typedef struct
{
	timeTriggeredTask_t super;
	IWDG_HandleTypeDef *mHiwdg_;
} taskWatchdog_t;

void taskWatchdog_constructor ( taskWatchdog_t *, uint32_t, uint32_t, uint32_t, uint32_t, IWDG_HandleTypeDef*);
void taskWatchdog_destructor  ( taskWatchdog_t * );
void taskWatchdog_start       ( taskWatchdog_t * );
void taskWatchdog_update      ( taskWatchdog_t * );

#endif /* INC_TASKWATCHDOG_H_ */