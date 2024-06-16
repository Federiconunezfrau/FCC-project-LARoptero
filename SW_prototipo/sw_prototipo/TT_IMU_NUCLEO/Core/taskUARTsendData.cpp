/*
 * taskUARTsendData.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#include "taskUARTsendData.h"
#include "CNI.h"
#include "timeTriggeredUART.h"

#define LEN_UART_DATA 24

void taskUARTsendData_constructor(taskUARTsendData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsg, uint8_t cmd)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskUARTsendData_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mHandleMsg_ = handleMsg;
	me->mCmd_ = cmd;
}

void taskUARTsendData_destructor(taskUARTsendData_t *me)
{
	//TODO: hay que hacer que si uno llama a la tarea nuevamente, la tarea simplemente no haga nada.
	timeTriggeredTask_destructor(&me->super);
}

void taskUARTsendData_start(taskUARTsendData_t *me)
{
	//TODO: acá podría uno decirle a la CNI que se va a encargar de enviar el mensaje
	// asociado al handleMsg.
}

void taskUARTsendData_update(taskUARTsendData_t *me)
{
	uint8_t auxBuffer[LEN_UART_DATA];
	uint32_t lenData;

	CNI_get_msg_content(me->mHandleMsg_, auxBuffer, LEN_UART_DATA, &lenData);

	timeTriggeredUART_add_to_queue(me->mCmd_, lenData, auxBuffer);
}
