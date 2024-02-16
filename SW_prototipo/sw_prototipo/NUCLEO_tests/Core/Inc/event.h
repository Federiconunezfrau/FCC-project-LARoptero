/*
 * event.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_EVENT_H_
#define INC_EVENT_H_

#include <cstdint>
#include "utils.hpp"

#define OFFSET_USER_DEFINED_EVENTS 4

#define SIZE_QUEUE_EVENTS 10

typedef uint8_t event_t;

enum systemEvents : event_t
{
	// Eventos asociados al sistema
	EV_SYSTEM_NEW_MSG = OFFSET_USER_DEFINED_EVENTS,
	EV_UART_ENABLE,
	EV_SET_FAIL_SAFE_MODE,
	EV_SET_IDLE_MODE,
	EV_SET_NORMAL_MODE
};

using evBuffer = circ_buffer<event_t, SIZE_QUEUE_EVENTS>;

#endif /* INC_EVENT_H_ */
