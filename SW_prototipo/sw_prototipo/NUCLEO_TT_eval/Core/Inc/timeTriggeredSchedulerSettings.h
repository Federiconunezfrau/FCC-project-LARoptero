/*
 * timeTriggeredSchedulerSettings.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_TIMETRIGGEREDSCHEDULERSETTINGS_H_
#define INC_TIMETRIGGEREDSCHEDULERSETTINGS_H_

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
#define DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL 2
#define WCET_TASK_CNI_SEND_IMU_US 100
#define BCET_TASK_CNI_SEND_IMU_US 0

#endif /* INC_TIMETRIGGEREDSCHEDULERSETTINGS_H_ */
