/*
 * localClock.cpp
 *
 *  Created on: Jan 7, 2024
 *      Author: fede
 */


#include "localClock.h"

/** La implementación del local clock utilizada en la choriboard IV, utiliza un timer de 32 bits.
 *  Este timer cuneta indefinidamente, sin tener interrupciones (se asume que el tiempo de vuelo
 *  nunca superará la cantidad máxima de microsegundos que puede contar un timer de 32 bits, es
 *  decir, 2^32 us = 72 hs.
 *  Cuando se solicita un timestamp, se retorna simplemente el valor de 32 bits de la cuenta
 *  del clock (esto es diferente a cuando se quiere utilizar un clock de 16 bits, ya que
 *  2^16 us = 65.536 ms, es muchísimo menos que 72 hs. Para poder contar hasta valores más grandes,
 *  utilizando un clock de 16 bits, se deben tomar otros recaudos. */

void localClock_constructor(localClock_t *me, TIM_HandleTypeDef *htim, STM32::gpio *ledClockRunning)
{
	me->m_htim = htim;
	me->m_ledClockRunning = ledClockRunning;
}

void localClock_start(localClock_t *me)
{
	me->m_ledClockRunning->write(GPIO_ST::HIGH);
	//__HAL_TIM_SET_COUNTER(me->m_htim, 0);
	HAL_TIM_GenerateEvent(me->m_htim, TIM_EVENTSOURCE_UPDATE);
	HAL_TIM_Base_Start_IT(me->m_htim);
}

void localClock_stop(localClock_t *me)
{
	HAL_TIM_Base_Stop_IT(me->m_htim);
	me->m_ledClockRunning->write(GPIO_ST::LOW);
}

timestamp_t localClock_get_timestamp(localClock_t *me)
{
	timestamp_t timestamp;

	timestamp.timeStampCounter = __HAL_TIM_GET_COUNTER(me->m_htim);

	return timestamp;
}
