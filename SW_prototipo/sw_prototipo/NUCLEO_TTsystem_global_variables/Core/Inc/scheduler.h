/***************************************************************************//**
 
 @file		Header_Template.h
	
 @brief		Brief description of the file's contents.

 @details	Detailed description.

 @version	Version number.

 @author	Name of the person(s) who had created/modified this file.
 @author	Add one "author" command for each person.
 @author	TCA - Testing Development Group
 @author	Delphi Electronics & Safety - Famar Fueguina S.A.
 @author	Buenos Aires, Argentina

 @date		Date

*******************************************************************************/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

/** @addtogroup Module_Name
 * @{
 */

/*******************************************************************************
	INCLUDED FILES
*******************************************************************************/

/*******************************************************************************
	PUBLIC CONSTANTS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Constants
    @{
 */

/** The number of ticks up to which the scheduler becomes periodic. */
#define HYPERPERIOD 2000

/** The timer peripheral used for generating the timebase */
#define TIMEBASE_PERIPHERAL TIM5

/** The timer's timebase prescale value. The timer's clock frequency is divided by TIMEBASE_PRESCALE + 1 */
#define TIMEBASE_PRESCALE 0

/** The timer's timebase period, in ticks. */
#define TIMEBASE_PERIOD 35999

/** This is the NVIC handler associated with the tick interrupt of the scheduler. */
#define TIMBEASE_TICK_NVIC_HANDLER NVIC_TIM5_IRQ



/** The max number of tasks to me handled by the scheduler */
#define MAX_NUMBER_TASKS 10


/** Scheduler error codes. These are used for reporting when an error occurred with a task execution, and take an action as a response. */
typedef enum
{
    OK = 0, 
	ERROR_TASKS_LIST_FULL,
	ERROR_ONE_SHOT_TASK,
	ERROR_SCHEDULER_ALREADY_RUNNING,
	ERROR_SCHEDULER_NOT_RUNNING
} schedulerStatus_t;

/** @}
 */

/*******************************************************************************
	PUBLIC STRUCTURES
*******************************************************************************/

/** @addtogroup Module_Name_Public_Structs
    @{
 */

// Nothing defined yet

/** @}
 */

/*******************************************************************************
	PUBLIC MACROS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Macros
    @{
 */

// Nothing defined yet

/** @}
 */

/*******************************************************************************
	PUBLIC FUNCTIONS PROTOTYPES
*******************************************************************************/
void scheduler_init     ( void );
void scheduler_start    ( void );
void scheduler_stop     ( void );
void scheduler_dispatch ( void );

uint32_t scheduler_add_task_cooperative ( void (*pTaskHandler)(void), const uint32_t delayTicks, const uint32_t periodTicks, const uint32_t WCETus, const uint32_t BCETus );


/** @}
 */
 
 #endif
