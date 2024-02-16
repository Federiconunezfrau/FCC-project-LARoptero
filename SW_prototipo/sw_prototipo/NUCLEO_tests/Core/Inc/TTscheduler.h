/*
 * TTscheduler.h
 *
 *  Created on: Jan 31, 2024
 *      Author: fede
 */

#ifndef INC_TTSCHEDULER_H_
#define INC_TTSCHEDULER_H_

#include "interface_TTtask.h"
#include "main.h"

/** The max number of tasks to me handled by the scheduler */
#define MAX_NUMBER_TASKS 10

class scheduler
{
	public:
		scheduler(TIM_HandleTypeDef*);

		void init(void);
		void start(void);

		void dispatch(void);
		void add_task_cooperative(interface_TTtask *task);

	private:
		TIM_HandleTypeDef *mTimer_;
		interface_TTtask *mTaskList_[MAX_NUMBER_TASKS];
};

#endif /* INC_TTSCHEDULER_H_ */
