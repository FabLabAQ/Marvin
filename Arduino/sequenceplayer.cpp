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

SequencePlayer::SequencePlayer(unsigned long stepLength)
	: m_stepLength(stepLength)
	, m_curPoint(0)
	, m_pointToFill(0)
	, m_stepTime(0)
	, m_bufferEmpty(true)
{
}

void SequencePlayer::begin()
{
// 	dfgdsfgdsfg
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
	// Incrementing counter and resetting the flag for the empty buffer
	m_pointToFill = (m_pointToFill + 1) % bufferDimension;
	m_bufferEmpty = false;
}

void SequencePlayer::forceNextPoint()
{
	// Here we reset timer and move the pointer forward (checking if the buffer is empty),
	m_stepTime = 0;
	m_curPoint = (m_curPoint + 1) % bufferDimension;
	m_bufferEmpty = (m_curPoint == m_pointToFill);
}

bool SequencePlayer::step()
{
	if (m_bufferEmpty) {
		return false;
	}

	// Computing the current step time in milliseconds
	const unsigned int millisStepTime = m_stepTime / 1000;

	// Checking what to do. Notice that if both timeToTarget and duration are 0, we move
	// to the target position directly
	if (millisStepTime > (m_buffer[m_curPoint].timeToTarget + m_buffer[m_curPoint].duration)) {
		// The current step has finished, moving to the next one and recursively calling self
		forceNextPoint();

		return step();
	} else {
		// Checking if we have to move (if not we simply wait)
		if (millisStepTime <= m_buffer[m_curPoint].timeToTarget) {
			// We have not reached the point yet, moving the servos
// 			compute the new position for each servo (handle the case timeToTarget is 0)
//
// 			move to reach the point
		}

		// Incrementing the step time
		m_stepTime += m_stepLength;
	}

	// If we get here, the buffer is not empty for sure (the only point in which the buffer
	// can become empty is when we recursively call self. In that case the check is at the
	// beginning of the function)
	return false;
}

void SequencePlayer::clearBuffer()
{
	m_curPoint = m_pointToFill;
	m_bufferEmpty = true;

	// We also set the step time to 0 to avoid problems with the first point that will be added
	m_stepTime = 0;
}
