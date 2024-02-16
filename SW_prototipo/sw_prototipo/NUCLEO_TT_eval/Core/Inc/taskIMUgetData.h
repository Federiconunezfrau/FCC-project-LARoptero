/*
 * taskIMUgetData.h
 *
 *  Created on: Feb 12, 2024
 *      Author: fede
 */

#ifndef INC_TASKIMUGETDATA_H_
#define INC_TASKIMUGETDATA_H_

#include "timeTriggeredTask.h"
#include "icm42688.hpp"

typedef struct
{
	timeTriggeredTask_t super;
	ICM42688::icm42688 *mIMU_;
	IMUData mIMUdata_;
} taskIMUgetData_t;

void taskIMUgetData_constructor ( taskIMUgetData_t *, uint32_t, uint32_t, uint32_t, uint32_t, ICM42688::icm42688* );
void taskIMUgetData_destructor  ( taskIMUgetData_t * );
void taskIMUgetData_start       ( taskIMUgetData_t * );
void taskIMUgetData_update      ( taskIMUgetData_t * );


#endif /* INC_TASKIMUGETDATA_H_ */
