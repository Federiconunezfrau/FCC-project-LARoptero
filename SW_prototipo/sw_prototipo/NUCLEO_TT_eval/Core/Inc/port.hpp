#ifndef _PORT_HPP_
#define _PORT_HPP_


#include "gpio_STM32.hpp"
#include "cout_STM32.hpp"
//#include "spi_STM32.hpp"
//#include "i2c_STM32.hpp"
#include "handler_spi.hpp"
#include "handler_i2c.hpp"


//using   spi             =   STM32::spi;
//using   i2c             =   STM32::i2c;
using   gpio            =   STM32::gpio;

template <uint8_t BufferSize = 64>
using   cout            =   STM32::cout<BufferSize>;
template <uint8_t BufferSize = 32>
using 	phandler_spi    =   STM32::PHandlerSPI<BufferSize>;
template <uint8_t BufferSize = 32>
using 	phandler_i2c    =   STM32::PHandlerI2C<BufferSize>;

#endif
