#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVector>

#include "deviceState.h"
#include "i2cDeviceInterface.h"

namespace trikControl {

/// Worker object that processes laser sensor output and updates stored reading. Meant to be executed in separate
/// thread.
class LaserSensorWorker : public QObject//, public DeviceInterface
{
	Q_OBJECT

public:
	/// Constructor.
	LaserSensorWorker(I2cDeviceInterface* laserSensorI2cDevice, DeviceState &state);

	void init();

public slots:
	int read();
	int getDistance();
//	void readNewDistance();

private slots:
	void down();
	void up();
	int rdWord(int reg);
	void pollingDelay();
	void dataInit();
	void deviceReadStrobe();
	int isAperture(int spadIndex);
	void setReferenceSpads();
	void staticInit();

private:
	I2cDeviceInterface *mI2c;
	int mReading;
	int mStopVariable;
	DeviceState &mState;
};

}
