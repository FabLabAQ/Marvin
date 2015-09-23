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
#include "sequenceplayer.h"
#include <stdlib.h>
// // import Wire library to use I²C
// #include <Wire.h>
// // import backpack library to use LED backpacks
// #include "Adafruit_LEDBackpack.h"
// // import GFX library to draw bitmaps on LED backpacks
// #include "Adafruit_GFX.h"

// The possible states
enum States {IdleState, StreamMode, ImmediateMode};

// The minimum and maximum PWM value of all servos
const unsigned int servoMin[SequencePoint::dim] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
const unsigned int servoMax[SequencePoint::dim] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};

// The current status
States status = IdleState;
// The baud rate to use for communication with computer
const long baudRate = 115200;
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
	startPos.point[0] = 127;
	startPos.point[1] = 127;
	startPos.point[2] = 127;
	startPos.point[3] = 127;
	startPos.point[4] = 127;
	startPos.point[5] = 127;
	startPos.point[6] = 127;
	startPos.point[7] = 127;
	startPos.point[8] = 127;
	startPos.point[9] = 127;
	startPos.point[10] = 127;
	startPos.point[11] = 127;
	startPos.point[12] = 127;
	startPos.point[13] = 127;
	startPos.point[14] = 127;
	startPos.point[15] = 127;

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
	sequencePlayer.step();

	// If the buffer was full and it is no longer full, sending a buffer not full package
	if ((status == StreamMode) && sequenceBufferWasFull && (!sequencePlayer.bufferFull())) {
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
					// Clearing the sequence player buffer and returning idle
					sequencePlayer.clearBuffer();
					status = IdleState;
				} else {
					serialCommunication.sendDebugPacket("Unexpected command");
				}
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
		// Sending battery charge
		serialCommunication.sendBatteryCharge(117);

		lastBatteryTime = curBatteryTime;
	}
}
