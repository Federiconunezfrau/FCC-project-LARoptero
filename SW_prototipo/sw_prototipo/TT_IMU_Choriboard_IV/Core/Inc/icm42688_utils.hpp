#ifndef _ICM42688_UTILS_HPP_
#define _ICM42688_UTILS_HPP_

#include "stdint.h"
//#include "icm42688_utils.hpp"
//#include "icm42688.hpp"

namespace ICM42688 {

    constexpr uint8_t IMU_DATA_N_REGS = 14; // 2 x temp + 2 * 3 (gyro x, y and z) 2 * 3 (accel x, y and z)
    constexpr uint8_t FIFO_PACKET_SIZE = 16; // 1 x header + 6 x accel + 6 x gyro + 2 x temp bytes

    enum class bankRegister {
        BANK0 	=	0	,
        BANK1 			,
        BANK2			,
        BANK3			,
        BANK4
    };
    
    enum class FIFO_MODE{
        BYPASS        =	0	,
        STREAM				,
        STOP_ON_FULL
    };

    enum class INT_SOURCE{
    	DISABLED	=	-1	,
    	FIFO_FULL	=	1	,
    	FIFO_THR			,
    	DATA_RDY			,
    };

    enum class INT_MODE{
    	DISABLED			=	-1	,
		PULSED_OD_LOW				,
		PULSED_OD_HIGH				,
		PULSED_PP_LOW				,
		PULSED_PP_HIGH				,
		LATCHED_OD_LOW				,
		LATCHED_OD_HIGH				,
		LATCHED_PP_LOW				,
		LATCHED_PP_HIGH				,
    };


    namespace GYRO_CNF{
        

    	enum class ODR{
            ODR_32000 = 0x01,
            ODR_16000 = 0x02,
            ODR_8000  = 0x03,
            ODR_4000  = 0x04,
            ODR_2000  = 0x05,
            ODR_1000  = 0x06,
            ODR_200   = 0x07,
            ODR_100   = 0x08,
            ODR_50    = 0x09,
            ODR_25    = 0x0A,
            ODR_12_5  = 0x0B,
            ODR_500   = 0x0F,
        };


        bool operator<=(ODR odr1, ODR odr2); 
        bool operator>=(ODR odr1, ODR odr2);
        uint8_t operator-(ODR odr1, ODR odr2); 
        constexpr uint32_t operator<<(uint8_t n, ODR odr)
        {
            return  n << static_cast<uint8_t> (odr);
        }
        constexpr uint32_t operator<<(ODR odr, uint8_t n)
        {
            return  static_cast<uint8_t> (odr) << n ;
        }
        //constexpr uint32_t operator<<(uint8_t n, ODR odr); 
        //constexpr uint32_t operator<<(ODR odr, uint8_t n); 
        

        enum class FSR
        {
            FSR_2000      = 0,
            FSR_1000      = 1,
            FSR_500       = 2,
            FSR_250       = 3,
            FSR_125       = 4,
            FSR_62_5      = 5,
            FSR_31_25     = 6,
            FSR_15_625    = 7,
        };

        bool operator<=(FSR fsr1, FSR fsr2);  
        constexpr uint32_t operator<<(uint8_t n, FSR fsr)
        {
            return  n << static_cast<uint8_t> (fsr);
        }   
        constexpr uint32_t operator<<( FSR fsr, uint8_t n)
        {
            return  static_cast<uint8_t> (fsr) << n;
        } 
    }

    namespace ACCEL_CNF{
        
        enum class ODR
        {
            ODR_32000 = 0x01,
            ODR_16000 = 0x02,
            ODR_8000  = 0x03,
            ODR_4000  = 0x04,
            ODR_2000  = 0x05,
            ODR_1000  = 0x06,
            ODR_200   = 0x07,
            ODR_100   = 0x08,
            ODR_50    = 0x09,
            ODR_25    = 0x0A,
            ODR_12_5  = 0x0B,
            ODR_6_25  = 0x0C,
            ODR_3_125 = 0x0D,
            ODR_1_562 = 0x0E,
            ODR_500   = 0x0F,
        };

        bool operator<=(ODR odr1, ODR odr2); 
        bool operator>=(ODR odr1, ODR odr2);
        uint8_t operator-(ODR odr1, ODR odr2); 
        constexpr uint32_t operator<<(uint8_t n, ODR odr)
        {
            return  n << static_cast<uint8_t> (odr);
        }
        constexpr uint32_t operator<<( ODR odr, uint8_t n)
        {
            return  static_cast<uint8_t> (odr) << n;
        }

        enum class FSR
        {
            FSR_16   = 0,
            FSR_8    = 1,
            FSR_4    = 2,
            FSR_2    = 3,
        };

        bool operator<=(FSR fsr1, FSR fsr2);  
        constexpr uint32_t operator<<(uint8_t n, FSR fsr)
        {
            return  n << static_cast<uint8_t> (fsr);
        } 
        constexpr uint32_t operator<<(FSR fsr, uint8_t n)
        {
            return  static_cast<uint8_t> (fsr) << n;
        } 
    }

    namespace FIFO_CNF{

        constexpr uint8_t FIFO_COUNT_REC    = 1<<6;
        constexpr uint8_t FIFO_TEMP_EN      = 1<<2;
        constexpr uint8_t FIFO_GYRO_EN      = 1<<1;
        constexpr uint8_t FIFO_ACCEL_EN     = 1<<0;
        constexpr uint8_t FIFO_THS_INT1_EN  = 1<<2;
    }

    struct config_FIFO{
    	uint16_t watermark;
    	FIFO_MODE mode;
    };

    struct config_INT{
    	INT_SOURCE int1Source;
    	INT_MODE int1Mode;
    	INT_SOURCE int2Source;
    	INT_MODE int2Mode;
    };

	struct Config{
		GYRO_CNF::FSR gyroFSR;
        GYRO_CNF::ODR gyroODR;
        ACCEL_CNF::FSR accelFSR;
        ACCEL_CNF::ODR accelODR;
        config_FIFO fifoCnf;
        config_INT intCnf;
        //bool saveIMUDataBlockingMode;       // true => communication is through a blocking function;

    };



    //float map_gyro_ODR_CNF_to_ODR(const config& cnf);
    float map_gyro_ODR_CNF_to_ODR(GYRO_CNF::ODR gyroODR);

    //float map_gyro_FSR_CNF_to_FSR(const config& cnf);
    float map_gyro_FSR_CNF_to_FSR(GYRO_CNF::FSR gyroFSR);

    //float map_accel_ODR_CNF_to_ODR(const config& cnf);
    float map_accel_ODR_CNF_to_ODR(ACCEL_CNF::ODR accelODR);

    //float map_accel_FSR_CNF_to_FSR(const config& cnf);
    float map_accel_FSR_CNF_to_FSR(ACCEL_CNF::FSR accelFSR);

    float convertTempData(uint8_t rawTempH, uint8_t rawTempL);
    
    float convertTempFIFOData(int8_t temp_data);

    //////////// Definiciones de direcciones y valores relacionados a los registros del ICM 42688 //////////////

    // BANK 0

    // Data address registers

    constexpr uint8_t INT_CONFIG_ADD 		= 0x14;
    constexpr uint8_t TEMP_DATA1_ADD 		= 0x1D;
    constexpr uint8_t TEMP_DATA0_ADD 		= 0x1E;
    
    constexpr uint8_t ACCEL_DATA_X1_ADD 	= 0x1F;
    constexpr uint8_t ACCEL_DATA_X0_ADD 	= 0x20;
    constexpr uint8_t ACCEL_DATA_Y1_ADD 	= 0x21;
    constexpr uint8_t ACCEL_DATA_Y0_ADD 	= 0x22;
    constexpr uint8_t ACCEL_DATA_Z1_ADD 	= 0x23;
    constexpr uint8_t ACCEL_DATA_Z0_ADD 	= 0x24;

    constexpr uint8_t GYRO_DATA_X1_ADD 		= 0x25;
    constexpr uint8_t GYRO_DATA_X0_ADD 		= 0x26;
    constexpr uint8_t GYRO_DATA_Y1_ADD 		= 0x27;
    constexpr uint8_t GYRO_DATA_Y0_ADD 		= 0x28;
    constexpr uint8_t GYRO_DATA_Z1_ADD 		= 0x29;
    constexpr uint8_t GYRO_DATA_Z0_ADD 		= 0x2A;
    

    constexpr uint8_t FIFO_COUNTH_ADD 		= 0x2E;

    constexpr uint8_t FIFO_DATA_ADD     	= 0x30;
    constexpr uint8_t INTF_CONFIG0_ADD 		= 0x4C;

    constexpr uint8_t PWR_MGMT0_ADD 		= 0x4E;

    constexpr uint8_t GYRO_CONFIG1_ADD 		= 0x51;
    constexpr uint8_t GYRO_ACCEL_CONFIG0_ADD = 0x52;
    constexpr uint8_t ACCEL_CONFIG1_ADD 	= 0x53;
    constexpr uint8_t FIFO_CONFIG1_ADD 		= 0x5F;
    constexpr uint8_t FIFO_CONFIG2_ADD 		= 0x60;
    constexpr uint8_t FIFO_CONFIG3_ADD 		= 0x61;

    constexpr uint8_t INT_SOURCE0_ADD 		= 0x65;
    constexpr uint8_t INT_SOURCE1_ADD 		= 0x66;



    constexpr uint8_t REG_BANK_SEL_ADD 		= 0x76;

    ///////////////////////////////////////////////////////////////////////////////

    //FIFOconfig addresses and bits
	constexpr uint8_t FIFO_CONFIG_ADD = 0x16;
	namespace FIFO_CONFIG {
		constexpr uint8_t FIFO_MODE_POS = 0x06;
		constexpr uint8_t FIFO_MODE_MASK = static_cast<uint8_t>(0xC0  << FIFO_MODE_POS);
		constexpr uint8_t SET_FIFO_MODE(FIFO_MODE fifoMode) {
			return  (static_cast<uint8_t>(fifoMode) << FIFO_MODE_POS);
		}

	}
	///////////////////////////////////////////////////////////////////////////////


    //Gyro config addresses and bits
    constexpr uint8_t GYRO_CONFIG0_ADD = 0x4F;
    namespace GYRO_CONFIG0 {
        constexpr uint8_t FS_SEL_POS = 0x05;
        constexpr uint8_t FS_SEL_MASK = 0x07  << FS_SEL_POS;
        constexpr uint8_t SET_FSR(uint8_t reg, GYRO_CNF::FSR fsr) {
            return  (reg & ~FS_SEL_MASK) | ( fsr << FS_SEL_POS);
        }

        constexpr uint8_t ODR_POS = 0x00;
        constexpr uint8_t ODR_MASK = 0x0F  << ODR_POS;
        constexpr uint8_t SET_ODR(uint8_t reg, GYRO_CNF::ODR odr) {
            return  (reg & ~ODR_MASK) | ( odr << ODR_POS);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////

    //Accel config addresses and bits
    constexpr uint8_t ACCEL_CONFIG0_ADD = 0x50;
    namespace ACCEL_CONFIG0 {
        constexpr uint8_t FS_SEL_POS = 0x05;
        constexpr uint8_t FS_SEL_MASK = 0x07  << FS_SEL_POS;
        constexpr uint8_t SET_FSR(uint8_t reg, ACCEL_CNF::FSR fsr) {
            return   (reg & ~FS_SEL_MASK)  | ( fsr << FS_SEL_POS);
        }

        constexpr uint8_t ODR_POS = 0x00;
        constexpr uint8_t ODR_MASK = 0x0F  << ODR_POS;
        constexpr uint8_t SET_ODR(uint8_t reg, ACCEL_CNF::ODR odr) {
            return   (reg & ~ODR_MASK) | ( odr << ODR_POS);
        }
    }



    constexpr uint8_t IMU_ENABLE_LOW_NOISE 	= 0x0F;
    constexpr uint8_t IMU_DISABLE 			= 0x00;

    ///////////////////////////////////////////////////////////////////////////////////////
    constexpr uint8_t SPI_ADD_READ(uint8_t add) {return (1 << 7) | add;}
    constexpr uint8_t SPI_ADD_WRITE(uint8_t add) {return add;}

}


#endif
