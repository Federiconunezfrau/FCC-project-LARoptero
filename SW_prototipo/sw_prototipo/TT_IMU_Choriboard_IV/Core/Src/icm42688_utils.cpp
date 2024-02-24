
//#include "icm42688.hpp"
#include "icm42688_utils.hpp"
#include "stdint.h"

namespace ICM42688{

    //float map_gyro_ODR_CNF_to_ODR(const config& cnf){

	float map_gyro_ODR_CNF_to_ODR(GYRO_CNF::ODR gyroODR){
        using namespace GYRO_CNF;

        if(gyroODR == ODR::ODR_500){
            return 500.0f;
        }

        if(gyroODR <= ODR::ODR_1000 && gyroODR >= ODR::ODR_32000){
            return 32000.0f / (1 << (gyroODR - ODR::ODR_32000));
        }

        if(gyroODR <= ODR::ODR_12_5 && gyroODR >= ODR::ODR_200){
            return 200.0f /  (1 << (gyroODR - ODR::ODR_200));
        }

        return -1.0f;
    }


    //float map_gyro_FSR_CNF_to_FSR(const config& cnf){
	float map_gyro_FSR_CNF_to_FSR(GYRO_CNF::FSR gyroFSR){
        using namespace GYRO_CNF;

        if(gyroFSR <= FSR::FSR_15_625){
            return 2000.0f / (1 << gyroFSR);
        }

        return -1.0f;
    }

    //float map_accel_ODR_CNF_to_ODR(const config& cnf){
	float map_accel_ODR_CNF_to_ODR(ACCEL_CNF::ODR accelODR){
        using namespace ACCEL_CNF;

        if(accelODR == ODR::ODR_500){
            return 500.0f;
        }

        if(accelODR <= ODR::ODR_1000 && accelODR >= ODR::ODR_32000){
            return 32000.0f / (1 << (accelODR - ODR::ODR_32000)) ;
        }

        if(accelODR <= ODR::ODR_1_562 && accelODR >= ODR::ODR_200){
            return 200.0f /  ( 1 << (accelODR - ODR::ODR_200));
        }

        return -1.0f;
    }

    //float map_accel_FSR_CNF_to_FSR(const config& cnf){
	float map_accel_FSR_CNF_to_FSR(ACCEL_CNF::FSR accelFSR){
        using namespace ACCEL_CNF;
  
        if(accelFSR <= FSR::FSR_2){
            return 16.0f / (1 << accelFSR);
        }

        return -1.0f;
    }

    
    float convertTempData(uint8_t rawTempH, uint8_t rawTempL)
    {
        int16_t temp_data = (rawTempH << 8) |  rawTempL;
        return (temp_data / 132.48f + 25);
    }

    float convertTempFIFOData(int8_t temp_data)
    {
        return (temp_data / 2.07f + 25);

    }
















    /////////////////////// DEFINICIÓN DE OPERADORES //////////////////////////


    /////////////////////////// bankRegister /////////////////////////////////
    


    /////////////////////////// GYRO_CNF /////////////////////////////////////

    namespace GYRO_CNF {

        bool operator<=(ODR odr1, ODR odr2) 
        {
            return static_cast<uint8_t> (odr1) <= static_cast<uint8_t> (odr2);
        }
        bool operator>=(ODR odr1, ODR odr2) 
        {
            return static_cast<uint8_t> (odr1) >= static_cast<uint8_t> (odr2);
        }
        uint8_t operator-(ODR odr1, ODR odr2)
        {
            return static_cast<uint8_t> (odr1) - static_cast<uint8_t> (odr2);
        }


        bool operator<=(FSR fsr1, FSR fsr2)
        {
            return static_cast<uint8_t> (fsr1) <= static_cast<uint8_t> (fsr2);
        }
    }




    /////////////////////////// ACCEL_CNF /////////////////////////////////////
    namespace ACCEL_CNF {

        bool operator<=(ODR odr1, ODR odr2) 
        {
            return static_cast<uint8_t> (odr1) <= static_cast<uint8_t> (odr2);
        }
        bool operator>=(ODR odr1, ODR odr2) 
        {
            return static_cast<uint8_t> (odr1) >= static_cast<uint8_t> (odr2);
        }
        uint8_t operator-(ODR odr1, ODR odr2)
        {
            return static_cast<uint8_t> (odr1) - static_cast<uint8_t> (odr2);
        }

        bool operator<=(FSR fsr1, FSR fsr2)
        {
            return static_cast<uint8_t> (fsr1) <= static_cast<uint8_t> (fsr2);
        }  
    }


}
    
