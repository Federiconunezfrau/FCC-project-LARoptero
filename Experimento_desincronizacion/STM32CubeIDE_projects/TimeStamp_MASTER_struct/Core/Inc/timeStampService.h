/*
 * timeStampService.h
 *
 *  Created on: Jan 4, 2024
 *      Author: fede
 */

#ifndef INC_TIMESTAMPSERVICE_H_
#define INC_TIMESTAMPSERVICE_H_

#include "stm32f7xx_hal.h"
#include "stateMachine.h"
#include "gpio_STM32.hpp"

typedef struct
{
	stateMachine_t super;
	TIM_HandleTypeDef *m_htim;
	//STM32::gpio *m_ledIndicator;
} timeStampService_t;


//void timeStampService_constructor(timeStampService_t *me, TIM_HandleTypeDef *htim, STM32::gpio *ledIndicator);
void timeStampService_constructor(timeStampService_t *me, TIM_HandleTypeDef *htim);
state_t timeStampService_initial(timeStampService_t *me);
state_t timeStampService_off(timeStampService_t *me, event_t const e);
state_t timeStampService_running(timeStampService_t *me, event_t const e);


#endif /* INC_TIMESTAMPSERVICE_H_ */
