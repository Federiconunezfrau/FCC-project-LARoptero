/*
 * config.h
 *
 *  Created on: Feb 20, 2024
 *      Author: fede
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

// Este archivo tiene configuraciones generales que simplifican el desarrollo y el debugging

#define SETTINGS_NODE_ID 3

#if SETTINGS_NODE_ID==1
#define THE_NODE_ID 1
#define DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_1_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_1_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_IMU_US WCET_TASK_CNI_SEND_IMU_1_US
#define BCET_TASK_CNI_SEND_IMU_US BCET_TASK_CNI_SEND_IMU_1_US
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 1

#define DELAY_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_ATTITUDE_1_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_ATTITUDE_1_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_ATTITUDE_US WCET_TASK_CNI_SEND_ATTITUDE_1_US
#define BCET_TASK_CNI_SEND_ATTITUDE_US BCET_TASK_CNI_SEND_ATTITUDE_1_US
#define MSG_CNI_SEND_ATTITUDE_DATA_POS_IN_TABLE 10

#define DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_1_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_1_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_1_US
#define BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_1_US
#define MSG_CNI_SEND_COMPARE_ATTITUDE_DATA_POS_IN_TABLE 13
#endif

#if SETTINGS_NODE_ID==2
#define THE_NODE_ID 2
#define DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_2_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_2_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_IMU_US WCET_TASK_CNI_SEND_IMU_2_US
#define BCET_TASK_CNI_SEND_IMU_US BCET_TASK_CNI_SEND_IMU_2_US
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 4

#define DELAY_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_ATTITUDE_2_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_ATTITUDE_2_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_ATTITUDE_US WCET_TASK_CNI_SEND_ATTITUDE_2_US
#define BCET_TASK_CNI_SEND_ATTITUDE_US BCET_TASK_CNI_SEND_ATTITUDE_2_US
#define MSG_CNI_SEND_ATTITUDE_DATA_POS_IN_TABLE 11

#define DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_2_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_2_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_2_US
#define BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_2_US
#define MSG_CNI_SEND_COMPARE_ATTITUDE_DATA_POS_IN_TABLE 16
#endif

#if SETTINGS_NODE_ID==3
#define THE_NODE_ID 3
#define DELAY_TASK_CNI_SEND_IMU_TICKS_NORMAL DELAY_TASK_CNI_SEND_IMU_3_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_IMU_TICKS_NORMAL PERIOD_TASK_CNI_SEND_IMU_3_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_IMU_US WCET_TASK_CNI_SEND_IMU_3_US
#define BCET_TASK_CNI_SEND_IMU_US BCET_TASK_CNI_SEND_IMU_3_US
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 7

#define DELAY_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_ATTITUDE_3_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_ATTITUDE_3_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_ATTITUDE_US WCET_TASK_CNI_SEND_ATTITUDE_3_US
#define BCET_TASK_CNI_SEND_ATTITUDE_US BCET_TASK_CNI_SEND_ATTITUDE_3_US
#define MSG_CNI_SEND_ATTITUDE_DATA_POS_IN_TABLE 12

#define DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL DELAY_TASK_CNI_SEND_COMPARE_ATTITUDE_3_TICKS_NORMAL
#define PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_TICKS_NORMAL PERIOD_TASK_CNI_SEND_COMPARE_ATTITUDE_3_TICKS_NORMAL
#define WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US WCET_TASK_CNI_SEND_COMPARE_ATTITUDE_3_US
#define BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_US BCET_TASK_CNI_SEND_COMPARE_ATTITUDE_3_US
#define MSG_CNI_SEND_COMPARE_ATTITUDE_DATA_POS_IN_TABLE 19
#endif

#endif /* INC_CONFIG_H_ */