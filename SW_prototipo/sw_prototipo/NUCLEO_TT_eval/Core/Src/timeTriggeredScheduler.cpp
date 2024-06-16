/*
 * timeTriggeredScheduler.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "timeTriggeredScheduler.h"
#include "CNI.h"

#define HANDLE_MSG_CNI_SYNC 0

static void timeTriggeredScheduler_tick(timeTriggeredScheduler_t *me, myTimer_t *timer)
{
	me->mTicks_++;
}

void timeTriggeredScheduler_constructor(timeTriggeredScheduler_t *me, myTimer_t *timer)
{
	uint32_t i;

	me->mTimer_ = timer;

	for(i = 0; i < MAX_NUM_TASKS ; i++)
	{
		me->mTaskList_[i] = 0;
	}

	me->mTicks_ = 0;
}

void timeTriggeredScheduler_init(timeTriggeredScheduler_t *me)
{
	HAL_TIM_Base_Stop_IT(me->mTimer_->mHtim_);
	subject_attach((subject_t*)(me->mTimer_), me, (notificationHandle_t)&timeTriggeredScheduler_tick);
}

void timeTriggeredScheduler_start(timeTriggeredScheduler_t *me)
{
	uint32_t i;
	timeTriggeredTask_t *task;

	for(i = 0; i < MAX_NUM_TASKS; i++)
	{
		if( (task = me->mTaskList_[i]) )
		{
			(*(task->mHandlerStart_))(task);
		}
	}

	CNI_start();

	CNI_send_msg(HANDLE_MSG_CNI_SYNC);

	HAL_TIM_Base_Start_IT(me->mTimer_->mHtim_);
}

void timeTriggeredScheduler_stop(timeTriggeredScheduler_t *me)
{
	HAL_TIM_Base_Stop_IT(me->mTimer_->mHtim_);
}

void timeTriggeredScheduler_dispatch(timeTriggeredScheduler_t *me)
{
	uint8_t updateRequired = 0;
	uint32_t i;
	timeTriggeredTask_t *task;

	__disable_irq();
	if(me->mTicks_ > 0)
	{
		me->mTicks_--;
		updateRequired = 1;
	}
	__enable_irq();

	while(updateRequired)
	{
		for(i = 0; i < MAX_NUM_TASKS; i++)
		{
			if((task = me->mTaskList_[i]))
			{
				//if( (--(me->mTaskList_[i]->mDelayTicks_)) == 0 )
				if( (--(task->mDelayTicks_)) == 0 )
				{
					//timeTriggeredTask_update((timeTriggeredTask_t*)(me->mTaskList_[i]));
					(*(task->mTaskHandler_))(task);
					//me->mTaskList_[i]->mDelayTicks_ = me->mTaskList_[i]->mPeriodTicks_;
					task->mDelayTicks_ = task->mPeriodTicks_;
				}
			}
		}

		__disable_irq();

		if(me->mTicks_ > 0)
		{
			me->mTicks_--;
			updateRequired = 1;
		}
		else
		{
			updateRequired = 0;
		}

		__enable_irq();
	}
	__asm__("wfi");
}

TTschStatus_t timeTriggeredScheduler_add_task(timeTriggeredScheduler_t *me, timeTriggeredTask_t *task)
{
	uint32_t i = 0;

	while( (me->mTaskList_[i] ) && (i < MAX_NUM_TASKS) )
	{
		i++;
	}

	if(i == MAX_NUM_TASKS)
	{
		return TTsch_ERROR_MAX_TASKS_REACHED;
	}

	me->mTaskList_[i] = task;

	return TTsch_OK;
}