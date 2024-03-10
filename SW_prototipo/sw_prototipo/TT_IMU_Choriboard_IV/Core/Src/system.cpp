/*
 * system.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */

#include <stdint.h>
#include "system.h"
#include "modeFailSilent.h"

#include "modeNormal.h"

#include "stm32f7xx_hal.h"

enum systemMode : uint32_t
{
	FAIL_SILENT = 0,
	NORMAL
};

static systemMode theSystemMode = FAIL_SILENT;

void system_init(void)
{
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		__HAL_RCC_CLEAR_RESET_FLAGS();

		theSystemMode = FAIL_SILENT;
	}
	else
	{
		theSystemMode = NORMAL;
	}
}

void system_run_active_mode(void)
{
	switch(theSystemMode)
	{
		case NORMAL:
			normal_mode_run();
			break;

		default:
		case FAIL_SILENT:
			fail_silent_mode_run();
			break;
	}
}

