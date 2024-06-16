/*
 * system.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */

#include "system.h"
#include <stdio.h>


#define MSG_WHO_AM_I "Ingrese un comando"

#define LEN_AUX_BUFFER MAX_PACKET_SIZE_PCIF

static PCIFmsg_t msgWhoAmI = {MSG_WHO_AM_I,sizeof(MSG_WHO_AM_I)-1,whoAmI};

void system_constructor(system_t *me, evBuffer *theEventQueue, timeTriggeredScheduler_t *theTTscheduler, myUART_t *myUART, gpio *ledFailSilent)
{
	stateMachine_constructor(&me->super, (stateHandler_t)&system_initial);
	me->mEventQueue_ = theEventQueue;
	me->mTTscheduler_ = theTTscheduler;
	me->mMyUART_ = myUART;
	me->mLEDFailSilent_ = ledFailSilent;
}

state_t system_initial(system_t *me, event_t const e)
{
	// [TODO]: ver si entré acá por el watchdog. En ese caso, se pasaría
	// automáticamente al modo fail-silent

	if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		__HAL_RCC_CLEAR_RESET_FLAGS();
		return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) system_fail_safe_mode);
	}

	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) system_idle);
}


state_t system_idle(system_t *me, event_t const e)
{
	uint8_t auxBuffer[LEN_AUX_BUFFER];
	uint8_t msgLen;
	uint8_t msgCmd;
	uint32_t auxLenMsg;
	PCIFmsg_t auxPCIFmsg;
	uint16_t bytesRead;

	switch(e)
	{
		case EV_ENTRY:
			// Se habilita la interrupción por UART, para recibir mensajes y
			// actuar en consecuencia
			myUART_enable_rx(me->mMyUART_);
			return HANDLED;

		case EV_SYSTEM_NEW_MSG:

			myUART_disable_rx(me->mMyUART_);

			// Se lee el primer byte. Este contiene info acerca de cuántos bytes tiene el mensaje
			if( (bytesRead = myUART_read(me->mMyUART_,&msgLen, 1)) != 1 )
			{
				myUART_enable_rx(me->mMyUART_);
				return IGNORED;
			}

			auxBuffer[0] = msgLen;

			// Se lee el resto de bytes de la uart
			if(msgLen > 1)
			{
				if( myUART_read_blocking(me->mMyUART_, &auxBuffer[1], msgLen-1) != 0 )
				{
					myUART_enable_rx(me->mMyUART_);
					return IGNORED;
				}

				bytesRead += msgLen-1;

				//bytesRead += myUART_read(me->mMyUART_,&auxBuffer[1], msgLen-1);
				if(bytesRead == msgLen)
				{
					// Se procesa el comando
					msgCmd = auxBuffer[1];
					switch(msgCmd)
					{
						case whoAmI:
							me->mTxBuffer_.write(msgWhoAmI);
							me->mEventQueue_->write(EV_SYSTEM_SEND_MSG);
							break;

						case runNormalMode:
							me->mEventQueue_->write(EV_SET_NORMAL_MODE);
							break;

						case getFloatSize:
							//aux = sizeof(float);
							break;

						default:
							break;
					}
				}
			}

			myUART_enable_rx(me->mMyUART_);
			return HANDLED;

		case EV_SYSTEM_SEND_MSG:
			if(!me->mTxBuffer_.is_empty())
			{
				me->mTxBuffer_.read(&auxPCIFmsg);
				PCIFmsg_to_bytes(&auxPCIFmsg, auxBuffer, LEN_AUX_BUFFER, &auxLenMsg);
				myUART_send(me->mMyUART_, auxBuffer, auxLenMsg);
			}

			return HANDLED;

		case EV_SET_NORMAL_MODE:
			myUART_disable_rx(me->mMyUART_);
			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) system_normal_mode);
	}
	return IGNORED;
}

state_t system_fail_safe_mode(system_t *me, event_t const e)
{
	me->mLEDFailSilent_->write(GPIO_ST::HIGH);

	while(1)
	{
		// El programa se queda acá, no puede salir. Es la idea justamente
	}

	return IGNORED;
}


state_t system_normal_mode(system_t *me, event_t const e)
{
	switch(e)
	{
		case EV_ENTRY:
			// Al entrar acá, el sistema ejecuta el scheduler y se bloquea en un
			// loop infinito

			timeTriggeredScheduler_start(me->mTTscheduler_);

			while(1)
			{
				timeTriggeredScheduler_dispatch(me->mTTscheduler_);
			}

			return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) system_idle);
	}

	return IGNORED;
}


uint16_t system_has_new_msg(system_t *me)
{
	return myUART_bytes_in_rx_buffer(me->mMyUART_);
}