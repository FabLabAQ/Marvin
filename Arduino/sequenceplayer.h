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

#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H

#include "sequencepoint.h"

/**
 * \brief The class controlling the servos
 *
 * This class stores sequence points and moves servos. Sequence points are in
 * a ring buffer. To add a sequence point use the pointer returned by the
 * function pointToFill(), then call pointFilled() when the sequence point is
 * valid. To play the sequence call step(). It is important that you call the
 * step() function at intervals of stepLength microseconds (see constructor)
 */
class SequencePlayer
{
public:
	/**
	 * \brief How many sequence points we can buffer
	 */
	static const int bufferDimension = 4;

public:
	/**
	 * \brief Constructor
	 *
	 * \param stepLength how long does a step last in microseconds
	 */
	explicit SequencePlayer(unsigned long stepLength);

	/**
	 * \brief Initializes servos
	 *
	 * We can't initialize servos in the constructor because it is called
	 * too early in the initialization phase. So we must call this function
	 * inside setup()
	 */
	void begin();

	/**
	 * \brief Returns a pointer to the next point in the buffer to fill
	 *
	 * This returns NULL if the buffer is full. This function keeps
	 * returning the same point until the pointFilled() function is called
	 * \return a pointer to the next point in the buffer to fill or NULL if
	 *         the buffer is full
	 */
	SequencePoint* pointToFill();

	/**
	 * \brief Sets the point previously returned by pointToFill as filled
	 *
	 * Call this after you have finished modifying the point returned by the
	 * pointToFill() function
	 */
	void pointFilled();

	/**
	 * \brief Forces a move to the next point
	 *
	 * The call to the step() function following a call to this function
	 * will move to the next point in the sequence regardless of whether the
	 * previous one has been reached or not
	 */
	void forceNextPoint();

	/**
	 * \brief Performs a step
	 *
	 * This function can either move servos, wait or move to the next
	 * sequence point. It only returns false if it cannot do anything
	 * because the buffer is empty
	 * \return false if the buffer is empty, true otherwise
	 */
	bool step();

	/**
	 * \brief Clears the buffer
	 *
	 * After a call to this function, bufferEmpty() returns true
	 */
	void clearBuffer();

	/**
	 * \brief Returns true if the buffer is empty
	 *
	 * \return true if the buffer is empty
	 */
	bool bufferEmpty() const
	{
		return m_bufferEmpty;
	}

	/**
	 * \brief Returns true if the buffer is full
	 *
	 * \return true if the buffer is full
	 */
	bool bufferFull() const
	{
		return (m_curPoint == m_pointToFill) && (!m_bufferEmpty);
	}

private:
	/**
	 * \brief How long does a step last in microseconds
	 */
	const unsigned long m_stepLength;

	/**
	 * \brief The buffer for sequence points
	 */
	SequencePoint m_buffer[bufferDimension];

	/**
	 * \brief The index of the current point in the buffer
	 */
	int m_curPoint;

	/**
	 * \brief The index of the point to fill in the buffer
	 */
	int m_pointToFill;

	/**
	 * \brief The time elapsed since the sequence point started in
	 *        microseconds
	 */
	unsigned long m_stepTime;

	/**
	 * \brief True if the buffer is empty
	 */
	bool m_bufferEmpty;

	/**
	 * \brief Copy constructor is disabled
	 */
	SequencePlayer(const SequencePlayer&);

	/**
	 * \brief Copy operator is disabled
	 */
	SequencePlayer& operator=(const SequencePlayer&);
};

#endif
