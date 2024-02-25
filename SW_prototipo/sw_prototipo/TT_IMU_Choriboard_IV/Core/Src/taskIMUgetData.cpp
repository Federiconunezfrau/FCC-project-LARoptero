/*
 * taskIMUgetData.cpp
 *
 *  Created on: Feb 12, 2024
 *      Author: fede
 */

#include <taskIMUgetData.h>
#include "CNI.h"


#define LEN_IMU_CNI_DATA 24

static void make_imu_data_for_cni(IMUData imuData, uint8_t *dataForCni);

void taskIMUgetData_constructor(taskIMUgetData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, ICM42688::icm42688 *imu, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskIMUgetData_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mIMU_ = imu;
	me->mHandleMsg_ = handleMsg;
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
	uint8_t dataForCni[LEN_IMU_CNI_DATA];

	me->mIMU_->save_data();
	me->mIMU_->read_data(&(me->mIMUdata_));

	// Convertir los datos de la IMU en bytes
	make_imu_data_for_cni(me->mIMUdata_, dataForCni);

	// Actualizarlos en la CNI
	CNI_update_msg_content(me->mHandleMsg_, dataForCni, LEN_IMU_CNI_DATA);
}

static void make_imu_data_for_cni(IMUData imuData, uint8_t *dataForCni)
{
	memcpy(dataForCni     , &imuData.accelX, 4);
	memcpy(&dataForCni[4] , &imuData.accelY, 4);
	memcpy(&dataForCni[8] , &imuData.accelZ, 4);
	memcpy(&dataForCni[12], &imuData.gyroX , 4);
	memcpy(&dataForCni[16], &imuData.gyroY , 4);
	memcpy(&dataForCni[20], &imuData.gyroZ , 4);
}
