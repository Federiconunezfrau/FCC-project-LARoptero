/*
 * stateMachine.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include "signal.h"

enum state_t : uint8_t
{
	HANDLED = 0,
	IGNORED = 1,
	TRANSITION = 2,
};

enum reservedSignals : signal_t
{
	SIG_NONE = 0,
	SIG_ENTRY = 1,
	SIG_EXIT = 2,
	SIG_INIT = 3
};

typedef struct
{
	signal_t mSignal_;
} event_t;

typedef state_t (*stateHandler_t) (void *me, event_t *e);

typedef struct
{
	stateHandler_t m_state;
} stateMachine_t;

void    stateMachine_constructor ( stateMachine_t *me, stateHandler_t initial );
void    stateMachine_init        ( stateMachine_t *me );
void    stateMachine_dispatch    ( stateMachine_t *me, event_t *e );
state_t stateMachine_transition  ( stateMachine_t *me, stateHandler_t target );

#endif /* INC_STATEMACHINE_H_ */
