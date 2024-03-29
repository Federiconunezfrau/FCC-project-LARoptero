/*
 * PCIFmsg.h
 *
 *  Created on: Dec 29, 2023
 *      Author: fede
 */

#ifndef INC_PCIFMSG_H_
#define INC_PCIFMSG_H_

#include "stm32f7xx_hal.h"
#include "types.h"

#define MAX_LEN_PAYLOAD_PCIF 32
#define MAX_PACKET_SIZE_PCIF MAX_LEN_PAYLOAD_PCIF+2

/** Estos son los posibles comandos que puedo enviar o recibir desde la PC.  */
enum command : uint8_t
{
	whoAmI = 0x01,
	runNormalMode = 0x02,
	getFloatSize = 0x03
};

/** Esta estructura sirve para administrar mejor los mensajes intercambiados con la PC. */
typedef struct
{
	uint8_t  payload[MAX_LEN_PAYLOAD_PCIF];
	uint32_t lenPayload;
	uint8_t  cmd;
} PCIFmsg_t;

status_t PCIFmsg_make_from_bytes ( PCIFmsg_t *, uint8_t *, uint32_t );
status_t PCIFmsg_to_bytes        ( PCIFmsg_t *, uint8_t *, uint32_t, uint32_t *);

#endif /* INC_PCIFMSG_H_ */
