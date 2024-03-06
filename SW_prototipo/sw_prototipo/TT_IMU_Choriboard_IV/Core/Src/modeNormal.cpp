/*
 * modeNormal.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: fede
 */

#include "modeNormal.h"

#include "taskHeartbeat.h"
#include "taskWatchdog.h"
#include "taskIMUgetData.h"
#include "taskCNIsendData.h"
#include "taskEstimateAttitude.h"

#include "timeTriggeredScheduler.h"

#include "main.h"

#include "CNI.h"

#include "attitudeEstimator.h"

#define HANDLE_MSG_CNI_SYNC 0
#define HANDLE_MSG_CNI_SEND_IMU_DATA MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE
#define HANDLE_MSG_CNI_SEND_ATTITUDE_DATA MSG_CNI_SEND_ATTITUDE_DATA_POS_IN_TABLE

#define ICM42688_CS_GPIO_Port IMU_CS_GPIO_Port
#define ICM42688_CS_Pin       IMU_CS_Pin

#define LED_HEARTBEAT_GPIO_Port LED2_GPIO_Port
#define LED_HEARTBEAT_GPIO_Pin  LED2_Pin

#define ALPHA_ATTITUDE_ESTIMATOR 0.025
#define DELTA_T_ATTITUDE_ESTIMATOR_S 0.1

static bool run = false;

void normal_mode_run(void)
{
	taskHeartbeat_t  taskHeartbeat;
	taskWatchdog_t   taskWatchdog;
	taskTimeTriggeredSync_t taskPeriodicSync;

	taskIMUgetData_t  taskIMUgetData;
	taskCNIsendData_t taskCNIsendIMUdata;

	taskEstimateAttitude_t taskEstimateAttitude;
	taskCNIsendData_t      taskCNIsendAttitudeData;


	// Inicialización de la IMU =====================================
	gpio imuCS(ICM42688_CS_GPIO_Port, ICM42688_CS_Pin);

	phandler_spi<> spi2(&hspi2, &imuCS, COMM_MODE::BLCK);

	ICM42688::config_FIFO icmFifoCnf =
	{
		  .watermark = 1,
		  .mode = ICM42688::FIFO_MODE::BYPASS,
	};

	ICM42688::config_INT icmIntCnf =
	{
			.int1Source = ICM42688::INT_SOURCE::DISABLED,
			.int1Mode = ICM42688::INT_MODE::DISABLED,
			//.int1Source = ICM42688::INT_SOURCE::DATA_RDY,
			//.int1Mode = ICM42688::INT_MODE::PULSED_PP_HIGH,
			.int2Source = ICM42688::INT_SOURCE::DISABLED,
			.int2Mode = ICM42688::INT_MODE::DISABLED,
	};

	ICM42688::Config icmCnf =
	{
		.gyroFSR 					= ICM42688::GYRO_CNF::FSR::FSR_250,
		.gyroODR 					= ICM42688::GYRO_CNF::ODR::ODR_1000,
		.accelFSR 					= ICM42688::ACCEL_CNF::FSR::FSR_4,
		.accelODR 					= ICM42688::ACCEL_CNF::ODR::ODR_1000,
		.fifoCnf					= icmFifoCnf,
		.intCnf						= icmIntCnf,
	};

	ICM42688::icm42688 imu(icmCnf, &spi2, &imuCS);

	// Inicialización del LED heartbeat==============================
	STM32::gpio ledHeartbeat(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_GPIO_Pin);

	// Inicialización de la CNI ==============================
	CNI_constructor(&hcan1);

	// Inicialización del attitude estimator ==============================
	attitudeEstimator_constructor(ALPHA_ATTITUDE_ESTIMATOR, DELTA_T_ATTITUDE_ESTIMATOR_S);

	// Creación de las tareas========================================
	taskWatchdog_constructor(&taskWatchdog,
			DELAY_TASK_WATCHDOG_TICKS_NORMAL,
			PERIOD_TASK_WATCHDOG_TICKS_NORMAL,
			WCET_TASK_WATCHDOG_US,
			BCET_TASK_WATCHDOG_US,
			&hiwdg);

	taskHeartbeat_constructor(&taskHeartbeat,
			DELAY_TASK_HEARTBEAT_TICKS_NORMAL,
			PERIOD_TASK_HEARTBEAT_TICKS_NORMAL,
			WCET_TASK_HEARTBEAT_US,
			BCET_TASK_HEARTBEAT_US,
			&ledHeartbeat);

	taskIMUgetData_constructor(&taskIMUgetData,
			DELAY_TASK_IMU_TICKS_NORMAL,
			PERIOD_TASK_IMU_TICKS_NORMAL,
			WCET_TASK_IMU_US,
			BCET_TASK_IMU_US,
			&imu, HANDLE_MSG_CNI_SEND_IMU_DATA);

	taskCNIsendData_constructor(&taskCNIsendIMUdata,
			DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL,
			PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL,
			WCET_TASK_CNI_SEND_IMU_US,
			BCET_TASK_CNI_SEND_IMU_US,
			HANDLE_MSG_CNI_SEND_IMU_DATA);

	taskEstimateAttitude_constructor(&taskEstimateAttitude,
			DELAY_TASK_ESTIMATE_ATTITUDE_TICKS_NORMAL,
			PERIOD_TASK_ESTIMATE_ATTITUDE_TICKS_NORMAL,
			WCET_TASK_ESTIMATE_ATTITUDE_US,
			BCET_TASK_ESTIMATE_ATTITUDE_US,
			HANDLE_MSG_CNI_SEND_ATTITUDE_DATA);

	taskCNIsendData_constructor(&taskCNIsendAttitudeData,
			DELAY_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL,
			PERIOD_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL,
			WCET_TASK_CNI_SEND_ATTITUDE_US,
			BCET_TASK_CNI_SEND_ATTITUDE_US,
			HANDLE_MSG_CNI_SEND_ATTITUDE_DATA);

	taskTimeTriggeredSync_constructor(&taskPeriodicSync,
			DELAY_TASK_SYNC_TICKS_NORMAL,
			PERIOD_TASK_SYNC_TICKS_NORMAL,
			WCET_TASK_SYNC_US,
			BCET_TASK_SYNC_US,
			HANDLE_MSG_CNI_SYNC,
			EXPECTED_SYNC_TIMESTAMP_TICKS,
			DELAY_SYNC_TICKS);

	// Inicialización del scheduler =================================
	timeTriggeredScheduler_constructor(&htim5, MICROTICKS_IN_MACROTICK);
	timeTriggeredScheduler_init();

	CNI_init();

	taskHeartbeat_start(&taskHeartbeat);
	taskIMUgetData_start(&taskIMUgetData);
	taskCNIsendData_start(&taskCNIsendIMUdata);

	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskHeartbeat);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskWatchdog);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskPeriodicSync);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskIMUgetData);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIsendIMUdata);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskEstimateAttitude);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIsendAttitudeData);

	CNI_start();

	// Se queda acá esperando hasta que uno presione el botón para comenzar
	while(!run)
	{
		//__asm__("wfi");
	}

#if IS_MASTER
	CNI_send_msg(HANDLE_MSG_CNI_SYNC);
#endif
	taskWatchdog_start(&taskWatchdog);

	timeTriggeredScheduler_start();

	while(1)
	{
		timeTriggeredScheduler_dispatch();
	}
}

#if IS_MASTER
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == USER_Btn_Pin)
    {
    	run = true;
    }
}
#else
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
	run = true;
}

#endif
