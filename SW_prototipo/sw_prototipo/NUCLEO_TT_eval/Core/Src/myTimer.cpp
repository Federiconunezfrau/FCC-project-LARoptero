/*
 * myTimer.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: fede
 */

#include "myTimer.h"

void myTimer_constructor(myTimer_t *me, TIM_HandleTypeDef *htim)
{
	subject_constructor(&me->super);
	me->mHtim_ = htim;
}

/*void myTimer_destructor(myTimer_t *me)
{

}*/

