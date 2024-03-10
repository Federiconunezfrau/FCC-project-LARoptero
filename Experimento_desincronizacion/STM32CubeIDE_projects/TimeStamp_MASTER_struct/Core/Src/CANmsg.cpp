/*
 * CANmsg.cpp
 *
 *  Created on: Jan 1, 2024
 *      Author: fede
 */

#include <cstring>
#include "CANmsg.h"

#define MASK_GET_NODE_ID 0x00000007

bool CANmsg_make_from_raw_data(CANmsg_t *me, uint8_t *rxData, uint32_t DLC, uint32_t StdId)
{
	if( (rxData == 0) || (me == 0) || (DLC > MAX_LEN_PAYLOAD_CAN) )
	{
		return false;
	}

	me->id = StdId>>3;
	me->lenPayload = DLC;
	me->nodeID = StdId & MASK_GET_NODE_ID;
	if(DLC>0)
	{
		memcpy(me->payload, rxData, DLC);
	}
	return true;
}

bool CANmsg_set_payload(CANmsg_t *me, uint8_t *payload, uint32_t lenPayload)
{
	if( (me==0) || (payload==0) )
	{
		return false;
	}

	if(lenPayload > MAX_LEN_PAYLOAD_CAN)
	{
		return false;
	}

	if(lenPayload>0)
	{
		memcpy(me->payload, payload, lenPayload);
		me->lenPayload = lenPayload;
	}
	else
	{
		me->lenPayload = 0;
	}

	return true;
}
