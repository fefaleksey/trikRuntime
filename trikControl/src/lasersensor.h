#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QScopedPointer>
#include <QtCore/QVector>

#include "lasersensorinterface.h"
#include "deviceState.h"
#include "i2cDeviceInterface.h"
#include "lasersensorworker.h"

namespace trikKernel {
class Configurer;
}

namespace trikHal {
class HardwareAbstractionInterface;
}

namespace trikControl {

class LaserSensorWorker;

/// Implementation of virtual laser sensor for real robot.
class LaserSensor : public LaserSensorInterface
{
	Q_OBJECT

public:
	/// Constructor.
	LaserSensor(I2cDeviceInterface *laserSensorI2cDevice);

	Status status() const override;

public slots:
	void init() override;

	int read() override;

private:
	/// State of the device, shared with worker.
	DeviceState mState;
	I2cDeviceInterface *mI2c;

	/// Worker object that handles sensor in separate thread.
	QScopedPointer<LaserSensorWorker> mLaserSensorWorker;

	/// Worker thread.
	QThread mWorkerThread;
};

}
