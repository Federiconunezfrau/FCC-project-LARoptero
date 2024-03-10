/*
 * system.h
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "CANmsg.h"
#include "systemEvents.h"
#include "circ_buffer.h"

#define SIZE_BUFFER_EVENTS 128

/** Esta es la cantidad de mensajes que puede almacenar el circular buffer usado para transmitir. */
#define SIZE_BUFFER_CAN_MSG 128


using evBuffer = circ_buffer<event_t, SIZE_BUFFER_EVENTS>;
using canMsgBuffer = circ_buffer<CANmsg_t, SIZE_BUFFER_CAN_MSG>;

extern evBuffer      * const p_eventsBuffer;

extern canMsgBuffer  * const p_canTxBuffer;
extern canMsgBuffer  * const p_canRxBuffer;

#endif /* INC_SYSTEM_H_ */
