/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "CANmsg.h"
#include "utils.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LEN_UART_BUFFER_SIZE 256

#define TIMEOUT_UART_TX_S 100

#define POS_LENGTH_RX_BUFFER 0
#define POS_CMD_ID_RX_BUFFER 1

#define GET_MSG_LEN(buffer)(buffer[POS_LENGTH_RX_BUFFER])
#define GET_MSG_CMD(buffer)(buffer[POS_CMD_ID_RX_BUFFER])

#define CMD_CAN_SEND_START_MSG 0x01
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static volatile uint8_t newCanMsgs = 0;
static volatile uint8_t newUARTmsg = 0;

static uint8_t UARTrxBuffer[64];


// Mensaje para inicializar el TTscheduler
static uint8_t CANmsgSync[] = {};
static CAN_TxHeaderTypeDef CANmsgSyncHeader;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void make_uart_msg(CANmsg_t *canMsg, uint8_t *uartBuffer, uint32_t *lenUartBuffer);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  circ_buffer<CANmsg_t, 30> bufferCanMsgs;
  uint32_t i;
  uint32_t fifoFillLevel;
  CAN_RxHeaderTypeDef rxHeader;
  uint8_t rxData[MAX_LEN_PAYLOAD_CAN];
  CANmsg_t canMsg;
  uint8_t uartBuffer[LEN_UART_BUFFER_SIZE];
  uint32_t lenUartBuffer = 0;

  uint8_t msgLen;
  uint8_t msgCmd;
  uint32_t txMailbox;

  CANmsgSyncHeader.DLC = 0;
  CANmsgSyncHeader.ExtId = 0;
  CANmsgSyncHeader.IDE = CAN_ID_STD;
  CANmsgSyncHeader.RTR = CAN_RTR_DATA;
  CANmsgSyncHeader.TransmitGlobalTime = DISABLE;
  CANmsgSyncHeader.StdId = 0x00000008;

  uint8_t accelX[] = {0x00, 0x00, 0x2C, 0xBC};
  uint8_t accelY[] = {0x00, 0x00, 0x80, 0x3C};
  uint8_t accelZ[] = {0x00, 0x48, 0x80, 0x3F};
  uint8_t gyroX[] = {0x00, 0xD0, 0x04, 0xBF};
  uint8_t gyroY[] = {0x00, 0x3A, 0x9D, 0xBF};
  uint8_t gyroZ[] = {0x00, 0xC0, 0xDA, 0x3D};

  float accelX_f, accelY_f, accelZ_f;
  float gyroX_f, gyroY_f, gyroZ_f;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  HAL_UART_Receive_IT(&huart2, UARTrxBuffer, 1);

  // Prueba de convertir arreglo de uint8_t en un float
  memcpy(&accelX_f, accelX, 4);
  memcpy(&accelY_f, accelY, 4);
  memcpy(&accelZ_f, accelZ, 4);
  memcpy(&gyroX_f, gyroX, 4);
  memcpy(&gyroY_f, gyroY, 4);
  memcpy(&gyroZ_f, gyroZ, 4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(newCanMsgs)
	  {
		  newCanMsgs = 0;
		  i = 0;
		  fifoFillLevel = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);
		  while(i < fifoFillLevel)
		  {
			  HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxData);
			  if( CANmsg_make_from_raw_data(&canMsg, rxData, rxHeader.DLC, rxHeader.StdId) == true )
			  {
				  bufferCanMsgs.write(canMsg);
			  }
			  i++;
		  }
		  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	  }

	  if(!bufferCanMsgs.is_empty())
	  {
		  bufferCanMsgs.read(&canMsg);
		  make_uart_msg(&canMsg, uartBuffer, &lenUartBuffer);
		  HAL_UART_Transmit(&huart2, uartBuffer, lenUartBuffer, TIMEOUT_UART_TX_S);
	  }

	  if(newUARTmsg)
	  {
		newUARTmsg = 0;
		msgLen = GET_MSG_LEN(UARTrxBuffer);

		if(msgLen > 1)
		{
			HAL_UART_Receive(&huart2, &UARTrxBuffer[1], msgLen-1, 100);
			msgCmd = GET_MSG_CMD(UARTrxBuffer);

			switch(msgCmd)
			{
				case CMD_CAN_SEND_START_MSG:
					HAL_CAN_AddTxMessage(&hcan, &CANmsgSyncHeader, CANmsgSync, &txMailbox);

					break;
			}
		}
		HAL_UART_Receive_IT(&huart2, UARTrxBuffer, 1);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */
  CAN_FilterTypeDef canFilter;
  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 3;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_3TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
	canFilter.FilterActivation = CAN_FILTER_ENABLE;
	canFilter.FilterBank = 0;
	canFilter.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilter.FilterMaskIdHigh = 0;
	canFilter.FilterMaskIdLow = 0x0000;
	canFilter.FilterIdHigh = 0;
	canFilter.FilterIdLow = 0x0000;
	canFilter.SlaveStartFilterBank = 0;

	HAL_CAN_ConfigFilter(&hcan, &canFilter);
  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void make_uart_msg(CANmsg_t *canMsg, uint8_t *uartBuffer, uint32_t *lenUartBuffer)
{
	memcpy(uartBuffer, &canMsg->stdID, 4);
	memcpy(&uartBuffer[4], &canMsg->lenPayload, 4);
	memcpy(&uartBuffer[8], canMsg->payload, canMsg->lenPayload);
	uartBuffer[canMsg->lenPayload + 8] = '\r';
	uartBuffer[canMsg->lenPayload + 9] = '\n';

	(*lenUartBuffer) = canMsg->lenPayload + 10;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	newCanMsgs = 1;
	HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	newUARTmsg = 1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
