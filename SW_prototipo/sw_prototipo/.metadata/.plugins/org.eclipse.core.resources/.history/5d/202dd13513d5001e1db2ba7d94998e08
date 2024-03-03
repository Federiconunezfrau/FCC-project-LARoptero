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
#define DELAY_TASK_SYNC_TICKS_NORMAL 500
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
#define PERIOD_TASK_IMU_TICKS_NORMAL 1000
#define DELAY_TASK_IMU_TICKS_NORMAL 1
#define WCET_TASK_IMU_US 100
#define BCET_TASK_IMU_US 0

// Task CNI IMU send data ===================================
#define PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL 1000
#define DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL DELAY_SEND_IMU_TICKS
#define WCET_TASK_CNI_SEND_IMU_US 100
#define BCET_TASK_CNI_SEND_IMU_US 0

void normal_mode_run ( void );

#endif /* INC_MODENORMAL_H_ */
