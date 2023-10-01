/*
 * ICM42688.h
 *
 *  Created on: Sep 26, 2023
 *      Author: fede
 */

#ifndef INC_ICM42688_H_
#define INC_ICM42688_H_

#include "stm32f7xx_hal.h"

// Estos enumerativos definen los posibles códigos de error devueltos por los métodos de la IMU.
// En el caso de que alguno de sus métodos presente algún comportamiento anómalo, se informa a la
// aplicaicón, utilizando estos valores.
enum errorCodeIMU_t : uint8_t
{
  IMU_OK = 0x00,
  IMU_ERROR_NULL_POINTER = 0x01,
  IMU_ERROR_INVALID_NUMBER_OF_REGISTERS = 0x02,
  IMU_ERROR_INVALID_PARAMETER = 0x03,
  IMU_ERROR_TIMEOUT_COMMUNICATION = 0x04,
  IMU_ERROR_HAL = 0x05,
  IMU_ERROR_HAL_BUSY = 0x06
};

// Clase ICM42688. Esta clase sirve como "driver" para el manejo de la IMU. Presenta una capa de abstracción,
// que se encarga de realizar las tareas asociadas a la IMU, como configurarla y obtener mediciones, abstrayéndose
// de cuestioens como el periférico para la comunicación, etc.
class ICM42688
{
	private:

		// Esta definición solamente sirve para no hardcodear el tipo de dato del atributo "timeOutSPIms_t"
		typedef uint32_t timeOutSPIms_t;

		// Handler para el periférico SPI asociado a la IMU. El micro se comunica con la IMU a través de
		// SPI, utilizando el periférico que le corresponde a este handler.
		SPI_HandleTypeDef *pmHandlerSPI_;

		// GPIO usado como Chip Select. Para iniciar la comunicación por SPI, el micro setea este pin en
		// 0. Cuando la comunicación finaliza, se vuelve a subir a 1.
		GPIO_TypeDef* pmGPIOportNSS_;
		uint16_t mGPIOpinNSS_;

		// GPIO usado para detectar la interrupción de la IMU. Este pin se debe configurar como GPIO_EXTI,
		// es decir, como input que detecta una interrupción por flanco. Cada vez que la IMU genera un dato
		// nuevo, esta genera togglea una de sus salidas, llamada INT1. Dicha salida debe estar conectada al
		// pin del micro asociado a estos atributos.
		GPIO_TypeDef* pmGPIOportINT_;
		uint16_t mGPIOpinINT_;

		// Timeout para el periférico SPI, en milisegundos. El micro se comunica con la IMU usando uno de
		// sus periféricos SPI. Este valor es un timeout que se utiliza, en caso de no obtener respuesta
		// de la IMU.
		timeOutSPIms_t mTimeOutSPIms_;

		// Factor de escala para las lecturas de acelerómetro. Los valores de aceleración que entrega la IMU,
		// no tienen unidades, sino que son números de 16 bits. Este factor de escala se utiliza para
		// convertir ese número, a un valor físico. Las unidades de este atributo son "g", donde 1g = 9.8 m/s^2.
		float mAccelScale_;

		// Factor de escala para las lecturas de giróscopo. Los valores de giróscopo que entrega la IMU,
		// no tienen unidades, sino que son números de 16 bits. Este factor de escala se utiliza para
		// convertir ese número, a un valor físico. Las unidades de este atributo son degrees-per-second (dps).
		float mGyroScale_;

		// Método privado, utilizado para escribir 1 registro de la IMU, por comunicación SPI.
		errorCodeIMU_t write_register(uint8_t, uint8_t);

		// Método privado, utilizado para leer varios registros de la IMU por comunicación SPI, usando
		// burst read.
		errorCodeIMU_t read_registers(uint8_t, uint16_t, uint8_t* );

		// Esta función devuelve un código de error en función del valor pasado de HAL_StatusTypeDef.
		// Los métodos de esta clase devuelven códigos de error, que simplifican el debugging. En el
		// caso en el que falle una de las funciones de la HAL de STM32, esta función devuelve un valor
		// de errorCodeIMU_t que se corresponde con el código de error devuelto por la HAL.
		errorCodeIMU_t get_error_code(HAL_StatusTypeDef);

	public:

		// La IMU puede configurarse para tener distintos rangos en sus acelerómetros. Este enumerativo
		// define las distintas opciones. Solamente una de estas opciones puede utilzarse para setear
		// el rango de aacelerómetros de la IMU.
		enum fullScaleAccel_t : uint8_t
		{
	      gpm16 = 0x00, // -16g , +16g
	      gpm8 = 0x01,  //  -8g , +8g
	      gpm4 = 0x02,  //  -4g , +4g
	      gpm2 = 0x03   //  -2g , +2g
	    };

		// La IMU puede configurarse para tener distintos rangos en sus giróscopos. Este enumerativo
		// define las distintas opciones. Solamente una de estas opciones puede utilzarse para setear
		// el rango de giróscopos de la IMU.
	    enum fullScaleGyro_t : uint8_t
		{
	      dps2000 = 0x00,   //  -2000°  , +2000°
	      dps1000 = 0x01,   //  -1000°  , +1000°
	      dps500 = 0x02,    //   -500°  , +500°
	      dps250 = 0x03,    //   -250°  , +250°
	      dps125 = 0x04,    //   -125°  , +125°
	      dps62_5 = 0x05,   //  -62.5°  , +62.5°
	      dps31_25 = 0x06,  //  -31.25° , -31.25°
	      dps15_625 = 0x07  // -15.265° , +15.265°
	    };

	    // Este valor indica cada cuánto tiempo los acelerómetros generan un nuevo dato,
	    // Solo uno de estos valores puede utilizarse.
	    enum odrAccel_t : uint8_t
		{
	      odrAccel32k = 0x01,    // 32 kHz (LN mode only)
	      odrAccel16k = 0x02,    // 16 kHz (LN mode only)
	      odrAccel8k = 0x03,     //  8 kHz (LN mode only)
	      odrAccel4k = 0x04,     //  4 kHz (LN mode only)
	      odrAccel2k = 0x05,     //  2 kHz (LN mode only)
	      odrAccel1k = 0x06,     //  1 kHz (LN mode only)
	      odrAccel200 = 0x07,    //  200 Hz
	      odrAccel100 = 0x08,    //  100 Hz
	      odrAccel50 = 0x09,     //   50 Hz
	      odrAccel25 = 0x0A,     //   25 Hz
	      odrAccel12_5 = 0x0B,   //   12.5 Hz
	      odrAccel6a25 = 0x0C,   //   6.25 Hz (LP mode only)
	      odrAccel3a125 = 0x0D,  //   3.125 Hz (LP mode only)
	      odr1Accela5625 = 0x0E, //   1.5625 Hz (LP mode only)
	      odrAccel500 = 0x0F,    //   500 Hz
	    };

	    // Este valor indica cada cuánto tiempo los giróscopos generan un nuevo dato,
	    // Solo uno de estos valores puede utilizarse.
	    enum odrGyro_t : uint8_t
		{
	      odrGyro32k = 0x01,
	      odrGyro16k = 0x02,
	      odrGyro8k = 0x03,
	      odrGyro4k = 0x04,
	      odrGyro2k = 0x05,
	      odrGyro1k = 0x06,
	      odrGyro200 = 0x07,
	      odrGyro100 = 0x08,
	      odrGyro50 = 0x09,
	      odrGyro25 = 0x0A,
	      odrGyro12_5 = 0x0B,
	      odrGyro500 = 0x0F,
	    };

		// Constructor. Recibe el handler al periférico SPI, que ya debe estar previamente configurado,
	    // el puerto y el pin correspondientes al chip select, que ya debe estar configurado como salida.
		ICM42688(SPI_HandleTypeDef *, GPIO_TypeDef*, uint16_t);

		// Destructor
		~ICM42688();

		// Setter del timeout de la comunicación SPI.
		void set_timeout_spi(timeOutSPIms_t mTimeOutSPIms);

		// Getter del timeout de la comunicación SPI
		timeOutSPIms_t get_timeout_spi(void);

		// Este método le pide al dispositivo IMU, un valor que sirve para identificarla, el cual
		// tiene definido en uno de sus registros, llamado "WHO_AM_I".
		errorCodeIMU_t who_am_i(uint8_t *);

		// Habilita los acelerómetros y giróscopos de la IMU.
		errorCodeIMU_t enable_accel_and_gyro(void);

		// Deshabilita los acelerómetros y giróscopos de la IMU.
		errorCodeIMU_t disable_accel_and_gyro(void);

		// Lee los valores más recientes de acelerómetros y giróscopos, generados por la IMU.
		errorCodeIMU_t read_raw_data(int16_t *, int16_t *, int16_t *, int16_t *, int16_t *, int16_t *);

		// Lee los valores más recientes de los acelerómetros y giróscopos, escalados por sus correspondientes
		// factores de escala. Los valores tienen unidades físicas.
		errorCodeIMU_t read_data(float *, float *, float *, float *, float *, float *);

		// Setea el rango de la IMU, para los acelerómetros.
		errorCodeIMU_t set_accel_full_scale_range(fullScaleAccel_t);

		// Setea el rango de la IMU, para los giróscopos.
		errorCodeIMU_t set_gyro_full_scale_range(fullScaleGyro_t);

		// Setea el output data rate (ODR) de los acelerómetros.
		errorCodeIMU_t set_accel_output_data_rate(odrAccel_t);

		// Setea el output data rate (ODR) de los giróscopos.
		errorCodeIMU_t set_gyro_output_data_rate(odrGyro_t);

		// Activa el modo de interrupción de la IMU.
		errorCodeIMU_t set_interrupt_mode(GPIO_TypeDef *, uint16_t);
};


#endif /* INC_ICM42688_H_ */
