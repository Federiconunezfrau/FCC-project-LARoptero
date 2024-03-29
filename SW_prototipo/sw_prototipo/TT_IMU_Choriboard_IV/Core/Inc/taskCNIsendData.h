/*
 * taskCNIsendData.h
 *
 *  Created on: Feb 15, 2024
 *      Author: fede
 */

#ifndef INC_TASKCNISENDDATA_H_
#define INC_TASKCNISENDDATA_H_

#include "timeTriggeredTask.h"

typedef struct
{
	timeTriggeredTask_t super;
	uint32_t mHandleMsg_;
} taskCNIsendData_t;

void taskCNIsendData_constructor ( taskCNIsendData_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t );
void taskCNIsendData_destructor  ( taskCNIsendData_t * );
void taskCNIsendData_start       ( taskCNIsendData_t * );
void taskCNIsendData_update      ( taskCNIsendData_t * );

#endif /* INC_TASKCNISENDDATA_H_ */
