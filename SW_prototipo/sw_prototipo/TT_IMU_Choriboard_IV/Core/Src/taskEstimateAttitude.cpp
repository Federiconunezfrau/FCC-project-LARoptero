/*
 * taskEstimateAttitude.cpp
 *
 *  Created on: Mar 3, 2024
 *      Author: fede
 */

#include "string.h"
#include "taskEstimateAttitude.h"
#include "attitudeEstimator.h"
#include "CNI.h"

#define LEN_ATTITUDE_CNI_DATA 8

void taskEstimateAttitude_constructor(taskEstimateAttitude_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskEstimateAttitude_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mHandleMsg_ = handleMsg;
}

void taskEstimateAttitude_destructor(taskEstimateAttitude_t *me)
{
	//TODO: hay que hacer que si uno llama a la tarea nuevamente, la tarea simplemente no haga nada.
	timeTriggeredTask_destructor(&me->super);
}

void taskEstimateAttitude_start(taskEstimateAttitude_t *me)
{
	//TODO: acá podría uno decirle a la CNI que se va a encargar de enviar el mensaje
	// asociado al handleMsg.
}

void taskEstimateAttitude_update(taskEstimateAttitude_t *me)
{
	attitudeData_t outputData;
	uint8_t dataForCni[LEN_ATTITUDE_CNI_DATA];

	attitudeEstimator_update();

	// Convertir los datos del estimador en bytes
	attitudeEstimator_read_data(&outputData);
	memcpy(dataForCni    , &outputData.mPitch_, 4);
	memcpy(&dataForCni[4], &outputData.mRoll_ , 4);

	// Actualizarlos en la CNI
	CNI_update_msg_content(me->mHandleMsg_, dataForCni, LEN_ATTITUDE_CNI_DATA);
}
