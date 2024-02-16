/*
 * Observer.h
 *
 *  Created on: Jan 29, 2024
 *      Author: fede
 */

#ifndef INC_INTERFACE_OBSERVER_H_
#define INC_INTERFACE_OBSERVER_H_

#include <stdint.h>

#define MAX_OBSERVERS 10

class interface_subject;

class interface_observer
{
	public:
		virtual ~interface_observer();
		virtual void update(interface_subject *theChangedSubject) = 0;

	protected:
		interface_observer();
};

class interface_subject
{
	public:
		virtual ~interface_subject();

		virtual bool attach(interface_observer*);
		virtual bool detach(interface_observer*);
		virtual void notify();

	protected:
		interface_subject();

	private:
		interface_observer *observersList[MAX_OBSERVERS];
};

#endif /* INC_INTERFACE_OBSERVER_H_ */
