/*
 * system.h
 *
 *  Created on: Feb 2, 2024
 *      Author: fede
 */

#ifndef INC_SYSTEM_H_v
#define INC_SYSTEM_H_

#include "iStateMachine.h"
#include "usart.h"
#include "utils.hpp"

#define SIZE_BUFFER_RX_MSGS 3

class system : public iStateMachine
{
	public:
		//system(UART_HandleTypeDef*);
		system(evBuffer *);
		void pass_msg(PCIFmsg_t *);

	private:
		static state_t initial        ( system*, event_t const );
		static state_t idle           ( system*, event_t const );
		static state_t fail_safe_mode ( system*, event_t const );
		static state_t normal_mode    ( system*, event_t const );

		circ_buffer<PCIFmsg_t, SIZE_BUFFER_RX_MSGS> rxMsgsBuffer;

		evBuffer *mEventQueue_;
};


#endif /* INC_SYSTEM_H_ */
