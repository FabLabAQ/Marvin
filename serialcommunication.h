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
#include <memory>
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
 * function starts the stream modality, which must be terminated using the
 * stop() function. When in immediate mode, this connects to the
 * curPointChanged() signal of the stream, thus sending a new command every time
 * the current point in the sequence changes. The startImmediate() function
 * starts the immediate modality, which terminates when all points have been
 * streamed or when the stop() function is called. In either cases the
 * streamStopped() signal is emitted. When in one modality it is not possible to
 * call a function of the other modality (an exception of type
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
class SerialCommunication : public QObject
{
	Q_OBJECT

public:
	/**
	 * \brief Constructor
	 *
	 * \param parent the parent object
	 */
	explicit SerialCommunication(QObject* parent = nullptr);

	/**
	 * \brief Destructor
	 *
	 * This stops any streamed sequence and closes the port
	 */
	virtual ~SerialCommunication();

	/**
	 * \brief Copy constructor is deleted
	 *
	 * \param other the object to copy
	 */
	SerialCommunication(const SerialCommunication& other) = delete;

	/**
	 * \brief Move constructor is deleted
	 *
	 * \param other the object to move into this
	 */
	SerialCommunication(SerialCommunication&& other) = delete;

	/**
	 * \brief Copy operator is deleted
	 */
	SerialCommunication& operator=(const SerialCommunication& other) = delete;

	/**
	 * \brief Move operator is deleted
	 */
	SerialCommunication& operator=(SerialCommunication&& other) = delete;

	/**
	 * \brief Opens the serial port
	 *
	 * If a port was already opened, closes it before opening the new one.
	 * If however a sequence is being sent, throws an exception.
	 * \param port the port to open
	 * \param baudRate the baudrate
	 * \return false in case of error, true if the port was opened
	 *         successfully
	 */
	bool openSerial(QString port, int baudRate);

	/**
	 * \brief Closes the serial port
	 *
	 * This cannot be called if a stream is being sent (call stop(),
	 * before). If the stream is already closed, this does nothing
	 */
	void closeSerial();

	/**
	 * \brief Starts streaming the sequence
	 *
	 * This throws an exception in case of errors. The sequence current
	 * point is updated as data is streamed (the current point will be the
	 * next point to sent through the port). If the current point is changed
	 * externally, the next point that is sent sequence will be the new
	 * current point.
	 * \param sequence the sequence to send. It must remain valid until the
	 *                 stop() function is called or the sequence is finished
	 * \param startFromCurrent if true the streaming starts from the current
	 *                         point, otherwise starts from the beginning
	 */
	void startStream(Sequence* sequence, bool startFromCurrent = false);

	/**
	 * \brief Pauses streaming data
	 *
	 * This throws an exception in case of errors. This simply temporarily
	 * stops sending data. To restart and continue from the point where
	 * sending was interrupted, call resumeStream()
	 */
	void pauseStream();

	/**
	 * \brief Resumes a paused stream
	 *
	 * This throws an exception in case of errors, but it the stream is
	 * simply not paused, nothing happends. See the description of the
	 * pauseStream() function for more information
	 */
	void resumeStream();

	/**
	 * \brief Starts sending the sequence in immediate mode
	 *
	 * This throws an exception in case of errors. Every time the current
	 * point changes, a sequence is send through the port.
	 * \param sequence the sequence to send. It must remain valid until the
	 *                 stop() function is called
	 */
	void startImmediate(Sequence* sequence);

	/**
	 * \brief Stops sending the sequence
	 *
	 * This throws an exception in case of errors
	 */
	void stop();

	/**
	 * \brief Returns true if we have an associated stream, either in stream
	 *        or immediate mode
	 *
	 * \return true if we have an associated stream, either in stream or
	 *         immediate mode
	 */
	bool isStreaming() const
	{
		return (m_sequence != nullptr);
	}

	/**
	 * \brief Returns true if we are in stream mode
	 *
	 * \return true if we are in stream mode
	 */
	bool isStreamMode() const
	{
		m_isStreamMode;
	}

	/**
	 * \brief Returns true if we are in immediate mode
	 *
	 * \return true if we are in immediate mode
	 */
	bool isImmediateMode() const
	{
		return m_isImmediateMode;
	}

signals:
	/**
	 * \brief The signal emitted when starting streaming in stream mode
	 */
	void streamStarted();

	/**
	 * \brief The signal emitted when the sequence streamed in stream mode
	 *        is completed
	 */
	void streamStopped();

	/**
	 * \brief The signal emitted if there is an error writing or reading
	 *        from the serial port
	 *
	 * \param error a description of the error
	 */
	void streamError(QString error);

private slots:
	/**
	 * \brief The slot called when there is data ready to be read
	 */
	void handleReadyRead();

	/**
	 * \brief The function called when there is an error in the serial
	 *        communication
	 *
	 * \param error the error code
	 */
	void handleError(QSerialPort::SerialPortError error);

	/**
	 * \brief The slot called a second after the serial port is opened to
	 *        start sending data
	 *
	 * This is needed to give Arduino time to boot. This function is also
	 * called if a sequence is started after arduino has booted. Basically
	 * this function automatically sends data if called from the timer and
	 * streaming has already been requested. If called from the timer but
	 * streaming hasn't been requested yet, it does nothing. When streaming
	 * is requested, this function is called by either startStream() or
	 * startImmediate() and starts sending data.
	 */
	void arduinoBootFinished();

	/**
	 * \brief The slot called when the current point in the sequence changes
	 */
	void curPointChanged();

private:
	/**
	 * \brief The function that actually sends data
	 *
	 * \param dataToSend the data to send through the serial port
	 */
	void sendData(const QByteArray& dataToSend);

	/**
	 * \brief The serial communication port
	 */
	QSerialPort m_serialPort;

	/**
	 * \brief The sequence to stream
	 *
	 * This is nullptr if there is no sequence to stream
	 */
	Sequence* m_sequence;

	/**
	 * \brief True if we are in stream modality
	 */
	bool m_isStreamMode;

	/**
	 * \brief True if we are in immediate modality
	 */
	bool m_isImmediateMode;

	/**
	 * \brief The timer to wait for Arduino boot to finish
	 *
	 * See arduinoBootFinished() description
	 */
	QTimer m_arduinoBoot;

	/**
	 * \brief The buffer of data from the serial port
	 */
	QByteArray m_incomingData;

	/**
	 * \brief If true streaming is paused in stream mode
	 */
	bool m_paused;
};

#endif // SERIALCOMMUNICATION_H
