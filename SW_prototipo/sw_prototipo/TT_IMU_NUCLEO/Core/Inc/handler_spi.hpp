#ifndef _HANDLER_SPI_HPP_
#define _HANDLER_SPI_HPP_

//#include "spi_STM32.hpp"
//#include "port.hpp"

#include "stm32f7xx_hal.h"
#include "gpio_STM32.hpp"
//#include "port.hpp"|
#include "iPHandler.hpp"

// Agregado por Fede Nuñez Frau: 17/02/2024
#include <string.h>

#define PER_HANDLER_SIMPLE


#ifndef PER_HANDLER_SIMPLE




    //iHANDLER_ST read_registers(uint8_t add, uint8_t size, void *slave, COMM_MODE mode) override;
    //iHANDLER_ST write_registers(uint8_t add, uint8_t *data, uint8_t size, void *slave, COMM_MODE mode) override;

    //iHANDLER_ST read_registers(const HandlerOpt &opt) ;
    //iHANDLER_ST write_registers(uint8_t *data, const HandlerOpt &opt) ;


    HandlerSPI(spi spiHandler, gpio *cs, COMM_MODE mode)
        : m_spiHandler(spiHandler)
        , m_cs(cs)
        , m_mode(mode)
        , m_mutex(false)
        {
            m_recvBuffer = &m_rxBuffer[1];
        }

    HandlerSPI(spi spiHandler, gpio *cs)
        : HandlerSPI(spiHandler, cs, COMM_MODE::BLCK)
        {}


    //iHANDLER_ST read_registers(uint8_t add, uint8_t size, void *cs, COMM_MODE mode)
    iHANDLER_ST read_registers(const HandlerOpt &opt) override
    {
        //SPI_ST ret = SPI_ST::OK;
        
        if(opt.size > BufferSize-1){
                return iHANDLER_ST::ERROR;
        }

        //if(m_mutex == true){
        //	return iHANDLER_ST::UNAVAILABLE;
        //}
        //m_mutex = true;

        m_txBuffer[0] = add_read_bit(opt.add, opt.rw);
        m_mode = opt.mode;
        m_size = opt.size + 1;
        m_cs = static_cast<gpio*>(opt.slave);
        m_recvBuffer = &m_rxBuffer[opt.addSize]; // recvBuffer apunta a donde comienza la recepción de SPI según el largo del address (en bytes)

        return start_transmition();
    }

    //iHANDLER_ST write_registers(uint8_t add, uint8_t *data, uint8_t size, void *cs, COMM_MODE mode)
    iHANDLER_ST write_registers(uint8_t *data, const HandlerOpt &opt) override
    {
        //SPI_ST ret = SPI_ST::OK;
        
        if(opt.size > BufferSize-1){
                return iHANDLER_ST::ERROR;
        }

        //if(m_mutex == true){
        //	return iHANDLER_ST::UNAVAILABLE;
        //}
        //m_mutex = true;

        m_txBuffer[0] = add_write_bit(opt.add, opt.rw);
        memcpy(m_txBuffer + 1, data, opt.size);
        m_mode = opt.mode;
        m_size = opt.size + 1;
        m_cs = static_cast<gpio*>(opt.slave);

        return start_transmition();
    }
    
     
private:
    spi        &m_spiHandler;
    gpio       *m_cs;
    COMM_MODE   m_mode;
    bool        m_mutex;
    uint8_t     m_rxBuffer[BufferSize];
    uint8_t     m_txBuffer[BufferSize];
    uint8_t     m_size; // El tamaño de lo que se va a enviar
    //uint8_t    *m_recvBuffer;

    iHANDLER_ST start_transmition(void *fn_pointer = nullptr ) // arreglar cuando se use la DMA
    {
        SPI_ST ret = SPI_ST::OK;
        m_spiHandler.m_csPin = m_cs;

        m_cs->write(GPIO_ST::LOW);
        switch (m_mode)
        {
        case COMM_MODE::BLCK:

            ret = m_spiHandler.transmit(m_txBuffer, m_rxBuffer, m_size);
            m_cs->write(GPIO_ST::HIGH);
            //m_mutex = false;
            if(ret != SPI_ST::OK){
                m_cs->write(GPIO_ST::HIGH);
                return iHANDLER_ST::ERROR;
            }
            break;
        
        case COMM_MODE::IT:
             return iHANDLER_ST::INVALID;
            break;

        case COMM_MODE::DMA:
            return iHANDLER_ST::INVALID;
            break;
        

        default:
            m_cs->write(GPIO_ST::HIGH);
            return iHANDLER_ST::INVALID;
            break;
        }

        return iHANDLER_ST::OK;
    }
};
#else

namespace STM32 {

template <uint8_t BufferSize = 32>
class PHandlerSPI : public iPHandler {
public:

	PHandlerSPI(SPI_HandleTypeDef* spiHandler, gpio *cs, COMM_MODE mode)
		: m_handler(spiHandler)
		, m_cs(cs)
		, m_mode(mode)
		, m_size(0)
		{

			m_mutex = false;
			m_cs->write(GPIO_ST::HIGH);
			m_recvBuffer = &m_rxBuffer[1];
			//m_blockDelay = delay == 0 ? HAL_MAX_DELAY : delay;
			m_blockDelay = 5000;
		}

	PHandlerSPI(SPI_HandleTypeDef *spiHandler, gpio *cs)
		: PHandlerSPI(spiHandler, cs, COMM_MODE::BLCK)
		{}


	//iHANDLER_ST read_registers(uint8_t add, uint8_t size, void *cs, COMM_MODE mode)
	iPHANDLER_ST read_registers(const PHandlerOpt &opt) override
	{
		//SPI_ST ret = SPI_ST::OK;

		if(opt.size > BufferSize-1){
				return iPHANDLER_ST::ERROR;
		}

		//if(m_mutex == true){
		//	return iHANDLER_ST::UNAVAILABLE;
		//}
		//m_mutex = true;

		m_txBuffer[0] = add_read_bit(opt.add, opt.rw, opt.rwFront);
		m_mode = opt.mode;
		m_size = opt.size + 1;
		m_cs = static_cast<gpio*>(opt.slave);
		m_recvBuffer = &m_rxBuffer[opt.addSize]; // recvBuffer apunta a donde comienza la recepción de SPI según el largo del address (en bytes)

		return start_transmition();
	}

	//iHANDLER_ST write_registers(uint8_t add, uint8_t *data, uint8_t size, void *cs, COMM_MODE mode)
	iPHANDLER_ST write_registers(uint8_t *data, const PHandlerOpt &opt) override
	{
		//SPI_ST ret = SPI_ST::OK;

		if(opt.size > BufferSize-1){
				return iPHANDLER_ST::ERROR;
		}

		//if(m_mutex == true){
		//	return iHANDLER_ST::UNAVAILABLE;
		//}
		//m_mutex = true;

		m_txBuffer[0] = add_write_bit(opt.add, opt.rw, opt.rwFront);
		memcpy(m_txBuffer + 1, data, opt.size);
		m_mode = opt.mode;
		m_size = opt.size + 1;
		m_cs = static_cast<gpio*>(opt.slave);

		return start_transmition();
	}




	iPHANDLER_ST start_transmition(void *fn_pointer = nullptr ) // arreglar cuando se use la DMA
	{
		iPHANDLER_ST ret = iPHANDLER_ST::OK;
		//m_spiHandler.m_csPin = m_cs;

		m_cs->write(GPIO_ST::LOW);
		switch (m_mode)
		{
		case COMM_MODE::BLCK:

			ret = transmit(m_txBuffer, m_rxBuffer, m_size);
			m_cs->write(GPIO_ST::HIGH);
			if(ret != iPHANDLER_ST::OK){
				return iPHANDLER_ST::ERROR;
			}
			break;

		case COMM_MODE::IT:
			 return iPHANDLER_ST::INVALID;
			break;

		case COMM_MODE::DMA:
			return iPHANDLER_ST::INVALID;
			break;


		default:
			m_cs->write(GPIO_ST::HIGH);
			return iPHANDLER_ST::INVALID;
			break;
		}

		return iPHANDLER_ST::OK;
	}



private:
	SPI_HandleTypeDef* m_handler;
	gpio       *m_cs;
	COMM_MODE   m_mode;
	uint8_t     m_rxBuffer[BufferSize];
	uint8_t     m_txBuffer[BufferSize];
	uint8_t     m_size; // El tamaño de lo que se va a enviar
	uint32_t 	m_blockDelay;
	//uint8_t    *m_recvBuffer;






   iPHANDLER_ST write(uint8_t *pTxData, uint16_t size)
   {

	   HAL_StatusTypeDef retHal = HAL_OK;
	   if(m_cs == nullptr)
		   return iPHANDLER_ST::ERROR;

	   //m_csPin->write(GPIO_ST::LOW);

	   retHal = HAL_SPI_Transmit(
				m_handler,
				   pTxData,
				   size,
				m_blockDelay
			   );

	   //m_csPin->write(GPIO_ST::HIGH);

	   return map_hal_st_to_handler_st(retHal);

   }

   iPHANDLER_ST read(uint8_t *pRxData, uint16_t size)
   {


	   HAL_StatusTypeDef retHal = HAL_OK;
	   if(m_cs == nullptr)
		   return iPHANDLER_ST::ERROR;

	   //m_csPin->write(GPIO_ST::LOW);

	   retHal = HAL_SPI_Receive(
				m_handler,
				   pRxData,
				   size,
				m_blockDelay
			   );
	   //m_csPin->write(GPIO_ST::HIGH);


	   return map_hal_st_to_handler_st(retHal);
   }

   iPHANDLER_ST transmit(uint8_t *pTxData, uint8_t *pRxData, uint16_t size)
   {

	   HAL_StatusTypeDef retHal = HAL_OK;
	   if(m_cs == nullptr)
		   return iPHANDLER_ST::ERROR;

	   //m_csPin->write(GPIO_ST::LOW);

	   retHal = HAL_SPI_TransmitReceive(
				m_handler,
				   pTxData,
				   pRxData,
				   size,
				m_blockDelay
			   );
	   //m_csPin->write(GPIO_ST::HIGH);

	   return map_hal_st_to_handler_st(retHal);

   }

   iPHANDLER_ST write_it(uint8_t *pTxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }
   iPHANDLER_ST read_it(uint8_t *pRxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }
   iPHANDLER_ST transmit_it(uint8_t *pTxData, uint8_t *pRxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }

   iPHANDLER_ST write_dma(uint8_t *pTxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }
   iPHANDLER_ST read_dma(uint8_t *pRxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }
   iPHANDLER_ST transmit_dma(uint8_t *pTxData, uint8_t *pRxData, uint16_t size)
   {
	   return iPHANDLER_ST::INVALID;
   }


   iPHANDLER_ST map_hal_st_to_handler_st(HAL_StatusTypeDef st)
   {
	   switch (st){
	   case HAL_OK:
		   return iPHANDLER_ST::OK;
		   break;
	   case HAL_TIMEOUT:
	   case HAL_ERROR:
	   default:
		   return iPHANDLER_ST::ERROR;
		   break;
	   }
   }




};

}
#endif
#endif
