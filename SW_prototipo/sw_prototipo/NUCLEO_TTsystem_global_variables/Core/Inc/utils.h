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

#ifndef __UTILS_H__
#define __UTILS_H__

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

/** The IWDG peripheral is a 12 bit counter, this is the max reload value to be set. */
#define MAX_RELOAD_IWDG 4095

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

// Independent watchdog util functions
uint32_t iwdg_init             ( uint32_t prescale, uint32_t reload );
uint32_t iwdg_is_reset_source  ( void );
void     iwdg_clear_reset_flag ( void );

// bxCAN peripheral util functions
uint32_t can_get_rx_fifo_fill_level ( uint32_t canport, uint8_t fifo );


/** @}
 */
 
 #endif
