/*
 * timeTriggeredTask.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "timeTriggeredTask.h"

void timeTriggeredTask_constructor(timeTriggeredTask_t *me, taskHandler_t taskHandler, taskHandler_t handlerStart, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds)
{
	me->mTaskHandler_ = taskHandler;
	me->mHandlerStart_ = handlerStart;

	me->mDelayTicks_ = delayTicks + 1;
	me->mPeriodTicks_ = periodTicks;
	me->mWcetMicroSeconds_ = wcetMicroSeconds;
	me->mBcetMicroSeconds_ = bcetMicroSeconds;
}

void timeTriggeredTask_destructor(timeTriggeredTask_t *me)
{
	me->mDelayTicks_ = 0;
	me->mPeriodTicks_ = 0;
	me->mWcetMicroSeconds_ = 0;
	me->mBcetMicroSeconds_ = 0;
}

/*void timeTriggeredTask_update(timeTriggeredTask_t *me)
{
	(*(me->mTaskHandler_))(me);
}*/
