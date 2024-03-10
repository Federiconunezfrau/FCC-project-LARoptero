/*
 * myUART.c
 *
 *  Created on: Feb 11, 2024
 *      Author: fede
 */

#include "myUART.h"

#define TIMEOUT_DEFAULT_MS 1000

void myUART_constructor(myUART_t *me, UART_HandleTypeDef *huart)
{
	me->mHuart_ = huart;
	me->mEnabled_ = 0;
	me->mTimeoutTx_ = TIMEOUT_DEFAULT_MS;
	circularBuffer_constructor(&me->mCbuf_, me->mRxBuf_, LEN_RX_BUFFER);
}

void myUART_enable_rx(myUART_t *me)
{
	if(!me->mEnabled_)
	{
		HAL_UART_Receive_IT(me->mHuart_, &me->mAux_, 1);
		me->mEnabled_ = 1;
	}
}

void myUART_disable_rx(myUART_t *me)
{
	if(me->mEnabled_)
	{
		HAL_UART_AbortReceive_IT(me->mHuart_);
		me->mEnabled_ = 0;
	}
}

uint16_t myUART_read(myUART_t *me, uint8_t *buffer, uint16_t lenBuffer)
{
	uint16_t available = circularBuffer_length_data(&me->mCbuf_);
	uint16_t read = 0;

	if(available > lenBuffer)
	{
		available = lenBuffer;
	}

	while(read < available)
	{
		circularBuffer_read(&me->mCbuf_, &buffer[read]);
		read++;
	}

	return read;
}

uint16_t myUART_read_blocking(myUART_t *me, uint8_t *buffer, uint16_t lenBuffer)
{
	HAL_StatusTypeDef aux;

	if((buffer != 0) && (lenBuffer > 0))
	{
		aux = HAL_UART_Receive(me->mHuart_, buffer, lenBuffer, 100);
	}

	if(aux == HAL_TIMEOUT)
	{
		return 1;
	}

	return 0;
}

uint16_t myUART_send(myUART_t *me, uint8_t *buffer, uint16_t lenBuffer)
{
	HAL_StatusTypeDef aux;

	aux = HAL_UART_Transmit(me->mHuart_, buffer, lenBuffer, me->mTimeoutTx_);

	if(aux == HAL_TIMEOUT)
	{
		return 1;
	}

	return 0;
}

uint16_t myUART_bytes_in_rx_buffer(myUART_t *me)
{
	return circularBuffer_length_data(&me->mCbuf_);

}

void myUART_flush_buffer(myUART_t *me)
{
	return circularBuffer_flush(&me->mCbuf_);

}

void myUART_IRQ_handler(myUART_t *me)
{
	circularBuffer_write(&me->mCbuf_, me->mAux_);
	//HAL_UART_Receive_IT(me->mHuart_, &me->mAux_, 1);
}
