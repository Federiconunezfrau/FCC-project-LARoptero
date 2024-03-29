/*
 * observer.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "observer.h"

void subject_constructor(subject_t *me)
{
	uint32_t i;

	for( i = 0; i < MAX_NUM_OBSERVERS; i++ )
	{
		me->mObservers_[i] = 0;
		me->mObserversHandle_[i] = 0;
	}
}

void subject_destructor(subject_t *me)
{
	uint32_t i;

	for( i = 0; i < MAX_NUM_OBSERVERS; i++ )
	{
		me->mObservers_[i] = 0;
		me->mObserversHandle_[i] = 0;
	}
}

uint8_t subject_attach(subject_t *me, void *observer, notificationHandle_t notificationHandle)
{
	uint32_t i = 0;
	uint8_t found = 0;

	while( (i < MAX_NUM_OBSERVERS) && (found == 0) )
	{
		if(!(me->mObservers_[i]))
		{
			found = 1;
			me->mObservers_[i] = observer;
			me->mObserversHandle_[i] = notificationHandle;
		}
		else
		{
			i++;
		}
	}

	return found;
}

uint8_t subject_dettach(subject_t *me, void *observer, notificationHandle_t notificationHandle)
{
	uint32_t i = 0;
	uint8_t found = 0;

	while( (i < MAX_NUM_OBSERVERS) && (found == 0))
	{
		if( (me->mObservers_[i] == observer ) && (me->mObserversHandle_[i] == notificationHandle) )
		{
			me->mObservers_[i] = 0;
			me->mObserversHandle_[i] = 0;
			found = 1;
		}
		else
		{
			i++;
		}
	}
	return found;
}

void subject_notify(subject_t *me)
{
	uint32_t i;
	notificationHandle_t handle;

	for(i = 0; i < MAX_NUM_OBSERVERS ; i++)
	{
		if( (me->mObservers_[i]) && (me->mObserversHandle_[i]) )
		{
			handle = me->mObserversHandle_[i];
			(*handle)(me->mObservers_[i], me);
		}
	}
}
