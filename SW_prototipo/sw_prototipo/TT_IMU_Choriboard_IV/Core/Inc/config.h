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
#define DELAY_SEND_IMU_TICKS 3
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 4
#endif

#if SETTINGS_NODE_ID==2
#define THE_NODE_ID 2
#define DELAY_SEND_IMU_TICKS 4
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 7
#endif

#if SETTINGS_NODE_ID==3
#define THE_NODE_ID 3
#define DELAY_SEND_IMU_TICKS 5
#define MSG_CNI_SEND_IMU_DATA_POS_IN_TABLE 10
#endif





#endif /* INC_CONFIG_H_ */
