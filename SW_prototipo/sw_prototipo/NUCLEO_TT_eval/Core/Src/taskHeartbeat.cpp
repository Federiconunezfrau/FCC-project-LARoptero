/*
 * taskHeartbeat.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "taskHeartbeat.h"

void taskHeartbeat_constructor(taskHeartbeat_t *me , uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, gpio *led)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskHeartbeat_update, (taskHandler_t)&taskHeartbeat_start, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);
	me->mLED_ = led;
}

void taskHeartbeat_destructor(taskHeartbeat_t *me)
{
	me->mLED_->write(GPIO_ST::LOW);
	timeTriggeredTask_destructor(&me->super);
}

void taskHeartbeat_start(taskHeartbeat_t *me)
{
	me->mLED_->write(GPIO_ST::LOW);
}

void taskHeartbeat_update(taskHeartbeat_t *me)
{
	me->mLED_->toggle();
}


