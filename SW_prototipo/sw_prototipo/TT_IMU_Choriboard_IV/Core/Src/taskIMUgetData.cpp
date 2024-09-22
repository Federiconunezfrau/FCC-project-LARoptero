/*
 * taskIMUgetData.cpp
 *
 *  Created on: Feb 12, 2024
 *      Author: fede
 */

#include "taskIMUgetData.h"
#include "attitudeEstimator.h"
#include "CNI.h"
#include "faultInjectionIMU.h"

#define LEN_IMU_CNI_DATA 24

#define N_CALIBRATION_SAMPLES 100

// FAULT INJECTION MID AIR ========================
#define SIMULATE_FAULT_IMU 0
#define INITIAL_DELAY_MID_AIR 1000

#if SIMULATE_FAULT_IMU == 1
static faultInjectorIMUbias_t _faultInjector;

#elif SIMULATE_FAULT_IMU == 2
static faultInjectorIMUrandomJump_t _faultInjector;

#elif SIMULATE_FAULT_IMU == 3
static faultInjectorIMUstuckAt_t _faultInjector;

#elif SIMULATE_FAULT_IMU == 4
static faultInjectorIMUinconsistent_t _faultInjector;

#endif

// ================================================

static void make_imu_data_for_cni(IMUData imuData, uint8_t *dataForCni);

void taskIMUgetData_constructor(taskIMUgetData_t *me, uint32_t delayTicks, uint32_t periodTicks, uint32_t wcetMicroSeconds, uint32_t bcetMicroSeconds, ICM42688::icm42688 *imu, STM32::gpio *led, uint32_t handleMsg)
{
	timeTriggeredTask_constructor(&me->super, (taskHandler_t)&taskIMUgetData_update, delayTicks, periodTicks, wcetMicroSeconds, bcetMicroSeconds);

	me->mIMU_ = imu;
	me->mLED_ = led;
	me->mHandleMsg_ = handleMsg;
#if SIMULATE_FAULT_IMU == 1
	faultInjectorIMUbias_constructor(&_faultInjector, INITIAL_DELAY_MID_AIR);

#elif SIMULATE_FAULT_IMU == 2
	faultInjectorIMUrandomJump_constructor(&_faultInjector, INITIAL_DELAY_MID_AIR);

#elif SIMULATE_FAULT_IMU == 3
	faultInjectorIMUstuckAt_constructor(&_faultInjector, INITIAL_DELAY_MID_AIR);

#elif SIMULATE_FAULT_IMU == 4
	faultInjectorIMUinconsistent_constructor(&_faultInjector, INITIAL_DELAY_MID_AIR);
#endif
}

void taskIMUgetData_destructor(taskIMUgetData_t *me)
{
	//TODO: hay que definir un criterio para desinicializar la IMU
	me->mIMU_->disable();

	timeTriggeredTask_destructor(&me->super);
}

// Ejecuta una rutina de calibración de la IMU
void taskIMUgetData_start(taskIMUgetData_t *me)
{
	IMUData sampleData;
	uint32_t i;

	me->mLED_->write(GPIO_ST::HIGH);

	me->mIMUoffsetData_.accelX = 0;
	me->mIMUoffsetData_.accelY = 0;
	me->mIMUoffsetData_.accelZ = 0;
	me->mIMUoffsetData_.gyroX  = 0;
	me->mIMUoffsetData_.gyroY  = 0;
	me->mIMUoffsetData_.gyroZ  = 0;

	HAL_Delay(1000);

	// Mido "N" muestras
	for(i = 0; i < N_CALIBRATION_SAMPLES ; i++)
	{
		me->mIMU_->save_data();
		me->mIMU_->read_data(&sampleData);

		me->mIMUoffsetData_.accelX += sampleData.accelX;
		me->mIMUoffsetData_.accelY += sampleData.accelY;
		me->mIMUoffsetData_.accelZ += sampleData.accelZ;
		me->mIMUoffsetData_.gyroX  += sampleData.gyroX;
		me->mIMUoffsetData_.gyroY  += sampleData.gyroY;
		me->mIMUoffsetData_.gyroZ  += sampleData.gyroZ;

		HAL_Delay(50);
	}

	// Calculo promedio para saber el offset
	me->mIMUoffsetData_.accelX /= N_CALIBRATION_SAMPLES;
	me->mIMUoffsetData_.accelY /= N_CALIBRATION_SAMPLES;
	me->mIMUoffsetData_.accelZ /= N_CALIBRATION_SAMPLES;
	me->mIMUoffsetData_.accelZ -= 1.0;
	me->mIMUoffsetData_.gyroX  /= N_CALIBRATION_SAMPLES;
	me->mIMUoffsetData_.gyroY  /= N_CALIBRATION_SAMPLES;
	me->mIMUoffsetData_.gyroZ  /= N_CALIBRATION_SAMPLES;

	// Simulación de fallas de la IMU en medio del vuelo
#if SIMULATE_FAULT_IMU > 0
	stateMachine_init((stateMachine_t*)&_faultInjector);
#endif

	me->mLED_->write(GPIO_ST::LOW);
}

void taskIMUgetData_update(taskIMUgetData_t *me)
{
	uint8_t dataForCni[LEN_IMU_CNI_DATA];
	IMUData imuData;
#if SIMULATE_FAULT_IMU > 0
	evTick_t tickEvent = { SIG_TICK, &imuData };
#endif

	me->mIMU_->save_data();
	me->mIMU_->read_data(&imuData);

	// Se corrige con los offsets
	imuData.accelX -= me->mIMUoffsetData_.accelX;
	imuData.accelY -= me->mIMUoffsetData_.accelY;
	imuData.accelZ -= me->mIMUoffsetData_.accelZ;
	imuData.gyroX  -= me->mIMUoffsetData_.gyroX;
	imuData.gyroY  -= me->mIMUoffsetData_.gyroY;
	imuData.gyroZ  -= me->mIMUoffsetData_.gyroZ;

// ================================================
// RANDOM BIAS IN MID AIR =========================
#if SIMULATE_FAULT_IMU > 0
	stateMachine_dispatch((stateMachine_t*)&_faultInjector, (event_t*)&tickEvent);
#endif
// ================================================

	// Se le pasan las mediciones del sensor al attitude estimator
	attitudeEstimator_set_imu_data(imuData);

	// Convertir los datos de la IMU en bytes
	make_imu_data_for_cni(imuData, dataForCni);

	// Actualizarlos en la CNI
	CNI_update_msg_content(me->mHandleMsg_, dataForCni, LEN_IMU_CNI_DATA);
}

static void make_imu_data_for_cni(IMUData imuData, uint8_t *dataForCni)
{
	memcpy(dataForCni     , &imuData.accelX, 4);
	memcpy(&dataForCni[4] , &imuData.accelY, 4);
	memcpy(&dataForCni[8] , &imuData.accelZ, 4);
	memcpy(&dataForCni[12], &imuData.gyroX , 4);
	memcpy(&dataForCni[16], &imuData.gyroY , 4);
	memcpy(&dataForCni[20], &imuData.gyroZ , 4);
}
