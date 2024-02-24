/*
 * taskHeartbeat.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TASKHEARTBEAT_H_
#define INC_TASKHEARTBEAT_H_

#include "timeTriggeredTask.h"
#include "port.hpp"

typedef struct
{
	timeTriggeredTask_t super;
	gpio *mLED_;
} taskHeartbeat_t;

void taskHeartbeat_constructor ( taskHeartbeat_t *, uint32_t, uint32_t, uint32_t, uint32_t, gpio *);
void taskHeartbeat_destructor  ( taskHeartbeat_t * );
void taskHeartbeat_start       ( taskHeartbeat_t * );
void taskHeartbeat_update      ( taskHeartbeat_t * );

#endif /* INC_TASKHEARTBEAT_H_ */
