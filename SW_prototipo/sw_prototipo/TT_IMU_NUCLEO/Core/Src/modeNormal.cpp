/*
 * modeNormal.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: fede
 */

#include "modeNormal.h"

#include "taskHeartbeat.h"
#include "taskWatchdog.h"
#include "taskCNIsendData.h"
#include "taskCNIreceiveData.h"
#include "taskUARTsendData.h"

#include "timeTriggeredScheduler.h"

#include "main.h"

#include "CNI.h"

#include "timeTriggeredUART.h"

#define HANDLE_MSG_CNI_SYNC 0
#define HANDLE_MSG_CNI_IMU_1 1
#define HANDLE_MSG_CNI_IMU_2 4
#define HANDLE_MSG_CNI_IMU_3 7

#define ICM42688_CS_GPIO_Port IMU_CS_GPIO_Port
#define ICM42688_CS_Pin       IMU_CS_Pin

#define LED_HEARTBEAT_GPIO_Port LD1_GPIO_Port
#define LED_HEARTBEAT_GPIO_Pin  LD1_Pin

static bool run = false;

void normal_mode_run(void)
{
	taskHeartbeat_t taskHeartbeat;
	taskWatchdog_t taskWatchdog;
	taskTimeTriggeredSync_t taskPeriodicSync;
	taskCNIreceiveData_t taskReceiveIMU1;
	taskCNIreceiveData_t taskReceiveIMU2;
	taskCNIreceiveData_t taskReceiveIMU3;
//	taskUARTsendData_t taskSendIMUUART1;
//	taskUARTsendData_t taskSendIMUUART2;
//	taskUARTsendData_t taskSendIMUUART3;

	// Inicialización del LED heartbeat==============================
	STM32::gpio ledHeartbeat(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_GPIO_Pin);

	// Inicialización de la CNI ==============================
	CNI_constructor(&hcan1);

	// Inicialización de la UART ============================
//	timeTriggeredUART_constructor(&huart3, 8);

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

	taskTimeTriggeredSync_constructor(&taskPeriodicSync,
			DELAY_TASK_SYNC_TICKS_NORMAL,
			PERIOD_TASK_SYNC_TICKS_NORMAL,
			WCET_TASK_SYNC_US,
			BCET_TASK_SYNC_US,
			HANDLE_MSG_CNI_SYNC,
			EXPECTED_SYNC_TIMESTAMP_TICKS,
			DELAY_SYNC_TICKS);

//	taskUARTsendData_constructor(&taskSendIMUUART1,
//			DELAY_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			PERIOD_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			WCET_TASK_SEND_IMU_UART_US,
//			BCET_TASK_SEND_IMU_UART_US,
//			HANDLE_MSG_CNI_IMU_1, 0x01);
//
//	taskUARTsendData_constructor(&taskSendIMUUART2,
//			DELAY_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			PERIOD_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			WCET_TASK_SEND_IMU_UART_US,
//			BCET_TASK_SEND_IMU_UART_US,
//			HANDLE_MSG_CNI_IMU_2, 0x02);
//
//	taskUARTsendData_constructor(&taskSendIMUUART3,
//			DELAY_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			PERIOD_TASK_SEND_IMU_UART_TICKS_NORMAL,
//			WCET_TASK_SEND_IMU_UART_US,
//			BCET_TASK_SEND_IMU_UART_US,
//			HANDLE_MSG_CNI_IMU_3, 0x03);

	taskCNIreceiveData_constructor(&taskReceiveIMU1,
			DELAY_TASK_RECEIVE_IMU_1_TICKS_NORMAL,
			PERIOD_TASK_RECEIVE_IMU_1_TICKS_NORMAL,
			WCET_TASK_RECEIVE_IMU_1_US,
			BCET_TASK_RECEIVE_IMU_1_US,
			HANDLE_MSG_CNI_IMU_1);

	taskCNIreceiveData_constructor(&taskReceiveIMU2,
			DELAY_TASK_RECEIVE_IMU_2_TICKS_NORMAL,
			PERIOD_TASK_RECEIVE_IMU_2_TICKS_NORMAL,
			WCET_TASK_RECEIVE_IMU_2_US,
			BCET_TASK_RECEIVE_IMU_2_US,
			HANDLE_MSG_CNI_IMU_2);

	taskCNIreceiveData_constructor(&taskReceiveIMU3,
			DELAY_TASK_RECEIVE_IMU_3_TICKS_NORMAL,
			PERIOD_TASK_RECEIVE_IMU_3_TICKS_NORMAL,
			WCET_TASK_RECEIVE_IMU_3_US,
			BCET_TASK_RECEIVE_IMU_3_US,
			HANDLE_MSG_CNI_IMU_3);


	// Inicialización del scheduler =================================
	timeTriggeredScheduler_constructor(&htim5, MICROTICKS_IN_MACROTICK);
	timeTriggeredScheduler_init();

	CNI_init();

	taskHeartbeat_start(&taskHeartbeat);

	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskHeartbeat);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskWatchdog);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskPeriodicSync);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskReceiveIMU1);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskReceiveIMU2);
	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskReceiveIMU3);
//	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskSendIMUUART1);
//	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskSendIMUUART2);
//	timeTriggeredScheduler_add_task((timeTriggeredTask_t*)&taskSendIMUUART3);



	CNI_start();

	// Se queda acá esperando hasta que se cumpla la condición para comenzar
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