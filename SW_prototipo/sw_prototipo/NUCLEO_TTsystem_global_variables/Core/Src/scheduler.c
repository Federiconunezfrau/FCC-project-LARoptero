/***************************************************************************//**
 
 @file		Source_Template.c
 
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

/** @addtogroup Module_Name
    @{
 */

/*******************************************************************************
	INCLUDED FILES
*******************************************************************************/
#include "scheduler.h"

#include "tim.h"

/*******************************************************************************
	PRIVATE CONSTANTS
*******************************************************************************/

/** @cond DEV
    @addtogroup Module_Name_Private_Constants
    @{
 */

/** Possible states for the scheduler */
typedef enum
{
    ST_STOP = 0,
    ST_RUNNING
} schedulerState_t;

/** @}
 */

/*******************************************************************************
	PRIVATE STRUCTURES
*******************************************************************************/

/** @addtogroup Module_Name_Private_Structures
    @{
 */

/** Structure used for defining a task's characteristics. */
typedef struct
{
    /** Pointer to the task function (must be a 'void (void)' function) */
    void (*m_pTaskHandler) (void);

    /** Delay (ticks) until the task will (next) be run. */
    uint32_t m_delayTicks;

    /** Interval (ticks) between subsequent runs. */
    uint32_t m_periodTicks;

    /** Worst-case execution time in microseconds */
    uint32_t m_WCETus;

    /** Best-case execution time in microseconds */
    uint32_t m_BCETus;

} task_t;

/** @}
 */

/*******************************************************************************
	GLOBAL VARIABLES
*******************************************************************************/

/** @addtogroup Module_Name_Global_Variables
    @{
 */

/** Pointer to the timer used for the scheduler */
static TIM_HandleTypeDef *mHtim_;

/** Variable used for saving the current state of the scheduler. */
// static schedulerState_t schedulerState = ST_STOP;

/** The task list used by the scheduler. It can contain at most MAX_NUMBER_TASKS. */
static task_t schedulerTaskList[MAX_NUMBER_TASKS];

/** Global variable, visible to all files, used for reporting errors to the scheduler. */
schedulerStatus_t schedulerErrorCode;


/** The current tick count */
static volatile uint32_t timebaseTickCount = 0;

/** Flag used for telling the scheduler that a tick event occured. */
volatile uint32_t tickEventsPending = 0;

/** @}
 */

/*******************************************************************************
	PRIVATE MACROS
*******************************************************************************/

/** @addtogroup Module_Name_Private_Macros
    @{
 */

// Nothing defined yet

/** @}
 */

/*******************************************************************************
	PRIVATE FUNCTIONS PROTOTYPES
*******************************************************************************/
static void scheduler_go_to_sleep             ( void );
static void scheduler_delete_task_cooperative ( uint32_t indexTask );

static void timebase_init  ( void );
static void timebase_start ( void );
static void timebase_stop  ( void );

/*******************************************************************************
	PRIVATE FUNCTIONS
*******************************************************************************/

/** @addtogroup Module_Name_Private_Functions
    @{   
 */

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		a	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		w	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
static void scheduler_go_to_sleep(void)
{
    __asm__("wfi");
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		a	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		w	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
static void scheduler_delete_task_cooperative(uint32_t indexTask)
{
    schedulerTaskList[indexTask].m_pTaskHandler = 0;
    schedulerTaskList[indexTask].m_delayTicks = 0;
    schedulerTaskList[indexTask].m_periodTicks = 0;
    schedulerTaskList[indexTask].m_WCETus = 0;
    schedulerTaskList[indexTask].m_BCETus = 0;
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		a	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		w	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
static void timebase_init(void)
{
    timebaseTickCount = 0;
    tickEventsPending = 0;
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		a	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		w	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
static void timebase_start(void)
{
	HAL_TIM_Base_Start_IT(mHtim_);
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		a	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		w	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
static void timebase_stop(void)
{
    timer_disable_counter(TIMEBASE_PERIPHERAL);
    nvic_disable_irq(TIMBEASE_TICK_NVIC_HANDLER);
}

/** @}
    @endcond
 */

/*******************************************************************************
	PUBLIC FUNCTIONS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Functions
    @{
 */

/***************************************************************************//**

 @brief		Initialization function for the scheduler.

 @details	Resets the tasks array, resets the error code variable, configures the periodic timer tick and initializes the execution time monitor. 

*******************************************************************************/
void scheduler_init(void)
{
    uint32_t i;

    // if(schedulerState != ST_STOP)
    // {
    //     schedulerErrorCode = ERROR_SCHEDULER_ALREADY_RUNNING;
    //     return;
    // }

    for(i = 0; i < MAX_NUMBER_TASKS; i++)
    {
        scheduler_delete_task_cooperative(i);
    }

    schedulerErrorCode = OK;

    timebase_init();
    //monitor_init();
}

/***************************************************************************//**

 @brief		Starts running the scheduler.

 @details	Enables the timer's timebase and activates its associated interrupt.

*******************************************************************************/
void scheduler_start(void)
{
    // if(schedulerState != ST_STOP)
    // {
    //     schedulerErrorCode = ERROR_SCHEDULER_ALREADY_RUNNING;
    //     return;
    // }

    timebase_start();
    // schedulerState = ST_RUNNING;
}

/***************************************************************************//**

 @brief		Stops scheduler execution.

 @details	Stops the timebase execution

*******************************************************************************/
void scheduler_stop(void)
{
    // if(schedulerState == ST_STOP)
    // {
    //     schedulerErrorCode = ERROR_SCHEDULER_NOT_RUNNING;
    //     return;
    // }  

    timebase_stop();
    // schedulerState = ST_STOP;
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		d	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		z	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
void scheduler_dispatch(void)
{   
    uint32_t i;
    uint8_t updateRequired = 0;

    //cm_disable_interrupts();
    if(tickEventsPending > 0)
    {
        tickEventsPending--;
        updateRequired = 1;
    }
    //cm_enable_interrupts();

    while(updateRequired)
    {
        for(i = 0; i < MAX_NUMBER_TASKS; i++)
        {
            if(schedulerTaskList[i].m_pTaskHandler)
            {
                if( (--(schedulerTaskList[i].m_delayTicks)) == 0)
                {
                    (*schedulerTaskList[i].m_pTaskHandler)();

                    schedulerTaskList[i].m_delayTicks = schedulerTaskList[i].m_periodTicks;
                }
            }         
        }
        
        //cm_disable_interrupts();
        if(tickEventsPending > 0)
        {
            tickEventsPending--;
            updateRequired = 1;
        }
        else
        {
            updateRequired = 0;
        }
        //cm_enable_interrupts();
    }

   scheduler_go_to_sleep();
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		d	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		z	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
uint32_t scheduler_add_task_cooperative(void (*pTaskHandler)(void), const uint32_t delayTicks, const uint32_t periodTicks, const uint32_t WCETus, const uint32_t BCETus)
{
    uint32_t errorCode = 0;
    uint32_t i = 0;

    while ((schedulerTaskList[i].m_pTaskHandler != 0) && (i < MAX_NUMBER_TASKS))
    {
        i++;
    }

    if(i == MAX_NUMBER_TASKS)
    {
        schedulerErrorCode = ERROR_TASKS_LIST_FULL;
        errorCode = MAX_NUMBER_TASKS;
    }
      
    if(periodTicks == 0)
    {
        schedulerErrorCode = ERROR_ONE_SHOT_TASK;
        errorCode = MAX_NUMBER_TASKS;
    }

    if(errorCode != MAX_NUMBER_TASKS)
    {
        schedulerTaskList[i].m_pTaskHandler  = pTaskHandler;
        schedulerTaskList[i].m_delayTicks    = delayTicks + 1;
        schedulerTaskList[i].m_periodTicks   = periodTicks;
        schedulerTaskList[i].m_WCETus        = WCETus;
        schedulerTaskList[i].m_BCETus        = BCETus;

        errorCode = i;
    }

    return errorCode;
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		d	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		z	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
void scheduler_set_timer(TIM_HandleTypeDef *tim)
{
	mHtim_ = tim;
}

/***************************************************************************//**

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		d	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		z	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
void tim5_isr(void)
{
    timer_clear_flag(TIMEBASE_PERIPHERAL, TIM_SR_UIF);
    
    if(timebaseTickCount >= MAX_TIMEBASE_COUNT)
    {
        timebaseTickCount = 0;
    }
    else
    {
        timebaseTickCount++;
    }

    tickEventsPending++;
}

/** @}
 */


/** @}
 */
