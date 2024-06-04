/*
 * faultInjectionIMU.cpp
 *
 *  Created on: Mar 16, 2024
 *      Author: fede
 */

#include <random>
#include "faultInjectionIMU.h"


#define BIAS_GYRO_X 10
#define BIAS_ACCEL_Z 0.5
#define BIAS_ACCEL_X 0.5

#define MIN_RND_TICK_FAULT_INJECT 200
#define MAX_RND_TICK_FAULT_INJECT 1000

#define DELAY_REMOVE_FAULT_INJECTION 10000

static std::default_random_engine generator;
static std::uniform_int_distribution<uint32_t> randomInt(MIN_RND_TICK_FAULT_INJECT, MAX_RND_TICK_FAULT_INJECT);
static std::uniform_int_distribution<int32_t> randomJump(-1000,1000);

void faultInjectorIMUbias_constructor(faultInjectorIMUbias_t *me, uint32_t initialDelay)
{
	stateMachine_constructor(&me->super, (stateHandler_t)faultInjectorIMUbias_initial);

	me->mDelayTicks_ = initialDelay;
}

state_t faultInjectorIMUbias_initial(faultInjectorIMUbias_t *me, event_t *e)
{
	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUbias_initial_delay);
}

state_t faultInjectorIMUbias_initial_delay(faultInjectorIMUbias_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUbias_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUbias_no_fault(faultInjectorIMUbias_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			//me->mDelayTicks_ = randomInt(generator);
			me->mDelayTicks_ = 50;
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUbias_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUbias_fault_injection(faultInjectorIMUbias_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = DELAY_REMOVE_FAULT_INJECTION;
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			((evTick_t*)e)->mIMUdata_->gyroX+=BIAS_GYRO_X;

			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUbias_no_fault);
			}

			return HANDLED;
	}

	return IGNORED;
}

void faultInjectorIMUrandomJump_constructor(faultInjectorIMUrandomJump_t *me, uint32_t initialDelay)
{
	stateMachine_constructor(&me->super, (stateHandler_t)faultInjectorIMUrandomJump_initial);

	me->mDelayTicks_ = initialDelay;
}

state_t faultInjectorIMUrandomJump_initial(faultInjectorIMUrandomJump_t *me, event_t *e)
{
	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUrandomJump_initial_delay);
}

state_t faultInjectorIMUrandomJump_initial_delay(faultInjectorIMUrandomJump_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUrandomJump_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUrandomJump_no_fault(faultInjectorIMUrandomJump_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = randomInt(generator);
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUrandomJump_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUrandomJump_fault_injection(faultInjectorIMUrandomJump_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = 2;
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			((evTick_t*)e)->mIMUdata_->gyroX+=randomJump(generator);
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUrandomJump_no_fault);
			}

			return HANDLED;
	}

	return IGNORED;
}

void faultInjectorIMUstuckAt_constructor(faultInjectorIMUstuckAt_t *me, uint32_t initialDelay)
{
	stateMachine_constructor(&me->super, (stateHandler_t)faultInjectorIMUstuckAt_initial);

	me->mDelayTicks_ = initialDelay;
}

state_t faultInjectorIMUstuckAt_initial(faultInjectorIMUstuckAt_t *me, event_t *e)
{
	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUstuckAt_initial_delay);
}

state_t faultInjectorIMUstuckAt_initial_delay(faultInjectorIMUstuckAt_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				me->mIMUdata_ = *(((evTick_t*)e)->mIMUdata_);
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUstuckAt_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUstuckAt_no_fault(faultInjectorIMUstuckAt_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = randomInt(generator);
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				me->mIMUdata_ = *(((evTick_t*)e)->mIMUdata_);
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUstuckAt_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUstuckAt_fault_injection(faultInjectorIMUstuckAt_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = DELAY_REMOVE_FAULT_INJECTION;
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			*(((evTick_t*)e)->mIMUdata_) = me->mIMUdata_;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUstuckAt_no_fault);
			}

			return HANDLED;
	}

	return IGNORED;
}

void faultInjectorIMUinconsistent_constructor(faultInjectorIMUinconsistent_t *me, uint32_t initialDelay)
{
	stateMachine_constructor(&me->super, (stateHandler_t)faultInjectorIMUinconsistent_initial);

	me->mDelayTicks_ = initialDelay;

}

state_t faultInjectorIMUinconsistent_initial(faultInjectorIMUinconsistent_t *me, event_t *e)
{
	return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUinconsistent_initial_delay);
}

state_t faultInjectorIMUinconsistent_initial_delay(faultInjectorIMUinconsistent_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUinconsistent_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUinconsistent_no_fault(faultInjectorIMUinconsistent_t *me, event_t *e)
{
	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = randomInt(generator);
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;
			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUinconsistent_fault_injection);
			}

			return HANDLED;
	}

	return IGNORED;
}

state_t faultInjectorIMUinconsistent_fault_injection(faultInjectorIMUinconsistent_t *me, event_t *e)
{
	float auxAccelX, auxAccelY;

	switch(e->mSignal_)
	{
		case SIG_ENTRY:
			me->mDelayTicks_ = DELAY_REMOVE_FAULT_INJECTION;
			return HANDLED;

		case SIG_TICK:
			me->mDelayTicks_--;

			auxAccelX = ((evTick_t*)e)->mIMUdata_->accelX;
			auxAccelY = ((evTick_t*)e)->mIMUdata_->accelY;

			((evTick_t*)e)->mIMUdata_->accelX = auxAccelX * (-0.70710678) - auxAccelY * 0.70710678;
			((evTick_t*)e)->mIMUdata_->accelY = auxAccelX *   0.70710678  - auxAccelY * 0.70710678;

			if(me->mDelayTicks_ == 0)
			{
				return stateMachine_transition((stateMachine_t*)me, (stateHandler_t)faultInjectorIMUinconsistent_no_fault);
			}

			return HANDLED;
	}

	return IGNORED;
}
