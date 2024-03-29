/*
 * timeTriggeredTask.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TIMETRIGGEREDTASK_H_
#define INC_TIMETRIGGEREDTASK_H_

#include <stdint.h>

typedef void (*taskHandler_t) (void*);

typedef struct
{
	taskHandler_t mTaskHandler_;

	uint32_t mDelayTicks_;
	uint32_t mPeriodTicks_;
	uint32_t mWcetMicroSeconds_;
	uint32_t mBcetMicroSeconds_;
} timeTriggeredTask_t;

void timeTriggeredTask_constructor ( timeTriggeredTask_t *, taskHandler_t, uint32_t, uint32_t, uint32_t, uint32_t );
void timeTriggeredTask_destructor  ( timeTriggeredTask_t * );

#endif /* INC_TIMETRIGGEREDTASK_H_ */
