/*
 * taskIMUgetData.cpp
 *
 *  Created on: Feb 12, 2024
 *      Author: fede
 */

#include "taskIMUgetData.h"

void taskIMUgetData_constructor(taskIMUgetData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, ICM42688::icm42688 *imu)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskIMUgetData_update, (taskHandler_t)&taskIMUgetData_start, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mIMU_ = imu;
}

void taskIMUgetData_destructor(taskIMUgetData_t *me)
{
	//TODO: hay que definir un criterio para desinicializar la IMU
	me->mIMU_->disable();

	timeTriggeredTask_destructor(&me->super);
}

void taskIMUgetData_start(taskIMUgetData_t *me)
{
	// Por ahora no hace nada
}

void taskIMUgetData_update(taskIMUgetData_t *me)
{
	me->mIMU_->save_data();
	me->mIMU_->read_data(&(me->mIMUdata_));
}


