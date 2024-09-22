/*
 * attitudeEstimator.cpp
 *
 *  Created on: Mar 3, 2024
 *      Author: fede
 */

#include "attitudeEstimator.h"
#include <cmath>

#define PI 3.14159265

static attitudeEstimator_t _instance;

void attitudeEstimator_constructor(float alpha, float deltaT)
{
	_instance.mAlpha_ = alpha;
	_instance.mDeltaT_ = deltaT;

	_instance.mOutputData_.mPitch_ = 0.0;
	_instance.mOutputData_.mRoll_ = 0.0;

	_instance.mIMUdata_.accelX = 0.0;
	_instance.mIMUdata_.accelY = 0.0;
	_instance.mIMUdata_.accelZ = -1.0;
	_instance.mIMUdata_.gyroX = 0.0;
	_instance.mIMUdata_.gyroY = 0.0;
	_instance.mIMUdata_.gyroZ = 0.0;
}

void attitudeEstimator_set_imu_data(IMUData imuData)
{
	//_instance.mIMUdata_ = imuData;
	_instance.mIMUdata_.accelX = (-1.0) * imuData.accelX;
	_instance.mIMUdata_.accelY =          imuData.accelY;
	_instance.mIMUdata_.accelZ = (-1.0) * imuData.accelZ;
	_instance.mIMUdata_.gyroX  = (-1.0) * imuData.gyroX;
	_instance.mIMUdata_.gyroY  =          imuData.gyroY;
	_instance.mIMUdata_.gyroZ  = (-1.0) * imuData.gyroZ;
}


//void attitudeEstimator_update(float *accelData, float *gyroData)
void attitudeEstimator_update(void)
{
	//float pitchAccel = atan2(-1 * accelData[POS_ACCEL_X], accelData[POS_ACCEL_Z]) * 180.0 / PI;
	//float rollAccel  = atan2(accelData[POS_ACCEL_Y], accelData[POS_ACCEL_Z]) * 180.0 / PI;

	float pitchAccel = atan2(_instance.mIMUdata_.accelX, -1.0 * _instance.mIMUdata_.accelZ) * 180.0 / PI;
	float rollAccel  = atan2(-1.0 * _instance.mIMUdata_.accelY, -1.0 * _instance.mIMUdata_.accelZ) * 180.0 / PI;

	//_instance.mPitch_ = _instance.mAlpha_ * pitchAccel + (1 - _instance.mAlpha_) * (_instance.mDeltaT_ * gyroData[POS_GYRO_Y] + _instance.mPitch_);
	//_instance.mRoll_  = _instance.mAlpha_ * rollAccel  + (1 - _instance.mAlpha_) * (_instance.mDeltaT_ * gyroData[POS_GYRO_X] + _instance.mRoll_);

	_instance.mOutputData_.mPitch_ = _instance.mAlpha_ * pitchAccel + (1 - _instance.mAlpha_) * (_instance.mDeltaT_ * _instance.mIMUdata_.gyroY + _instance.mOutputData_.mPitch_);
	_instance.mOutputData_.mRoll_  = _instance.mAlpha_ * rollAccel  + (1 - _instance.mAlpha_) * (_instance.mDeltaT_ * _instance.mIMUdata_.gyroX + _instance.mOutputData_.mRoll_);
}

void attitudeEstimator_read_data(attitudeData_t *outputData)
{
	(*outputData) = _instance.mOutputData_;
}
