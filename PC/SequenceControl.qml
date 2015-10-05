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

// Qt 5.4
//import QtQuick 2.4
//import QtQuick.Controls 1.3
//import QtQuick.Layouts 1.1

// Qt 5.2
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

// The items to control the sequence execution (pause, stop, immediate, ...)
Item {
	id: mainItem
	implicitHeight: mainLayout.implicitHeight + (2 * mainLayout.anchors.margins)
	implicitWidth: mainLayout.implicitWidth + (2 * mainLayout.anchors.margins)

	ColumnLayout {
		id: mainLayout
		anchors.fill: parent
		anchors.margins: 5

		Button {
			text: "Play sequence from start"
			enabled: serialCommunication.isConnected && (!serialCommunication.isStreaming)

			Layout.fillWidth: true

			onClicked: serialCommunication.startStream(sequence, false);
		}

		Button {
			text: "Play sequence from current step"
			enabled: serialCommunication.isConnected && (!serialCommunication.isStreaming)

			Layout.fillWidth: true

			onClicked: serialCommunication.startStream(sequence, true);
		}

		Button {
			text: serialCommunication.isPaused ? "Resume" : "Pause"
			enabled: serialCommunication.isStreamMode

			Layout.fillWidth: true

			onClicked: {
				if (serialCommunication.isPaused) {
					serialCommunication.resumeStream();
				} else {
					serialCommunication.pauseStream();
				}
			}
		}

		Button {
			text: "Stop"
			enabled: serialCommunication.isStreamMode

			Layout.fillWidth: true

			onClicked: serialCommunication.stop()
		}

		CheckBox {
			text: "Immediate mode"
			enabled: serialCommunication.isConnected && (!serialCommunication.isStreamMode)

			Layout.fillWidth: true

			onCheckedChanged: {
				if (checked) {
					serialCommunication.startImmediate(sequence);
				} else {
					serialCommunication.stop();
				}
			}
		}

		CheckBox {
			text: "Continuous stream"
			enabled: serialCommunication.isConnected && (!serialCommunication.isImmediateMode)
			checked: !serialCommunication.oneShotSequence

			Layout.fillWidth: true

			onCheckedChanged: serialCommunication.oneShotSequence = !checked
		}

		Button {
			text: serialCommunication.isConnected ? "Disconnect" : "Connect"
			enabled: !serialCommunication.isStreaming

			Layout.fillWidth: true

			onClicked: {
				if (serialCommunication.isConnected) {
					serialCommunication.closeSerial();
				} else {
					serialCommunication.openSerial();
				}
			}
		}

		Text {
			text: "No error"

			Layout.fillWidth: true

			Component.onCompleted: {
				serialCommunication.streamError.connect(writeErrorMessage)
			}

			function writeErrorMessage(message)
			{
				text = "Last error: " + message
			}
		}

		Text {
			text: "No debug message"

			Layout.fillWidth: true

			Component.onCompleted: {
				serialCommunication.debugMessage.connect(writeDebugMessage)
			}

			function writeDebugMessage(message)
			{
				text = "Debug message: " + message
			}
		}

		Text {
			text: "Battery charge: " + ((serialCommunication.batteryCharge < 0) ? "unknown" : (serialCommunication.batteryCharge.toFixed(1) + "%"))

			Layout.fillWidth: true
		}
	}
}

