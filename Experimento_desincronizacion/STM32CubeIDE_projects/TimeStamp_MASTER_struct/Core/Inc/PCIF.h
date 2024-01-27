/*
 * computerIF.h
 *
 *  Created on: Dec 28, 2023
 *      Author: fede
 */

#ifndef INC_PCIF_H_
#define INC_PCIF_H_

#include "stm32f7xx_hal.h"
#include "stateMachine.h"

typedef struct
{
	stateMachine_t super;
} PCIF_t;


void PCIF_constructor(PCIF_t *me);
state_t PCIF_initial(PCIF_t *me);
state_t PCIF_active(PCIF_t *me, event_t const e);


#endif /* INC_PCIF_H_ */
