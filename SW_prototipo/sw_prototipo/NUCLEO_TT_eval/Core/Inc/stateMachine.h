/*
 * stateMachine.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include "event.h"

enum state_t : uint8_t
{
	HANDLED = 0,
	IGNORED = 1,
	TRANSITION = 2,
};

enum reservedEvents : event_t
{
	EV_NONE = 0,
	EV_ENTRY = 1,
	EV_EXIT = 2,
	EV_INIT = 3
};

typedef state_t (*stateHandler_t) (void *, event_t const);

typedef struct
{
	stateHandler_t m_state;
} stateMachine_t;

void    stateMachine_constructor ( stateMachine_t *me, stateHandler_t initial );
void    stateMachine_init        ( stateMachine_t *me );
void    stateMachine_dispatch    ( stateMachine_t *me, event_t const e );
state_t stateMachine_transition  ( stateMachine_t *me, stateHandler_t target );

#endif /* INC_STATEMACHINE_H_ */
