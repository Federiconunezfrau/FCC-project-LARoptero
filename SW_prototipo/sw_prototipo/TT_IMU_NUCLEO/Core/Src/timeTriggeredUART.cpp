/*
 * timeTriggeredUART.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#include "timeTriggeredUART.h"
#include "myUtils.h"

#define MAX_LEN_PAYLOAD 32

#define TIMEOUT_UART_SEND_MS 10

static timeTriggeredUART_t _instance;

void timeTriggeredUART_constructor(UART_HandleTypeDef *huart, uint8_t bytesToSend)
{
	_instance.mHuart_ = huart;
	_instance.mBytesToSend_ = bytesToSend;
}

TTUART_status_t timeTriggeredUART_add_to_queue(uint8_t cmd, uint8_t lenPayload, uint8_t *payload)
{
	uint8_t buffer[MAX_LEN_PAYLOAD+2];

	if(lenPayload > MAX_LEN_PAYLOAD)
	{
		return TTUART_PAYLOAD_TOO_LONG;
	}

	if(_instance.mBuffer_.is_full())
	{
		return TTUART_BUFFER_FULL;
	}

	buffer[0] = cmd;
	buffer[1] = lenPayload;
	if(lenPayload > 0)
	{
		my_copy_array_of_uint8(&buffer[2], payload, lenPayload);
	}

	_instance.mBuffer_.copy_from(buffer, lenPayload+2);

	return TTUART_OK;
}

TTUART_status_t timeTriggeredUART_send(void)
{
	uint8_t buffer[MAX_LEN_PAYLOAD+2];
	uint8_t numElementsInBuffer;

	if(_instance.mBuffer_.is_empty())
	{
		return TTUART_BUFFER_EMPTY;
	}

	numElementsInBuffer = _instance.mBuffer_.get_size();

	if(numElementsInBuffer > _instance.mBytesToSend_)
	{
		numElementsInBuffer = _instance.mBytesToSend_;
	}

	_instance.mBuffer_.copy_to(buffer, numElementsInBuffer);

	HAL_UART_Transmit(_instance.mHuart_,buffer, numElementsInBuffer, TIMEOUT_UART_SEND_MS);

	return TTUART_OK;
}

