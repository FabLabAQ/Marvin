/******************************************************************************
 * SequencerGUI                                                               *
 * Copyright (C) 2015                                                         *
 * Tomassino Ferrauto <t_ferrauto@yahoo.it>                                   *
 * Luca Anastasio <anastasio.lu@gmail.com>                                    *
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
#include "Adafruit_PWMServoDriver.h"

/**
 * \brief The class controlling the servos
 *
 * This class stores sequence points and moves servos. Sequence points are in
 * a ring buffer. To add a sequence point use the pointer returned by the
 * function pointToFill(), then call pointFilled() when the sequence point is
 * valid. To play the sequence call step(). This class internally uses millis()
 * to compute the position of servos. Never move servos controlled by this class
 * externally: here we need to keep the current position to compute the velocity
 * at which servos must move to a new postition. The current position of servos
 * is stored in the buffer but it never cleared. After instantiating this class,
 * always call begin before starting to use the object. We internally use an
 * Adafruit_PWMServoDriver object to control the servos
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
	 * \param servoMin the vector with the minimum valus of the PWM of
	 *                 servos
	 * \param servoMax the vector with the maximum valus of the PWM of
	 *                 servos
	 */
	SequencePlayer(const unsigned int servoMin[SequencePoint::dim], const unsigned int servoMax[SequencePoint::dim]);

	/**
	 * \brief Initializes servos
	 *
	 * We can't initialize servos in the constructor because it is called
	 * too early in the initialization phase. So we must call this function
	 * inside setup()
	 * \param curPos the current position of servos
	 */
	void begin(const SequencePoint& curPos);

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
		return (m_curPoint == m_pointToFill);
	}

	/**
	 * \brief Returns true if the buffer is full
	 *
	 * \return true if the buffer is full
	 */
	bool bufferFull() const
	{
		return (m_prevPoint == m_pointToFill);
	}

private:
	/**
	 * \brief Computes the position the servo it should have at the given
	 *        time
	 *
	 * \param servo the index of the servo to move
	 * \param curTime the current step time. This MUST be lower than the
	 *                timeToTarget of the current step
	 * \return the position the servo should have
	 */
	unsigned char currentServoPos(int servo, unsigned long curTime);

	/**
	 * \brief Moves one servo to specified position
	 *
	 * \param servo the index of the servo to move
	 * \param pos the position to which the servo should be moved
	 */
	void moveServo(int servo, unsigned char pos);

	/**
	 * \brief The driver of motors
	 */
	Adafruit_PWMServoDriver m_pwm;

	/**
	 * \brief The buffer for sequence points
	 */
	SequencePoint m_buffer[bufferDimension];

	/**
	 * \brief The index of the current point in the buffer
	 */
	int m_curPoint;

	/**
	 * \brief The index of the previous point in the buffer
	 */
	int m_prevPoint;

	/**
	 * \brief The index of the point to fill in the buffer
	 */
	int m_pointToFill;

	/**
	 * \brief The time the sequence point started in milliseconds
	 *
	 * This is set using millis()
	 */
	unsigned long m_stepStartTime;

	/**
	 * \brief Set to true when starting a new sequence point
	 *
	 * This is needed to store the m_stepStartTime
	 */
	bool m_startingNewPoint;

	/**
	 * \brief The minimum value for servos PWM
	 */
	unsigned int m_servoMin[SequencePoint::dim];

	/**
	 * \brief The range of servos PWM
	 *
	 * Valid values for PWM will be from m_servoMin to (m_servoMin +
	 * m_servoRange)
	 */
	unsigned int m_servoRange[SequencePoint::dim];

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
