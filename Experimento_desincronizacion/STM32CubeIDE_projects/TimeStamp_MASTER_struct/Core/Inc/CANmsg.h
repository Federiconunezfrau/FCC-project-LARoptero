/*
 * CANmsg.h
 *
 *  Created on: Dec 29, 2023
 *      Author: fede
 */

#ifndef INC_CANMSG_H_
#define INC_CANMSG_H_

#include "stm32f7xx_hal.h"

#define CAN_MSG_GET_STD_ID(id,nodeID)((id<<3) | nodeID)

/** Cantidad máxima de bytes de datos útiles para la trama CAN  */
#define MAX_LEN_PAYLOAD_CAN 8

/** El service ID es parte del campo ID que se setea en la trama CAN. Esta sirve para identificar
 *  el contenido del mensaje. Como hay redundancias, habrá muchos nodos que hagan lo mismo en
 *  paralelo. Esto quiere decir que muchos nodos compartirán el mismo tipo de mensaje a sus pares.
 *  Para que no haya colisiones, lo que se hace es se define un formato del campo ID de CAN, que
 *  contemple esto:
 *  StdId [11 bits] = 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
 * 					 ----------------------------------------
 * 					       Service ID                | Número de Nodo
 * Con esta definición, Serive ID tiene 8 bits, es decir se pueden crear 256 servicios.
 * Número de Nodo tiene 3 bits, permitiendo que hasta 8 nodos envíen mensajes con el
 * mismo tipo de información.
 * Este enumerativo representa los bits [10-3]. Los últimos 3 bits se setean automáticamente. */
enum serviceID : uint32_t
{
	// Estos ID corresponden al caso del timestamp service
	timeStampServiceStart = 0x05,
	timeStampServiceStop = 0x06,
	timeStampServiceData  = 0x07,

	// Este ID quiere decir que es un mensaje que proviene directo desde la PC. Sirve para
	// hacer pruebas.
	forwardedData = 0x08
};

/** Esta estructura sirve para administrar los mensajes que se quieren enviar a través de CAN.
 *  Usando esta variable se pueden crear queues donde en cada posición, haya uno de estos. */
typedef struct
{
	uint8_t  payload[MAX_LEN_PAYLOAD_CAN];
	uint32_t lenPayload;
	uint32_t id;
	uint32_t nodeID;
} CANmsg_t;

bool CANmsg_make_from_raw_data(CANmsg_t *me, uint8_t *rxData, uint32_t DLC, uint32_t StdId);
bool CANmsg_set_payload(CANmsg_t *me, uint8_t *payload, uint32_t lenPayload);

#endif /* INC_CANMSG_H_ */
