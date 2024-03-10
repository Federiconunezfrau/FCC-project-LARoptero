/*
 * systemFunctions.h
 *
 *  Created on: Dec 29, 2023
 *      Author: fede
 */

#ifndef INC_SYSTEMFUNCTIONS_H_
#define INC_SYSTEMFUNCTIONS_H_

typedef struct
{
	CAN_HandleTypeDef *masterCANhandler;
} Dut_t;

void system_init(Dut_t *);
void system_run(void);


#endif /* INC_SYSTEMFUNCTIONS_H_ */
