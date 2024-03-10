/*
 * stateMachine.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#include "stateMachine.h"

#define EV_NONE 0

void stateMachine_constructor(stateMachine_t *me, stateHandler_t initial)
{
	me->m_state = initial;
}

void stateMachine_init(stateMachine_t *me)
{
	event_t e = EV_NONE;

	(me->m_state)(me, e);
	(me->m_state)(me, EV_ENTRY);
}

void stateMachine_dispatch(stateMachine_t *me, event_t const e)
{
	stateHandler_t s = me->m_state;
	state_t r = (*s)(me, e);

	if(r == TRANSITION)
	{
		(*s)(me, EV_EXIT);
		(me->m_state)(me, EV_ENTRY);
	}
}

state_t stateMachine_transition(stateMachine_t *me, stateHandler_t target)
{
	me->m_state = target;
	return TRANSITION;
}

