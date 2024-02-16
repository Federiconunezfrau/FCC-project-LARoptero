/*
 * interface_event.h
 *
 *  Created on: Jan 30, 2024
 *      Author: fede
 */

#ifndef INC_INTERFACE_EVENT_H_
#define INC_INTERFACE_EVENT_H_

#include "interface_TTtask.h"
#include "signal.h"

#define MAX_OBSERVERS 10

class interface_event
{
	public:
		virtual bool attach(interface_TTtask*);
		virtual bool detach(interface_TTtask*);
		virtual void notify(void);

	protected:
		interface_event(signal_t);
		signal_t msignal_;

	private:
		interface_TTtask *observersList[MAX_OBSERVERS];

};

#endif /* INC_INTERFACE_EVENT_H_ */
