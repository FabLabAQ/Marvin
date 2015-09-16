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

// Qt 5.4
//import QtQuick 2.4
//import QtQuick.Controls 1.3
//import QtQuick.Window 2.2
//import QtQuick.Dialogs 1.2
//import QtQuick.Layouts 1.1

// Qt 5.2
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

// A window to set configuration parameters. Parameters are changed as they are
// edited, not when the window is closed
Window {
	id: mainItem

	flags: Qt.Dialog
	modality: Qt.ApplicationModal

	ColumnLayout {
		anchors.fill: parent

		GridLayout {
			columns: 2
			Layout.fillHeight: true
			enabled: !serialCommunication.isStreaming

			Text {
				text: "Serial port:"
			}

			// This is the field to set the serial port name
			TextField {
				id: portField
				Layout.fillWidth: true

				text: serialCommunication.serialPortName

				onTextChanged: serialCommunication.serialPortName = text;
			}

			Text {
				text: "Baud Rate:"
			}

			// This is the field to set the baud rate
			TextField {
				id: baudRateField
				Layout.fillWidth: true

				validator: IntValidator {
					bottom: 0
					top: 1000000
				}

				text: serialCommunication.baudRate;

				onTextChanged: serialCommunication.baudRate = parseFloat(text)
			}
		}

		Button {
			id: closeButton
			Layout.alignment: Qt.AlignRight

			text: "Close"

			onClicked: mainItem.close();
		}
	}
}

