/*
 * PCIFmsg.h
 *
 *  Created on: Dec 29, 2023
 *      Author: fede
 */

#ifndef INC_PCIFMSG_H_
#define INC_PCIFMSG_H_

#include "stm32f7xx_hal.h"

#define MAX_LEN_PAYLOAD_PCIF 32
#define MAX_PACKET_SIZE_PCIF MAX_LEN_PAYLOAD_PCIF+10

/** Estos son los posibles comandos que puedo enviar o recibir desde la PC.  */
enum command : uint8_t
{
	welcomeMsg = 0x00,
	whoAmI = 0x01,
	startTimeStampService = 0x02,
	stopTimeStampService  = 0x03,
	dataTimeStampService  = 0x04,
	enableCANbus = 0x05,
	disableCANbus = 0x06,
	forwardCANmsg = 0x07,
	activateCANsniffer = 0x08,
	deactivateCANsniffer = 0x09
};

/** Esta estructura sirve para administrar mejor los mensajes intercambiados con la PC. */
typedef struct
{
	uint8_t  payload[MAX_LEN_PAYLOAD_PCIF];
	uint8_t  lenPayload;
	uint8_t  cmd;
} PCIFmsg_t;

uint8_t PCIFmsg_make_from_bytes(uint8_t *buffer, uint8_t lenBuffer, PCIFmsg_t *msg);

#endif /* INC_PCIFMSG_H_ */
