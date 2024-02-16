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
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/can.h>

#include "utils.h"

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
uint32_t iwdg_init(uint32_t prescale, uint32_t reload)
{
    if( !((prescale == IWDG_PR_DIV4) || (prescale == IWDG_PR_DIV8) ||
    (prescale == IWDG_PR_DIV16) || (prescale == IWDG_PR_DIV32) ||
    (prescale == IWDG_PR_DIV64) || (prescale == IWDG_PR_DIV128) ||
    (prescale == IWDG_PR_DIV256)) )
    {
        return 1;
    }

    if( reload > MAX_RELOAD_IWDG )
    {
        return 2;
    }
    
    while (iwdg_prescaler_busy());
	IWDG_KR = IWDG_KR_UNLOCK;
	IWDG_PR = prescale;
	while (iwdg_reload_busy());
	IWDG_KR = IWDG_KR_UNLOCK;
	IWDG_RLR = reload;

    return 0;
}

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
uint32_t iwdg_is_reset_source(void)
{
    return ((RCC_CSR & RCC_CSR_IWDGRSTF) != 0);
}

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
void iwdg_clear_reset_flag(void)
{
    RCC_CSR |= RCC_CSR_RMVF;
}

/***************************************************************************//**

 @brief		Returns the number of available messages in the rx fifo.

 @details	This is a self implementation of the STM32 HAL function called
            "HAL_CAN_GetRxFifoFillLevel". Libopencm3 has no function for 
			getting the number of available messages on each fifo, so here is
			a function for doing so.

 @param		canport	CAN register base address @ref can_reg_base.

 @param		fifo	FIFO id, either 0 or 1.

 @return    Number of messages available in Rx FIFO.

*******************************************************************************/
uint32_t can_get_rx_fifo_fill_level(uint32_t canport, uint8_t fifo)
{
	uint32_t fillLevel = 0;

	if( (canport == CAN1) | (canport == CAN2) )
	{
		if(fifo == 0)
		{
			fillLevel = CAN_RF0R(canport) & CAN_RF0R_FMP0_MASK;
		}
		else
		{
			fillLevel = CAN_RF1R(canport) & CAN_RF1R_FMP1_MASK;
		}
	}
	else
	{
		fillLevel = 0;
	}

	return fillLevel;
}

/** @}
 */


/** @}
 */