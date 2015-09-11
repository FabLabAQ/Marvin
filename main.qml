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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

// The main window of the Application
ApplicationWindow {
	id: mainWindow
	title: qsTr("Sequencer GUI")
	width: 640
	height: 480
	visible: true

	menuBar: MenuBar {
		Menu {
			title: qsTr("&File")
			MenuItem {
				text: qsTr("&New sequence")
				onTriggered: messageDialog.show(qsTr("New action triggered"));
			}
			MenuItem {
				text: qsTr("&Open sequence")
				onTriggered: messageDialog.show(qsTr("Open action triggered"));
			}
			MenuItem {
				text: qsTr("&Save sequence")
				onTriggered: messageDialog.show(qsTr("Save action triggered"));
			}
			MenuItem {
				text: qsTr("S&ave sequence as...")
				onTriggered: messageDialog.show(qsTr("SaveAs action triggered"));
			}
			MenuItem {
				text: qsTr("E&xit")
				onTriggered: Qt.quit();
			}
		}
		Menu {
			title: qsTr("&Edit")
			MenuItem {
				text: qsTr("O&ptions")
				onTriggered: messageDialog.show(qsTr("Option action triggered"));
			}
		}
	}

	SplitView {
		anchors.fill: parent
		orientation: Qt.Horizontal

		SplitView {
			orientation: Qt.Vertical
			Layout.minimumWidth: Math.max(stepControl.implicitWidth, sequenceControl.implicitWidth)

			StepControl {
				id: stepControl
				Layout.minimumHeight: implicitHeight

				Layout.fillWidth: true
			}

			SequenceControl {
				id: sequenceControl
				Layout.minimumHeight: implicitHeight
			}
		}

		ServoControl {
			id: servoControl
			Layout.minimumWidth: mainWindow.width * 0.3

			Layout.fillWidth: true
		}
	}
}
