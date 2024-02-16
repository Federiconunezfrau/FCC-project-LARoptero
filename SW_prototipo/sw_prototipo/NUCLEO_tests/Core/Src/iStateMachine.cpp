/*
 * iStateMachine.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */


#include "iStateMachine.h"

iStateMachine::iStateMachine(stateHandler_t initial)
{
	mState_ = initial;
}

iStateMachine::~iStateMachine()
{
	// no hace nada
}

void iStateMachine::init(void)
{
	//event_t e = EV_NONE;

	(*mState_)(this, EV_NONE);
	(*mState_)(this, EV_ENTRY);
}

void iStateMachine::dispatch(event_t const e)
{
	stateHandler_t s = mState_;
	state_t r = (*s)(this,e);

	if( r == TRANSITION )
	{
		(*s)(this, EV_EXIT);
		mState_(this, EV_ENTRY);
	}
}

state_t iStateMachine::transition(stateHandler_t target)
{
	mState_ = target;

	return TRANSITION;
}

