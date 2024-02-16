/*
 * TTsystem_mode_normal.cpp
 *
 *  Created on: Jan 31, 2024
 *      Author: fede
 */

#include "TTscheduler.h"
#include "TTsystem_mode_normal.h"
#include "TTtask_heartbeat.h"

#include "port.hpp"
#include "icm42688.hpp"

#include "tim.h"

void system_run_mode_normal(void)
{
	//  // Inicialización de la IMU ==========================
	//
	//  /* Constructor para inicializar el GPIO usado como CS para la IMU.
	//   * Los argumentos pasados al método son definidos por STMcubeMX. Están
	//   * definidos en main.h.
	//   *
	//   * Arg 1: Puerto al que corresponde el pin, en este caso el port.
	//   * Arg 2: Pin de ese puerto. */
	//  gpio imuCS(IMU_CS_GPIO_Port, IMU_CS_Pin);
	//
	//  /* Constructor para el handler que se encarga de manejar el periférico SPI.
	//   *
	//   * Arg 1: handler de spi definido por la HAL de STM32.
	//   * Arg 2: handler que administra el GPIO usado como CS.
	//   * Arg 3: modo de inicialización. En este caso, en modo bloqueante. */
	//  phandler_spi<> imuSPI(&hspi2, &imuCS, COMM_MODE::BLCK);
	//
	//  /* Estructura definida en el namespace del dispositivo ICM42688, que tiene
	//   * configuraciones del uso de la FIFO. En este caso se deshabilita. */
	//  ICM42688::config_FIFO icmFifoCnf =
	//  {
	//			.watermark	= 1,
	//			.mode	= ICM42688::FIFO_MODE::BYPASS,
	//  };
	//
	//  /* Estructura definida en el namespace del dispositivo ICM42688, que tiene
	//   * configuraciones del uso de la FIFO. En este caso se deshabilita. */
	//  ICM42688::config_FIFO icmFifoCnf =
	//  {
	//			.watermark	= 1,
	//			.mode	= ICM42688::FIFO_MODE::BYPASS,
	//  };
	//
	//  /* Estructura definida en el namespace del dispositivo ICM42688, que tiene
	//   * configuraciones de la interrupción generada por la IMU.  */
	//  ICM42688::config_INT icmIntCnf =
	//  {
	//			.int1Source = ICM42688::INT_SOURCE::DISABLED,
	//			.int1Mode = ICM42688::INT_MODE::DISABLED,
	//			//.int1Source = ICM42688::INT_SOURCE::DATA_RDY,
	//			//.int1Mode = ICM42688::INT_MODE::PULSED_PP_HIGH,
	//			.int2Source = ICM42688::INT_SOURCE::DISABLED,
	//			.int2Mode = ICM42688::INT_MODE::DISABLED,
	//  };
	//
	//  /* Esto es una estructura perteneciente al namespace de la ICM42688.
	//   * La estructura sirve para configurar:
	//   * Full Scale Range del gyro,
	//   * Output Data Rate del gyro,
	//   * Full Scale Range del accel.
	//   * Output Data Rate del accel,
	//   *
	//   * El anteúltimo parámetro son configuraciones que administran
	//   * el uso de la FIFO interna de la ICM42688.
	//   * El último parámetro sirve para configurar el modo de interrupción
	//   * del dispositivo ICM42688 */
	//  ICM42688::Config icmCnf =
	//  {
	//		.gyroFSR 					= ICM42688::GYRO_CNF::FSR::FSR_250,
	//		.gyroODR 					= ICM42688::GYRO_CNF::ODR::ODR_1000,
	//		.accelFSR 					= ICM42688::ACCEL_CNF::FSR::FSR_4,
	//		.accelODR 					= ICM42688::ACCEL_CNF::ODR::ODR_1000,
	//		.fifoCnf					= icmFifoCnf,
	//		.intCnf						= icmIntCnf,
	//  };
	//
	//  /* Constructor para la IMU.
	//   *
	//   * Arg 1: estructura que contiene información acerca de cómo
	//   * debe configurarse el dispositivo ICM42688.
	//   * Arg 2: handler SPI para comunicación con la IMU.
	//   * Arg 3: handler del GPIO usado como CS. El mismo objeto
	//   * se encarga luego de manejar ese GPIO.
	//   * Arg 4: ídem pero para el pin de interrupción. */
	//  ICM42688::icm42688 imu(icmCnf, &imuSPI, &imuCS);

	// ================= Se inicializan las tareas del sistema =================
	// [TODO]: inicializar tarea de watchdog

	// Tarea heartbeat: LED que parpadea cada un segundo
	gpio LEDheartbeat(LD2_GPIO_Port, LD2_Pin);
	task_heartbeat taskHeartbeat(DELAY_TASK_HEARTBEAT_TICKS, PERIOD_TASK_HEARTBEAT_TICKS, WCET_TASK_HEARTBEAT_US, BCET_TASK_HEARTBEAT_US, &LEDheartbeat);
	taskHeartbeat.init();

	// ================= Se inicializa el scheduler =================
	scheduler systemScheduler(&htim3);

	// Se agregan todas las tareas al scheduler
	systemScheduler.add_task_cooperative(&taskHeartbeat);

	// Se ejecuta el scheduler
	systemScheduler.start();

	// La ejecución se bloquea en un while(1)
	while(1)
	{
		systemScheduler.dispatch();
	}
}
