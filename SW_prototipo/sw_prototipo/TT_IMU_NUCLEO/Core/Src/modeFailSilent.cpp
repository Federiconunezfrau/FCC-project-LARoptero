/*
 * modeFailSilent.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: fede
 */

#include "modeFailSilent.h"
#include "gpio_STM32.hpp"
#include "main.h"

#define LED_FAIL_SILENT_GPIO_Port LD3_GPIO_Port
#define LED_FAIL_SILENT_GPIO_Pin  LD3_Pin

void fail_silent_mode_run(void)
{
	STM32::gpio ledFailSilent(LED_FAIL_SILENT_GPIO_Port, LED_FAIL_SILENT_GPIO_Pin);


	ledFailSilent.write(GPIO_ST::HIGH);

	while(1)
	{
		// El programa se queda acá, no puede salir. Es la idea justamente
	}
}

