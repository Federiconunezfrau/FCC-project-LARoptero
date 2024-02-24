/**
 * @file icm42688.h
 * @author your name (you@domain.com)
 * @brief Este archivo define la implementación de la IMU ICM 42688 
 * 
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdint.h>
#include <string.h>
#include <functional>

#include "port.hpp"
#include "iPHandler.hpp"
#include "handler_i2c.hpp"
#include "handler_spi.hpp"

#include "icm42688_utils.hpp"
#include "icm42688.hpp"

namespace ICM42688{

    icm42688::icm42688( Config& cnf, iPHandler* handler, gpio *cs,
                        uint8_t slaveAdd, gpio *INT1, gpio *INT2)
        : imuDataBuffer()
        , m_cnf(cnf)
        , m_handler(handler)
        , m_cs(cs)
        , m_i2cAdd(slaveAdd)
        , m_INT1(INT1)
        , m_INT2(INT2)
        , m_currentBankReg(bankRegister::BANK0)

    {
        m_mode = COMM_MODE::BLCK;
        m_gyroODRValue    = map_gyro_ODR_CNF_to_ODR(m_cnf.gyroODR);
        m_gyroFSRValue    = map_gyro_FSR_CNF_to_FSR(m_cnf.gyroFSR);
        m_accelODRValue   = map_accel_ODR_CNF_to_ODR(m_cnf.accelODR);
        m_accelFSRValue   = map_accel_FSR_CNF_to_FSR(m_cnf.accelFSR);
    }



    icm42688::icm42688(Config& cnf, iPHandler *handlerSpi, gpio *cs, gpio *INT1)
        : icm42688(cnf, handlerSpi, cs, 0xFF, INT1, nullptr)
    {
        m_spiSet = true;
        //write_config();
        enable();
    }

    icm42688::icm42688(Config& cnf, iPHandler *handlerSpi, gpio *cs)
        : icm42688(cnf, handlerSpi, cs, 0xFF, nullptr, nullptr)
    {
        m_spiSet = true;
        //write_config();
        enable();
    }

    icm42688::icm42688(Config& cnf, iPHandler *handlerI2c, uint8_t slaveAdd, gpio *INT1)
        : icm42688(cnf, handlerI2c, nullptr, slaveAdd, INT1, nullptr)
    {
        m_spiSet = false;
        //write_config();
        enable();
    }

    icm42688::icm42688(Config& cnf, iPHandler *handlerI2c, uint8_t slaveAdd)
        : icm42688(cnf, handlerI2c, nullptr, slaveAdd, nullptr, nullptr)
    {
        m_spiSet = false;
        //write_config();
        enable();
    }


    iIMU_ST icm42688::set_bank_register(bankRegister bankReg)
    {
        //iHANDLER_ST retHandler(iHANDLER_ST::OK);
        uint8_t bank;
        PHandlerOpt opt;

        if(m_currentBankReg != bankReg){

            bank = static_cast<uint8_t>(bankReg);
            opt.add = REG_BANK_SEL_ADD;
            opt.size = 1;
            opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
            m_TxBuffer[0] = bank;

            if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            	return iIMU_ST::ERROR_COMM;
            }

            m_currentBankReg = bankReg;
        }

        return iIMU_ST::OK;
    }

    


    /////////////////////////////////////// PUBLIC ///////////////////////////////////////

        
    iIMU_ST icm42688::save_data()
    {
        iIMU_ST ret = iIMU_ST::OK;
        iPHANDLER_ST retHandler(iPHANDLER_ST::OK);
        PHandlerOpt opt;

        ret = set_bank_register(bankRegister::BANK0);
        if( ret != iIMU_ST::OK){
            return ret;
        }
        

        opt.add = TEMP_DATA1_ADD;
        opt.size = IMU_DATA_N_REGS;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;


        if(!m_handler->lock()){
        	return iIMU_ST::ERROR_HANDLER_UNAVAILABLE;
        }

        retHandler = m_handler->read_registers(opt);
        if(retHandler != iPHANDLER_ST::OK){
            return map_handler_ret_to_iIMU_ret(retHandler);
        }
         

        if(m_mode == COMM_MODE::BLCK){
        	memcpy(m_RxBuffer, m_handler->m_recvBuffer, opt.size);
            m_handler->unlock();
        	//load_data(dummyIMUData, m_RxBuffer);
            load_data(m_RxBuffer);
            //m_handler->m_mutex = false;
        }


        return ret;
    }

    void icm42688::save_data_callback()
    {
        //IMUData dummyIMUData;
        load_data(m_handler->m_recvBuffer);
        m_handler->unlock();
        //m_handler->m_mutex = false;
    }
    


    iIMU_ST icm42688::save_fifo_data(uint8_t qty)
    {
        /*
        iSPI_ST spiRet;
        iIMU_ST ret = iIMU_ST::OK;
        IMUData dummyIMUData;
        
        if(mCnf_.saveIMUDataBlockingMode){
        	for(int i = 0; i< mCnf_.fifoCnf.watermark; i++){

        		ret = read_register(FIFO_DATA_ADD, bankRegister::BANK0, FIFO_PACKET_SIZE);
				if(ret != iIMU_ST::OK){
					return ret;
				}
				dummyIMUData.accelX = convertAccelData(mRxBuffer[1], mRxBuffer[2]);
				dummyIMUData.accelY = convertAccelData(mRxBuffer[3], mRxBuffer[4]);
				dummyIMUData.accelZ = convertAccelData(mRxBuffer[5], mRxBuffer[6]);
				dummyIMUData.gyroX  = convertGyroData(mRxBuffer[7], mRxBuffer[8]);
				dummyIMUData.gyroY  = convertGyroData(mRxBuffer[9], mRxBuffer[10]);
				dummyIMUData.gyroZ  = convertGyroData(mRxBuffer[11], mRxBuffer[12]);
				//dummyIMUData.temp   = convertTempData(mRxBuffer[13], mRxBuffer[14]);
				dummyIMUData.temp   = convertTempFIFOData(mRxBuffer[13]);

				if(imuDataBuffer.write(dummyIMUData) != EXIT_OK){
					return iIMU_ST::ERROR_BUFFER;
				}
        	}

        }else{
        	if(mCnf_.fifoCnf.watermark*FIFO_PACKET_SIZE + 1 < BUFFER_SIZE){
				return iIMU_ST::ERROR_BUFFER;
			}

			pmSpiHandler_->pmTxBuffer[0] = SPI_ADD_READ(FIFO_DATA_ADD);
			spiRet = pmSpiHandler_->transmit_dma(pmSpiHandler_->pmTxBuffer, pmSpiHandler_->pmRxBuffer, mCnf_.fifoCnf.watermark * FIFO_PACKET_SIZE + 1); // N_watermark *16 + 1 = N_watermark * 16 bytes for packet gyro and accel + first address register
			if(spiRet != iSPI_ST::OK){
				return iIMU_ST::ERROR_COMM;
			}
			spiRet = pmSpiHandler_->set_txrx_cplt_callback(&icm42688::save_fifo_imu_data_callback, this);
			if(spiRet != iSPI_ST::OK){
				return iIMU_ST::ERROR_BUFFER;
			}
        }

        */
        return iIMU_ST::OK;
        
    }

    void icm42688::save_fifo_data_callback()
    {
        /*
        IMUData dummyIMUData;


        memcpy(mRxBuffer, pmSpiHandler_->pmRxBuffer +1, FIFO_PACKET_SIZE*mCnf_.fifoCnf.watermark);

        for(int i = 0; i < mCnf_.fifoCnf.watermark; i++ ){

            // Position high/low register = FIFO_PACKET_SIZE*i + offset_inside_fifo_packet + first address used to send FIFO ADDRESS
            // timestamp is not stored
            // Check page 39 of https://invensense.tdk.com/wp-content/uploads/2021/04/DS-000439-ICM-42688-V-v1.1.pdf
            dummyIMUData.accelX = convertAccelData(mRxBuffer[FIFO_PACKET_SIZE*i + 1], mRxBuffer[FIFO_PACKET_SIZE*i + 2]);
            dummyIMUData.accelY = convertAccelData(mRxBuffer[FIFO_PACKET_SIZE*i + 3], mRxBuffer[FIFO_PACKET_SIZE*i + 4]);
            dummyIMUData.accelZ = convertAccelData(mRxBuffer[FIFO_PACKET_SIZE*i + 5], mRxBuffer[FIFO_PACKET_SIZE*i + 6]);
            dummyIMUData.gyroX  = convertGyroData(mRxBuffer[FIFO_PACKET_SIZE*i + 7], mRxBuffer[FIFO_PACKET_SIZE*i + 8]);
            dummyIMUData.gyroY  = convertGyroData(mRxBuffer[FIFO_PACKET_SIZE*i + 9], mRxBuffer[FIFO_PACKET_SIZE*i + 10]);
            dummyIMUData.gyroZ  = convertGyroData(mRxBuffer[FIFO_PACKET_SIZE*i + 11], mRxBuffer[FIFO_PACKET_SIZE*i + 12]);
            dummyIMUData.temp   = convertTempFIFOData(mRxBuffer[FIFO_PACKET_SIZE*i + 13]);
            imuDataBuffer.write(dummyIMUData);
        }
        */
        //return;
    }


    iIMU_ST icm42688::read_data(IMUData *pData)
    {
        if(imuDataBuffer.read(pData) == EXIT_OK){
            return iIMU_ST::OK;
        }
        return iIMU_ST::ERROR_BUFFER;
    }


    iIMU_ST icm42688::set_gyro_odr(uint8_t odr) 
    {
        GYRO_CNF::ODR gyroNewODR = static_cast<GYRO_CNF::ODR> (odr);
        PHandlerOpt opt;
        uint8_t new_GYRO_CONFIG0;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        new_GYRO_CONFIG0 = GYRO_CONFIG0::SET_ODR(m_reg.GYRO_CONFIG0, gyroNewODR);
        m_TxBuffer[0] = new_GYRO_CONFIG0;

        opt.add = GYRO_CONFIG0_ADD;
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_reg.GYRO_CONFIG0 = new_GYRO_CONFIG0;
        m_cnf.gyroODR = gyroNewODR;
        m_gyroODRValue = map_gyro_ODR_CNF_to_ODR(m_cnf.gyroODR);

        return iIMU_ST::OK;
    }


    iIMU_ST icm42688::set_gyro_fsr(uint8_t fsr)
    {
        GYRO_CNF::FSR gyroNewFSR = static_cast<GYRO_CNF::FSR> (fsr);
        PHandlerOpt opt;
        uint8_t new_GYRO_CONFIG0;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        new_GYRO_CONFIG0 = GYRO_CONFIG0::SET_FSR(m_reg.GYRO_CONFIG0, gyroNewFSR);
        m_TxBuffer[0] = new_GYRO_CONFIG0;

        opt.add = GYRO_CONFIG0_ADD;
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_reg.GYRO_CONFIG0 = new_GYRO_CONFIG0;
        m_cnf.gyroFSR = gyroNewFSR; 
        m_gyroODRValue = map_gyro_FSR_CNF_to_FSR(m_cnf.gyroFSR);

        return iIMU_ST::OK;

    } 


    iIMU_ST icm42688::set_accel_odr(uint8_t odr)
    {
        ACCEL_CNF::ODR accelNewODR = static_cast<ACCEL_CNF::ODR> (odr);
        PHandlerOpt opt;
        uint8_t new_ACCEL_CONFIG0;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        new_ACCEL_CONFIG0 = ACCEL_CONFIG0::SET_ODR(m_reg.ACCEL_CONFIG0, accelNewODR);
        m_TxBuffer[0] = new_ACCEL_CONFIG0;

        opt.add = ACCEL_CONFIG0_ADD;
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_reg.ACCEL_CONFIG0 = new_ACCEL_CONFIG0;
        m_cnf.accelODR = accelNewODR; 
        m_accelODRValue = map_accel_ODR_CNF_to_ODR(m_cnf.accelODR);

        return iIMU_ST::OK;
    }


    iIMU_ST icm42688::set_accel_fsr(uint8_t fsr)
    {
        ACCEL_CNF::FSR accelNewFSR = static_cast<ACCEL_CNF::FSR> (fsr);
        PHandlerOpt opt;
        uint8_t new_ACCEL_CONFIG0;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        new_ACCEL_CONFIG0 = ACCEL_CONFIG0::SET_FSR(m_reg.ACCEL_CONFIG0, accelNewFSR);
        m_TxBuffer[0] = new_ACCEL_CONFIG0;

        opt.add = ACCEL_CONFIG0_ADD;
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_reg.ACCEL_CONFIG0 = new_ACCEL_CONFIG0;
        m_cnf.accelFSR = accelNewFSR; 
        m_accelFSRValue = map_accel_FSR_CNF_to_FSR(m_cnf.accelFSR);

        return iIMU_ST::OK;
    }

    float icm42688::get_accel_odr_value()
    {
        return m_accelODRValue;
    }
    float icm42688::get_accel_fsr_value()
    {
        return m_accelFSRValue;
    }
    float icm42688::get_gyro_odr_value() 
    {
        return m_gyroODRValue;
    }
    float icm42688::get_gyro_fsr_value() 
    {
        return m_gyroFSRValue;
    }
    
    float icm42688::get_accel_resolution()
    {
        return m_accelFSRValue/(1<<15);
    }
    
    float icm42688::get_gyro_resolution()
    {
        return m_gyroFSRValue/(1<<15);
    }



    float icm42688::convertGyroData(uint8_t rawGyroH, uint8_t rawGyroL)
    {
        int16_t gyro_data = (rawGyroH << 8)  | rawGyroL;
        return gyro_data * get_gyro_resolution();

    }

    float icm42688::convertAccelData(uint8_t rawAccelH, uint8_t rawAccelL)
    {
        int16_t gyro_data = (rawAccelH << 8)  | rawAccelL;
        return gyro_data * get_accel_resolution();
    }

    iIMU_ST icm42688::write_config()
    {
        iIMU_ST ret = iIMU_ST::OK; 
        
        
        if( (ret = write_config_accel_gyro()) != iIMU_ST::OK){
            return ret;
        }

        if( (ret = write_config_FIFO()) != iIMU_ST::OK){
            return ret;
        }

        if( (ret = write_config_int()) != iIMU_ST::OK){
            return ret;
        }

        return ret;
        
    }

    iIMU_ST icm42688::write_config_accel_gyro()
    {
        PHandlerOpt opt;
        uint8_t new_GYRO_CONFIG0;
        uint8_t new_ACCEL_CONFIG0;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }


        new_GYRO_CONFIG0 = GYRO_CONFIG0::SET_FSR(m_reg.GYRO_CONFIG0, m_cnf.gyroFSR);
        new_GYRO_CONFIG0 = GYRO_CONFIG0::SET_ODR(new_GYRO_CONFIG0, m_cnf.gyroODR);
        new_ACCEL_CONFIG0 = ACCEL_CONFIG0::SET_FSR(m_reg.ACCEL_CONFIG0, m_cnf.accelFSR);
        new_ACCEL_CONFIG0 = ACCEL_CONFIG0::SET_ODR(new_ACCEL_CONFIG0, m_cnf.accelODR);


        //m_TxBuffer[1] = new_ACCEL_CONFIG0;

        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        opt.add = GYRO_CONFIG0_ADD;
        m_TxBuffer[0] = new_GYRO_CONFIG0;
        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_TxBuffer[0] = new_ACCEL_CONFIG0;
        opt.add = ACCEL_CONFIG0_ADD;
        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }

        m_reg.GYRO_CONFIG0 = new_GYRO_CONFIG0;
        m_reg.ACCEL_CONFIG0 = new_ACCEL_CONFIG0;
        
        m_gyroODRValue = map_gyro_ODR_CNF_to_ODR(m_cnf.gyroODR);
        m_gyroODRValue = map_gyro_FSR_CNF_to_FSR(m_cnf.gyroFSR);
        m_accelODRValue = map_accel_ODR_CNF_to_ODR(m_cnf.accelODR);
        m_accelFSRValue = map_accel_FSR_CNF_to_FSR(m_cnf.accelFSR);

        return iIMU_ST::OK;
    }
    
    
    iIMU_ST icm42688::write_config_FIFO()
    {
        PHandlerOpt opt;
        uint8_t new_INTF_CONFIG0;
        uint8_t new_FIFO_CONFIG;
        uint8_t new_FIFO_CONFIG1, new_FIFO_CONFIG2, new_FIFO_CONFIG3;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }


        // Configuracion de registro INTF_CONFIG0
        opt.add = INTF_CONFIG0_ADD;
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;

        new_INTF_CONFIG0 = m_reg.INTF_CONFIG0 | FIFO_CNF::FIFO_COUNT_REC;
        if(m_handler->write_registers(&new_INTF_CONFIG0, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        m_reg.INTF_CONFIG0 = new_INTF_CONFIG0;

        // Configuracion de registro FIFO_CONFIG
        opt.add = FIFO_CONFIG_ADD;
        new_FIFO_CONFIG = FIFO_CONFIG::SET_FIFO_MODE(m_cnf.fifoCnf.mode);
        if(m_handler->write_registers(&new_FIFO_CONFIG, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }        
        m_reg.FIFO_CONFIG = new_FIFO_CONFIG;


        // Configuracion de los registros FIFO_CONFIG1, FIFO_CONFIG2 y FIFO_CONFIG3
        if(m_cnf.fifoCnf.mode != FIFO_MODE::BYPASS){
        	//Sets packets and watermark
        	new_FIFO_CONFIG1 = 0;
        	new_FIFO_CONFIG1 = FIFO_CNF::FIFO_TEMP_EN | FIFO_CNF::FIFO_GYRO_EN | FIFO_CNF::FIFO_ACCEL_EN;
        	new_FIFO_CONFIG2 = m_cnf.fifoCnf.watermark;
        	new_FIFO_CONFIG3 = m_cnf.fifoCnf.watermark >> 8;

            
             opt.size = 1;

            opt.add = FIFO_CONFIG1_ADD;
            m_TxBuffer[0] = new_FIFO_CONFIG1;
            //m_TxBuffer[1] = new_FIFO_CONFIG2;
            //m_TxBuffer[2] = new_FIFO_CONFIG3;

            if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
                return iIMU_ST::ERROR_COMM;
            }

            opt.add = FIFO_CONFIG2_ADD;
            m_TxBuffer[0] = new_FIFO_CONFIG2;
             if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
                return iIMU_ST::ERROR_COMM;
            }

            opt.add = FIFO_CONFIG3_ADD;
            m_TxBuffer[0] = new_FIFO_CONFIG3;
             if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
                return iIMU_ST::ERROR_COMM;
            }

            m_reg.FIFO_CONFIG1 = new_FIFO_CONFIG1;
            m_reg.FIFO_CONFIG1 = new_FIFO_CONFIG2;
            m_reg.FIFO_CONFIG1 = new_FIFO_CONFIG3;
        }

        return iIMU_ST::OK;
    }


    iIMU_ST icm42688::write_config_int()
    {

        PHandlerOpt opt;
        uint8_t new_INT_CONFIG = 0x00;
        uint8_t new_INT_SOURCE0;
        uint8_t new_INT_SOURCE1;

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        opt.mode = m_mode;


    	if(m_cnf.intCnf.int1Source != INT_SOURCE::DISABLED){
    		new_INT_CONFIG |= static_cast<uint8_t>(m_cnf.intCnf.int1Mode);
    		new_INT_SOURCE0 = (1 << static_cast<uint8_t>(m_cnf.intCnf.int1Source ));
            
            opt.add = INT_SOURCE0_ADD;
            if(m_handler->write_registers(&new_INT_SOURCE0, opt) != iPHANDLER_ST::OK){
                return iIMU_ST::ERROR_COMM;
            }
            switch(m_cnf.intCnf.int1Source){
            default:
            case INT_SOURCE::DATA_RDY:
            	m_INT1->interrupt_handler = std::bind(&icm42688::save_data, *this);
				break;
            case INT_SOURCE::FIFO_THR:
            	m_INT1->interrupt_handler = std::bind(&icm42688::save_fifo_data, *this, m_cnf.fifoCnf.watermark);
					break;
            }
    	}

    	if(m_cnf.intCnf.int2Source != INT_SOURCE::DISABLED){
    		new_INT_CONFIG |= (static_cast<uint8_t>(m_cnf.intCnf.int2Mode) << 3);
    		new_INT_SOURCE1 = (1 << static_cast<uint8_t>(m_cnf.intCnf.int2Source ));
            
            opt.add = INT_SOURCE1_ADD;
            if(m_handler->write_registers(&new_INT_SOURCE1, opt) != iPHANDLER_ST::OK){
                return iIMU_ST::ERROR_COMM;
            }  
		}

        //Enables watermark interrupt on INT1
        opt.add = INT_CONFIG_ADD;
        if(m_handler->write_registers(&new_INT_CONFIG, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        } 

        return iIMU_ST::OK;
    }


    iIMU_ST icm42688::enable()
    {        
        PHandlerOpt opt;
        
        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        
        opt.add = PWR_MGMT0_ADD;
        opt.size = 1;        
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        if(m_handler->write_registers((uint8_t*)&IMU_ENABLE_LOW_NOISE, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        } 

        if(write_config() != iIMU_ST::OK){
        	return iIMU_ST::ERROR_COMM;
        }

        return iIMU_ST::OK;

    	//return write_register(PWR_MGMT0_ADD, bankRegister::BANK0, IMU_ENABLE_LOW_NOISE);
    	// Algunos milisegundos deben pasar luego de habilitar la imu para que funcione (leer pag 77 de datasheet)
    }



    iIMU_ST icm42688::disable()
	{
        PHandlerOpt opt;
        uint8_t nullValue = 0x00;
        
//        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
//            return iIMU_ST::ERROR_COMM;
//        }

        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        opt.size = 1;
        opt.slave = m_spiSet ? static_cast<void*>(m_cs) : static_cast<void*>(&m_i2cAdd);
        

        // Desabilita todos las configuraciones de interrupciones
		opt.add = INT_SOURCE0_ADD;
		if(m_handler->write_registers(&nullValue, opt) != iPHANDLER_ST::OK){
			return iIMU_ST::ERROR_COMM;
		}
		opt.add = INT_SOURCE1_ADD;
		if(m_handler->write_registers(&nullValue, opt) != iPHANDLER_ST::OK){
			return iIMU_ST::ERROR_COMM;
		}
        opt.add = INT_CONFIG_ADD;
        if(m_handler->write_registers(&nullValue, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }


        //Low power
        opt.add = PWR_MGMT0_ADD;
        if(m_handler->write_registers((uint8_t*)&IMU_DISABLE, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        } 
        return iIMU_ST::OK;
    	
        
        //return write_register(PWR_MGMT0_ADD, bankRegister::BANK0, IMU_DISABLE);
    	// Algunos milisegundos deben pasar luego de habilitar la imu para que funcione (leer pag 77 de datasheet)
	}




    iIMU_ST icm42688::flush_FIFO()
    {
        PHandlerOpt opt;
        
        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        
        opt.add = 0x4B;
        opt.size = 1;
        m_TxBuffer[0] = (1 << 1);
        if(m_handler->write_registers(m_TxBuffer, opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        } 
        return iIMU_ST::OK;
    }


    iIMU_ST icm42688::get_FIFO_count(uint16_t &count)
    {
        PHandlerOpt opt;
        
        if(set_bank_register(bankRegister::BANK0) != iIMU_ST::OK){
            return iIMU_ST::ERROR_COMM;
        }
        
        opt.add = FIFO_COUNTH_ADD;
        opt.size = 2;
        if(m_handler->read_registers(opt) != iPHANDLER_ST::OK){
            return iIMU_ST::ERROR_COMM;
        } 
		count = m_handler->m_recvBuffer[0] << 8;
		count |= m_handler->m_recvBuffer[1];

        return iIMU_ST::OK;


    }

    void icm42688::load_data(uint8_t *buffertData)
    {
    	IMUData data;
        data.temp   = convertTempData(buffertData[0], buffertData[1]);
        data.accelX = convertAccelData(buffertData[2], buffertData[3]);
        data.accelY = convertAccelData(buffertData[4], buffertData[5]);
        data.accelZ = convertAccelData(buffertData[6], buffertData[7]);
        data.gyroX  = convertGyroData(buffertData[8], buffertData[9]);
        data.gyroY  = convertGyroData(buffertData[10], buffertData[11]);
        data.gyroZ  = convertGyroData(buffertData[12], buffertData[13]);
        imuDataBuffer.write(data);
    }

    iIMU_ST icm42688::map_handler_ret_to_iIMU_ret(iPHANDLER_ST iHandlerRet){
    	iIMU_ST ret;

    	switch(iHandlerRet){
    	case iPHANDLER_ST::OK:
    		ret = iIMU_ST::OK;
    		break;
    	case iPHANDLER_ST::ERROR:
    	default:
    		ret = iIMU_ST::ERROR_COMM;
    		break;
    	case iPHANDLER_ST::UNAVAILABLE:
    		ret = iIMU_ST::ERROR_HANDLER_UNAVAILABLE;
    		break;
    	case iPHANDLER_ST::INVALID:
    		ret = iIMU_ST::ERROR_INVALID_OP;
    		break;

    	}

    	return ret;
    }


}
