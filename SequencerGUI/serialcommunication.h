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

#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QSerialPort>
#include <QByteArray>
#include <QObject>
#include <QTimer>
#include <memory>
#include "sequence.h"

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
 * stop() function. It is possible to decide whether the sequence should be
 * played once (i.e. streaming stops as soon as the last point is reached) or
 * continuously (i.e. the sequnce is restarted from the beginning after the last
 * point is reached). The startImmediate() function starts the immediate
 * modality, which terminates when the stop() function is called. When in
 * immediate mode, this connects to the curPointChanged() signal of the stream,
 * thus sending a new command every time the current point in the sequence
 * changes.  In either cases the streamStopped() signal is emitted when points
 * are no longer streamed. When in one modality it is not possible to call a
 * function of the other modality (functions will return false). It is also an
 * error when functions of one modality are called before the modality is
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
 *	- sequence finished
 *	- debug packet
 *	- battery charge packet
 *
 * The "start sequence" and "start immediate mode" packets tell the hardware in
 * which modality it should work. The "start sequence" makes the hardware expect
 * the sequence to be continuously sent and timing of each point of the sequence
 * are respected. Moreover the hardware responds to each sequence packet with
 * either a "sequence buffer not full" or a "sequence buffer full" packet. This
 * way the PC can send sequence packets until the hardware internal buffer is
 * full, to avoid delays in sequence timings. If the hardware sent a "sequence
 * buffer full" packet, it will send a "sequence buffer not full" packet as soon
 * as the buffer is no longer full (this "sequence buffer not full" packet can
 * be sent at any time, not only in response to a packet from the PC). To
 * terminate sequence execution the PC sends a "stop" packet. The hardware then
 * answers with a "sequence finished" packet as soon as the last point is
 * reached and kept for its whose duration. The "start immediate mode" packet
 * instructs the hardware to immediately process incoming sequence packets. This
 * means that the "duration" property of sequence points is not respected and
 * that a pose it kept until a new sequence point arrives. In this case the
 * hardware does not send any packet back (there is no buffer). To termiante the
 * immediate mode, the PC sends a "stop" packet. Packets sent before either
 * "start sequence" or "start immediate mode" are discarded.
 *
 * The debug packet is used by the hardware for debugging purpouse. It contains
 * a string of maximum length 255 bytes which is simply displayed (no other
 * action is performed). The battery charge packet is used to communicate the
 * current charge of batteries. It could be sent at any time.
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
 *
 * "sequence finished"
 * the characted 'E' (1 byte)
 *
 * "debug packet"
 * the character 'D' (1 byte) - length of message (1 byte) - message (length of
 * message elements)
 *
 * "battery charge packet" (battery charge is 0 to indicate depleted battery,
 * 255 for fully charged batteries)
 * the character 'B' (1 byte) - battery charge (1 byte)
 */
class SerialCommunication : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString serialPortName READ serialPortName WRITE setSerialPortName NOTIFY serialPortNameChanged)
	Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
	Q_PROPERTY(bool oneShotSequence READ oneShotSequence WRITE setOneShotSequence NOTIFY oneShotSequenceChanged)
	Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
	Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)
	Q_PROPERTY(bool isStreamMode READ isStreamMode NOTIFY isStreamModeChanged)
	Q_PROPERTY(bool isImmediateMode READ isImmediateMode NOTIFY isImmediateModeChanged)
	Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
	Q_PROPERTY(float batteryCharge READ batteryCharge NOTIFY batteryChargeChanged)

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
	 * \brief Returns the name of the serial port to open
	 *
	 * \return the name of the serial port to open
	 */
	QString serialPortName() const
	{
		return m_serialPortName;
	}

	/**
	 * \brief Sets the name of the serial port to open
	 *
	 * \param serialPortName the name of the serial port to open
	 */
	void setSerialPortName(QString serialPortName);

	/**
	 * \brief Returns the baud rate
	 *
	 * \return the baud rate
	 */
	int baudRate() const
	{
		return m_baudRate;
	}

	/**
	 * \brief Sets the baud rate
	 *
	 * \param baudRate the new baud rate
	 */
	void setBaudRate(int baudRate);

	/**
	 * \brief Returns whether the sequence is played once or continuously
	 *
	 * \return whether the sequence is played once or continuously
	 */
	bool oneShotSequence() const
	{
		return m_oneShotSequence;
	}

	/**
	 * \brief Sets whether the sequence is played once or continuously
	 *
	 * \param oneShot whether the sequence is played once or continuously
	 */
	void setOneShotSequence(bool oneShot);

	/**
	 * \brief Opens the serial port
	 *
	 * If a port was already opened, closes it before opening the new one.
	 * \return false in case of error, true if the port was opened
	 *         successfully
	 */
	Q_INVOKABLE bool openSerial();

	/**
	 * \brief Closes the serial port
	 *
	 * This cannot be called if a stream is being sent (call stop(),
	 * before). If the stream is already closed, this does nothing
	 * \return false in case of error
	 */
	Q_INVOKABLE bool closeSerial();

	/**
	 * \brief Starts streaming the sequence
	 *
	 * The sequence current point is updated as data is streamed (the
	 * current point will be the next point to sent through the port). If
	 * the current point is changed externally, the next point that is sent
	 * sequence will be the new current point.
	 * \param sequence the sequence to send. It must remain valid until the
	 *                 stop() function is called or the sequence is finished
	 * \param startFromCurrent if true the streaming starts from the current
	 *                         point, otherwise starts from the beginning
	 * \return false in case of error
	 */
	Q_INVOKABLE bool startStream(Sequence* sequence, bool startFromCurrent = false);

	/**
	 * \brief Pauses streaming data
	 *
	 * This simply temporarily stops sending data. To restart and continue
	 * from the point where sending was interrupted, call resumeStream()
	 * \return false in case of error
	 */
	Q_INVOKABLE bool pauseStream();

	/**
	 * \brief Resumes a paused stream
	 *
	 * If the stream is simply not paused, nothing happends. See the
	 * description of the pauseStream() function for more information
	 * \return false in case of error
	 */
	Q_INVOKABLE bool resumeStream();

	/**
	 * \brief Starts sending the sequence in immediate mode
	 *
	 * Every time the current point changes, a sequence is send through the
	 * port.
	 * \param sequence the sequence to send. It must remain valid until the
	 *                 stop() function is called
	 * \return false in case of error
	 */
	Q_INVOKABLE bool startImmediate(Sequence* sequence);

	/**
	 * \brief Stops sending the sequence
	 *
	 * \return false in case of error
	 */
	Q_INVOKABLE bool stop();

	/**
	 * \brief Return true if the serial port is open
	 *
	 * \return true if the serial port is open
	 */
	bool isConnected() const
	{
		return m_serialPort.isOpen();
	}

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
		return m_isStreamMode;
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

	/**
	 * \brief Returns true if streaming is paused
	 *
	 * \return true if streaming is paused
	 */
	bool isPaused() const
	{
		return m_paused;
	}

	/**
	 * \brief Returns the current battery charge
	 *
	 * This is a value between 0 and 100 (charge percentage), it is -1 if we
	 * have not received the charge yet
	 * \return the battery charge in percentage
	 */
	float batteryCharge() const
	{
		return m_batteryCharge;
	}

signals:
	/**
	 * \brief The signal emitted when the serial port name changes
	 */
	void serialPortNameChanged();

	/**
	 * \brief The signal emitted when the baud rate changes
	 */
	void baudRateChanged();

	/**
	 * \brief The signal emitted when the oneShotSequence property changes
	 */
	void oneShotSequenceChanged();

	/**
	 * \brief The signal emitted when the serial port is opened/closed
	 */
	void isConnectedChanged();

	/**
	 * \brief The signal emitted when the isStreaming property changes
	 */
	void isStreamingChanged();

	/**
	 * \brief The signal emitted when the isStreamMode property changes
	 */
	void isStreamModeChanged();

	/**
	 * \brief The signal emitted when the isImmediateMode property changes
	 */
	void isImmediateModeChanged();

	/**
	 * \brief The signal emitted when streaming is paused/resumed
	 */
	void isPausedChanged();

	/**
	 * \brief The signal emitted if there is an error writing or reading
	 *        from the serial port
	 *
	 * \param error a description of the error
	 */
	void streamError(QString error);

	/**
	 * \brief The signal emitted when we receive a debug message from the
	 *        hardware
	 *
	 * \param msg the message the hardware sent
	 */
	void debugMessage(QString msg);

	/**
	 * \brief The signal emitted when the battery charge changes
	 */
	void batteryChargeChanged();

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
	 *
	 * This is connected to both the curPointChanged() and the
	 * curPointValuesChanged() signals of the sequence
	 */
	void curPointChanged();

private:
	/**
	 * \brief Returns a sequence packet for the given point
	 *
	 * This function doesn't check that p has the length that was used in
	 * the start stream or start immediate package, ensure this externally
	 * (this condition is fulfilled if the same sequence is used for the
	 * start and this function, as it should be)
	 * \param p the point for which to create a packet
	 * \return the packet for the point
	 */
	QByteArray createSequencePacketForPoint(const SequencePoint& p) const;

	/**
	 * \brief Processes received packets
	 */
	void processReceivedPackets();

	/**
	 * \brief The function to call when the sequence is no longer streamed
	 *
	 * This is called when either immediate mode stops or the "sequence
	 * finished" packet is received
	 */
	void sequenceStreamEnded();

	/**
	 * \brief Moves the current point forward
	 *
	 * This function moves the current point forward by one. If we reach the
	 * end of the sequence in stream mode, terminates the streaming.
	 */
	void incrementCurPoint();

	/**
	 * \brief The function that actually sends data
	 *
	 * \param dataToSend the data to send through the serial port
	 */
	void sendData(const QByteArray& dataToSend);

	/**
	 * \brief Changes the value of the m_isStreamMode flag and emits the
	 *        changed signal if needed
	 *
	 * \param v the new value of the flag
	 */
	void setIsStreamMode(bool v);

	/**
	 * \brief Changes the value of the m_isImmediateMode flag and emits the
	 *        changed signal if needed
	 *
	 * \param v the new value of the flag
	 */
	void setIsImmediateMode(bool v);

	/**
	 * \brief Changes the value of the battery charge and emits the changed
	 *        signal if needed
	 *
	 * \param v the new value of the battery charge (if negative, -1.0 is
	 *          used)
	 */
	void setBatteryCharge(float v);

	/**
	 * \brief The name of the serial port to open
	 */
	QString m_serialPortName;

	/**
	 * \brief The baud rate of the serial port
	 */
	int m_baudRate;

	/**
	 * \brief Whether the sequence is played only once or continuously
	 *
	 * If true the sequence is played only once, if false continuously
	 */
	bool m_oneShotSequence;

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
	 * \brief The index in m_incomingData to process
	 */
	int m_indexToProcess;

	/**
	 * \brief If true streaming is paused in stream mode
	 */
	bool m_paused;

	/**
	 * \brief True if we cannot send more sequence points because the queue
	 *        of the hardware is full
	 */
	bool m_hardwareQueueFull;

	/**
	 * \brief The current charge level of the battery
	 */
	float m_batteryCharge;

	/**
	 * \brief True if we have sent a stop sequence packet and are waiting
	 *        for the end of the sequence
	 */
	bool m_stopping;
};

#endif // SERIALCOMMUNICATION_H
