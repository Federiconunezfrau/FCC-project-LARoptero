/*
 * iObserver.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: fede
 */

#include "interface_observer.h"

interface_subject::interface_subject(void)
{
	uint32_t i;

	for(i = 0; i < MAX_OBSERVERS ; i++)
	{
		observersList[i] = 0;
	}
}

interface_subject::~interface_subject(void)
{
	uint32_t i;

	for(i = 0; i < MAX_OBSERVERS ; i++)
	{
		observersList[i] = 0;
	}
}


bool interface_subject::attach(interface_observer* o)
{
	uint32_t i = 0;
	bool foundSpace = false;

	while( (i < MAX_OBSERVERS) && (!foundSpace) )
	{
		if(observersList[i] == 0)
		{
			observersList[i] = o;
			foundSpace = true;
		}
		else
		{
			i++;
		}
	}

	return foundSpace;
}


bool interface_subject::detach(interface_observer *o)
{
	uint32_t i = 0;
	bool found = false;

	while( (i < MAX_OBSERVERS) && (!found) )
	{
		if(observersList[i] == o)
		{
			observersList[i] = 0;
			found = true;
		}
		else
		{
			i++;
		}
	}

	return found;
}

void interface_subject::notify(void)
{
	uint32_t i = 0;

	for(i = 0; i < MAX_OBSERVERS; i++)
	{
		if(observersList[i] != 0)
		{
			observersList[i]->update(this);
		}
	}
}
