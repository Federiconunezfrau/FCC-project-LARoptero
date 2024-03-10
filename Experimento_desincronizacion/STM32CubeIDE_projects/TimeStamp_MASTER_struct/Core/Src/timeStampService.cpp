/*
 * timeStampService.cpp
 *
 *  Created on: Jan 4, 2024
 *      Author: fede
 */

/*
 * task_CANsender.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: fede
 */

#include <cstring>
#include "timeStampService.h"
#include "systemEvents.h"
#include "system.h"

/** Si este define es 1, el timestamp service activa una interrupción y manualmente le envía mensajes
 *  a los slaves, preguntándoles por sus timestamps. Si vale 0, se desactiva esto, asumiendo que los
 *  slaves enviarán el valor de timestamp cuando ellos quieran. */
#define ASK_FOR_TIMESTAMPS_PERIODICALLY 1

extern volatile uint8_t flagRequestTimestampData;

static CANmsg_t msgTimeStampServiceStart = {{},0,timeStampServiceStart,0x00};
static CANmsg_t msgTimeStampServiceStop = {{},0,timeStampServiceStop,0x00};
static CANmsg_t msgTimeStampServiceData = {{},0,timeStampServiceData,0x00};


//void timeStampService_constructor(timeStampService_t *me, TIM_HandleTypeDef *htim, STM32::gpio *ledIndicator)
void timeStampService_constructor(timeStampService_t *me, TIM_HandleTypeDef *htim)
{
	stateMachine_constructor(&me->super, (stateHandler_t)&timeStampService_initial);
	me->m_htim = htim;
	//me->m_ledIndicator = ledIndicator;
}


state_t timeStampService_initial(timeStampService_t *me)
{
	//me->m_ledIndicator->write(GPIO_ST::LOW);
	flagRequestTimestampData = 0;

	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) timeStampService_off);
}

state_t timeStampService_off(timeStampService_t *me, event_t const e)
{
	switch(e)
	{
		case EV_START_TIMESTAMP_SERVICE:
			if( p_canTxBuffer->write(msgTimeStampServiceStart) == EXIT_OK )
			{
				//me->m_ledIndicator->write(GPIO_ST::HIGH);
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) timeStampService_running);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t timeStampService_running(timeStampService_t *me, event_t const e)
{
	switch(e)
	{
		case EV_ENTRY:

#if ASK_FOR_TIMESTAMPS_PERIODICALLY == 1
			flagRequestTimestampData = 0;
			HAL_TIM_GenerateEvent(me->m_htim, TIM_EVENTSOURCE_UPDATE);
			HAL_TIM_Base_Start_IT(me->m_htim);
#endif

			return HANDLED;

		case EV_EXIT:

#if ASK_FOR_TIMESTAMPS_PERIODICALLY == 1
			HAL_TIM_Base_Stop_IT(me->m_htim);

#endif
			return HANDLED;

		case EV_STOP_TIMESTAMP_SERVICE:
			if( p_canTxBuffer->write(msgTimeStampServiceStop) == EXIT_OK )
			{
				//me->m_ledIndicator->write(GPIO_ST::LOW);
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) timeStampService_off);
			}
			return HANDLED;

		case EV_REQUEST_TIMESTAMP:
			p_canTxBuffer->write(msgTimeStampServiceData);
			return HANDLED;
	}
	return IGNORED;
}

#if ASK_FOR_TIMESTAMPS_PERIODICALLY == 1
/** Callback para la interrupción del timer del servicio de timestamps.
	Esta interrupción sirve para pedir los próximos datos de timestamps. */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(flagRequestTimestampData == 0)
	{
		flagRequestTimestampData = 1;
	}
}
#endif
