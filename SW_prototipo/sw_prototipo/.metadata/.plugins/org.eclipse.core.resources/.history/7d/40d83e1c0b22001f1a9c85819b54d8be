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
#include "taskCNIreceiveData.h"
#include "taskEstimateAttitude.h"
#include "taskCompareAttitude.h"

#include "timeTriggeredScheduler.h"

#include "main.h"

#include "CNI.h"

#include "attitudeEstimator.h"

#define HANDLE_MSG_CNI_SYNC 0
#define HANDLE_MSG_CNI_SEND_IMU_DATA MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE
#define HANDLE_MSG_CNI_SEND_ATTITUDE_DATA MSG_CNI_SEND_ATTITUDE_DATA_POS_IN_TABLE
#define HANDLE_MSG_CNI_ATTITUDE_1 10
#define HANDLE_MSG_CNI_ATTITUDE_2 11
#define HANDLE_MSG_CNI_ATTITUDE_3 12
#define HANDLE_MSG_CNI_COMPARE_ATTITUDE MSG_CNI_SEND_COMPARE_ATTITUDE_DATA_POS_IN_TABLE

#define ICM42688_CS_GPIO_PORT IMU_CS_GPIO_Port
#define ICM42688_CS_PIN       IMU_CS_Pin

#define LED_IMU_CALIBRATION_GPIO_PORT LED5_GPIO_Port
#define LED_IMU_CALIBRATION_GPIO_PIN  LED5_Pin

#define LED_HEARTBEAT_GPIO_PORT LED2_GPIO_Port
#define LED_HEARTBEAT_GPIO_PIN  LED2_Pin

#define ALPHA_ATTITUDE_ESTIMATOR 0.025
#define DELTA_T_ATTITUDE_ESTIMATOR_S 0.01

#define TIMEOUT_RX_MSG_ATTITUDE 0

static bool run = false;

void normal_mode_run(void)
{
	taskHeartbeat_t         taskHeartbeat;
	taskWatchdog_t          taskWatchdog;
	taskTimeTriggeredSync_t taskPeriodicSync;
	taskIMUgetData_t        taskIMUgetData;
//	taskCNIsendData_t       taskCNIsendIMUdata;
	taskEstimateAttitude_t  taskEstimateAttitude;
	taskCNIsendData_t       taskCNIsendAttitudeData;
	taskCompareAttitude_t   taskCompareAttitudeData;
	taskCNIsendData_t       taskCNIsendCompareAttitudeData;

#if SETTINGS_NODE_ID!=1
	taskCNIreceiveData_t   taskCNIreceiveAttitudeData1;
	//taskCNIreceiveData_t   taskCNIreceiveCompareAttitudeData1;
#endif

#if SETTINGS_NODE_ID!=2
	taskCNIreceiveData_t   taskCNIreceiveAttitudeData2;
	//taskCNIreceiveData_t   taskCNIreceiveCompareAttitudeData2;
#endif

#if SETTINGS_NODE_ID!=3
	taskCNIreceiveData_t   taskCNIreceiveAttitudeData3;
	//taskCNIreceiveData_t   taskCNIreceiveCompareAttitudeData3;
#endif

	// Inicialización de la IMU =====================================
	gpio imuCS(ICM42688_CS_GPIO_PORT, ICM42688_CS_PIN);

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

	// Inicialización del LED para calibración de la IMU==============
	STM32::gpio ledIMUcalibration(LED_IMU_CALIBRATION_GPIO_PORT, LED_IMU_CALIBRATION_GPIO_PIN);

	// Inicialización del LED heartbeat==============================
	STM32::gpio ledHeartbeat(LED_HEARTBEAT_GPIO_PORT, LED_HEARTBEAT_GPIO_PIN);

	// Inicialización de la CNI ==============================
	CNI_constructor(&hcan1);

	// Inicialización del attitude estimator ==============================
	attitudeEstimator_constructor(ALPHA_ATTITUDE_ESTIMATOR, DELTA_T_ATTITUDE_ESTIMATOR_S);

	// Creación de las tareas========================================
	taskWatchdog_constructor(&taskWatchdog, DELAY_TASK_WATCHDOG_TICKS_NORMAL, PERIOD_TASK_WATCHDOG_TICKS_NORMAL, WCET_TASK_WATCHDOG_US, BCET_TASK_WATCHDOG_US,
			&hiwdg);

	taskHeartbeat_constructor(&taskHeartbeat, DELAY_TASK_HEARTBEAT_TICKS_NORMAL, PERIOD_TASK_HEARTBEAT_TICKS_NORMAL, WCET_TASK_HEARTBEAT_US, BCET_TASK_HEARTBEAT_US,
			&ledHeartbeat);
// ==================== Datos IMU raw ====================
	taskIMUgetData_constructor(&taskIMUgetData, DELAY_TASK_IMU_TICKS_NORMAL, PERIOD_TASK_IMU_TICKS_NORMAL, WCET_TASK_IMU_US, BCET_TASK_IMU_US,
			&imu, &ledIMUcalibration, HANDLE_MSG_CNI_SEND_IMU_DATA);

//	taskCNIsendData_constructor(&taskCNIsendIMUdata, DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL, PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL, WCET_TASK_CNI_SEND_IMU_US, BCET_TASK_CNI_SEND_IMU_US,
//			HANDLE_MSG_CNI_SEND_IMU_DATA);
// =======================================================
// =================== Datos attitude  ===================
	taskEstimateAttitude_constructor(&taskEstimateAttitude, DELAY_TASK_ESTIMATE_ATTITUDE_TICKS_NORMAL, PERIOD_TASK_ESTIMATE_ATTITUDE_TICKS_NORMAL, WCET_TASK_ESTIMATE_ATTITUDE_US, BCET_TASK_ESTIMATE_ATTITUDE_US,
			HANDLE_MSG_CNI_SEND_ATTITUDE_DATA);

	taskCNIsendData_constructor(&taskCNIsendAttitudeData, DELAY_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL, PERIOD_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL, WCET_TASK_CNI_SEND_ATTITUDE_US, BCET_TASK_CNI_SEND_ATTITUDE_US,
			HANDLE_MSG_CNI_SEND_ATTITUDE_DATA);
#if SETTINGS_NODE_ID!=1
	taskCNIreceiveData_constructor(&taskCNIreceiveAttitudeData1, DELAY_TASK_CNI_RECEIVE_ATTITUDE_1_TICKS_NORMAL, PERIOD_TASK_CNI_RECEIVE_ATTITUDE_1_TICKS_NORMAL, WCET_TASK_CNI_RECEIVE_ATTITUDE_1_US, BCET_TASK_CNI_RECEIVE_ATTITUDE_1_US,
			HANDLE_MSG_CNI_ATTITUDE_1, TIMEOUT_RX_MSG_ATTITUDE);
#endif
#if SETTINGS_NODE_ID!=2
	taskCNIreceiveData_constructor(&taskCNIreceiveAttitudeData2, DELAY_TASK_CNI_RECEIVE_ATTITUDE_2_TICKS_NORMAL, PERIOD_TASK_CNI_RECEIVE_ATTITUDE_2_TICKS_NORMAL, WCET_TASK_CNI_RECEIVE_ATTITUDE_2_US, BCET_TASK_CNI_RECEIVE_ATTITUDE_2_US,
			HANDLE_MSG_CNI_ATTITUDE_2, TIMEOUT_RX_MSG_ATTITUDE);
#endif
#if SETTINGS_NODE_ID!=3
	taskCNIreceiveData_constructor(&taskCNIreceiveAttitudeData3, DELAY_TASK_CNI_RECEIVE_ATTITUDE_3_TICKS_NORMAL, PERIOD_TASK_CNI_RECEIVE_ATTITUDE_3_TICKS_NORMAL, WCET_TASK_CNI_RECEIVE_ATTITUDE_3_US, BCET_TASK_CNI_RECEIVE_ATTITUDE_3_US,
			HANDLE_MSG_CNI_ATTITUDE_3, TIMEOUT_RX_MSG_ATTITUDE);
#endif
// =======================================================
//================== Comparacion attitude ================
	taskCompareAttitude_constructor(&taskCompareAttitudeData, DELAY_TASK_COMPARE_ATTITUDE_TICKS_NORMAL, PERIOD_TASK_COMPARE_ATTITUDE_TICKS_NORMAL, WCET_TASK_COMPARE_ATTITUDE_US, BCET_TASK_COMPARE_ATTITUDE_US,
			HANDLE_MSG_CNI_ATTITUDE_1, HANDLE_MSG_CNI_ATTITUDE_2, HANDLE_MSG_CNI_ATTITUDE_3, HANDLE_MSG_CNI_COMPARE_ATTITUDE);

	taskCNIsendData_constructor(&taskCNIsendCompareAttitudeData, DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL, PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL, WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US, BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US,
			HANDLE_MSG_CNI_COMPARE_ATTITUDE);
// =======================================================
//================== Syncronización ================
	taskTimeTriggeredSync_constructor(&taskPeriodicSync, DELAY_TASK_SYNC_TICKS_NORMAL, PERIOD_TASK_SYNC_TICKS_NORMAL, WCET_TASK_SYNC_US, BCET_TASK_SYNC_US,
			HANDLE_MSG_CNI_SYNC, EXPECTED_SYNC_TIMESTAMP_TICKS, DELAY_SYNC_TICKS);
// =======================================================

	// Inicialización del scheduler =================================
	timeTriggeredScheduler_constructor(&htim5, MICROTICKS_IN_MACROTICK);
	timeTriggeredScheduler_init();

	CNI_init();

	taskHeartbeat_start(&taskHeartbeat);
	taskIMUgetData_start(&taskIMUgetData);
//	taskCNIsendData_start(&taskCNIsendIMUdata);

	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskWatchdog);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskIMUgetData);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskHeartbeat);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskPeriodicSync);
//	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIsendIMUdata);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskEstimateAttitude);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIsendAttitudeData);
#if SETTINGS_NODE_ID!=1
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIreceiveAttitudeData1);
#endif
#if SETTINGS_NODE_ID!=2
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIreceiveAttitudeData2);
#endif
#if SETTINGS_NODE_ID!=3
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIreceiveAttitudeData3);
#endif
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCompareAttitudeData);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskCNIsendCompareAttitudeData);
	CNI_start();

	//taskIMUgetData_update(&taskIMUgetData);
	//taskIMUgetData_update(&taskIMUgetData);
	//taskIMUgetData_update(&taskIMUgetData);

	// Se queda acá esperando hasta que uno presione el botón para comenzar
	while(!run)
	{
		//__asm__("wfi");
	}

	taskWatchdog_start(&taskWatchdog);

	timeTriggeredScheduler_start();

	while(1)
	{
		timeTriggeredScheduler_dispatch();
	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
	run = true;
}
