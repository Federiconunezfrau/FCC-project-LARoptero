/*
 * CNI.h
 *
 *  Created on: Feb 13, 2024
 *      Author: fede
 */

#ifndef INC_CNI_H_
#define INC_CNI_H_

#include "stm32f7xx_hal.h"

#define NODE_ID_0 0
#define NODE_ID_1 1
#define NODE_ID_2 2

#define MY_NODE_ID NODE_ID_0

typedef struct
{
	CAN_HandleTypeDef *mHcan_;
	uint32_t mNodeID_;

	CAN_TxHeaderTypeDef mCANtxHeader_;
} CNI_t;

typedef enum
{
	CNI_OK = 0,
	CNI_MSG_HANDLE_NOT_VALID,
	CNI_MSG_HANDLE_TX_NOT_ALLOWED,
	CNI_MSG_HANDLE_RX_NOT_ALLOWED
} CNI_status_t;

void CNI_constructor ( CAN_HandleTypeDef*);
void CNI_init        ( void );
void CNI_start       ( void );
CNI_status_t CNI_send_msg    ( uint32_t );
CNI_status_t CNI_receive_msg ( uint32_t );
CNI_status_t CNI_update_msg_content ( uint32_t, uint8_t[], uint32_t );

#endif /* INC_CNI_H_ */
