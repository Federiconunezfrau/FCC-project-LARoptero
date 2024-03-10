/*
 * timestamp.h
 *
 *  Created on: Jan 7, 2024
 *      Author: fede
 */

#ifndef INC_LOCALCLOCK_H_
#define INC_LOCALCLOCK_H_

#include <timestamp.h>
#include "stm32f7xx_hal.h"
#include "gpio_STM32.hpp"

typedef struct
{
	TIM_HandleTypeDef *m_htim;
	STM32::gpio *m_ledClockRunning;
} localClock_t;

void localClock_constructor(localClock_t *me, TIM_HandleTypeDef *htim, STM32::gpio *ledClockRunning);
void localClock_start(localClock_t *me);
void localClock_stop(localClock_t *me);
timestamp_t localClock_get_timestamp(localClock_t *me);

#endif /* INC_LOCALCLOCK_H_ */
