/*
 * taskCompareAttitude.h
 *
 *  Created on: Mar 6, 2024
 *      Author: fede
 */

#ifndef INC_TASKCOMPAREATTITUDE_H_
#define INC_TASKCOMPAREATTITUDE_H_

#include "timeTriggeredTask.h"

typedef struct
{
	timeTriggeredTask_t super;
	uint32_t mHandleMsgAttitude1_;
	uint32_t mHandleMsgAttitude2_;
	uint32_t mHandleMsgAttitude3_;
	uint32_t mHandleMsg_;
} taskCompareAttitude_t;

void taskCompareAttitude_constructor ( taskCompareAttitude_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t );
void taskCompareAttitude_destructor  ( taskCompareAttitude_t * );
void taskCompareAttitude_start       ( taskCompareAttitude_t * );
void taskCompareAttitude_update      ( taskCompareAttitude_t * );



#endif /* INC_TASKCOMPAREATTITUDE_H_ */