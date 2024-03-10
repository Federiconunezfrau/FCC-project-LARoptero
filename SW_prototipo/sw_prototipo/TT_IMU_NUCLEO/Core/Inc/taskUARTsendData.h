/*
 * taskUARTsendData.h
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#ifndef INC_TASKUARTSENDDATA_H_
#define INC_TASKUARTSENDDATA_H_

#include "timeTriggeredTask.h"

typedef struct
{
	timeTriggeredTask_t super;
	uint32_t mHandleMsg_;
	uint8_t mCmd_;
} taskUARTsendData_t;

void taskUARTsendData_constructor ( taskUARTsendData_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t );
void taskUARTsendData_destructor  ( taskUARTsendData_t * );
void taskUARTsendData_start       ( taskUARTsendData_t * );
void taskUARTsendData_update      ( taskUARTsendData_t * );



#endif /* INC_TASKUARTSENDDATA_H_ */
