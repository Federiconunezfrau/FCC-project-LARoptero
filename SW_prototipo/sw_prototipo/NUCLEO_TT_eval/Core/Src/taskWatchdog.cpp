/*
 * taskWatchdog.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: fede
 */

#include "taskWatchdog.h"

void taskWatchdog_constructor(taskWatchdog_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, IWDG_HandleTypeDef *hiwdg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskWatchdog_update, (taskHandler_t)&taskWatchdog_start, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);
	me->mHiwdg_ = hiwdg;
}

void taskWatchdog_destructor(taskWatchdog_t *me)
{
	timeTriggeredTask_destructor(&me->super);
}

void taskWatchdog_start(taskWatchdog_t *me)
{
	HAL_IWDG_Init(me->mHiwdg_);
}

void taskWatchdog_update(taskWatchdog_t *me)
{
	HAL_IWDG_Refresh(me->mHiwdg_);
}



