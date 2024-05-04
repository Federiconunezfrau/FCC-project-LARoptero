/*
 * stateMachine.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#include "stateMachine.h"

static event_t reservedEvents[] =
{
		{SIG_NONE},
		{SIG_ENTRY},
		{SIG_EXIT},
		{SIG_INIT}
};

void stateMachine_constructor(stateMachine_t *me, stateHandler_t initial)
{
	me->m_state = initial;
}

void stateMachine_init(stateMachine_t *me)
{
	(me->m_state)(me, &reservedEvents[SIG_NONE]);
	(me->m_state)(me, &reservedEvents[SIG_ENTRY]);
}

void stateMachine_dispatch(stateMachine_t *me, event_t *e)
{
	stateHandler_t s = me->m_state;
	state_t r = (*s)(me, e);

	if(r == TRANSITION)
	{
		(*s)(me, &reservedEvents[SIG_EXIT]);
		(me->m_state)(me, &reservedEvents[SIG_ENTRY]);
	}
}

state_t stateMachine_transition(stateMachine_t *me, stateHandler_t target)
{
	me->m_state = target;
	return TRANSITION;
}

