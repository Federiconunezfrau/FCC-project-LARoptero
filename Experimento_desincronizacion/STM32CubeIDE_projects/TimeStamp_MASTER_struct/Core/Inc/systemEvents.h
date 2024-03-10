/*
 * systemEvents.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_SYSTEMEVENTS_H_
#define INC_SYSTEMEVENTS_H_

#include "event.h"

enum systemEvents : event_t
{
	// Eventos asociados a la interfaz CAN
	EV_CAN_TURN_ON = OFFSET_USER_DEFINED_EVENTS,
	EV_CAN_TURN_OFF,
	EV_CAN_SEND_NEXT_MSG,
	EV_CAN_PROCESS_NEW_MSG,
	EV_CAN_FETCH_NEW_MSGS,
	EV_CAN_ACTIVATE_SNIFFER,
	EV_CAN_DEACTIVATE_SNIFFER,

	// Eventos asociados a la interfaz de comunicación con la PC
	EV_COMPUTERIF_SEND_MSG,
	EV_COMPUTERIF_PROCESS_NEW_MSG,

	// Eventos asociados al servicio de timestamps
	EV_START_TIMESTAMP_SERVICE,
	EV_STOP_TIMESTAMP_SERVICE,
	EV_REQUEST_TIMESTAMP
};

#endif /* INC_SYSTEMEVENTS_H_ */
