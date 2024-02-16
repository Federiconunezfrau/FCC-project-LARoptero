/*
 * iTTtask.c
 *
 *  Created on: Jan 29, 2024
 *      Author: fede
 */

#include "interface_TTtask.h"

interface_TTtask::interface_TTtask(uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds)
{
	mDelayTicks_ = delayTicks;
	mPeriodTicks_ = periodTicks;
	mWcetMicroSeconds_ = wcetMicroSeconds;
	mBcetMicroSeconds_ = bcetMicroSeconds;
}
