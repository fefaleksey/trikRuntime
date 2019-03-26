#include <QsLog.h>
#include <QtCore/QThread>
#include <QTimer>

#include "lasersensorworker.h"

using namespace trikControl;

LaserSensorWorker::LaserSensorWorker(I2cDeviceInterface* laserSensorI2cDevice, DeviceState &state)
		: mState(state)
{
	mI2c = laserSensorI2cDevice;

	int freq = 100;
	QTimer *timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(read()));
	timer->setInterval(freq);
	timer->start();
}

int LaserSensorWorker::getDistance()
{
	return mReading;
}

void LaserSensorWorker::down()
{
	mI2c->sendByte(0x80, 0x01);
	mI2c->sendByte(0xFF, 0x01);
	mI2c->sendByte(0x00, 0x00);
}

void LaserSensorWorker::up()
{
	mI2c->sendByte(0x00, 0x01);
	mI2c->sendByte(0xFF, 0x00);
	mI2c->sendByte(0x80, 0x00);
}

void LaserSensorWorker::pollingDelay()
{
	int k, m;
	for(int i = 0; i < 250; i++)
	{
		m = i;
		k = m;
		//do nothing
	}
	m = k;
}

void LaserSensorWorker::dataInit()
{
	mI2c->sendByte(0x88, 0x00);
	mI2c->read(0xc0);
	down();
	mStopVariable = mI2c->read(0x91);
	up();
	mI2c->sendByte(0x01, 0xFF);
}

int LaserSensorWorker::rdWord(int reg)
{
	int res = 0;
	res |= mI2c->read(reg);

	for(int i = 1; i < 4; i++){
		res <<= 8;
		reg += 1;
		res |= mI2c->read(reg);
	}
	return res;
}

void LaserSensorWorker::deviceReadStrobe()
{
	mI2c->sendByte(0x83, 0x00);
	int strobe;
	for(int i = 0; i < 200; i++)
	{
		strobe = mI2c->read(0x83);
		if (strobe != 0x00){
			break;
		}
	}
	mI2c->sendByte(0x83, 0x01);
}

int LaserSensorWorker::isAperture(int spadIndex)
{
	int quadrant;
	int isAperture = 1;
	quadrant = spadIndex >> 6;

	if (quadrant != 2)
	{
		isAperture = 0;
	}

	return isAperture;
}

void LaserSensorWorker::setReferenceSpads()
{
	int startSelect = 0xB4;
	int currentSpadIndex = 0;
	int maxSpadCount = 44;
	mI2c->sendByte(0xFF, 0x01);
	mI2c->sendByte(0x4F, 0x00);
	mI2c->sendByte(0x4E, 0x2c);
	mI2c->sendByte(0xFF, 0x00);
	mI2c->sendByte(0xb6, startSelect);

	while (isAperture(startSelect + currentSpadIndex) == 0) {
		if (currentSpadIndex == maxSpadCount)
		{
			break;
		}
		currentSpadIndex ++;
	}
}

void LaserSensorWorker::staticInit()
{
	down();
	mI2c->sendByte(0xFF, 0x06);
	int byte = mI2c->read(0x83);
	byte |= 4;
	mI2c->sendByte(0x83, byte);
	mI2c->sendByte(0xFF, 0x07);
	mI2c->sendByte(0x81, 0x01);
	pollingDelay();
	mI2c->sendByte(0x80, 0x01);

	mI2c->sendByte(0x94, 0x6b);
	deviceReadStrobe();
	int tmpWord = mI2c->read(0x90);
	int referenceSpadCount = (tmpWord >> 8) & 0x7F;
	int referenceSpadType = (tmpWord >> 15) & 0x01;
	mI2c->sendByte(0x94, 0x24);
	deviceReadStrobe();
	mI2c->sendByte(0x94, 0x25);
	deviceReadStrobe();

	mI2c->sendByte(0x81, 0x00);
	mI2c->sendByte(0xFF, 0x06);
	byte = mI2c->read(0x83);
	byte &= 0xfb;
	mI2c->sendByte(0x83, byte);
	mI2c->sendByte(0xFF, 0x01);
	mI2c->sendByte(0x00, 0x01);

	mI2c->sendByte(0xFF, 0x00);
	mI2c->sendByte(0x80, 0x00);

	int count = referenceSpadCount;
	int apertureSpads = referenceSpadType;

	if (apertureSpads > 1 ||
			(apertureSpads == 1 && count > 32) ||
			(apertureSpads == 0 && count > 12))
	{} else {
		setReferenceSpads();
	}
}

void LaserSensorWorker::init()
{
	if (!mState.isFailed()) {
		dataInit();
		staticInit();
		down();
		mI2c->sendByte(0x91, mStopVariable);
		up();
	}
}

int LaserSensorWorker::read()
{

	if (mState.isReady()) {
		int startStopByte = 0x01;
		int byte = startStopByte;
		mI2c->sendByte(0x00, startStopByte);

		for(int i = 0; i < 200; i++){
			if (i > 0)
			{
				byte = mI2c->read(0x00);
			}
			if ((byte & startStopByte) != startStopByte)
			{
				break;
			}
		}

		int h1, h2;
		for(int i = 0; i < 200; i++){
			h1 = i;
			h2 = h1;
			h1 = h2;
		}

		int result = (mI2c->read(0x1e) << 8) + mI2c->read(0x1f);
		mReading = result;
		return result;
	} else {
		QLOG_WARN() << "Calling 'read' for sensor which is not ready";
		mReading = -2;
		return -2;
	}
	mReading = -1;
	return -1;
}
