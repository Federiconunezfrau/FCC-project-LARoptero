/*
 * CANmsg.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: fede
 */

#include <cstring>
#include "PCIFmsg.h"

/** Recibe un array de bytes y lo convierte en un tipo de dato PCIFmsg_t, rellenando los campos correspondientes.
 *  */
uint8_t PCIFmsg_make_from_bytes(uint8_t *buffer, uint8_t lenBuffer, PCIFmsg_t *msg)
{
	if((buffer == 0) || (msg == 0))
	{
		return 1;
	}
	if(lenBuffer < 4)
	{
		return 2;
	}

	msg->cmd = buffer[0];
	msg->lenPayload = buffer[1];
	if(msg->lenPayload > 0)
	{
		memcpy(&(msg->payload), &buffer[2], msg->lenPayload);
	}

	return 0;
}

