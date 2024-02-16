/*
 * task.h
 *
 *  Created on: Jan 29, 2024
 *      Author: fede
 */

#ifndef INC_INTERFACE_TTTASK_H_
#define INC_INTERFACE_TTTASK_H_

#include <stdint.h>

class interface_TTtask
{
	public:
		virtual void init(void) = 0;
		virtual void update(void) = 0;
		//virtual void dispatch(interface_event *) = 0;

	protected:
		interface_TTtask(uint32_t, uint32_t, uint32_t, uint32_t);

		uint32_t mDelayTicks_;
		uint32_t mPeriodTicks_;
		uint32_t mWcetMicroSeconds_;
		uint32_t mBcetMicroSeconds_;
};


#endif /* INC_INTERFACE_TTTASK_H_ */
