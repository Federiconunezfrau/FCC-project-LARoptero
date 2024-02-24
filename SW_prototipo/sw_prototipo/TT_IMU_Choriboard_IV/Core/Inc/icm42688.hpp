/**
 * @file icm42688.h
 * @author your name (you@domain.com)
 * @brief Este archivo declara la implementación de la IMU ICM 42688 
 * 
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _ICM42688_HPP_
#define _ICM42688_HPP_

#include "stdint.h"


//#include "spi_STM32.hpp"
#include "gpio_STM32.hpp"
#include "iIMU.hpp"
#include "icm42688_utils.hpp"
#include "utils.hpp"
#include "iPHandler.hpp"
#include "handler_spi.hpp"
#include "handler_i2c.hpp"
#include "port.hpp"


namespace ICM42688{

    constexpr uint16_t BUFFER_SIZE = 32;
    constexpr uint16_t IMU_DATA_BUFFER_SIZE = 16;

    struct Registers {
        uint8_t GYRO_CONFIG0 = 0x06;
        uint8_t ACCEL_CONFIG0 = 0x06;
        uint8_t INTF_CONFIG0 = 0x30;
        uint8_t FIFO_CONFIG = 0x00;
        uint8_t FIFO_CONFIG1 = 0x00;
        uint8_t FIFO_CONFIG2 = 0x00;
        uint8_t FIFO_CONFIG3 = 0x00;
    };

    class icm42688: public iIMU{
    public:
        circ_buffer<IMUData, IMU_DATA_BUFFER_SIZE> imuDataBuffer;
        Config          m_cnf;
        uint8_t 		m_TxBuffer[BUFFER_SIZE], m_RxBuffer[BUFFER_SIZE];
        Registers       m_reg;

        //icm42688(config& cnf, iSPI* pSpiHandler, iGPIO *pInt1Handler, iGPIO *pInt2Handler);
        //icm42688(config& cnf, iSPI* pSpiHandler, iGPIO *pInt1Handler);
        
        icm42688(Config& cnf, iPHandler *handler, gpio *cs,
                            uint8_t slaveAdd, gpio *INT1, gpio *INT2);

        icm42688(Config& cnf, iPHandler *handler, gpio *cs, gpio *INT1);
        icm42688(Config& cnf, iPHandler *handlerSpi, gpio *cs);
        icm42688(Config& cnf, iPHandler *handler, uint8_t slaveAdd, gpio *INT1);
        icm42688(Config& cnf, iPHandler *handler, uint8_t slaveAdd);

        iIMU_ST save_data() override;
        void save_data_callback() override;
        
        iIMU_ST save_fifo_data(uint8_t qty) override;
        void save_fifo_data_callback() override;

        iIMU_ST read_data(IMUData *pData) override;
        


        iIMU_ST enable(void) override;

        iIMU_ST disable(void) override;

        

        // iIMU_st read_FIFO(  float* pGyroX, float* pGyroY, float* pGyroZ,
        //                                     float* pAccelX, float* pAccelY, float* pAccelZ,
        //                                     float* pTemp) override; 
        /**
         * @brief Configura la IMU y actualiza la información del objeto con un nuevo valor de ODR
         * 
         * @param odr Nueva configuración de ODR. Debe ser alguno de los GYRO_ODR definidos como static en esta clase
         * @return iIMU_st 
         */
        iIMU_ST set_gyro_odr(uint8_t odr) override;

        /**
         * @brief Configura la IMU y actualiza la información del objeto con un nuevo valor de FSR
         * 
         * @param fsr Nueva configuración de FSR. Debe ser alguno de los GYRO_FSR definidos como static en esta clase
         * @return iIMU_st 
         */
        iIMU_ST set_gyro_fsr(uint8_t fsr) override;

        /**
         * @brief Configura la IMU y actualiza la información del objeto con un nuevo valor de ODR
         * 
         * @param odr Nueva configuración de ODR. Debe ser alguno de los ACCEL_ODR definidos como static en esta clase
         * @return iIMU_st 
         */
        iIMU_ST set_accel_odr(uint8_t odr) override;

        /**
         * @brief Configura la IMU y actualiza la información del objeto con un nuevo valor de FSR
         * 
         * @param fsr Nueva configuración de FSR. Debe ser alguno de los ACCEL_FSR definidos como static en esta clase.
         * @return iIMU_st 
         */
        iIMU_ST set_accel_fsr(uint8_t fsr) override;


        
        float get_accel_odr_value(void) override;
        float get_accel_fsr_value(void) override;
        float get_gyro_odr_value(void) override;
        float get_gyro_fsr_value(void) override;

        float get_accel_resolution(void) override;
        float get_gyro_resolution(void) override;

        float convertGyroData(uint8_t rawGyroH, uint8_t rawGyroL);
        float convertAccelData(uint8_t rawAccelH, uint8_t rawAccelL);

        iIMU_ST write_config(void) override;
        iIMU_ST write_config_accel_gyro();
        iIMU_ST write_config_FIFO();
        iIMU_ST write_config_int();

        iIMU_ST flush_FIFO();
        iIMU_ST get_FIFO_count(uint16_t &count);


        
    private:

        iPHandler        *m_handler;
        gpio            *m_cs;
        uint8_t         m_i2cAdd;
        bool            m_spiSet;
        COMM_MODE       m_mode;
        gpio            *m_INT1;
        gpio            *m_INT2;
        bankRegister    m_currentBankReg;
        float           m_gyroFSRValue;
        float           m_gyroODRValue;
        float           m_accelFSRValue;
        float           m_accelODRValue;

        iIMU_ST set_bank_register(bankRegister bankReg);
        //void load_data(IMUData &data);

        void load_data(uint8_t *buffertData);

        static iIMU_ST map_handler_ret_to_iIMU_ret(iPHANDLER_ST iPHandler);


    };

}


#endif
