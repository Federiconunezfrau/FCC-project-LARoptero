/*
 * timeTriggeredUART.h
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#ifndef INC_TIMETRIGGEREDUART_H_
#define INC_TIMETRIGGEREDUART_H_

#include "usart.h"
#include "utils.hpp"

#define SIZE_BUFFER_UART 256

typedef struct
{
	UART_HandleTypeDef *mHuart_;
	circ_buffer<uint8_t, SIZE_BUFFER_UART> mBuffer_;
	uint8_t mBytesToSend_;
} timeTriggeredUART_t;

typedef enum
{
	TTUART_OK = 0,
	TTUART_BUFFER_FULL,
	TTUART_PAYLOAD_TOO_LONG,
	TTUART_BUFFER_EMPTY
} TTUART_status_t;

void timeTriggeredUART_constructor ( UART_HandleTypeDef*, uint8_t );
TTUART_status_t timeTriggeredUART_add_to_queue ( uint8_t, uint8_t, uint8_t* );
TTUART_status_t timeTriggeredUART_send ( void );

#endif /* INC_TIMETRIGGEREDUART_H_ */
