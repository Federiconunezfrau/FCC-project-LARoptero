/**
 * @file iIMU.h
 * @author your name (you@domain.com)
 * @brief Este archivo define la interface básica de las IMU. Debe poder adaptarse para funcionar como interface para acelerómetros solamente.
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef iIMU_HPP_
#define iIMU_HPP_

#include "stdint.h"

enum class iIMU_ST {
    OK                  = 0,
    ERROR_COMM          = 1,   
    ERROR_CNF           = 2,
    ERROR_INVALID_OP    = 3,
    ERROR_BUFFER 		,
	ERROR_HANDLER_UNAVAILABLE,
};
/*
enum class axis {
    X = 0,
    Y = 1,
    Z = 2
};
*/

/*
struct RawData_t {
    uint16_t gyroX;
    uint16_t gyroY;
    uint16_t gyroZ;
    uint16_t AccelX;
    uint16_t AccelY;
    uint16_t AccelZ;
    uint16_t temp;
};
*/

struct IMUData {
    float temp;
    float gyroX;
    float gyroY;
    float gyroZ;
    float accelX;
    float accelY;
    float accelZ;
};

/**
 * @brief Define un struct vacio para que se haga un struct herencia del mismo en los archivos de implementación. Esto es para dejar a la implementación la forma en obtener los  datos de la IMU. Por ejemplo, si se tiene 1 IMU con lectura sobre 1 solo eje vs 3 ejes. De esta forma también se podría usar esta interfaz para implementar acelerómetros.
 * 
 */
// struct iImu_data{};

class iIMU {
public:

    virtual iIMU_ST save_data() = 0;
    virtual void    save_data_callback() = 0;

    virtual iIMU_ST save_fifo_data(uint8_t qty) = 0;
    virtual void    save_fifo_data_callback() = 0;

    virtual iIMU_ST read_data(IMUData *pData) = 0;
    
    virtual iIMU_ST enable() = 0;
    virtual iIMU_ST disable() = 0;


    /**
     * @brief Configura el ODR del giróscopo
     * 
     * @param odr ODR a configurar. En la clase de implementación debe ser un enum correspondiente a los ODR que soporta la IMU 
     * @return iIMU_st Devuelve el estado de la operación de configuración
     *                  - IIMU_ST_OK: Se logró configurar la IMU
     *                  - IIMU_ST_ERROR_COMM: Se detectó un error en la comunicación con la IMU.
     */
    virtual iIMU_ST set_gyro_odr(uint8_t odr) = 0;

    /**
     * @brief Configura el FSR del giróscopo
     * 
     * @param odr FSR a configurar. En la clase de implementación debe ser un enum correspondiente a los FSR que soporta la IMU 
     * @return iIMU_st Devuelve el estado de la operación de configuración
     *                  - IIMU_ST_OK: Se logró configurar la IMU
     *                  - IIMU_ST_ERROR_COMM: Se detectó un error en la comunicación con la IMU.
     */
    virtual iIMU_ST set_gyro_fsr(uint8_t odr) = 0;

        /**
     * @brief Configura el ODR del acelerómetro
     * 
     * @param odr ODR a configurar. En la clase de implementación debe ser un enum correspondiente a los ODR que soporta la IMU 
     * @return iIMU_st Devuelve el estado de la operación de configuración
     *                  - IIMU_ST_OK: Se logró configurar la IMU
     *                  - IIMU_ST_ERROR_COMM: Se detectó un error en la comunicación con la IMU.
     */
    virtual iIMU_ST set_accel_odr(uint8_t odr) = 0;

        /**
     * @brief Configura el FSR del acelerómetro
     * 
     * @param odr FSR a configurar. En la clase de implementación debe ser un enum correspondiente a los FSR que soporta la IMU 
     * @return iIMU_st Devuelve el estado de la operación de configuración
     *                  - IIMU_ST_OK: Se logró configurar la IMU
     *                  - IIMU_ST_ERROR_COMM: Se detectó un error en la comunicación con la IMU.
     */
    virtual iIMU_ST set_accel_fsr(uint8_t fsr) = 0;

    virtual float get_accel_odr_value(void)     = 0;
    virtual float get_accel_fsr_value(void)     = 0;
    virtual float get_gyro_odr_value(void)      = 0;
    virtual float get_gyro_fsr_value(void)      = 0;
    
    virtual float get_accel_resolution(void)    = 0;
    virtual float get_gyro_resolution(void)     = 0;

    virtual iIMU_ST write_config(void)          = 0;
   

};

#endif
