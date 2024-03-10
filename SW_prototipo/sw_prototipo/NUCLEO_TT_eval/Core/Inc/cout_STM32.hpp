#ifndef _COUT_STM32_HPP_
#define _COUT_STM32_HPP_

#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <string.h>

namespace STM32 {

template <uint8_t BufferSize = 64>
class cout{
public:

    cout(UART_HandleTypeDef *handler, uint16_t delay = 5000)
        : m_handler(handler)
    {
        m_blockDelay = delay;
    }
    cout &operator<<(char *str)
    {
        write_str(str);
        return *this;
    }
    
    cout &operator<<(int num)
    {
        write_num(num);
        return *this;
    }
    
    cout &operator<<(float fl)
    {
        write_float(fl);
        return *this;
    }

private:
    UART_HandleTypeDef *m_handler;
    uint8_t m_TxBuffer[BufferSize], m_RxBuffer[BufferSize];
    uint16_t m_blockDelay;


    void write_str(char* str)
    {
        uint32_t len = strlen(str);
        if(len > BufferSize)
            return;
        
        HAL_UART_Transmit(m_handler, (uint8_t*)str, len, m_blockDelay) ;
    }

    void write_num(int num)
    {
        char strNum[BufferSize];
        memset(strNum, 0, BufferSize);
        sprintf(strNum, "%i", num);
        
        HAL_UART_Transmit(m_handler, (uint8_t*)strNum, sizeof(strNum), m_blockDelay) ;
        
    }

    void write_float(float fl)
    {
        char strFloat[BufferSize];
        memset(strFloat, 0, BufferSize);
        sprintf(strFloat, "%.3f", fl);
        
        HAL_UART_Transmit(m_handler, (uint8_t*)strFloat, sizeof(strFloat), m_blockDelay) ;
    }




};


}




#endif /* _COUT_STM32_HPP_ */
