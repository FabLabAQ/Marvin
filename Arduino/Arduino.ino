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

// import Wire library to use I²C (I have to include it here because doing it
// only in Adafruit_PWMServoDriver.cpp doesn't work...)
#include <Wire.h>
#include "serialcommunication.h"
#include "sequenceplayer.h"
#include <stdlib.h>
// // import Wire library to use I²C
// #include <Wire.h>
// // import backpack library to use LED backpacks
// #include "Adafruit_LEDBackpack.h"
// // import GFX library to draw bitmaps on LED backpacks
// #include "Adafruit_GFX.h"

// The possible states
enum States {IdleState, StreamMode, StreamModeStopping, ImmediateMode};

// The minimum and maximum PWM value of all servos
const unsigned int servoMin[SequencePoint::dim] = {1150, 1800,  500,  800,  900,  550,  800,  550,  920,  500,  750, 1000,  500,  750,  650, 1450};
const unsigned int servoMax[SequencePoint::dim] = {1770, 1800, 1800, 2200, 1700, 1750, 2050, 1670, 2000, 1700, 2020, 1800, 1800, 1650, 2000, 2200};
const unsigned int servoMid[SequencePoint::dim] = {1500, 1800, 1000, 1150, 1300, 1400, 1160, 1250, 1300, 1320, 1100, 1420, 1350, 1650, 1750, 1800};

// The current status
States status = IdleState;
// The baud rate to use for communication with computer
const long baudRate = 57600;
// The object that handles communication
SerialCommunication serialCommunication;
// The microseconds of the last loop
unsigned long lastTime = 0;
// The object controlling the servos
SequencePlayer sequencePlayer(servoMin, servoMax);
// Each how many milliseconds we should send the battery charge
const unsigned long batteryInterval = 1000;
// The milliseconds we last sent the battery charge
unsigned long lastBatteryTime = 0;
// This is true if the sequence buffer was full
bool sequenceBufferWasFull = false;
// Battery pin
const int batteryPin = 3;

// // The face object
// Adafruit_8x8matrix face = Adafruit_8x8matrix();

// // A bitmap for a smile
// static const uint8_t PROGMEM smile_bmp[] =
//   { B00000000,
//     B01100110,
//     B00000000,
//     B00100100,
//     B00000000,
//     B01000010,
//     B00111100,
//     B00000000 };

// /**
//  * \brief Initializes led for the face
//  */
// void initializeFace()
// {
// 	// initialize LED backpack over I²C at the given address
// 	face.begin(0x70);
//
// 	// set rotation to match the position on the robot
// 	face.setRotation(3);
//
// 	// set an appropriate brightness (0-15)
// 	face.setBrightness(7);
// }

// /**
//  * \brief Draws a smiling face
//  */
// void smile()
// {
// 	// clear whatever was left on the display
// 	face.clear();
//
// 	// send bitmap to cover the entire display area
// 	face.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
//
// 	// show image
// 	face.writeDisplay();
// }

void setup()
{
	// Initializing the object handling serial communication
	serialCommunication.begin(baudRate);

	// The initial position of servos
	SequencePoint startPos;
	startPos.duration = 0;
	startPos.timeToTarget = 0;
	for (int i = 0; i < SequencePoint::dim; ++i) {
		unsigned long p = (unsigned long)(servoMid[i] - servoMin[i]) * 256 / (unsigned long)(servoMax[i] - servoMin[i]);
		startPos.point[i] = p;
	}

	// Initializing the object handling servos
	sequencePlayer.begin(startPos);

	// Setting the point to fill. The buffer cannot be full at this stage!
	serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());

// 	// initialize Adafruit's LED backpack
// 	initializeFace();
// 	// draw a smiling face
// 	smile();
}

void loop()
{
	// Moving servos.We do this even when idle because in that case we are sure the buffer is empty
	bool emptyBuffer = sequencePlayer.step();

	if ((status == StreamModeStopping) && emptyBuffer) {
		// We have finally stopped, clearing the sequence player buffer and returning idle
		sequencePlayer.clearBuffer();
		status = IdleState;
		serialCommunication.sendDebugPacket("-=== Stream finished ===-");
	} else if ((status == StreamMode) && sequenceBufferWasFull && (!sequencePlayer.bufferFull())) {
		// If the buffer was full and it is no longer full, sending a buffer not full package
		serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());
		serialCommunication.sendBufferNotFull();

		sequenceBufferWasFull = false;
	}

	// Checking if there are new commands
	if (serialCommunication.commandReceived()) {
		switch (status) {
			case IdleState:
				if (serialCommunication.isStartStream()) {
					// Checking that we got the correct point dimension
					if (serialCommunication.pointDimension() != SequencePoint::dim) {
						serialCommunication.sendDebugPacket("Invalid point dimension");
					} else {
						status = StreamMode;
						sequenceBufferWasFull = false;
						serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());
					}
				} else if (serialCommunication.isStartImmediate()) {
					// Checking that we got the correct point dimension
					if (serialCommunication.pointDimension() != SequencePoint::dim) {
						serialCommunication.sendDebugPacket("Invalid point dimension");
					} else {
						status = ImmediateMode;
						sequenceBufferWasFull = false;
						serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());
					}
				} else {
					serialCommunication.sendDebugPacket("Unexpected command");
				}
				break;
			case StreamMode:
				if (serialCommunication.isSequencePoint()) {
					// If the queue was full, sending a debug packet
					if (serialCommunication.nextSequencePointToFill() == NULL) {
						serialCommunication.sendDebugPacket("Sequence point received but buffer full");
					} else {
						// Marking the point as complete
						sequencePlayer.pointFilled();

						// Checking what to send in response and setting the next object
						// to fill
						sequenceBufferWasFull = sequencePlayer.bufferFull();
						if (sequenceBufferWasFull) {
							serialCommunication.setNextSequencePointToFill(NULL);
							serialCommunication.sendBufferFull();
						} else {
							serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());
							serialCommunication.sendBufferNotFull();
						}
					}
				} else if (serialCommunication.isStop()) {
					// Setting status to stopping. We still have to play all remaining sequence points
					status = StreamModeStopping;
				} else {
					serialCommunication.sendDebugPacket("Unexpected command");
				}
				break;
			case StreamModeStopping:
				// We do not expect any packet here
				serialCommunication.sendDebugPacket("Unexpected command (stopping)");
				break;
			case ImmediateMode:
				if (serialCommunication.isSequencePoint()) {
					// If the queue was full, sending a debug packet
					if (serialCommunication.nextSequencePointToFill() == NULL) {
						serialCommunication.sendDebugPacket("Sequence point received but buffer full");
					} else {
						// Setting both sequence point duration and timeToTarget to 0, so that the new
						// position is immediately reached
						serialCommunication.nextSequencePointToFill()->duration = 0;
						serialCommunication.nextSequencePointToFill()->timeToTarget = 0;

						// Marking the point as complete
						sequencePlayer.pointFilled();
						serialCommunication.setNextSequencePointToFill(sequencePlayer.pointToFill());
					}
				} else if (serialCommunication.isStop()) {
					// Clearing the sequence player buffer and returning idle
					sequencePlayer.clearBuffer();
					status = IdleState;
				} else {
					serialCommunication.sendDebugPacket("Unexpected command");
				}
				break;
		}
	}

	// Checking if we have to send the battery level
	const unsigned long curBatteryTime = millis();
	if ((curBatteryTime - lastBatteryTime) > batteryInterval) {
		// Sending battery charge. 420 = 100% - 300 = 0%
		int v = (analogRead(batteryPin) - 300) * 256 / (420 - 300);
		if (v < 0) {
			v = 0;
		} else if (v > 255) {
			v = 255;
		}
		serialCommunication.sendBatteryCharge(v);

		lastBatteryTime = curBatteryTime;
	}
}
