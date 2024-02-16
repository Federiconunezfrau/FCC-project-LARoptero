/*
 * system_mode_normal.h
 *
 *  Created on: Jan 31, 2024
 *      Author: fede
 */

#ifndef INC_TTSYSTEM_MODE_NORMAL_H_
#define INC_TTSYSTEM_MODE_NORMAL_H_

// Settings de la tarea heartbeat
#define DELAY_TASK_HEARTBEAT_TICKS  0
#define PERIOD_TASK_HEARTBEAT_TICKS 1000
#define WCET_TASK_HEARTBEAT_US 100
#define BCET_TASK_HEARTBEAT_US 0

void system_run_mode_normal(void);

#endif /* INC_TTSYSTEM_MODE_NORMAL_H_ */
