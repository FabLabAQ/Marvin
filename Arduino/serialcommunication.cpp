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
#include <string.h>
#include <math.h>
#include <Arduino.h>

SerialCommunication::SerialCommunication()
	: m_pointToFill(NULL)
	, m_receivedCommand(0)
	, m_receivedPacketBytes(0)
	, m_receivedPointDim(0)
{
}

void SerialCommunication::begin(long baudRate)
{
	Serial.begin(baudRate);
}

bool SerialCommunication::commandReceived()
{
	bool retVal = false;
	while (Serial.available() > 0) {
		// Reading one byte
		const int v = Serial.read();

		// Checking what to do
		if (previousCommandComplete()) {
			// New package. First of all resetting the number of bytes for the package
			m_receivedPacketBytes = 0;

			// Setting the received command to the byte we just read and checking if the
			// command if finished here (the only command that ends in one byte is 'H')
			m_receivedCommand = (char) v;
			if (m_receivedCommand == 'H') {
				retVal = true;
				break;
			}
		} else if ((m_receivedCommand == 'S') || (m_receivedCommand == 'I')) {
			// The byte we received is the point dimension, storing and returning true
			m_receivedPointDim = (unsigned char) v;
			retVal = true;
			break;
		} else if (m_receivedCommand == 'P') {
			++m_receivedPacketBytes;

			if (m_pointToFill != NULL) {
				switch (m_receivedPacketBytes) {
					case 1:
						m_pointToFill->duration = ((unsigned char) v) << 8;
						break;
					case 2:
						m_pointToFill->duration += (unsigned char) v;
						break;
					case 3:
						m_pointToFill->timeToTarget = ((unsigned char) v) << 8;
						break;
					case 4:
						m_pointToFill->timeToTarget += (unsigned char) v;
						break;
					default:
						m_pointToFill->point[m_receivedPacketBytes - 5] = (unsigned char) v;
						break;
				}
			}

			if (m_receivedPacketBytes == (SequencePoint::dim + 4)) {
				retVal = true;
				break;
			}
		} else {
			// If we get here the previous packet was unknown. Here we set m_receivedCommand
			// to what we received and do another cycle
			m_receivedCommand = (char) v;
			m_receivedPacketBytes = 0;
		}
	}

	return retVal;
}

void SerialCommunication::setNextSequencePointToFill(SequencePoint* p)
{
	m_pointToFill = p;
}

void SerialCommunication::sendBufferNotFull()
{
	Serial.write('N');
}

void SerialCommunication::sendBufferFull()
{
	Serial.write('F');
}

void SerialCommunication::sendDebugPacket(const char* msg)
{
	const unsigned int msgLen = min(strlen(msg), 255);

	Serial.write('D');
	Serial.write(msgLen);
	for (unsigned int i = 0; i < msgLen; ++i) {
		Serial.write(msg[i]);
	}
}

void SerialCommunication::sendBatteryCharge(unsigned char v)
{
	Serial.write('B');
	Serial.write(v);
}

bool SerialCommunication::previousCommandComplete() const
{
	return (m_receivedCommand == 0) ||
	       (m_receivedCommand == 'H') ||
	       ((m_receivedPacketBytes == 1) && ((m_receivedCommand == 'S') || (m_receivedCommand == 'I'))) ||
	       ((m_receivedPacketBytes == (SequencePoint::dim + 4)) && (m_receivedCommand == 'P'));
}
