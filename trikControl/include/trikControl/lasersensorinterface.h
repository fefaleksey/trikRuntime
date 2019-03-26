#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>

#include "deviceInterface.h"

#include "declSpec.h"

namespace trikControl {

/// Uses virtual line sensor to detect x coordinate of a center of an object that was in camera's field of view
/// when "detect" method was called. Used mainly to follow the line.
class TRIKCONTROL_EXPORT LaserSensorInterface : public QObject, public DeviceInterface
{
	Q_OBJECT

public slots:
	/// Initializes a sensor.
	virtual void init() = 0;

	/// Returns distanse to object.
	virtual int read() = 0;
};

}
