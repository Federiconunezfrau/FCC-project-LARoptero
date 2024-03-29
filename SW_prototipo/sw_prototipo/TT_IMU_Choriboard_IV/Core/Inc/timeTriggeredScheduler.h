/*
 * timeTriggeredScheduler.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TIMETRIGGEREDSCHEDULER_H_
#define INC_TIMETRIGGEREDSCHEDULER_H_

#include "timeTriggeredTask.h"
#include "tim.h"

#define MAX_NUM_TASKS 20

#define MAX_DELTA_TIME_MICRO_TICKS 100

typedef enum
{
	TTsch_OK = 0,
	TTsch_ERROR_MAX_TASKS_REACHED
} TTschStatus_t;

typedef struct
{
	TIM_HandleTypeDef *mTimer_;
	volatile uint32_t mTicks_;
	timeTriggeredTask_t *mTaskList_[MAX_NUM_TASKS];

	uint32_t mMacroTick_;
	volatile uint8_t mSyncExecuted_;
} timeTriggeredScheduler_t;

typedef struct
{
	timeTriggeredTask_t super;
	uint32_t mHandleMsg_;
	uint32_t mExpectedTimestamp_;
	uint32_t mDelaySync_;
} taskTimeTriggeredSync_t;

void taskTimeTriggeredSync_constructor ( taskTimeTriggeredSync_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t );
void taskTimeTriggeredSync_destructor  ( taskTimeTriggeredSync_t * );
void taskTimeTriggeredSync_start       ( taskTimeTriggeredSync_t * );
void taskTimeTriggeredSync_update      ( taskTimeTriggeredSync_t * );

void timeTriggeredScheduler_constructor ( TIM_HandleTypeDef *, uint32_t );
void timeTriggeredScheduler_init        ( void );
void timeTriggeredScheduler_start       ( void );
void timeTriggeredScheduler_dispatch    ( void );
TTschStatus_t timeTriggeredScheduler_add_task ( timeTriggeredTask_t* );

#endif /* INC_TIMETRIGGEREDSCHEDULER_H_ */
