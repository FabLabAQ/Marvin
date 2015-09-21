/******************************************************************************
 * SequencerGUI                                                               *
 * Copyright (C) 2015                                                         *
 * Tomassino Ferrauto <t_ferrauto@yahoo.it>                                   *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 ******************************************************************************/

#include "serialcommunication.h"
#include <stdlib.h>

// The baud rate to use for communication with computer
const long baudRate = 115200;
// The object that handles communication
SerialCommunication serialComm;

// JUST FOR TESTING
SequencePoint p;
unsigned long lastTime = 0;

void setup()
{
	serialComm.begin(baudRate);

	serialComm.setNextSequencePointToFill(&p);
}

void loop()
{
	char buf[256];

	// Adaptive sleep, so that the loop() function lasts approximately 1000 microseconds
	const unsigned long curTime = micros();
	const unsigned long timeSpent = curTime - lastTime;
	const int sleepTime = 998 - timeSpent; // 998 and not 1000 to compensate for all these instructions
	if (sleepTime > 0) {
		delayMicroseconds(sleepTime);
	}
	lastTime = micros();

	if (serialComm.commandReceived()) {
		bool sendBufNotFull = false;
		if (serialComm.isSequencePoint()) {
// 			sprintf(buf, "%c (dt: %lu) Got POINT: %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u", serialComm.receivedCommand(), timeSpent, p.duration, p.timeToTarget, p.point[0], p.point[1], p.point[2], p.point[3], p.point[4], p.point[5], p.point[6], p.point[7], p.point[8], p.point[9], p.point[10], p.point[11], p.point[12], p.point[13], p.point[14], p.point[15]);
			sprintf(buf, "%c (dt: %lu) Got POINT", serialComm.receivedCommand(), timeSpent);

			sendBufNotFull = true;
		} else if (serialComm.isStartStream()) {
			sprintf(buf, "%c (dt: %lu) Got STREAM (%i)", serialComm.receivedCommand(), timeSpent, serialComm.pointDimension());
		} else if (serialComm.isStartImmediate()) {
			sprintf(buf, "%c (dt: %lu) Got IMMEDIATE (%i)", serialComm.receivedCommand(), timeSpent, serialComm.pointDimension());
		} else if (serialComm.isStop()) {
			sprintf(buf, "%c (dt: %lu) Got STOP", serialComm.receivedCommand(), timeSpent);
		} else {
			sprintf(buf, "%c (dt: %lu) Unknown command", serialComm.receivedCommand(), timeSpent);
		}

		serialComm.sendDebugPacket(buf);
		if (sendBufNotFull) {
			serialComm.sendBufferNotFull();
		}
	}
}
