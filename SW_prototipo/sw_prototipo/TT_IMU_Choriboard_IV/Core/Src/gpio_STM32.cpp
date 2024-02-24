#include "gpio_STM32.hpp"
#include "stm32f7xx_hal.h"


namespace STM32{

    gpio::gpio(GPIO_TypeDef* pPortHandler, uint16_t pin)
    {
        m_portHandler = pPortHandler;
        m_pin = pin;
    }

    GPIO_ST gpio::read()
    {
        GPIO_PinState ret = HAL_GPIO_ReadPin(m_portHandler, m_pin);
        return ret == GPIO_PIN_SET ? GPIO_ST::HIGH : GPIO_ST::LOW;
    }

    void gpio::toggle()
    {
        HAL_GPIO_TogglePin(m_portHandler, m_pin);
    }

    void gpio::write(GPIO_ST st)
    {
        GPIO_PinState pinState = map_hal_st_to_igpio_st(st); 
        HAL_GPIO_WritePin(m_portHandler, m_pin, pinState);
    }

    GPIO_PinState gpio::map_hal_st_to_igpio_st(GPIO_ST st)
	{
		return st == GPIO_ST::HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET;
	}

}
