#include "lasersensor.h"

#include <trikKernel/configurer.h>
#include <QsLog.h>

using namespace trikControl;


LaserSensor::LaserSensor(I2cDeviceInterface* laserSensorI2cDevice)
	: mState("Line Sensor on I2C")
{

	mI2c = laserSensorI2cDevice;
	mState.ready();

	mLaserSensorWorker.reset(new LaserSensorWorker(mI2c, mState));

	mLaserSensorWorker->moveToThread(&mWorkerThread);
	connect(mLaserSensorWorker.data(), SIGNAL(stopped()), this, SLOT(onStopped()), Qt::DirectConnection);

	QLOG_INFO() << "Starting LaserSensor worker thread" << &mWorkerThread;

	mWorkerThread.start();
}


LaserSensor::Status LaserSensor::status() const
{
	return mState.status();
}

void LaserSensor::init()
{
	mLaserSensorWorker->init();
}

int LaserSensor::read()
{
	return mLaserSensorWorker->getDistance();
}
