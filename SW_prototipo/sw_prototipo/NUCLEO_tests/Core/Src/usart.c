/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "string.h"

#define LEN_RX_BUFFER 24

#define POS_LENGTH_RX_BUFFER 0
#define POS_CMD_ID_RX_BUFFER 1
#define POS_PAYLOAD_RX_BUFFER 2

#define GET_MSG_LEN(buffer)(buffer[POS_LENGTH_RX_BUFFER])
#define GET_MSG_ID(buffer)(buffer[POS_CMD_ID_RX_BUFFER])
#define GET_MSG_PAYLOAD(buffer,i)(buffer[POS_PAYLOAD_RX_BUFFER+i])

// Cuando hay 1 byte nuevo, se setea en 1
static volatile uint8_t flagNewUART3data = 0;

// Array estático donde se copia el byte recibido de UART
static uint8_t rxBuffer[LEN_RX_BUFFER];

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  // Se habilita la interrupción de recepción de la UART3
  HAL_UART_Receive_IT(&huart3, rxBuffer, 1);
  /* USER CODE END USART3_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// Función para chequear si hay datos de UART
uint8_t uart_get_new_data(PCIFmsg_t *msg)
{
	uint8_t msgLen = GET_MSG_LEN(rxBuffer);
	uint8_t i;

	if(!flagNewUART3data)
	{
		return 1;
	}

	if(msg == 0)
	{
		return 2;
	}

	if(msgLen > 0)
	{
		HAL_UART_Receive(&huart3, &rxBuffer[1], msgLen, 100);
	}

	msg->lenPayload = msgLen;
	msg->cmd = GET_MSG_ID(rxBuffer);
	if(msg->lenPayload > 0)
	{
		for(i = 0; i < msg->lenPayload ; i++)
		{
			msg->payload[i] = GET_MSG_PAYLOAD(rxBuffer,i);
		}
	}

	flagNewUART3data = 0;
	HAL_UART_Receive_IT(&huart3, rxBuffer, 1);

	return 0;
}

// Callback para la UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	flagNewUART3data = 1;
}

/* USER CODE END 1 */
