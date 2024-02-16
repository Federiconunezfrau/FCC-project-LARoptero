/*
 * task_heartbeat.cpp
 *
 *  Created on: Jan 30, 2024
 *      Author: fede
 */

#include "TTtask_heartbeat.h"

task_heartbeat::task_heartbeat(uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, gpio *led) : interface_TTtask(delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds)
{
	mLED_ = led;
}


void task_heartbeat::init(void)
{
	mLED_->write(GPIO_ST::LOW);
}


void task_heartbeat::update(void)
{
	mLED_->toggle();
}
