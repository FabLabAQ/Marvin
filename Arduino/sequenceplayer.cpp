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

#include "sequenceplayer.h"
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

SequencePlayer::SequencePlayer(const unsigned int servoMin[SequencePoint::dim], const unsigned int servoMax[SequencePoint::dim])
	: m_curPoint(1)
	, m_prevPoint(0)
	, m_pointToFill(1)
	, m_stepStartTime(0)
	, m_startingNewPoint(true)
{
	// Copying the minimum PWM for servos and computing the range
	memcpy(m_servoMin, servoMin, sizeof(m_servoMin));
	for (int i = 0; i < SequencePoint::dim; ++i) {
		m_servoRange[i] = servoMax[i] - servoMin[i];
	}
}

void SequencePlayer::begin(const SequencePoint& curPos)
{
	memcpy(&(m_buffer[m_prevPoint]), &curPos, sizeof(SequencePoint));

	// Moving all servos to their position
	for (int i = 0; i < SequencePoint::dim; ++i) {
		moveServo(i, curPos.point[i]);
	}
}

SequencePoint* SequencePlayer::pointToFill()
{
	if (bufferFull()) {
		return NULL;
	}

	return &(m_buffer[m_pointToFill]);
}

void SequencePlayer::pointFilled()
{
	m_pointToFill = (m_pointToFill + 1) % bufferDimension;
}

void SequencePlayer::forceNextPoint()
{
	m_startingNewPoint = true;
	m_curPoint = (m_curPoint + 1) % bufferDimension;
	m_prevPoint = (m_prevPoint + 1) % bufferDimension;
}

bool SequencePlayer::step()
{
	if (bufferEmpty()) {
		return false;
	}

	if (m_startingNewPoint) {
		// Storing the start time
		m_stepStartTime = millis();
	}

	// Now checking how much has passed since we being move
	const unsigned long stepTime = millis() - m_stepStartTime;

	// Checking what to do. Notice that if both timeToTarget and duration are 0, we move
	// to the target position directly. If the first check, the !m_startingNewPoint condition
	// is checked to avoid skipping a point that has both timeToTarget and duration to 0 when
	// millis() changes between the two calls above
	if ((!m_startingNewPoint) && (stepTime > (m_buffer[m_curPoint].timeToTarget + m_buffer[m_curPoint].duration))) {
		// The current step has finished, moving to the next one and recursively calling self
		forceNextPoint();

		return step();
	} else {
		// Checking if we have to move (if not we simply wait)
		if ((m_startingNewPoint) || (stepTime <= m_buffer[m_curPoint].timeToTarget)) {
			// We have not reached the point yet, moving servos
			for (int i = 0; i < SequencePoint::dim; ++i) {
				moveServo(i, currentServoPos(i, stepTime));
			}
		}
	}

	// Resetting the startingNewPoint flag
	m_startingNewPoint = false;

	// If we get here, the buffer is not empty for sure (the only point in which the buffer
	// can become empty is when we recursively call self. In that case the check is at the
	// beginning of the function)
	return false;
}

void SequencePlayer::clearBuffer()
{
	// Changing m_pointToFill so that we do not have to also change m_prevPoint
	m_pointToFill = m_curPoint;

	// We also set the flag for the starting of a new point to true to store the start time
	// the first time step() is called with a point
	m_startingNewPoint = true;
}

unsigned char SequencePlayer::currentServoPos(int servo, unsigned long curTime)
{
	// Computing the new position. This is still a value between 0 and 255.
	// If timeToTarget is 0, the new position will be the one in curPoint
	unsigned char newPos;

	if (m_buffer[m_curPoint].timeToTarget == 0) {
		newPos = m_buffer[m_curPoint].point[servo];
	} else {
		const long d = m_buffer[m_curPoint].point[servo] - m_buffer[m_prevPoint].point[servo];
		const long newP = m_buffer[m_prevPoint].point[servo] + ((d * curTime) / m_buffer[m_curPoint].timeToTarget);
		newPos = (unsigned char) newP;
	}

	return newPos;
}

void SequencePlayer::moveServo(int servo, unsigned char pos)
{
	// Here we map the position in the PWM range. pos is always a value between 0 and 255
	const long mappedValue = ((long(pos) * m_servoRange[servo]) / 255) + m_servoMin[servo];

// 	MOVE WITH ADAFRUIT LIBRARY
}
