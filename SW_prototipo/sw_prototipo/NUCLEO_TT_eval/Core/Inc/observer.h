/*
 * observer.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_OBSERVER_H_
#define INC_OBSERVER_H_

#include <stdint.h>

#define MAX_NUM_OBSERVERS 5

typedef struct __subject_t subject_t;
typedef void (*notificationHandle_t)(void*, subject_t*);

struct __subject_t
{
	void *mObservers_[MAX_NUM_OBSERVERS];
	notificationHandle_t mObserversHandle_[MAX_NUM_OBSERVERS];
};

void subject_constructor  ( subject_t* );
void subject_destructor   ( subject_t* );
uint8_t subject_attach    ( subject_t*, void*, notificationHandle_t );
uint8_t subject_dettach   ( subject_t*, void*, notificationHandle_t );
void    subject_notify    ( subject_t *);

#endif /* INC_OBSERVER_H_ */