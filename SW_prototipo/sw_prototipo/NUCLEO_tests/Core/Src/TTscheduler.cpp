/*
 * TTscheduler.cpp
 *
 *  Created on: Jan 31, 2024
 *      Author: fede
 */

#include "TTscheduler.h"

scheduler::scheduler(TIM_HandleTypeDef *timer)
{
	uint32_t i;

	mTimer_ = timer;

	for(i = 0; i < MAX_NUMBER_TASKS ; i++)
	{
		mTaskList_[i] = 0;
	}
}

void scheduler::init(void)
{
	// Esta implementació no hace nada
}

void scheduler::start(void)
{
	// Se habilita la interrupción de timer
	// Comienza a correr el timer
	HAL_TIM_Base_Start_IT(mTimer_);
}

void scheduler::add_task_cooperative(interface_TTtask *task)
{

}

