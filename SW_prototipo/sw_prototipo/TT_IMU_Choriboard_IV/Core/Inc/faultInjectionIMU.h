/*
 * faultInjectionIMU.h
 *
 *  Created on: Mar 16, 2024
 *      Author: fede
 */

#ifndef INC_FAULTINJECTIONIMU_H_
#define INC_FAULTINJECTIONIMU_H_

#include <random>
#include "stateMachine.h"
#include "iIMU.hpp"

typedef struct
{
	stateMachine_t super;
	uint32_t mDelayTicks_;
} faultInjectorIMUbias_t;

typedef struct
{
	stateMachine_t super;
	uint32_t mDelayTicks_;
} faultInjectorIMUrandomJump_t;

typedef struct
{
	stateMachine_t super;
	uint32_t mDelayTicks_;
	IMUData mIMUdata_;
} faultInjectorIMUstuckAt_t;

typedef struct
{
	stateMachine_t super;
	uint32_t mDelayTicks_;
} faultInjectorIMUinconsistent_t;

typedef struct
{
	event_t super;
	IMUData *mIMUdata_;
} evTick_t;

enum faultInjectorSignals : signal_t
{
	SIG_TICK = OFFSET_USER_DEFINED_SIGNALS
};

void    faultInjectorIMUbias_constructor     ( faultInjectorIMUbias_t *me, uint32_t initialDelay );
state_t faultInjectorIMUbias_initial         ( faultInjectorIMUbias_t *me, event_t *e );
state_t faultInjectorIMUbias_initial_delay   ( faultInjectorIMUbias_t *me, event_t *e );
state_t faultInjectorIMUbias_no_fault        ( faultInjectorIMUbias_t *me, event_t *e );
state_t faultInjectorIMUbias_fault_injection ( faultInjectorIMUbias_t *me, event_t *e );

void    faultInjectorIMUrandomJump_constructor     ( faultInjectorIMUrandomJump_t *me, uint32_t initialDelay );
state_t faultInjectorIMUrandomJump_initial         ( faultInjectorIMUrandomJump_t *me, event_t *e );
state_t faultInjectorIMUrandomJump_initial_delay   ( faultInjectorIMUrandomJump_t *me, event_t *e );
state_t faultInjectorIMUrandomJump_no_fault        ( faultInjectorIMUrandomJump_t *me, event_t *e );
state_t faultInjectorIMUrandomJump_fault_injection ( faultInjectorIMUrandomJump_t *me, event_t *e );

void    faultInjectorIMUstuckAt_constructor     ( faultInjectorIMUstuckAt_t *me, uint32_t initialDelay );
state_t faultInjectorIMUstuckAt_initial         ( faultInjectorIMUstuckAt_t *me, event_t *e );
state_t faultInjectorIMUstuckAt_initial_delay   ( faultInjectorIMUstuckAt_t *me, event_t *e );
state_t faultInjectorIMUstuckAt_no_fault        ( faultInjectorIMUstuckAt_t *me, event_t *e );
state_t faultInjectorIMUstuckAt_fault_injection ( faultInjectorIMUstuckAt_t *me, event_t *e );

void    faultInjectorIMUinconsistent_constructor     ( faultInjectorIMUinconsistent_t *me, uint32_t initialDelay );
state_t faultInjectorIMUinconsistent_initial         ( faultInjectorIMUinconsistent_t *me, event_t *e );
state_t faultInjectorIMUinconsistent_initial_delay   ( faultInjectorIMUinconsistent_t *me, event_t *e );
state_t faultInjectorIMUinconsistent_no_fault        ( faultInjectorIMUinconsistent_t *me, event_t *e );
state_t faultInjectorIMUinconsistent_fault_injection ( faultInjectorIMUinconsistent_t *me, event_t *e );


#endif /* INC_FAULTINJECTIONIMU_H_ */
