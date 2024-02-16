/*
 * CANmsg.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: fede
 */

#include "PCIFmsg.h"

/** Recibe un array de bytes y lo convierte en un tipo de dato PCIFmsg_t, rellenando los campos correspondientes.
 *  */
status_t PCIFmsg_from_bytes(PCIFmsg_t *me, uint8_t buffer[], uint32_t lenBuffer)
{
	uint32_t i;

	if((buffer == 0) || (me == 0))
	{
		return ERROR_NULL_POINTER;
	}

	if(lenBuffer < 2)
	{
		return ERROR_BUFFER_TOO_SMALL;
	}

	me->cmd = buffer[1];
	me->lenPayload = buffer[0];
	if(me->lenPayload > 0)
	{
		for(i = 0; i < me->lenPayload ; i++)
		{
			me->payload[i] = buffer[i+2];
		}
	}

	return OK;
}

status_t PCIFmsg_to_bytes(PCIFmsg_t *me, uint8_t buffer[], uint32_t lenBuffer, uint32_t *lenMsg)
{
	uint32_t i;

	if( (buffer == 0) || (me==0) )
	{
		return ERROR_NULL_POINTER;
	}

	if(lenBuffer < me->lenPayload + 2)
	{
		return ERROR_BUFFER_TOO_SMALL;
	}

	buffer[0] = me->lenPayload + 2;
	buffer[1] = me->cmd;

	for(i = 0 ; i < me->lenPayload; i++)
	{
		buffer[2+i] = me->payload[i];
	}

	*lenMsg = buffer[0];

	return OK;
}

