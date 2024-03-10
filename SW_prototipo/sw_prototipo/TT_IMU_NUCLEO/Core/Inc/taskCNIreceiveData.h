/*
 * taskCNIreceiveData.h
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#ifndef INC_TASKCNIRECEIVEDATA_H_
#define INC_TASKCNIRECEIVEDATA_H_

#include "timeTriggeredTask.h"

typedef struct
{
	timeTriggeredTask_t super;
	uint32_t mHandleMsg_;
} taskCNIreceiveData_t;

void taskCNIreceiveData_constructor ( taskCNIreceiveData_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t );
void taskCNIreceiveData_destructor  ( taskCNIreceiveData_t * );
void taskCNIreceiveData_start       ( taskCNIreceiveData_t * );
void taskCNIreceiveData_update      ( taskCNIreceiveData_t * );



#endif /* INC_TASKCNIRECEIVEDATA_H_ */
