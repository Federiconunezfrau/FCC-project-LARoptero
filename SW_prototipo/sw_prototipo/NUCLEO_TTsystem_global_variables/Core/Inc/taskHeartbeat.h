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

#ifndef __TASKHEARTBEAT_H__
#define __TASKHEARTBEAT_H__

/** @addtogroup Module_Name
 * @{
 */

/*******************************************************************************
	INCLUDED FILES
*******************************************************************************/

#include <libopencm3/stm32/gpio.h>

/*******************************************************************************
	PUBLIC CONSTANTS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Constants
    @{
 */


#define TASK_HEARTBEAT_LED_GPIO_PORT GPIOB
#define TASK_HEARTBEAT_LED_GPIO_PIN GPIO0

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
void task_heartbeat_init   ( void );
void task_heartbeat_update ( void );

/** @}
 */
 
 #endif