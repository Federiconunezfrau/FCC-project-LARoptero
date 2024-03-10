#ifndef _HANDLER_I2C_HPP_
#define _HANDLER_I2C_HPP_

//#include "port.hpp"
//#include "i2c_STM32.hpp"
#include "iPHandler.hpp"

#include "stm32f7xx_hal.h"

#define PER_HANDLER_SIMPLE


#ifndef PER_HANDLER_SIMPLE


template <uint8_t BufferSize = 32>
class PHandlerI2C : public iHandler {
public:


    //iHANDLER_ST read_registers(uint8_t add, uint8_t size, void *slave, COMM_MODE mode) override;
    //iHANDLER_ST write_registers(uint8_t add, uint8_t *data, uint8_t size, void *slave, COMM_MODE mode) override;


    //iHANDLER_ST read_registers(const HandlerOpt &opt) ;
    //iHANDLER_ST write_registers(uint8_t *data, const HandlerOpt &opt) ;

    PHandlerI2C(i2c i2cHandler, COMM_MODE mode)
        : m_i2cHandler(i2cHandler)
        , m_slaveAdd(0x00)
        , m_mode(mode)
        , m_mutex(false)
        {
            m_recvBuffer = m_rxBuffer;
        }

    PHandlerI2C(i2c i2cHandler)
        : PHandlerI2C(i2cHandler, COMM_MODE::BLCK)
        {}

    //iHANDLER_ST read_registers(  
    //                    uint8_t add, 
    //                    uint8_t size, 
    //                    void *slave, 
    //                    COMM_MODE mode,
    //                    void *fn = NULL)

    iHANDLER_ST read_registers(const HandlerOpt &opt) override
    {

        if(opt.size > BufferSize-1)
            return iHANDLER_ST::ERROR;

        //if(m_mutex == true){
        //	return iHANDLER_ST::UNAVAILABLE;
        //}
        //m_mutex = true;

        m_add = opt.add;
        m_size = opt.size;
        m_slaveAdd = opt.slave != nullptr ? *static_cast<uint8_t*>(opt.slave) : 0xFF;
        m_mode = opt.mode;

        return start_reading_registers();
    }


    //iHANDLER_ST write_registers( uint8_t add, 
    //                                uint8_t *data, 
    //                                uint8_t size, 
    //                                void *slave, 
    //                                COMM_MODE mode,
    //                                void *fn = NULL)
    iHANDLER_ST write_registers(uint8_t *data, const HandlerOpt &opt) override
    {
        memcpy(m_txBuffer, data, opt.size);
        m_add = opt.add;
        m_size = opt.size;
        m_slaveAdd = opt.slave != nullptr ? *static_cast<uint8_t*>(opt.slave) : 0xFF;
        m_mode = opt.mode;

        return start_writing_registers();
    }


    
     
private:
    bool        m_mutex;
    i2c         &m_i2cHandler;
    uint8_t     m_slaveAdd;
    uint8_t     m_rxBuffer[BufferSize];
    uint8_t     m_txBuffer[BufferSize];
    uint8_t     m_size; // El tamaño de lo que se va a enviar
    COMM_MODE   m_mode;
    uint8_t     m_memAdd;
    uint8_t     m_add;



    iHANDLER_ST start_reading_registers()
    {
        I2C_ST ret;
    
        switch (m_mode)
        {
        case COMM_MODE::BLCK:
            if(m_slaveAdd != 0xFF)
                ret = read(m_slaveAdd, m_add, 1, m_rxBuffer, m_size);
            else
                ret = read(m_add, m_rxBuffer, m_size);


            if(ret != I2C_ST::OK){
            	//m_mutex = false;
                return iHANDLER_ST::ERROR;
            }
            //m_mutex = false;
            break;
        
        case COMM_MODE::IT:
             return iHANDLER_ST::INVALID;
            break;

        case COMM_MODE::DMA:
            return iHANDLER_ST::INVALID;
            break;
        
        default:
            //m_mutex = false;
            return iHANDLER_ST::INVALID;
            break;
        }

        return iHANDLER_ST::OK;

    }



    iHANDLER_ST start_writing_registers(void *fn = NULL)
    {
        I2C_ST ret;

        switch (m_mode)
        {
        case COMM_MODE::BLCK:
            if(m_slaveAdd != 0xFF)
                ret = write(m_slaveAdd, m_add, 1, m_txBuffer, m_size);
            else
                ret = write(m_add, m_txBuffer, m_size);

            if(ret != I2C_ST::OK){
                //m_mutex = false;
                return iHANDLER_ST::ERROR;
            }

            //m_mutex = false;
            break;
        
        case COMM_MODE::IT:
             return iHANDLER_ST::INVALID;
            break;

        case COMM_MODE::DMA:
            return iHANDLER_ST::INVALID;
            break;
        
        default:
            //m_mutex = false;
            return iHANDLER_ST::INVALID;
            break;
        }

        return iHANDLER_ST::OK;
    }

};

#endif





namespace STM32 {

template <uint8_t BufferSize = 32>
class PHandlerI2C : public iPHandler {
public:

    PHandlerI2C(I2C_HandleTypeDef* i2cHandler, uint8_t slaveAdd, COMM_MODE mode)
        : m_handler(i2cHandler)
        , m_slaveAdd(slaveAdd)
    	, m_size(0)
        , m_mode(mode)
        {

        	m_mutex = false;
            m_recvBuffer = m_rxBuffer;
			m_blockDelay = 5000;
        }

    PHandlerI2C(I2C_HandleTypeDef *i2cHandler)
        : PHandlerI2C(i2cHandler, 0xFF, COMM_MODE::BLCK)
        {
            //m_mode = COMM_MODE::BLCK;
            //m_slaveAdd = 0x00;
        }

    iPHANDLER_ST read_registers(const PHandlerOpt &opt) override
    {

        if(opt.size > BufferSize-1)
            return iPHANDLER_ST::ERROR;

        //if(m_mutex == true){
        //	return iHANDLER_ST::UNAVAILABLE;
        //}
        //m_mutex = true;

        m_add = opt.add;
        m_size = opt.size;
        m_slaveAdd 	= opt.slave != nullptr ?
        		  	  add_read_bit(*static_cast<uint8_t*>(opt.slave), opt.rw, opt.rwFront) :
					  0xFF;
        m_mode = opt.mode;

        return start_reading_registers();
    }


    //iHANDLER_ST write_registers( uint8_t add, 
    //                                uint8_t *data, 
    //                                uint8_t size, 
    //                                void *slave, 
    //                                COMM_MODE mode,
    //                                void *fn = NULL)
    iPHANDLER_ST write_registers(uint8_t *data, const PHandlerOpt &opt) override
    {
        memcpy(m_txBuffer, data, opt.size);
        m_add 		= 	opt.add;
        m_size 		= 	opt.size;
        m_slaveAdd 	= 	opt.slave != nullptr ?
        				add_write_bit(*static_cast<uint8_t*>(opt.slave), opt.rw, opt.rwFront)  :
						0xFF;
        m_mode 		= 	opt.mode;

        return start_writing_registers();
    }




    iPHANDLER_ST start_reading_registers()
    {
        iPHANDLER_ST ret;
    
        switch (m_mode)
        {
        case COMM_MODE::BLCK:
            if(m_slaveAdd != 0xFF)
                ret = read(m_slaveAdd, m_add, 1, m_rxBuffer, m_size);
            else
                ret = read(m_add, m_rxBuffer, m_size);


            if(ret != iPHANDLER_ST::OK){
            	//m_mutex = false;
                return iPHANDLER_ST::ERROR;
            }
            //m_mutex = false;
            break;
        
        case COMM_MODE::IT:
             return iPHANDLER_ST::INVALID;
            break;

        case COMM_MODE::DMA:
            return iPHANDLER_ST::INVALID;
            break;
        
        default:
            //m_mutex = false;
            return iPHANDLER_ST::INVALID;
            break;
        }

        return iPHANDLER_ST::OK;

    }



    iPHANDLER_ST start_writing_registers(void *fn = NULL)
    {
        iPHANDLER_ST ret;

        switch (m_mode)
        {
        case COMM_MODE::BLCK:
            if(m_slaveAdd != 0xFF)
                ret = write(m_slaveAdd, m_add, 1, m_txBuffer, m_size);
            else
                ret = write(m_add, m_txBuffer, m_size);

            if(ret != iPHANDLER_ST::OK){
                //m_mutex = false;
                return iPHANDLER_ST::ERROR;
            }

            //m_mutex = false;
            break;
        
        case COMM_MODE::IT:
             return iPHANDLER_ST::INVALID;
            break;

        case COMM_MODE::DMA:
            return iPHANDLER_ST::INVALID;
            break;
        
        default:
            //m_mutex = false;
            return iPHANDLER_ST::INVALID;
            break;
        }

        return iPHANDLER_ST::OK;
    }

    
     
private:
    I2C_HandleTypeDef*  m_handler;
    uint8_t             m_slaveAdd;
    uint8_t             m_rxBuffer[BufferSize];
    uint8_t             m_txBuffer[BufferSize];
    uint8_t             m_size; // El tamaño de lo que se va a enviar
    COMM_MODE           m_mode;
    uint8_t             m_add;
    uint32_t            m_blockDelay;



    iPHANDLER_ST write(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {

        HAL_StatusTypeDef resHal = HAL_OK;

        resHal = HAL_I2C_Master_Transmit(   
            m_handler,
            devAdd, 
            pData, 
            size, 
            m_blockDelay
        );

        return map_hal_st_to_handler_st(resHal);
    }

    iPHANDLER_ST write(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize, uint8_t *pData, uint16_t size)
    {

        HAL_StatusTypeDef resHal = HAL_OK;

        resHal = HAL_I2C_Mem_Write( 
            m_handler,
            devAdd,
            memAdd,
            memAddSize, 
            pData, 
            size,
            m_blockDelay
        );

        return map_hal_st_to_handler_st(resHal);
    }

    iPHANDLER_ST read(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {


        HAL_StatusTypeDef resHal = HAL_OK;

        resHal = HAL_I2C_Master_Receive(    
            m_handler,
            devAdd, 
            pData, 
            size, 
            m_blockDelay
        );

        return map_hal_st_to_handler_st(resHal);

    }

    iPHANDLER_ST read(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize,  uint8_t *pData, uint16_t size)
    {

        HAL_StatusTypeDef resHal = HAL_OK;

        resHal = HAL_I2C_Mem_Read(    
            m_handler,
            devAdd,
            memAdd,
            memAddSize,
            pData, 
            size, 
            m_blockDelay
        );

        return map_hal_st_to_handler_st(resHal);

    }

    iPHANDLER_ST write_IT(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST write_IT(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize, uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST read_IT(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST read_IT(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize,  uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }

    iPHANDLER_ST write_DMA(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST write_DMA(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize, uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST read_DMA(uint16_t devAdd,  uint8_t *pData, uint16_t size)
    {
        return iPHANDLER_ST::INVALID;
    }
    iPHANDLER_ST read_DMA(uint16_t devAdd, uint16_t memAdd, uint16_t memAddSize,  uint8_t *pData, uint16_t size)
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
