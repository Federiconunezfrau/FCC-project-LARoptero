/*
 * timestamp.h
 *
 *  Created on: Jan 7, 2024
 *      Author: fede
 */

#ifndef INC_TIMESTAMP_H_
#define INC_TIMESTAMP_H_

#include "stm32f7xx_hal.h"

typedef struct
{
	uint32_t timeStampCounter;
} timestamp_t;


#endif /* INC_TIMESTAMP_H_ */
