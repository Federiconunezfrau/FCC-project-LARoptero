/*
 * myTimer.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_MYTIMER_H_
#define INC_MYTIMER_H_

#include "stm32f7xx_hal.h"
#include "observer.h"

typedef struct
{
	subject_t super;
	TIM_HandleTypeDef *mHtim_;
} myTimer_t;

void myTimer_constructor ( myTimer_t*, TIM_HandleTypeDef* );
// void myTimer_destructor  ( myTimer_t* );

#endif /* INC_MYTIMER_H_ */
