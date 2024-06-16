/*
 * modeNormal.h
 *
 *  Created on: Feb 17, 2024
 *      Author: fede
 */

#ifndef INC_MODENORMAL_H_
#define INC_MODENORMAL_H_

#include "stm32f7xx_hal.h"

// Configuraciones del scheduler
#define MICROTICKS_IN_MACROTICK 1000-1
#define EXPECTED_SYNC_TIMESTAMP_TICKS 300
#define DELAY_SYNC_TICKS 300

// Task Sync ===================================
#define PERIOD_TASK_SYNC_TICKS_NORMAL 1000
#define DELAY_TASK_SYNC_TICKS_NORMAL 450
#define WCET_TASK_SYNC_US 100
#define BCET_TASK_SYNC_US 0

// Task Heartbeat ===================================
#define PERIOD_TASK_HEARTBEAT_TICKS_NORMAL 1000
#define DELAY_TASK_HEARTBEAT_TICKS_NORMAL  0
#define WCET_TASK_HEARTBEAT_US 100
#define BCET_TASK_HEARTBEAT_US 0

// Task Watchdog ===================================
#define PERIOD_TASK_WATCHDOG_TICKS_NORMAL 1
#define DELAY_TASK_WATCHDOG_TICKS_NORMAL  0
#define WCET_TASK_WATCHDOG_US 100
#define BCET_TASK_WATCHDOG_US 0

// Task IMU get data ===================================
#define PERIOD_TASK_IMU_TICKS_NORMAL 100
#define DELAY_TASK_IMU_TICKS_NORMAL 1
#define WCET_TASK_IMU_US 100
#define BCET_TASK_IMU_US 0

// Task CNI IMU send data 1 ===================================
#define PERIOD_TASK_CNI_SEND_IMU_1_TICKS_NORMAL 100
#define DELAY_TASK_CNI_SEND_IMU_1_TICKS_NORMAL 2
#define WCET_TASK_CNI_SEND_IMU_1_US 100
#define BCET_TASK_CNI_SEND_IMU_1_US 0

// Task CNI IMU send data 2 ===================================
#define PERIOD_TASK_CNI_SEND_IMU_2_TICKS_NORMAL 100
#define DELAY_TASK_CNI_SEND_IMU_2_TICKS_NORMAL 3
#define WCET_TASK_CNI_SEND_IMU_2_US 100
#define BCET_TASK_CNI_SEND_IMU_2_US 0

// Task CNI IMU send data 3 ===================================
#define PERIOD_TASK_CNI_SEND_IMU_3_TICKS_NORMAL 100
#define DELAY_TASK_CNI_SEND_IMU_3_TICKS_NORMAL 4
#define WCET_TASK_CNI_SEND_IMU_3_US 100
#define BCET_TASK_CNI_SEND_IMU_3_US 0

// Task CNI IMU receive data 1 ===================================
#define PERIOD_TASK_RECEIVE_IMU_1_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_1_TICKS_NORMAL
#define DELAY_TASK_RECEIVE_IMU_1_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_1_TICKS_NORMAL
#define WCET_TASK_RECEIVE_IMU_1_US WCET_TASK_CNI_SEND_IMU_1_US
#define BCET_TASK_RECEIVE_IMU_1_US BCET_TASK_CNI_SEND_IMU_1_US

// Task CNI IMU receive data 2 ===================================
#define PERIOD_TASK_RECEIVE_IMU_2_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_2_TICKS_NORMAL
#define DELAY_TASK_RECEIVE_IMU_2_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_2_TICKS_NORMAL
#define WCET_TASK_RECEIVE_IMU_2_US WCET_TASK_CNI_SEND_IMU_2_US
#define BCET_TASK_RECEIVE_IMU_2_US BCET_TASK_CNI_SEND_IMU_2_US

// Task CNI IMU receive data 3 ===================================
#define PERIOD_TASK_RECEIVE_IMU_3_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_3_TICKS_NORMAL
#define DELAY_TASK_RECEIVE_IMU_3_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_3_TICKS_NORMAL
#define WCET_TASK_RECEIVE_IMU_3_US WCET_TASK_CNI_SEND_IMU_3_US
#define BCET_TASK_RECEIVE_IMU_3_US BCET_TASK_CNI_SEND_IMU_3_US

void normal_mode_run ( void );

#endif /* INC_MODENORMAL_H_ */
