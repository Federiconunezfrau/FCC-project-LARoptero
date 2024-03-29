/*
 * myUART.h
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#ifndef INC_MYUART_H_
#define INC_MYUART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "circularBuffer.h"

#define LEN_RX_BUFFER 32

typedef enum
{
	uart_OK = 0,
	uart_TX_TIMEOUT
} uartStatus_t;

typedef struct
{
	UART_HandleTypeDef *mHuart_;
	circularBuffer_t mCbuf_;
	uint8_t mRxBuf_[LEN_RX_BUFFER];
	uint8_t mAux_;
	uint8_t mEnabled_;
	uint32_t mTimeoutTx_;
} myUART_t;

void myUART_constructor ( myUART_t*, UART_HandleTypeDef* );
void myUART_enable_rx ( myUART_t* );
void myUART_disable_rx ( myUART_t* );
uint16_t myUART_read ( myUART_t*, uint8_t *, uint16_t );
uint16_t myUART_read_blocking ( myUART_t *, uint8_t *, uint16_t );
uint16_t myUART_send ( myUART_t*, uint8_t*, uint16_t );
uint16_t myUART_bytes_in_rx_buffer ( myUART_t *);
void myUART_IRQ_handler ( myUART_t* );

#ifdef __cplusplus
}
#endif


#endif /* INC_MYUART_H_ */
