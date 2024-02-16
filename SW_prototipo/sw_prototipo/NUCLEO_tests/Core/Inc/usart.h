/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define MAX_LEN_PAYLOAD_PCIF 32
//#define MAX_PACKET_SIZE_PCIF MAX_LEN_PAYLOAD_PCIF+10

///** Estos son los posibles comandos que puedo enviar o recibir desde la PC.  */
//enum command : uint8_t
//{
//	welcomeMsg = 0x00,
//	whoAmI = 0x01,
//	startTimeStampService = 0x02,
//	stopTimeStampService  = 0x03,
//	dataTimeStampService  = 0x04,
//	enableCANbus = 0x05,
//	disableCANbus = 0x06,
//	forwardCANmsg = 0x07,
//	activateCANsniffer = 0x08,
//	deactivateCANsniffer = 0x09
//};

/** Esta estructura sirve para administrar mejor los mensajes intercambiados con la PC. */
typedef struct
{
	uint8_t  payload[MAX_LEN_PAYLOAD_PCIF];
	uint8_t  lenPayload;
	uint8_t  cmd;
} PCIFmsg_t;


/* USER CODE END Private defines */

void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t uart_get_new_data(PCIFmsg_t *);

uint8_t PCIFmsg_make_from_bytes(uint8_t *, uint8_t , PCIFmsg_t *);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

