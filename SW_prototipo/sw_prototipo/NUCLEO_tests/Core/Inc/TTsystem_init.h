/*
 * TTsystem.h
 *
 *  Created on: Jan 31, 2024
 *      Author: fede
 */

#ifndef INC_TTSYSTEM_H_
#define INC_TTSYSTEM_H_

#include "stdint.h"

// Enumerativo que define los posibles modos del sistema
enum systemMode_t : uint32_t
{
	NORMAL
};

systemMode_t system_identify_mode(void);


#endif /* INC_TTSYSTEM_H_ */
