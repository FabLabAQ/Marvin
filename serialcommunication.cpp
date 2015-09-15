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

SerialCommunication::SerialCommunication(QObject* parent)
	: QObject(parent)
	, m_serialPort()
	, m_sequence(nullptr)
	, m_isStreamMode(false)
	, m_isImmediateMode(false)
	, m_arduinoBoot()
	, m_incomingData()
	, m_paused(false)
{
	// Connecting signals from the serial port
	connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialCommunication::handleReadyRead);
	connect(&m_serialPort, &QSerialPort::error, this, &SerialCommunication::handleError);

	// Connecting the signal for the Arduino boot timer. Also setting the timer to be singleShot
	m_arduinoBoot.setSingleShot(true);
	connect(&m_arduinoBoot, &QTimer::timeout, this, &SerialCommunication::arduinoBootFinished);
}

SerialCommunication::~SerialCommunication()
{
	// No exception must leave the destructor, so we catch everything here. We use two separated
	// try-catch statements because we must always call both stop() and closeSerial()
	try {
		stop();
	} catch (...) {
	}

	try {
		closeSerial();
	} catch (...) {
	}
}

bool SerialCommunication::openSerial(QString port, int baudRate)
{
	if (isStreaming()) {
		throw SerialCommunicationException("Cannot open port while a sequence is being streamed");
	}

	// Closing the old port
	if (m_serialPort.isOpen()) {
		m_serialPort.close();
		m_serialPort.clearError();
	}

	// Setting the name and baud rate of the port
	m_serialPort.setPortName(port);
	m_serialPort.setBaudRate(baudRate);

	// Trying to open the port
	if (!m_serialPort.open(QIODevice::ReadWrite)) {
		throw SerialCommunicationException(QString("Cannot open port %1").arg(port).toLatin1().data());
	}

	// This is necessary to give time to Arduino to "boot" (the board reboots every time the serial port
	// is opened, and then there are 0.5 seconds taken by the bootloader)
	m_arduinoBoot.start(1000);
}

void SerialCommunication::closeSerial()
{
	if (isStreaming()) {
		throw SerialCommunicationException("Cannot close port while a sequence is being streamed");
	}

	// Closing the port
	if (m_serialPort.isOpen()) {
		m_serialPort.close();
		m_serialPort.clearError();
	}
}

void SerialCommunication::startStream(Sequence* sequence, bool startFromCurrent)
{
	if (!m_serialPort.isOpen()) {
		throw SerialCommunicationException("Cannot start streaming with a closed serial port");
	}
	if (isStreaming()) {
		throw SerialCommunicationException("Cannot start a new stream while a sequence is already being streamed");
	}

	// Resetting the pause flag and setting the m_is*Mode flags
	m_paused = false;
	m_isStreamMode = true;
	m_isImmediateMode = false;

	// Saving the sequence and resetting the current point if needed
	m_sequence = sequence;
	if (!startFromCurrent) {
		m_sequence->setCurPoint(0);
	}

	// If the m_arduinoBoot timer is running, we have to wait, otherwise we explicitly call
	// the arduinoBootFinished() function to start sending the sequence
	if (!m_arduinoBoot.isActive()) {
		arduinoBootFinished();
	}
}

void SerialCommunication::pauseStream()
{
	if (!isStreamMode()) {
		throw SerialCommunicationException("Cannot pause when no sequence is being streamed");
	}

	m_paused = true;
}

void SerialCommunication::resumeStream()
{
	if (!isStreamMode()) {
		throw SerialCommunicationException("Cannot pause when no sequence is being streamed");
	}

	if (!m_paused) {
		return;
	}

	// Resuming streaming
	m_paused = false;
	SEND CUR POINT IN STREAM
}

void SerialCommunication::startImmediate(Sequence* sequence)
{
	if (!m_serialPort.isOpen()) {
		throw SerialCommunicationException("Cannot start streaming with a closed serial port");
	}
	if (isStreaming()) {
		throw SerialCommunicationException("Cannot start a new stream while a sequence is already being streamed");
	}

	// Setting the m_is*Mode flags
	m_isStreamMode = false;
	m_isImmediateMode = true;

	// Saving the sequence
	m_sequence = sequence;

	// Connecting the signal of the sequence telling us when the current point changes
	connect(m_sequence, &Sequence::curPointChanged, this, &SerialCommunication::curPointChanged);

	// If the m_arduinoBoot timer is running, we have to wait, otherwise we explicitly call
	// the arduinoBootFinished() function to send the current point of the sequence
	if (!m_arduinoBoot.isActive()) {
		arduinoBootFinished();
	}
}

void SerialCommunication::stop()
{
	if (!isStreaming()) {
		throw SerialCommunicationException("No stream to stop");
	}

	// Disconnecting all signals from the sequence to us
	m_sequence->disconnect(this);

	// Setting the sequence to nullptr
	m_sequence = nullptr;

	// If we were in stream mode, emitting signal
	if (isStreamMode()) {
		emit streamStopped();
	}

	// Resetting flags
	m_paused = false;
	m_isStreamMode = false;
	m_isImmediateMode = false;
}

void SerialCommunication::handleReadyRead()
{
	* \brief The slot called when there is data ready to be read

	/**
	 * \brief The signal emitted when the sequence streamed in stream mode
	 *        is completed
	 */
	void streamStopped()
}

void SerialCommunication::handleError(QSerialPort::SerialPortError error)
{
	 * \brief The function called when there is an error in the serial
	 *        communication
	 *
	 * \param error the error code

			void streamError(QString error);
}

void SerialCommunication::arduinoBootFinished()
{
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

			/**
			 * \brief The signal emitted when starting streaming in stream mode
			 */
			void streamStarted();
}

void SerialCommunication::curPointChanged()
{
	* \brief The slot called when the current point in the sequence changes
}

void SerialCommunication::sendData(const QByteArray& dataToSend)
{
	 * \brief The function that actually sends data
	 *
	 * \param dataToSend the data to send through the serial port
	gsdfgsdfgdf

			void streamError(QString error);
}








DA WHAC-A-MAKER



void SerialCommunication::setSerialPort(QString port)
{
	// Closing the old port
	if (m_serialPort.isOpen()) {
		m_serialPort.close();
		m_serialPort.clearError();
	}

	// Setting the name of the port
	m_serialPort.setBaudRate(115200);
	m_serialPort.setPortName(port);

	// Trying to open the port
	if (!m_serialPort.open(QIODevice::ReadWrite)) {
		// Here we can't throw an exception because otherwise it would be impossible to change the settings
		// Simply printing a message to stderr
		std::cerr << "Error opening serial port at " << port.toLatin1().data() << std::endl;
	}

	// This is necessary to give time to Arduino to "boot" (the board reboots every time the serial port
	// is opened, and then there are 0.5 seconds taken by the bootloader)
	m_arduinoBoot.start(1000);
	m_preBootCommands.clear();
}

bool SerialCommunication::extractReceivedCommand()
{
	if (m_endCommandPosition == -1) {
		return false;
	}

	// Taking the command. Here we don't take the closing newline. Wehn we split we keep empty parts to have
	// a behavior compliant with the one on Arduino
	QString command = m_incomingData.left(m_endCommandPosition);
	m_receivedCommandParts = command.split(' ', QString::KeepEmptyParts);

	// Removing data for the extracted command from the queue
	m_incomingData = m_incomingData.mid(m_endCommandPosition + 1);

	// Now fixing m_endCommandPosition
	m_endCommandPosition = m_incomingData.indexOf('\n');

// std::cerr << "===extractReceivedCommand=== command: \"" << command.toLatin1().data() << "\", m_endCommandPosition: " << m_endCommandPosition << ", all data: \"" << m_incomingData.data() << "\"" << std::endl;

	return true;
}

int SerialCommunication::receivedCommandNumParts() const
{
	return m_receivedCommandParts.size();
}

QString SerialCommunication::receivedCommandPart(int i) const
{
	return m_receivedCommandParts[i];
}

int SerialCommunication::receivedCommandPartAsInt(int i) const
{
	return m_receivedCommandParts[i].toInt();
}

float SerialCommunication::receivedCommandPartAsFloat(int i) const
{
	return m_receivedCommandParts[i].toFloat();
}

void SerialCommunication::newCommandToSend()
{
	m_commandPartsToSend.clear();
}

void SerialCommunication::appendCommandPart(QString part)
{
	if (m_commandPartsToSend.size() != 0) {
		m_commandPartsToSend.append(' ');
	}

	m_commandPartsToSend.append(part.toLatin1());
}

void SerialCommunication::appendCommandPart(int part)
{
	appendCommandPart(QString::number(part));
}

void SerialCommunication::appendCommandPart(float part)
{
	appendCommandPart(QString::number(part));
}

void SerialCommunication::sendCommand()
{
	// Adding the final endline if is not there already
	if ((m_commandPartsToSend.size() != 0) && (!m_commandPartsToSend.endsWith('\n'))) {
		m_commandPartsToSend.append('\n');
	}

	if (m_arduinoBoot.isActive()) {
		// We cannot send the command yet, we have to wait for the boot procedure to finish
		m_preBootCommands.append(m_commandPartsToSend);
	} else {
		sendData(m_commandPartsToSend);
	}
}

void SerialCommunication::handleReadyRead()
{
	// Getting data
	QByteArray newData = m_serialPort.readAll();

	// Adding data to the buffer. We have to save the previous size of m_incomingData to
	// have the correct m_endCommandPosition
	const int oldSize = m_incomingData.size();
	m_incomingData.append(newData);

	// Now looking in new data for the first '\n' and the number of them
	int firstNewlinePos = -1;
	int numNewlines = 0;
	for (int i = 0; i < newData.size(); i++) {
		if (newData[i] == '\n') {
			if (firstNewlinePos == -1) {
				firstNewlinePos = i + oldSize;
			}
			++numNewlines;
		}
	}

// std::cerr << "===handleReadyRead=== new data: \"" << newData.data() << "\", oldSize: " << oldSize << ", firstNewlinePos: " << firstNewlinePos << ", numNewlines: " << numNewlines << ", m_endCommandPosition: " << m_endCommandPosition << ", all data: \"" << m_incomingData.data() << "\"" << std::endl;

	// If there was at least one '\n' in the new data, checking what to do
	if (numNewlines != 0) {
		// If there was no command in the queue, saving the position of the '\n' of the first
		// command (which is the first one)
		if (m_endCommandPosition == -1) {
			m_endCommandPosition = firstNewlinePos;
		}

		// Now telling the controller we have received commands. We call commandReceived() once for each
		// received command
		for (int i = 0; i < numNewlines; i++) {
			m_controller->commandReceived();
		}
	}
}

void SerialCommunication::handleError(QSerialPort::SerialPortError error)
{
	if (error != QSerialPort::NoError) {
		std::cerr << "Serial Communication error, code: " << error << std::endl;
	}
}

void SerialCommunication::arduinoBootFinished()
{
	// Sending all queued command and clearing the queue
	for (int i = 0; i < m_preBootCommands.size(); i++) {
		sendData(m_preBootCommands[i]);
	}
	m_preBootCommands.clear();
}

void SerialCommunication::sendData(const QByteArray& dataToSend)
{
	// Writing data
	qint64 bytesWritten = m_serialPort.write(dataToSend);

// std::cerr << "Command sent: " << dataToSend.data() << std::endl;

	if (bytesWritten == -1) {
		std::cerr  << "Error writing data, error: " << m_serialPort.errorString().toLatin1().data() << std::endl;
	} else if (bytesWritten != dataToSend.size()) {
		std::cerr  << "Cannot write all data, error: " << m_serialPort.errorString().toLatin1().data() << std::endl;
	}
}
