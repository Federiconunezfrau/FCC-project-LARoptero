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
#include "system.h"
#include "systemModeNormal.h"
#include "scheduler.h"
#include "taskWatchdog.h"
#include "taskHeartbeat.h"
#include "taskFailSilentLed.h"
#include "utils.h"

/*******************************************************************************
	PRIVATE CONSTANTS
*******************************************************************************/

/** @cond DEV
    @addtogroup Module_Name_Private_Constants
    @{
 */

// Nothing defiend yet

/** @}
 */

/*******************************************************************************
	PRIVATE STRUCTURES
*******************************************************************************/

/** @addtogroup Module_Name_Private_Structures
    @{
 */

// Nothing defiend yet

/** @}
 */

/*******************************************************************************
	GLOBAL VARIABLES
*******************************************************************************/

/** @addtogroup Module_Name_Global_Variables
    @{
 */

/** Global variable, visible to all files, used for setting the system mode. */
static systemMode_t systemMode;

/** @}
 */

/*******************************************************************************
	PRIVATE MACROS
*******************************************************************************/

/** @addtogroup Module_Name_Private_Macros
    @{
 */

// Nothing defiend yet

/** @}
 */

/*******************************************************************************
	PRIVATE FUNCTIONS PROTOTYPES
*******************************************************************************/
static void system_identify_required_mode  ( void );
static void system_configure_required_mode ( void );
static void system_perform_safe_shutdown   ( void );

/*******************************************************************************
	PRIVATE FUNCTIONS
*******************************************************************************/

/** @addtogroup Module_Name_Private_Functions
    @{   
 */

/***************************************************************************//**

 @brief		Checks conditions and detects the required system mode.

 @details	Detailed description. May span more than one line.

*******************************************************************************/
static void system_identify_required_mode(void)
{
	systemMode = NORMAL;
}

/***************************************************************************//**

 @brief		Configures the system mode according.

 @details	The system mode is saved in variable "systemMode". This function performs different configuration optiosn depending on this variable. The default behaviour is assuming a fail silent condition.

*******************************************************************************/
static void system_configure_required_mode(void)
{
    switch(systemMode)
    {
    	default:
		case NORMAL:
		scheduler_init();

		task_watchdog_init();
		task_heartbeat_init();
		//task_generate_message_init();
		//task_can_init();
		//task_uart_init();

		scheduler_add_task_cooperative(task_watchdog_update, DELAY_TASK_WATCHDOG_TICKS_NORMAL, PERIOD_TASK_WATCHDOG_TICKS_NORMAL, WCET_TASK_WATCHDOG_US, BCET_TASK_WATCHDOG_US);

		scheduler_add_task_cooperative(task_heartbeat_update, DELAY_TASK_HEARTBEAT_TICKS_NORMAL, PERIOD_TASK_HEARTBEAT_TICKS_NORMAL, WCET_TASK_HEARTBEAT_US, BCET_TASK_HEARTBEAT_US);

		task_watchdog_update();

		break;
        


    }
}

/***************************************************************************//**

 @brief		Performs the system shutdown in a safe manner.

 @details	When called, this function simply blocks microcontroller tasks execution. In this state, just an LED is turned on, as an indicator that something went wrong. The monitor is stopped, as well as the scheduler.

*******************************************************************************/
static void system_perform_safe_shutdown(void)
{
    uint32_t i;
    
    cm_disable_interrupts();
    scheduler_stop();

    task_failSilentLED_init();

    while(1)
    {
        for(i = 0; i < 20000 ; i++)
        {
            task_failSilentLED_update();
        }
    }
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

 @brief		Configures the microcontroller in the required mode.

 @details	Checks system conditions and configures according to the required mode.

*******************************************************************************/
void system_init(void)
{
    system_identify_required_mode();

    system_configure_required_mode();
}




/** @}
 */


/** @}
 */
