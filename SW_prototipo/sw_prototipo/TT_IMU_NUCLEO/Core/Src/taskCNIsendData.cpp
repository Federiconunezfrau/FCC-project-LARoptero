/*
 * taskCNIsendData.cpp
 *
 *  Created on: Feb 15, 2024
 *      Author: fede
 */

#include "taskCNIsendData.h"
#include "CNI.h"

void taskCNIsendData_constructor(taskCNIsendData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskCNIsendData_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mHandleMsg_ = handleMsg;
}

void taskCNIsendData_destructor(taskCNIsendData_t *me)
{
	//TODO: hay que hacer que si uno llama a la tarea nuevamente, la tarea simplemente no haga nada.
	timeTriggeredTask_destructor(&me->super);
}

void taskCNIsendData_start(taskCNIsendData_t *me)
{
	//TODO: acá podría uno decirle a la CNI que se va a encargar de enviar el mensaje
	// asociado al handleMsg.
}

void taskCNIsendData_update(taskCNIsendData_t *me)
{
	CNI_send_msg(me->mHandleMsg_);
}
