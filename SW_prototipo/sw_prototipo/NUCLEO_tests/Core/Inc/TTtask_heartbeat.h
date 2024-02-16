/*
 * task_heartbeat.h
 *
 *  Created on: Jan 29, 2024
 *      Author: fede
 */

#ifndef INC_TASK_HEARTBEAT_H_
#define INC_TASK_HEARTBEAT_H_

#include "interface_TTtask.h"
#include "port.hpp"

class task_heartbeat : public interface_TTtask
{
	public:
		task_heartbeat(uint32_t, uint32_t, uint32_t, uint32_t, gpio *);

		void init(void) override;
		void update(void) override;

	private:
		gpio *mLED_;
};

#endif /* INC_TASK_HEARTBEAT_H_ */
