/*
 * ICM42688.cpp
 *
 *  Created on: Sep 26, 2023
 *      Author: fede
 */

#include <ICM42688.h>
#include <string.h>

// Tamaño máximo del buffer usado para leer registros de la IMU por burst read SPI.
#define MAX_SIZE_READ_BUFFER 32

// Cuando se llama al constructor, por defecto se setea este valor de timeout para la
// comunicación SPI.
#define TIMEOUT_SPI_DEFAULT_MS 1000

// Registros de la IMU, utilizados para distintas funcionalidades. Los valores representan
// las posiciones de memoria de la IMU y se extrajeron del datasheet.
#define ADDRESS_PWR_MGMT0     0x4E
#define ADDRESS_WHO_AM_I      0x75
#define ADDRESS_ACCEL_CONFIG0 0x50
#define ADDRESS_GYRO_CONFIG0  0x4F
#define ADDRESS_ACCEL_DATA_X1 0x1F
#define ADDRESS_ACCEL_DATA_X0 0x20
#define ADDRESS_ACCEL_DATA_Y1 0x21
#define ADDRESS_ACCEL_DATA_Y0 0x22
#define ADDRESS_ACCEL_DATA_Z1 0x23
#define ADDRESS_ACCEL_DATA_Z0 0x24
#define ADDRESS_INT_CONFIG    0x14
#define ADDRESS_INT_CONFIG1   0x64
#define ADDRESS_INT_SOURCE0   0x65

// Macro utilizadas para configurar la INT1 de la IMU. "reg" es un uint8_t que representa
// el valor a escribir en un registro. Cada una de estas macros setea un bit en 1 ó 0.
// Registro: INT_CONFIG
#define INT1_SET_MODE_PULSED(reg) reg = reg & 0xFB
#define INT1_SET_MODE_LATCHED(reg) reg = reg | 0x04
#define INT1_SET_DRIVE_CIRCUIT_OPEN_DRAIN(reg) reg = reg & 0xFD
#define INT1_SET_DRIVE_CIRCUIT_PUSH_PULL(reg) reg = reg | 0x02
#define INT1_SET_POLARITY_LOW(reg) reg = reg & 0xFE
#define INT1_SET_POLARITY_HIGH(reg) reg = reg | 0x01

// Registro: INT_CONFIG1
#define INT_SET_PULSE_DURATION_100US(reg) reg = reg & 0xBF
#define INT_SET_PULSE_DURATION_8US(reg) reg = reg | 0x40
#define INT_SET_DEASSERT_ENABLE(reg) reg = reg & 0xDF
#define INT_SET_DEASSERT_DISABLE(reg) reg = reg | 0x20
#define INT_ENABLE(reg) reg = reg & 0xEF

// Registro: INT_SOURCE0
#define INT1_SET_SOURCE_DATA_READY(reg) reg = 0x18

// ============================ Constructor ============================
//
// Este método setea los valores de los atributos solamente. La IMU aún no comienza su comunicación con
// el micrcontrolador. Esto debe habilitarse con llamado a otras funciones. Este método setea el valor
// de timeout de la comunicación por SPI, en un valor por defecto. Luego puede configurarse con otros
// métodos.
//
// pHandlerSPI: es la dirección de memoria del handler al periférico SPI. El driver utilizará dicho
//              periférico para la comunicación con el dispositivo. DEBE ESTAR PREVIAMENTE CONFIGURADO!!!
//
// pGPIOportNSS: tipo de dato de la HAL, que represetna un puerto del micro. Este, junto con el parámetro
// GPIOpinNSS, sirven para indicarle al driver cuál es el pin utilizado como chip select de SPI.
//
// GPIOpinNSS: pin del micro utilizado como chip select.
ICM42688::ICM42688(SPI_HandleTypeDef *pHandlerSPI, GPIO_TypeDef *pGPIOportNSS, uint16_t GPIOpinNSS)
{
	pmHandlerSPI_ = pHandlerSPI;
	mTimeOutSPIms_ = TIMEOUT_SPI_DEFAULT_MS;

	pmGPIOportNSS_ = pGPIOportNSS;
	mGPIOpinNSS_ = GPIOpinNSS;

//	enable_accel_and_gyro();
//
//	set_accel_full_scale_range(gpm16);
//	set_gyro_full_scale_range(dps2000);
}

// ============================ Destructor ============================
//
// Método destructor de la IMU.
ICM42688::~ICM42688()
{

}

// ============================ get_error_code ============================

// Esta función devuelve un código de error en función del valor pasado de HAL_StatusTypeDef.
// Los métodos de esta clase devuelven códigos de error, que simplifican el debugging. En el
// caso en el que falle una de las funciones de la HAL de STM32, esta función devuelve un valor
// de errorCodeIMU_t que se corresponde con el código de error devuelto por la HAL.
//
// errorCodeHAL: código de error de la HAL de STM32.
errorCodeIMU_t ICM42688::get_error_code(HAL_StatusTypeDef errorCodeHAL)
{
	switch(errorCodeHAL)
	{
		case HAL_OK:
			return IMU_OK;

		case HAL_ERROR:
			return IMU_ERROR_HAL;

		case HAL_BUSY:
			return IMU_ERROR_HAL_BUSY;

		case HAL_TIMEOUT:
			return IMU_ERROR_TIMEOUT_COMMUNICATION;

		default:
			return IMU_ERROR_HAL;
	}
}

// ============================ set_timeout_spi ============================
//
// Setter del timeout de la comunicación SPI.
//
// mTimeOutSPIms: timeout utilizdo para la comunicación por SPI, en milisegundos.
void ICM42688::set_timeout_spi(timeOutSPIms_t mTimeOutSPIms)
{
	mTimeOutSPIms_ = mTimeOutSPIms;
}

// ============================ get_timeout_spi ============================
// Getter del timeout de la comunicación SPI
ICM42688::timeOutSPIms_t ICM42688::get_timeout_spi(void)
{
	return mTimeOutSPIms_;
}

// ============================ write_register ============================
//
// Método privado, utilizado para escribir 1 registro de la IMU, por comunicación SPI.
// El método togglea el pin chip select a 0, le indica al handler de SPI que se comunique
// con la IMU, para escribir un registro y vuelve a subir la salida chip select a 1.
// Para poder escribir un registro de la IMU, el datasheet indica que el bit 7 del address
// debe ser igual a 0. Este método se encarga de hacer eso acá, no es necesario que el valor
// address venga con el bit 7 seteado en 1.
//
// address: dirección de memoria del registro de la IMU a escribir.
// data: valor a escribir en el registro "address".
errorCodeIMU_t ICM42688::write_register(uint8_t address, uint8_t data)
{
	uint8_t pDataBuffer[2];
	HAL_StatusTypeDef errorCodeHAL;

	pDataBuffer[0] = address & 0x7F;
	pDataBuffer[1] = data;

	HAL_GPIO_WritePin(pmGPIOportNSS_, mGPIOpinNSS_, GPIO_PIN_RESET);

	errorCodeHAL = HAL_SPI_Transmit(pmHandlerSPI_, pDataBuffer, 2, mTimeOutSPIms_);

	HAL_GPIO_WritePin(pmGPIOportNSS_, mGPIOpinNSS_, GPIO_PIN_SET);

	return get_error_code(errorCodeHAL);
}

// ============================ read_registers ============================
//
// Método privado, utilizado para leer varios registros de la IMU a la vez,
// por comunicación SPI, utilizando burst read. El método togglea el pin chip
// select a 0, le indica al handler de SPI que se comunique con la IMU, para
// leer los registros y vuelve a subir la salida chip select a 1. Para poder
// leer un registro de la IMU, el datasheet indica que el bit 7 del address
// debe ser igual a 1. Este método se encarga de hacer eso acá, no es necesario
// que el valor address venga con el bit 7 seteado en 1.
//
// startingAddress: dirección de memoria del primer registro de la IMU a leer.
// numberOfRegisters: cantidad de registros a leer, comenzando por startingAddress.
// Este no puede superar a la constante MAX_SIZE_READ_BUFFER. En cuyo caso, se
// devuelve un código de error.
// dataRead[]: arraeglo de uint8_t donde se devuelven los registros leidos.
errorCodeIMU_t ICM42688::read_registers(uint8_t startingAddress, uint16_t numberOfRegisters, uint8_t dataRead[])
{
	uint8_t txBuffer[MAX_SIZE_READ_BUFFER] = {0};
	HAL_StatusTypeDef errorCodeHAL;
	uint8_t i = 0;
	uint8_t count = 0;

	if( dataRead == NULL )
	{
		return IMU_ERROR_NULL_POINTER;
	}

	if( numberOfRegisters <= 0 || numberOfRegisters > (MAX_SIZE_READ_BUFFER-1) )
	{
		return IMU_ERROR_INVALID_NUMBER_OF_REGISTERS;
	}

	txBuffer[0] = startingAddress | 0x80;


	HAL_GPIO_WritePin(pmGPIOportNSS_, mGPIOpinNSS_,GPIO_PIN_RESET);

	errorCodeHAL = HAL_SPI_Receive(pmHandlerSPI_, txBuffer, numberOfRegisters+1, mTimeOutSPIms_);

	HAL_GPIO_WritePin(pmGPIOportNSS_, mGPIOpinNSS_,GPIO_PIN_SET);


	for(i = 0; i < numberOfRegisters ; i++)
	{
		dataRead[i] = txBuffer[i+1];
	}

	return get_error_code(errorCodeHAL);
}

// ============================ enable_accel_and_gyro ============================
//
// Habilita los acelerómetros y giróscopos de la IMU, a través de la escritura de
// una serie de registros de la IMU.

errorCodeIMU_t ICM42688::enable_accel_and_gyro(void)
{
	return write_register(ADDRESS_PWR_MGMT0, 0x0F);
}

// ============================ disable_accel_and_gyro ============================
//
// Deshabilita los acelerómetros y giróscopos de la IMU, a través de la escritura de
// una serie de registros de la IMU.

errorCodeIMU_t ICM42688::disable_accel_and_gyro(void)
{
	return write_register(ADDRESS_PWR_MGMT0, 0x00);
}

// ============================ read_raw_data ============================
//
// Lee los valores más recientes de acelerómetros y giróscopos, generados por la IMU.
// Esto se hace a través de SPI, leyendo una serie de registros de la IMU. Los
// valores devueltos por este método son números de 16 bits, sin unidades.
//
// pAccelX: dirección de memoria donde guardar el resultado del acelerómetro X.
// pAccelY: dirección de memoria donde guardar el resultado del acelerómetro Y.
// pAccelZ: dirección de memoria donde guardar el resultado del acelerómetro Z.
// pGyroX: dirección de memoria donde guardar el resultado del giróscopo X.
// pGyroY: dirección de memoria donde guardar el resultado del giróscopo Y.
// pGyroZ: dirección de memoria donde guardar el resultado del giróscopo Z.
errorCodeIMU_t ICM42688::read_raw_data(int16_t *pAccelX, int16_t *pAccelY, int16_t *pAccelZ, int16_t *pGyroX, int16_t *pGyroY, int16_t *pGyroZ)
{
	uint8_t rxBuffer[14];
	int16_t rawData[7];
	errorCodeIMU_t errorCode;
	uint8_t i;

	if( pAccelX == NULL || pAccelY == NULL || pAccelZ == NULL || pGyroX == NULL || pGyroY == NULL || pGyroZ == NULL )
	{
		return IMU_ERROR_NULL_POINTER;
	}

	if( (errorCode = read_registers(ADDRESS_ACCEL_DATA_X1, 14, rxBuffer)) != IMU_OK )
	{
		return errorCode;
	}

	for(i = 0 ; i < 7 ; i++)
	{
		rawData[i] = ((int16_t)(rxBuffer[2*i] << 8)) | rxBuffer[2*i+1];
	}

	(*pAccelX) = rawData[0];
	(*pAccelY) = rawData[1];
	(*pAccelZ) = rawData[2];

	(*pGyroX) = rawData[3];
	(*pGyroY) = rawData[4];
	(*pGyroZ) = rawData[5];

	return IMU_OK;
}

// ============================ read_data ============================
//
// Lee los valores más recientes de acelerómetros y giróscopos, generados por la IMU.
// Esto se hace a través de SPI, leyendo una serie de registros de la IMU. Los
// valores devueltos por este método se encuentran escalados y tienen las unidades
// físicas correspondientes.
//
// pAccelX: dirección de memoria donde guardar el resultado del acelerómetro X.
// pAccelY: dirección de memoria donde guardar el resultado del acelerómetro Y.
// pAccelZ: dirección de memoria donde guardar el resultado del acelerómetro Z.
// pGyroX: dirección de memoria donde guardar el resultado del giróscopo X.
// pGyroY: dirección de memoria donde guardar el resultado del giróscopo Y.
// pGyroZ: dirección de memoria donde guardar el resultado del giróscopo Z.
errorCodeIMU_t ICM42688::read_data(float *pAccelX, float *pAccelY, float *pAccelZ, float *pGyroX, float *pGyroY, float *pGyroZ)
{
	errorCodeIMU_t errorCode;
	uint8_t rxBuffer[14];
	int16_t rawData[7];
	uint8_t i;

	if( pAccelX == NULL || pAccelY == NULL || pAccelZ == NULL || pGyroX == NULL || pGyroY == NULL || pGyroZ == NULL )
	{
		return IMU_ERROR_NULL_POINTER;
	}

	if( (errorCode = read_registers(ADDRESS_ACCEL_DATA_X1, 14, rxBuffer)) != IMU_OK )
	{
		return errorCode;
	}

	for(i = 0 ; i < 7 ; i++)
	{
		rawData[i] = ((int16_t)(rxBuffer[2*i] << 8)) | rxBuffer[2*i+1];
	}

	(*pAccelX) = rawData[0] * mAccelScale_;
	(*pAccelY) = rawData[1] * mAccelScale_;
	(*pAccelZ) = rawData[2] * mAccelScale_;

	(*pGyroX) = rawData[3] * mGyroScale_;
	(*pGyroY) = rawData[4] * mGyroScale_;
	(*pGyroZ) = rawData[5] * mGyroScale_;

	return IMU_OK;
}

// ============================ set_accel_full_scale_range ============================
//
// Setea el rango de la IMU, para los acelerómetros. A través de una comunicación por SPI
// con la IMU, setea el valor del full scale range. Además, dependiendo del valor
// seteado, se guarda en el objeto, un factor de escala utilizado para convertir
// las lecturas de acelerómetros a unidades físicas.
//
// fullScale: enumerativo que represetna el valor del rango del acelerómetro.
errorCodeIMU_t ICM42688::set_accel_full_scale_range(fullScaleAccel_t fullScale)
{
	uint8_t auxReg;
	errorCodeIMU_t errorCode;

	if( (errorCode = read_registers(ADDRESS_ACCEL_CONFIG0, 1, &auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	auxReg = (fullScale << 5) | (auxReg & 0x1F);

	if( (errorCode = write_register(ADDRESS_ACCEL_CONFIG0, auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	mAccelScale_ = static_cast<float>(1 << (4 - fullScale)) / 32768.0f;

	return IMU_OK;
}

// ============================ set_gyro_full_scale_range ============================
//
// Setea el rango de la IMU, para los giróscopos. A través de una comunicación por SPI
// con la IMU, setea el valor del full scale range. Además, dependiendo del valor
// seteado, se guarda en el objeto, un factor de escala utilizado para convertir
// las lecturas de giróscopos a unidades físicas.
//
// fullScale: enumerativo que represetna el valor del rango del acelerómetro.
errorCodeIMU_t ICM42688::set_gyro_full_scale_range(fullScaleGyro_t fullScale)
{
	uint8_t auxReg;
	errorCodeIMU_t errorCode;

	if( (errorCode = read_registers(ADDRESS_GYRO_CONFIG0, 1, &auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	auxReg = (fullScale << 5) | (auxReg & 0x1F);

	if( (errorCode = write_register(ADDRESS_GYRO_CONFIG0, auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	mGyroScale_ = (2000.0f / static_cast<float>(1 << fullScale)) / 32768.0f;

	return IMU_OK;
}

// ============================ set_accel_output_data_rate ============================
//
// Setea el output data rate de los acelerómetros de la IMU.
//
// odr: enumerativo que represetna el valor del output data rate a setear.
errorCodeIMU_t ICM42688::set_accel_output_data_rate(odrAccel_t odr)
{
	uint8_t auxReg;
	errorCodeIMU_t errorCode;

	if( (errorCode = read_registers(ADDRESS_ACCEL_CONFIG0, 1, &auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	auxReg = odr | (auxReg & 0xF0);

	if( (errorCode = write_register(ADDRESS_ACCEL_CONFIG0, auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	return IMU_OK;
}

// ============================ set_gyro_output_data_rate ============================
//
// Setea el output data rate de los giróscopos de la IMU.
//
// odr: enumerativo que represetna el valor del output data rate a setear.
errorCodeIMU_t ICM42688::set_gyro_output_data_rate(odrGyro_t odr)
{
	uint8_t auxReg;
	errorCodeIMU_t errorCode;

	if( (errorCode = read_registers(ADDRESS_GYRO_CONFIG0, 1, &auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	auxReg = odr | (auxReg & 0xF0);

	if( (errorCode = write_register(ADDRESS_GYRO_CONFIG0, auxReg)) != IMU_OK )
	{
		return errorCode;
	}

	return IMU_OK;
}

// ============================ set_interrupt_mode ============================
//
// Activa el modo de interrupción de la IMU. La IMU cuenta con dos salidas digitales
// que pueden configurarse para togglear sus salidas cada vez que esta tiene datos nuevos.
// Si se conectan esas salidas digitales a entradas digitales del micro, pueden
// detectarse esos cambios. En esos casos, se puede saber en qué momento la IMU tiene
// datos nuevos disponibles. Este método le indica a la IMU, a través de comunicación
// SPI, que toglee INT1 cada vez que tiene un dato nuevo. La configuración es la siguiente:
//
// Pulsed Mode: la IMU genera un pulso en INT1 cada vez que tiene un dato nuevo.
// Push-Pull: la salida digital de la IMU es push-pull.
// Polarity High: la IMU genera un flanco ascendente en INT1
//
//
// pGPIOportINT: tipo de dato de la HAL, que represetna un puerto del micro. Este, junto con el parámetro
// GPIOpinINT, sirven para indicarle al driver cuál es el pin utilizado para detectar la interrupción.
//
// GPIOpinINT: pin del micro utilizado para detectar la interrupción.
errorCodeIMU_t ICM42688::set_interrupt_mode(GPIO_TypeDef *pGPIOportINT, uint16_t GPIOpinINT)
{
	uint8_t intConfig = 0x00;
	uint8_t intConfig1 = 0x00;
	uint8_t intSource0 = 0x10;
	errorCodeIMU_t errorCode;

	pmGPIOportINT_ = pGPIOportINT;
	mGPIOpinINT_ = GPIOpinINT;

	INT1_SET_MODE_PULSED(intConfig);
	INT1_SET_DRIVE_CIRCUIT_PUSH_PULL(intConfig);
	INT1_SET_POLARITY_HIGH(intConfig);

	if( (errorCode = write_register(ADDRESS_INT_CONFIG, intConfig)) != IMU_OK )
	{
		return errorCode;
	}

	INT_SET_PULSE_DURATION_100US(intConfig1);
	INT_SET_DEASSERT_ENABLE(intConfig1);
	INT_ENABLE(intConfig1);

	if( (errorCode = write_register(ADDRESS_INT_CONFIG1, intConfig1)) != IMU_OK )
	{
		return errorCode;
	}

	INT1_SET_SOURCE_DATA_READY(intSource0);
	if( (errorCode = write_register(ADDRESS_INT_SOURCE0, intSource0)) != IMU_OK )
	{
		return errorCode;
	}

	return IMU_OK;
}

// ============================ who_am_i ============================
//
// Este método le pide al dispositivo IMU, un valor que sirve para identificarla,
// el cual tiene definido en uno de sus registros, llamado "WHO_AM_I".
//
// pResponse: dirección de memoria donde se guarda la respuesta de la IMU.
errorCodeIMU_t ICM42688::who_am_i(uint8_t *pResponse)
{
	return read_registers(ADDRESS_WHO_AM_I,1,pResponse);
}
