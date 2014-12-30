# Copyright 2014 CuberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(../global.pri)

HEADERS += \
	$$PWD/include/trikTelemetry/trikTelemetry.h \
	$$PWD/src/connection.h \

SOURCES += \
	$$PWD/src/trikTelemetry.cpp \
	$$PWD/src/connection.cpp \

TEMPLATE = lib

QT += network

DEFINES += TRIKTELEMETRY_LIBRARY

uses(trikControl trikKernel qslog)

INCLUDEPATH += \
	../trikControl/include/ \
	../trikKernel/include/ \
	../qslog/ \