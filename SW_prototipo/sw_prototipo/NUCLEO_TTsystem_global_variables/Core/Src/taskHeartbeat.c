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
#include "taskHeartbeat.h"

/*******************************************************************************
	PRIVATE CONSTANTS
*******************************************************************************/

/** @cond DEV
    @addtogroup Module_Name_Private_Constants
    @{
 */

// Nothing defined yet

/** @}
 */

/*******************************************************************************
	PRIVATE STRUCTURES
*******************************************************************************/

/** @addtogroup Module_Name_Private_Structures
    @{
 */

// Nothing defined yet

/** @}
 */

/*******************************************************************************
	GLOBAL VARIABLES
*******************************************************************************/

/** @addtogroup Module_Name_Global_Variables
    @{
 */

// Nothing defined yet

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
// Nothing defined yet

/*******************************************************************************
	PRIVATE FUNCTIONS
*******************************************************************************/

/** @addtogroup Module_Name_Private_Functions
    @{   
 */

// Nothing defined yet

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

 @brief		Brief description of functions behavior (One line only).

 @details	Detailed description. May span more than one line.

 @param		c	Parameter explanation. May contain an enumeration of
			possible values.
			- Option_1:	Brief explanation.
			- Option_2:	Brief explanation.

 @param		y	Parameter explanation.

 @return     	Explanation of the possible returned values.
		- Option_1:	Brief explanation.
		- Option_2:	Brief explanation.

*******************************************************************************/
void task_heartbeat_init(void)
{
    gpio_mode_setup(TASK_HEARTBEAT_LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TASK_HEARTBEAT_LED_GPIO_PIN);
    gpio_set_output_options(TASK_HEARTBEAT_LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TASK_HEARTBEAT_LED_GPIO_PIN);
    gpio_clear(TASK_HEARTBEAT_LED_GPIO_PORT, TASK_HEARTBEAT_LED_GPIO_PIN);
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
void task_heartbeat_update(void)
{
    gpio_toggle(TASK_HEARTBEAT_LED_GPIO_PORT, TASK_HEARTBEAT_LED_GPIO_PIN);
}

/** @}
 */


/** @}
 */