#ifndef _GPIO_STM32_HPP_
#define _GPIO_STM32_HPP_

#include "stm32f7xx_hal.h"
#include <functional>
enum class GPIO_ST {
    LOW         =   0, /* Logic state low*/
    HIGH        =   1, /* Logic state high*/
    UNDEFINED   =   2, /* Logic state undefined*/
    MAX         =   3,
    INVALID     =   -1,
};


namespace STM32{

    class gpio{
    public:
        std::function<void(void)> interrupt_handler;

        gpio(GPIO_TypeDef* pHandler, uint16_t pin);
        GPIO_ST read(void);
        void toggle(void);
        void write(GPIO_ST st);


    private:
        GPIO_TypeDef *m_portHandler ;
        uint16_t m_pin;

        static GPIO_PinState map_hal_st_to_igpio_st(GPIO_ST st);

    };

}

#endif
