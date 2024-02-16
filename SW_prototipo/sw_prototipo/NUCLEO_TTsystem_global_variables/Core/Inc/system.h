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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/** @addtogroup Module_Name
 * @{
 */

/*******************************************************************************
	INCLUDED FILES
*******************************************************************************/

// Nothing defiend yet

/*******************************************************************************
	PUBLIC CONSTANTS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Constants
    @{
 */

/** Different system modes. */
typedef enum
{
    FAIL_SILENT = 0,
    NORMAL
} systemMode_t;

/** @}
 */

/*******************************************************************************
	PUBLIC STRUCTURES
*******************************************************************************/

/** @addtogroup Module_Name_Public_Structs
    @{
 */

// Nothing defiend yet

/** @}
 */

/*******************************************************************************
	PUBLIC MACROS
*******************************************************************************/

/** @addtogroup Module_Name_Public_Macros
    @{
 */

// Nothing defiend yet

/** @}
 */

/*******************************************************************************
	PUBLIC FUNCTIONS PROTOTYPES
*******************************************************************************/
void system_init ( void );


/** @}
 */
 
 #endif