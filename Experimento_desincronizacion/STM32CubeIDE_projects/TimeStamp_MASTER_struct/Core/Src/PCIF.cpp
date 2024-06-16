/*
 * computerIF.cpp
 *
 *  Created on: Dec 28, 2023
 *      Author: fede
 */

#include "PCIF.h"
#include "systemEvents.h"
#include "system.h"
#include "usbd_cdc_if.h"
#include "main.h"
#include "gpio_STM32.hpp"

#define MAX_CONSECUTIVE_TRANSFERS 3
#define MAX_CONSECUTIVE_MSG_PROCESSING 3

static PCIFmsg_t msgWhoAmI = {"Soy el CAN master",sizeof("Soy el CAN master")-1,whoAmI};
static PCIFmsg_t welcome = {"CAN master esperando comandos", sizeof("CAN master esperando comandos")-1, welcomeMsg};
//static char welcomeString[64] = "CAN master esperando comandos\r\n";
static CANmsg_t msgToForward;

// Este flag se setea en 1 cada vez que se va a enviar un dato. Hay una interrupción que se llama cuando se terminó de enviar
// que setea este flag en 0 de nuevo, para indicar que ya es posible seguir enviando datos.
volatile uint8_t flagUSBsendBusy = 0;

STM32::gpio redLED(PORT_RED_LED, PIN_RED_LED);
uint8_t flagBusy = 0;

void PCIF_constructor(PCIF_t *me)
{
	stateMachine_constructor(&me->super, (stateHandler_t)&PCIF_initial);
}

state_t PCIF_initial(PCIF_t *me)
{
	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t) PCIF_active);
}

state_t PCIF_active(PCIF_t *me, event_t const e)
{
	uint32_t i;
	PCIFmsg_t msg;
	uint8_t buf[MAX_PACKET_SIZE_PCIF];

	switch(e)
	{
		case EV_ENTRY:
			p_PCIFTxBuffer->write(welcome);
			return HANDLED;

		case EV_COMPUTERIF_SEND_MSG:
			i = 0;

			while( (flagUSBsendBusy == 0) && (!(p_PCIFTxBuffer->is_empty())) && (i < MAX_CONSECUTIVE_TRANSFERS))
			{
				p_PCIFTxBuffer->read(&msg);

				// Setea el command
				buf[0] = msg.cmd;

				// Setea el len del payload
				buf[1] = msg.lenPayload;

				// Copia el payload
				memcpy(&buf[2], msg.payload, msg.lenPayload);

				// Se appendea el \r y \n
				buf[2+msg.lenPayload]   = '\r';
				buf[2+msg.lenPayload+1] = '\n';

				if( CDC_Transmit_FS(buf, msg.lenPayload+4) == 1 )
				{
					if( flagBusy == 0 )
					{
						redLED.write(GPIO_ST::HIGH);
						flagBusy = 1;
					}
				}
				i++;
			}

			return HANDLED;

		case EV_COMPUTERIF_PROCESS_NEW_MSG:
			i = 0;
			while( (p_PCIFRxBuffer->read(&msg) != 0) && (i < MAX_CONSECUTIVE_MSG_PROCESSING) )
			{
				switch(msg.cmd)
				{
					case whoAmI:
						p_PCIFTxBuffer->write(msgWhoAmI);
						break;

					case enableCANbus:
						p_eventsBuffer->write(EV_CAN_TURN_ON);
						break;

					case disableCANbus:
						p_eventsBuffer->write(EV_CAN_TURN_OFF);
						break;

					case startTimeStampService:
						p_eventsBuffer->write(EV_START_TIMESTAMP_SERVICE);
						break;

					case stopTimeStampService:
						p_eventsBuffer->write(EV_STOP_TIMESTAMP_SERVICE);
						break;

					case forwardCANmsg:
						if( CANmsg_set_payload(&msgToForward, msg.payload, msg.lenPayload) == true )
						{
							msgToForward.id = forwardedData;
							p_canTxBuffer->write(msgToForward);
						}
						break;

					case activateCANsniffer:
						p_eventsBuffer->write(EV_CAN_ACTIVATE_SNIFFER);
						break;

					case deactivateCANsniffer:
						p_eventsBuffer->write(EV_CAN_DEACTIVATE_SNIFFER);
						break;

					case dataTimeStampService:
						p_eventsBuffer->write(EV_REQUEST_TIMESTAMP);

					default:
						break;
				}
				i++;
			}

			return HANDLED;
	}

	return IGNORED;

}
