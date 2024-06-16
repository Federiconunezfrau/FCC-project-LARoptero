/*
 * taskCNIreceiveData.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: fede
 */

#include "taskCNIreceiveData.h"
#include "CNI.h"

void taskCNIreceiveData_constructor(taskCNIreceiveData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskCNIreceiveData_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mHandleMsg_ = handleMsg;
}

void taskCNIreceiveData_destructor(taskCNIreceiveData_t *me)
{
	//TODO: hay que hacer que si uno llama a la tarea nuevamente, la tarea simplemente no haga nada.
	timeTriggeredTask_destructor(&me->super);
}

void taskCNIreceiveData_start(taskCNIreceiveData_t *me)
{
	//TODO: acá podría uno decirle a la CNI que se va a encargar de enviar el mensaje
	// asociado al handleMsg.
}

void taskCNIreceiveData_update(taskCNIreceiveData_t *me)
{
	CNI_receive_msg(me->mHandleMsg_);
}



