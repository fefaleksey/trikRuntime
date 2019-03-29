/* Copyright 2018 Aleksey Fefelov and CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "i2cDevice.h"

using namespace trikControl;

I2cDevice::I2cDevice(const trikKernel::Configurer &configurer, trikHal::MspI2cInterface &i2c, int bus, int address)
	: mState("I2cDevice")
	, mCommunicator(configurer, i2c, bus, address)
{
	mState.ready();
}

I2cDevice::Status I2cDevice::status() const
{
	return combine(mCommunicator, mState.status());
}

void I2cDevice::sendByte(int reg, int value)
{
	if (status() == DeviceInterface::Status::ready) {
		QByteArray command(3, '\0');
		command[0] = reg & 0xFF;
		command[1] = static_cast<char>(0x00);
		command[2] = value & 0xFF;

		mCommunicator.send(command);
	}
}

void I2cDevice::sendWord(int reg, int value)
{
	if (status() == DeviceInterface::Status::ready) {
		QByteArray command(4, '\0');
		command[0] = reg & 0xFF;
		command[1] = static_cast<char>(0x00);
		command[2] = value & 0xFF;
		command[3] = value & 0xFF00;

		mCommunicator.send(command);
	}
}

void I2cDevice::sendBlock(unsigned char reg, unsigned char *values, unsigned char length)
{
	if (status() == DeviceInterface::Status::ready) {
		mCommunicator.sendBlockData(reg, length, values);
	}
}

int I2cDevice::read(int reg)
{
	QByteArray command(2, '\0');
	command[0] = reg & 0xFF;
	command[1] = static_cast<char>(0x00);
	return mCommunicator.read(command) & 0xFF;
}
