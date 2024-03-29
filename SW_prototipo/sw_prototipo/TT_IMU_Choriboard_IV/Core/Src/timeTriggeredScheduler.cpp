/*
 * timeTriggeredScheduler.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "timeTriggeredScheduler.h"
#include "CNI.h"

static timeTriggeredScheduler_t _instance;

static void timeTriggeredScheduler_tick(TIM_HandleTypeDef *htim)
{
	_instance.mTicks_++;
}

void timeTriggeredScheduler_constructor(TIM_HandleTypeDef *timer, uint32_t macroTick)
{
	uint32_t i;

	_instance.mTimer_ = timer;

	for(i = 0; i < MAX_NUM_TASKS ; i++)
	{
		_instance.mTaskList_[i] = 0;
	}

	_instance.mTicks_ = 0;
	_instance.mMacroTick_ = macroTick;
	_instance.mSyncExecuted_ = 0;
}

void timeTriggeredScheduler_init(void)
{
	HAL_TIM_Base_Stop_IT(_instance.mTimer_);
	HAL_TIM_RegisterCallback(_instance.mTimer_, HAL_TIM_PERIOD_ELAPSED_CB_ID, timeTriggeredScheduler_tick);

	_instance.mSyncExecuted_ = 0;
}

void timeTriggeredScheduler_start(void)
{
	HAL_TIM_Base_Start_IT(_instance.mTimer_);
}

void timeTriggeredScheduler_stop(void)
{
	HAL_TIM_Base_Stop_IT(_instance.mTimer_);
}

void timeTriggeredScheduler_dispatch(void)
{
	uint8_t updateRequired = 0;
	uint32_t i;
	timeTriggeredTask_t *task;

	__disable_irq();
	if(_instance.mTicks_ > 0)
	{
		_instance.mTicks_--;
		updateRequired = 1;
#if !IS_MASTER
		if(_instance.mSyncExecuted_ == 1)
		{
			_instance.mSyncExecuted_ = 0;
			__HAL_TIM_SET_AUTORELOAD(_instance.mTimer_, _instance.mMacroTick_);
		}
#endif
	}

	__enable_irq();

	while(updateRequired)
	{
		for(i = 0; i < MAX_NUM_TASKS; i++)
		{
			if((task = _instance.mTaskList_[i]))
			{
				if( (--(task->mDelayTicks_)) == 0 )
				{
					(*(task->mTaskHandler_))(task);
					task->mDelayTicks_ = task->mPeriodTicks_;
				}
			}
		}

		__disable_irq();

		if(_instance.mTicks_ > 0)
		{
			_instance.mTicks_--;
			updateRequired = 1;
#if !IS_MASTER
			if(_instance.mSyncExecuted_ == 1)
			{
				_instance.mSyncExecuted_ = 0;
				__HAL_TIM_SET_AUTORELOAD(_instance.mTimer_, _instance.mMacroTick_);
			}
#endif
		}
		else
		{
			updateRequired = 0;
		}

		__enable_irq();
	}
	__asm__("wfi");
}

TTschStatus_t timeTriggeredScheduler_add_task(timeTriggeredTask_t *task)
{
	uint32_t i = 0;

	while( (_instance.mTaskList_[i] ) && (i < MAX_NUM_TASKS) )
	{
		i++;
	}

	if(i == MAX_NUM_TASKS)
	{
		return TTsch_ERROR_MAX_TASKS_REACHED;
	}

	_instance.mTaskList_[i] = task;

	return TTsch_OK;
}

void taskTimeTriggeredSync_constructor(taskTimeTriggeredSync_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsg, uint32_t expectedTimestamp, uint32_t delaySync)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskTimeTriggeredSync_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);
	me->mHandleMsg_ = handleMsg;
	me->mExpectedTimestamp_ = expectedTimestamp;
	me->mDelaySync_ = delaySync;
}

void taskTimeTriggeredSync_destructor(taskTimeTriggeredSync_t *me)
{
	timeTriggeredTask_destructor(&me->super);
}

void taskTimeTriggeredSync_start(taskTimeTriggeredSync_t *me)
{
	// Por ahora no hace nada
}

void taskTimeTriggeredSync_update(taskTimeTriggeredSync_t *me)
{
#if !IS_MASTER
	uint32_t timestamp;
	uint32_t deltaTime;

	// Espero a que me llegue el mensaje de sincronización
	if( CNI_receive_msg(me->mHandleMsg_) == CNI_OK )
	{
		// Tomo un timestamp del mensaje recibido
		timestamp = __HAL_TIM_GET_COUNTER(_instance.mTimer_);

		// Comparo ese valor con el valor esperado
		if(timestamp > me->mExpectedTimestamp_)
		{
			deltaTime = timestamp - me->mExpectedTimestamp_;
			if(deltaTime > MAX_DELTA_TIME_MICRO_TICKS)
			{
				deltaTime = MAX_DELTA_TIME_MICRO_TICKS;
			}
			// Actualizo el timer
			__HAL_TIM_SET_AUTORELOAD(_instance.mTimer_, _instance.mMacroTick_ + deltaTime);
		}
		else
		{
			deltaTime = me->mExpectedTimestamp_ - timestamp;
			if(deltaTime > MAX_DELTA_TIME_MICRO_TICKS)
			{
				deltaTime = MAX_DELTA_TIME_MICRO_TICKS;
			}
			// Actualizo el timer
			__HAL_TIM_SET_AUTORELOAD(_instance.mTimer_, _instance.mMacroTick_ - deltaTime);
		}

		// Le aviso al scheduler que en el próximo tick, vuelva a dejar el timer como estaba antes
		_instance.mSyncExecuted_ = 1;
	}
#else
	// Espero un rato
	while(__HAL_TIM_GET_COUNTER(_instance.mTimer_) < me->mDelaySync_);
	// Envío el mensaje de sync
	CNI_send_msg(me->mHandleMsg_);
#endif
}
