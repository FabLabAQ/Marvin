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

void setup()
{
	serialComm.begin(baudRate);

	serialComm.setNextSequencePointToFill(&p);
}

void loop()
{
	char buf[256];

	if (serialComm.commandReceived()) {
		if (serialComm.isSequencePoint()) {
			sprintf(buf, "Got POINT: %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i", p.duration, p.timeToTarget, p.point[0], p.point[1], p.point[2], p.point[3], p.point[4], p.point[5], p.point[6], p.point[7], p.point[8], p.point[9], p.point[10], p.point[11], p.point[12], p.point[13], p.point[14], p.point[15]);
		} else if (serialComm.isStartStream()) {
			sprintf(buf, "Got STREAM (%i)", serialComm.pointDimension());
		} else if (serialComm.isStartImmediate()) {
			sprintf(buf, "Got IMMEDIATE (%i)", serialComm.pointDimension());
		} else if (serialComm.isStop()) {
			strcpy(buf, "Got STOP");
		} else {
			strcpy(buf, "Unknown command");
		}

		serialComm.sendDebugPacket(buf);
	}

	delay(50);
}
