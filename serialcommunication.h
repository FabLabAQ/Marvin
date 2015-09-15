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

#include <QSerialPort>
#include <QByteArray>
#include <QObject>
#include <QTimer>
#include <exception>
#include "sequence.h"

/**
 * \brief The exception thrown by the SerialCommunication class
 */
class SerialCommunicationException : public std::exception
{
public:
	/**
	 * \brief Constructor
	 *
	 * \param reason a description of the exception. The buffer for this is
	 *               at most 256 characters (including the '\0' terminator)
	 */
	SerialCommunicationException(const char* reason) throw() :
		std::exception()
	{
		strncpy(m_reason, reason, 256);
		m_reason[255] = '\0';
		sprintf(m_errorMessage, "Serial communication exception, reason: %s", m_reason);
		m_errorMessage[511] = '\0';
	}

	/**
	 * \brief Copy constructor
	 *
	 * \param other the exception to copy
	 */
	SerialCommunicationException(const SerialCommunicationException& other) throw() :
		std::exception(other)
	{
		strncpy(m_reason, other.m_reason, 256);
		m_reason[255] = '\0';
		strncpy(m_errorMessage, other.m_errorMessage, 512);
		m_errorMessage[511] = '\0';
	}

	/**
	 * \brief Copy operator
	 *
	 * \param other the exception to copy
	 */
	SerialCommunicationException& operator=(const SerialCommunicationException& other) throw()
	{
		if (&other == this) {
			return *this;
		}

		std::exception::operator=(other);
		strncpy(m_reason, other.m_reason, 256);
		m_reason[255] = '\0';
		strncpy(m_errorMessage, other.m_errorMessage, 512);
		m_errorMessage[511] = '\0';

		return *this;
	}

	/**
	 * \brief Destructor
	 */
	virtual ~SerialCommunicationException() throw()
	{
	}

	/**
	 * \brief Returns a C string describing the exception
	 *
	 * \return a C string describing the exception
	 */
	virtual const char *what() const throw()
	{
		return m_errorMessage;
	}

	/**
	 * \brief Returns the reason why the exception was thrown
	 *
	 * \return the reason why the exception was thrown
	 */
	const char *reason() const throw()
	{
		return m_reason;
	}

private:
	/**
	 * \brief The reason why the exception was thrown
	 */
	char m_reason[256];

	/**
	 * \brief The message describing the error
	 */
	char m_errorMessage[512];
};

/**
 * \brief The class handling the communication with Arduino through the serial
 *        port
 *
 * This class is responsible of handling the communication with the hardware. It
 * takes a sequence and streams it through the serial port. Before attempting to
 * use any function, you must open the serial port to connect to the board,
 * using the openSerial() function.
 *
 * This class has two functionining modalities: stream and immediate. In stream
 * mode it is possible to pause and resume the stream, though the buffering
 * performed by the Arduino board prevents a precise stop. The startStream()
 * function starts the stream modalities, which must be terminated using the
 * stopStream() function. When in immediate mode, this connects to the
 * curPointChanged() signal of the stream, thus sending a new command every time
 * the current point in the sequence changes. The startImmediate() function
 * starts the immediate modality, which must be terminated using the
 * stopImmediate() function. When in one modality it is not possible to call a
 * function of the other modality (an exception of type
 * SerialCommunicationException is thrown if this is attempted). An exception is
 * also thrown when functions of one modality are called before the modality is
 * started or when the serial port is not open.
 *
 * The communication protocol between this program and the Arduino board is the
 * following. The packets the PC may send to the hardware are the following
 * ones:
 *	- sequence packet
 *	- start sequence
 *	- start immediate mode
 *	- stop
 *
 * The packes the hardware may send to the PC are the following ones:
 *	- sequence buffer not full
 *	- sequence buffer full
 *
 * The "start sequence" and "start immediate mode" packets tell the hardware in
 * which modality it should work. The "start sequence" makes the hardware expect
 * the sequence to be continuously sent and timing of each point of the sequence
 * are respected. Moreover the hardware responds to each sequence packet with
 * either a "sequence buffer not full" or a "sequence buffer full" packet. This
 * way the PC can send sequence packets until the hardware internal buffer is
 * full, to avoid delays in sequence timings. To terminate sequence execution
 * the PC sends a "stop" packet. The "start immediate mode" packet instructs the
 * hardware to immediately process incoming sequence packets. This means that
 * the "duration" property of sequence points is not respected and that a pose
 * it kept until a new sequence point arrives. In this case the hardware does
 * not send any packet back (there is no buffer). To termiante the immediate
 * mode, the PC sends a "stop" packet. Packets sent before either "start
 * sequence" or "start immediate mode" are discarded.
 *
 * Here is the detailed description of every packet in the protocol.
 *
 * "sequence packet"
 * the character 'P' (1 byte) - step duration (2 bytes, milliseconds, most
 * significant byte first) - step time to target (2 bytes, milliseconds, most
 * significant byte first) - positions (numElements bytes, one byte per point
 * dimension)
 *
 * "start sequence" (numElements is the dimension of each point of the sequence)
 * the character 'S' (1 byte) - numElements (1 byte)
 *
 * "start immediate mode" (numElements is the dimension of each point of the
 * sequence)
 * the character 'I' (1 byte) - numElements (1 byte)
 *
 * "stop"
 * the character 'H' (1 byte)
 *
 * "sequence buffer not full"
 * the character 'N' (1 byte)
 *
 * "sequence buffer full"
 * the character 'F' (1 byte)
 */
//class SerialCommunication : public QObject
//{
//	Q_OBJECT

//public:
//	/**
//	 * \brief Constructor
//	 *
//	 * \param parent the parent object
//	 */
//	explicit SerialCommunication(QObject* parent = nullptr);

//openSerial()
//startStream()
//pauseStream()
//resumeStream()
//stopStream()
//startImmediate()
//stopImmediate()

//	/**
//	 * \brief Destructor
//	 */
//	virtual ~SerialCommunication();

//	/**
//	 * \brief Sets the serial port to use and opens it
//	 *
//	 * \param port the name of the port to use
//	 */
//	void setSerialPort(QString port);

//	/**
//	 * \brief Returns true if a command has been received and extracts the
//	 *        command from the queue
//	 *
//	 * Ater a call to this function returns true, you can get command parts
//	 * with receivedCommandParts*() functions
//	 * \return true if a command has been received
//	 */
//	bool extractReceivedCommand();

//	/**
//	 * \brief The number of parts in the received command
//	 *
//	 * This is only valid after extractReceivedCommand returns true
//	 * \return the number of parts in the the received command
//	 */
//	int receivedCommandNumParts() const;

//	/**
//	 * \brief The i-th part of the received command as a string
//	 *
//	 * This is only valid after extractReceivedCommand returns true
//	 * \param i the part to return
//	 * \return the i-th part of the received command as a string
//	 */
//	QString receivedCommandPart(int i) const;

//	/**
//	 * \brief The i-th part of the received command as an int
//	 *
//	 * This is only valid after extractReceivedCommand returns true
//	 * \param i the part to return
//	 * \return the i-th part of the received command as an int
//	 */
//	int receivedCommandPartAsInt(int i) const;

//	/**
//	 * \brief The i-th part of the received command as a float
//	 *
//	 * This is only valid after extractReceivedCommand returns true
//	 * \param i the part to return
//	 * \return the i-th part of the received command as a float
//	 */
//	float receivedCommandPartAsFloat(int i) const;

//	/**
//	 * \brief Creates a new command to send
//	 *
//	 * After calling this function you can start appending command parts
//	 * with the appendCommandPart() function, which adds parts separated by
//	 * a whitespace
//	 */
//	void newCommandToSend();

//	/**
//	 * \brief Adds a part to the command to send
//	 *
//	 * Use this for strings
//	 * \param part the part to add
//	 */
//	void appendCommandPart(QString part);

//	/**
//	 * \brief Adds a part to the command to send
//	 *
//	 * Use this for integers
//	 * \param part the part to add
//	 */
//	void appendCommandPart(int part);

//	/**
//	 * \brief Adds a part to the command to send
//	 *
//	 * Use this for floats
//	 * \param part the part to add
//	 */
//	void appendCommandPart(float part);

//	/**
//	 * \brief Sends the last command
//	 *
//	 * If called multiple times without calling newCommandToSend() first,
//	 * sends the same command over and over again
//	 */
//	void sendCommand();

//private slots:
//	/**
//	 * \brief The slot called when there is data ready to be read
//	 *
//	 * This calls the commandReceived function of controller
//	 */
//	void handleReadyRead();

//	/**
//	 * \brief The function called when there is an error in the serial
//	 *        communication
//	 *
//	 * \param error the error code
//	 */
//	void handleError(QSerialPort::SerialPortError error);

//	/**
//	 * \brief The slot called a second after the serial port is opened
//	 *
//	 * This is needed to give Arduino time to boot. All commands sent
//	 * before this timeout expires are stored and send as soon as this is
//	 * called
//	 */
//	void arduinoBootFinished();

//private:
//	/**
//	 * \brief The function that actually sends data
//	 *
//	 * \param dataToSend the data  osend through the serial port
//	 */
//	void sendData(const QByteArray& dataToSend);

//	/**
//	 * \brief The controller object of the game
//	 */
//	Controller* const m_controller;

//	/**
//	 * \brief The serial communication port
//	 */
//	QSerialPort m_serialPort;

//	/**
//	 * \brief The timer to wait for Arduino boot to finish
//	 *
//	 * See arduinoBootFinished() description
//	 */
//	QTimer m_arduinoBoot;

//	/**
//	 * \brief The list of commands sent prior to Arduino finishing boot
//	 *
//	 * See arduinoBootFinished() description
//	 */
//	QList<QByteArray> m_preBootCommands;

//	/**
//	 * \brief The buffer of data from the serial port
//	 */
//	QByteArray m_incomingData;

//	/**
//	 * \brief When different from -1, a command has been received and is in
//	 *        m_incomingData from position 0 to this position
//	 */
//	int m_endCommandPosition;

//	/**
//	 * \brief The list of received command parts
//	 */
//	QStringList m_receivedCommandParts;

//	/**
//	 * \brief The buffer with parts of the command to send
//	 */
//	QByteArray m_commandPartsToSend;
//};

#endif // SERIALCOMMUNICATION_H
