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
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "system.h"
#include "CANslave.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define CAN_SLAVE_NODE_ID 1
//#define CAN_SLAVE_NODE_ID 2
#define CAN_SLAVE_NODE_ID 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/** Flag global para decirle al main, desde la interrupción, que hay nuevos datos de CAN disponibles. */
volatile uint8_t flagNewCANdataRx0 = 0;

/** Interfaz slave de CAN */
static CANslave_t canSlave;

/** Este buffer se usa para encolar eventos que hayan ocurrido. Luego el dispatcher los toma y ejecuta en respuesta. */
static evBuffer eventsBuffer;

/** Cada vez que alguien quiere enviar un mensaje a través del bus CAN, debe depositar el mensaje en este buffer. */
static canMsgBuffer canTxBuffer;

/** Cada vez que se recibe un mensaje de CAN, el mensaje se copia en este buffer, para no perderlo y permitir que lleguen nuevos mensajes. */
static canMsgBuffer canRxBuffer;

/** Punteros globales, sirven para que todos los miembros del sistema puedan acceder a estos elementos. */
evBuffer * const p_eventsBuffer = &eventsBuffer;
canMsgBuffer * const p_canTxBuffer = &canTxBuffer;
canMsgBuffer * const p_canRxBuffer = &canRxBuffer;

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

#ifdef __cplusplus
}
#endif

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  event_t event;
  localClock_t localClock;

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
  MX_SPI2_Init();
  MX_I2C4_Init();
  MX_USB_DEVICE_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  STM32::gpio redLED(LED1_GPIO_Port, LED1_Pin);
  STM32::gpio greenLED(LED2_GPIO_Port, LED2_Pin);

  // Prendo un LED para indicar que hay alimentación en la chori
  greenLED.write(GPIO_ST::HIGH);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  localClock_constructor(&localClock, &htim5, &redLED);
  CANslave_constructor(&canSlave, &hcan1, CAN_SLAVE_NODE_ID, &localClock);

  stateMachine_init((stateMachine_t*)&canSlave);

  HAL_Delay(5000);

  p_eventsBuffer->write(EV_CAN_TURN_ON);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// Verifica si hay nuevos datos provenientes del bus CAN.
	if(flagNewCANdataRx0 == 1)
	{
		p_eventsBuffer->write(EV_CAN_FETCH_NEW_MSGS);

		// Bajo el flag, indicando que ya le avisé al CAN master acerca de los datos nuevos
		flagNewCANdataRx0 = 0;
	}

	// Verifica si hay mensajes de CAN esperando para ser enviados. En dicho caso, genera un evento.
	if(!p_canTxBuffer->is_empty())
	{
		p_eventsBuffer->write(EV_CAN_SEND_NEXT_MSG);
	}

	// Verifica si hay mensajes de CAN que están esperando para ser leidos. En dicho caso, genera un evento.
	if(!p_canRxBuffer->is_empty())
	{
		p_eventsBuffer->write(EV_CAN_PROCESS_NEW_MSG);
	}

	// Si hay eventos en el buffer de eventos, toma uno y lo dispatchea.
	if(!p_eventsBuffer->is_empty())
	{
		p_eventsBuffer->read(&event);
		stateMachine_dispatch((stateMachine_t*)&canSlave, event);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
