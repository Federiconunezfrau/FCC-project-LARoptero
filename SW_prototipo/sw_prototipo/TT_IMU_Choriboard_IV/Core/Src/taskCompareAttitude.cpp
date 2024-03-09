/*
 * taskCompareAttitude.cpp
 *
 *  Created on: Mar 6, 2024
 *      Author: fede
 */

#include "string.h"
#include <cmath>
#include "taskCompareAttitude.h"
#include "CNI.h"

#define LEN_CNI_DATA 8
#define LEN_COMPARE_ATTITUDE_CNI_DATA 24

void taskCompareAttitude_constructor(taskCompareAttitude_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, uint32_t handleMsgAttitude1, uint32_t handleMsgAttitude2, uint32_t handleMsgAttitude3, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskCompareAttitude_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mHandleMsgAttitude1_ = handleMsgAttitude1;
	me->mHandleMsgAttitude2_ = handleMsgAttitude2;
	me->mHandleMsgAttitude3_ = handleMsgAttitude3;
	me->mHandleMsg_ = handleMsg;
}


void taskCompareAttitude_destructor(taskCompareAttitude_t *me)
{
	//TODO: hay que hacer que si uno llama a la tarea nuevamente, la tarea simplemente no haga nada.
	timeTriggeredTask_destructor(&me->super);
}

void taskCompareAttitude_start(taskCompareAttitude_t *me)
{
	//TODO: acá podría uno decirle a la CNI que se va a encargar de enviar el mensaje
	// asociado al handleMsg.
}

void taskCompareAttitude_update(taskCompareAttitude_t *me)
{
	uint8_t dataCNI[LEN_CNI_DATA];
	uint8_t dataCNIcompareAttitude[LEN_COMPARE_ATTITUDE_CNI_DATA];
	uint32_t lenDataReadFromCNI = 0;
	float pitch1, roll1;
	float pitch2, roll2;
	float pitch3, roll3;
	float comparePitch12, compareRoll12;
	float comparePitch13, compareRoll13;
	float comparePitch23, compareRoll23;

	// Leer los datos de pitch y roll de las 3 placas, de la tabla CNI
	CNI_get_msg_content(me->mHandleMsgAttitude1_, dataCNI, LEN_CNI_DATA, &lenDataReadFromCNI);
	memcpy(&pitch1, dataCNI, 4);
	memcpy(&roll1, &dataCNI[4], 4);

	CNI_get_msg_content(me->mHandleMsgAttitude2_, dataCNI, LEN_CNI_DATA, &lenDataReadFromCNI);
	memcpy(&pitch2, dataCNI, 4);
	memcpy(&roll2, &dataCNI[4], 4);

	CNI_get_msg_content(me->mHandleMsgAttitude3_, dataCNI, LEN_CNI_DATA, &lenDataReadFromCNI);
	memcpy(&pitch3, dataCNI, 4);
	memcpy(&roll3, &dataCNI[4], 4);

	// Calcular los residuos 12, 13, 23
	comparePitch12 = fabs(pitch1 - pitch2);
	comparePitch13 = fabs(pitch1 - pitch3);
	comparePitch23 = fabs(pitch2 - pitch3);
	compareRoll12  = fabs(roll1  - roll2);
	compareRoll13  = fabs(roll1  - roll3);
	compareRoll23  = fabs(roll2  - roll3);

	// Convertir los datos en bytes
	memcpy(dataCNIcompareAttitude     , &comparePitch12 , 4);
	memcpy(&dataCNIcompareAttitude[4] , &compareRoll12  , 4);
	memcpy(&dataCNIcompareAttitude[8] , &comparePitch13 , 4);
	memcpy(&dataCNIcompareAttitude[12], &compareRoll13  , 4);
	memcpy(&dataCNIcompareAttitude[16], &comparePitch23 , 4);
	memcpy(&dataCNIcompareAttitude[20], &compareRoll23  , 4);

	// Actualizar los valores en la CNI
	CNI_update_msg_content(me->mHandleMsg_, dataCNIcompareAttitude, LEN_COMPARE_ATTITUDE_CNI_DATA);
}
