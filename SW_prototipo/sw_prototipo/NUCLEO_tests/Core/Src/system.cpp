/*
 * system.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */


#include "system.h"


//system::system(UART_HandleTypeDef *huart) : iStateMachine(&system::initial)
//{
//	mHuart_ = huart;
//}

system::system(evBuffer *theEventQueue) : iStateMachine(reinterpret_cast<stateHandler_t>(&system::initial))
{
	mEventQueue_ = theEventQueue;
}

state_t system::initial(system *me, event_t const e)
{
	// [TODO]: ver si entré acá por el watchdog. En ese caso, se pasaría
	// automáticamente al modo fail-silent

	return me->transition(reinterpret_cast<stateHandler_t>(&system::idle));
}

state_t system::idle(system *me, event_t const e)
{
	switch(e)
	{
		case EV_ENTRY:

			// Se habilita la interrupción por UART, para recibir mensajes y
			// actuar en consecuencia
			mEventQueue_->write(EV_UART_ENABLE);

			return HANDLED;

		case EV_SYSTEM_NEW_MSG:

			return HANDLED;

		case EV_SET_NORMAL_MODE:

			return me->transition(reinterpret_cast<stateHandler_t>(&system::normal_mode));
	}
	return IGNORED;

}

state_t system::fail_safe_mode(system *me, event_t const e)
{
	return IGNORED;
}

state_t system::normal_mode(system*, event_t const e)
{
	switch(e)
	{
		case EV_ENTRY:


	}

	return IGNORED;
}

void system::pass_msg(PCIFmsg_t *msg)
{
	rxMsgsBuffer.write(*msg);
}
