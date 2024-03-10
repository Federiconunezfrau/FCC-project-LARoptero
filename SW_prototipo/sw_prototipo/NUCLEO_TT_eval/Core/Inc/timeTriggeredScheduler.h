/*
 * timeTriggeredScheduler.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TIMETRIGGEREDSCHEDULER_H_
#define INC_TIMETRIGGEREDSCHEDULER_H_

#include "myTimer.h"
#include "timeTriggeredTask.h"
#include "timeTriggeredSchedulerSettings.h"

#define MAX_NUM_TASKS 10

typedef enum
{
	TTsch_OK = 0,
	TTsch_ERROR_MAX_TASKS_REACHED
} TTschStatus_t;

typedef struct
{
	myTimer_t *mTimer_;
	volatile uint32_t mTicks_;
	timeTriggeredTask_t *mTaskList_[MAX_NUM_TASKS];
} timeTriggeredScheduler_t;

void timeTriggeredScheduler_constructor ( timeTriggeredScheduler_t*, myTimer_t* );
void timeTriggeredScheduler_init        ( timeTriggeredScheduler_t* );
void timeTriggeredScheduler_start       ( timeTriggeredScheduler_t* );
void timeTriggeredScheduler_dispatch    ( timeTriggeredScheduler_t* );
TTschStatus_t timeTriggeredScheduler_add_task    ( timeTriggeredScheduler_t*, timeTriggeredTask_t* );

#endif /* INC_TIMETRIGGEREDSCHEDULER_H_ */
