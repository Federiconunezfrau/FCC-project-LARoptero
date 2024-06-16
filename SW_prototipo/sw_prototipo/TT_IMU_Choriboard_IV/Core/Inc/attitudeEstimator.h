/*
 * attitudeEstimator.h
 *
 *  Created on: Mar 3, 2024
 *      Author: fede
 */

#ifndef INC_ATTITUDEESTIMATOR_H_
#define INC_ATTITUDEESTIMATOR_H_

#include "iIMU.hpp"

#define POS_ACCEL_X 0
#define POS_ACCEL_Y 1
#define POS_ACCEL_Z 2

#define POS_GYRO_X  0
#define POS_GYRO_Y  1
#define POS_GYRO_Z  2

typedef struct
{
	float mPitch_;
	float mRoll_;
} attitudeData_t;

typedef struct
{
	float mAlpha_;
	float mDeltaT_;
	attitudeData_t mOutputData_;
	IMUData mIMUdata_;
} attitudeEstimator_t;

typedef enum
{
	ATTITUDE_ESTIMATOR_OK = 0
} attitudeEstimator_status_t;

void attitudeEstimator_constructor  ( float, float );
void attitudeEstimator_set_imu_data ( IMUData );
void attitudeEstimator_update       ( void );
void attitudeEstimator_read_data    ( attitudeData_t* );

#endif /* INC_ATTITUDEESTIMATOR_H_ */