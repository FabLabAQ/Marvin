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

#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include "sequencepoint.h"

/**
 * \brief The class handling the serial communication with the PC
 *
 * This class reads and parses commands received through the serial line. It
 * also creates and sends packets back to the PC. To read from the serial line,
 * call the commandReceived() function: if it returns true, you can check what
 * has arrived using the is*() functions(); if it returns false, you must call
 * it again until it returns true to be able to rely on the value of the is*()
 * functions. Sequence points are written directly inside a SequencePoint object
 * that is provided using the setNextSequencePointToFill() function.
 *
 * NOTE: we read the point dimension from start packages, but we always expect
 *       points to have a dimension equal to SequencePoint::dim. Check
 *       externally that this is true when a start package is received
 */
class SerialCommunication
{
public:
	/**
	 * \brief Constructor
	 */
	SerialCommunication();

	/**
	 * \brief Initializes serial communication
	 *
	 * We can't call Serial.begin() in the constructor because it is called
	 * too early in the initialization phase. So we must call this function
	 * inside setup()
	 * \param baudRate the baud rate
	 */
	void begin(long baudRate);

	/**
	 * \brief Returns true if a command has been received
	 *
	 * You can check what type of command and retrieve the associated data
	 * only after this function returns true and before you call this
	 * function again. If this function returns false, do not rely on other
	 * functions related to received data because they could have wrong
	 * values.
	 * \return true if a command has been received
	 */
	bool commandReceived();

	/**
	 * \brief Returns true if we received a sequence point
	 *
	 * \return true if we received a sequence point
	 */
	bool isSequencePoint() const
	{
		return (m_receivedCommand == 'P');
	}

	/**
	 * \brief Returns true if we received a start stream command
	 *
	 * \return true if we received a start stream command
	 */
	bool isStartStream() const
	{
		return (m_receivedCommand == 'S');
	}

	/**
	 * \brief Returns true if we received a start immediate command
	 *
	 * \return true if we received a start immediate command
	 */
	bool isStartImmediate() const
	{
		return (m_receivedCommand == 'I');
	}

	/**
	 * \brief Returns true if we received a stop command
	 *
	 * \return true if we received a stop command
	 */
	bool isStop() const
	{
		return (m_receivedCommand == 'H');
	}

	/**
	 * \brief Returns the received command
	 *
	 * \return the received command
	 */
	char receivedCommand() const
	{
		return m_receivedCommand;
	}

	/**
	 * \brief Sets the object to fill with the next sequence packet
	 *
	 * The object provided in the function parameter will be filled by the
	 * next sequence packet. Change it after receiveing a sequence packet or
	 * it will be overwritten by the subsequent sequence packet
	 * \param p the object to store the sequence point
	 */
	void setNextSequencePointToFill(SequencePoint* p);

	/**
	 * \brief Returns the object to fill with the next sequence packet
	 *        (const version)
	 *
	 * See the description of setNextSequencePointToFill()
	 * \return the object to fill with the next sequence packet
	 */
	const SequencePoint* nextSequencePointToFill() const
	{
		return m_pointToFill;
	}

	/**
	 * \brief Returns the object to fill with the next sequence packet
	 *
	 * See the description of setNextSequencePointToFill()
	 * \return the object to fill with the next sequence packet
	 */
	SequencePoint* nextSequencePointToFill()
	{
		return m_pointToFill;
	}

	/**
	 * \brief Returns the received point dimension
	 *
	 * This is only valid after we received a start packet
	 * \return the received point dimension
	 */
	unsigned char pointDimension() const
	{
		return m_receivedPointDim;
	}

	/**
	 * \brief Sends a buffer not full package
	 */
	void sendBufferNotFull();

	/**
	 * \brief Sends a buffer full package
	 */
	void sendBufferFull();

	/**
	 * \brief Sends a debug packet
	 *
	 * \param msg the message to send. This cannot be longer than 255 bytes
	 */
	void sendDebugPacket(const char* msg);

	/**
	 * \brief Sends a battery charge packet
	 *
	 * \param v the current battery charge between 0 (depleted) and 255
	 *          (full charge)
	 */
	void sendBatteryCharge(unsigned char v);

private:
	/**
	 * \brief Returns true if the previous command we received is complete
	 *
	 * \return true if the previous command we received is complete
	 */
	bool previousCommandComplete() const;

	/**
	 * \brief The pointer to the next SequencePoint object to fill
	 */
	SequencePoint* m_pointToFill;

	/**
	 * \brief The command we received
	 *
	 * This is 0 if we didn't received any command yet
	 */
	char m_receivedCommand;

	/**
	 * \brief The number of bytes past the command type we received
	 *
	 * This is the number of bytes after the first one (that is the command
	 * type) that we received. This is needed for start* packages (we have
	 * to receive the point dimension) and for sequence points (we have to
	 * receive the points)
	 */
	unsigned char m_receivedPacketBytes;

	/**
	 * \brief The received point dimension
	 */
	unsigned char m_receivedPointDim;

	/**
	 * \brief Copy constructor is disabled
	 */
	SerialCommunication(const SerialCommunication&);

	/**
	 * \brief Copy operator is disabled
	 */
	SerialCommunication& operator=(const SerialCommunication&);
};

#endif
