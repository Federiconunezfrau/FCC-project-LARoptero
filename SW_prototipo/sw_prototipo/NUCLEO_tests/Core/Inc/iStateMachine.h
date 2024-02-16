/*
 * state_machine.h
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */

#ifndef INC_ISTATEMACHINE_H_
#define INC_ISTATEMACHINE_H_

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

typedef state_t (*stateHandler_t) (void * const, event_t const);

class iStateMachine
{
	public:
		virtual ~iStateMachine();

		void init();
		void dispatch(event_t const);

	protected:
		state_t transition(stateHandler_t);
		iStateMachine(stateHandler_t);

	private:
		stateHandler_t mState_;
};


#endif /* INC_ISTATEMACHINE_H_ */
