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

// The main window of the Application
ApplicationWindow {
	id: mainWindow
	title: qsTr("Sequencer GUI")
    width: 730
    height: 900
	visible: true

	// This component keeps som internal variables
	QtObject {
		id: internal

		// The name of the last file that has been loaded
		property string filename: ""

		// Set to true when we should close without checking whether the
		// current sequence was modified
		property bool forceClose: false

		// The name of the file selected to save
		property string selectedFilenameToSave
	}

	menuBar: MenuBar {
		Menu {
			title: qsTr("&File")

			MenuItem {
				text: qsTr("&New sequence")

				onTriggered: {
					if (sequence.isModified) {
						// Asking the user for confirmation. The code in the
						// dialog will create a new sequence if needed
						newConfirmDialog.open();
					} else {
						newSequence();
					}
				}
			}
			MenuItem {
				text: qsTr("&Open sequence")

				onTriggered:  {
					if (sequence.isModified) {
						// Asking the user for confirmation. The code in the
						// dialog will create a new sequence if needed
						openConfirmDialog.open();
					} else {
						openSequenceDialog.open();
					}
				}
			}
			MenuItem {
				text: qsTr("&Save sequence")

				onTriggered: {
					if (internal.filename == "") {
						// Using the Save As.. dialog
						saveSequenceDialog.open();
					} else {
						saveSequence(internal.filename);
					}
				}
			}
			MenuItem {
				text: qsTr("S&ave sequence as...")

				onTriggered: saveSequenceDialog.open();
			}
			MenuItem {
				text: qsTr("E&xit")
				onTriggered: mainWindow.close();
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

	MessageDialog {
		id: confirmClosingDialog

		title: "Discard changes?"
		text: "The current sequence has unsaved modifications. Discard changes and exit?"
		standardButtons: StandardButton.Yes | StandardButton.No

		onYes: {
			internal.forceClose = true;
			mainWindow.close();
		}
	}

	MessageDialog {
		id: newConfirmDialog

		title: "Discard changes?"
		text: "The current sequence has unsaved modifications. Discard changes and create a new sequence?"
		standardButtons: StandardButton.Yes | StandardButton.No

		onYes: newSequence();
	}

	MessageDialog {
		id: openConfirmDialog

		title: "Discard changes?"
		text: "The current sequence has unsaved modifications. Discard changes and open a new sequence?"
		standardButtons: StandardButton.Yes | StandardButton.No

		onYes: openSequenceDialog.open();
	}

	FileDialog {
		id: openSequenceDialog
		title: "Open..."
		nameFilters: ["Sequence files (*.seq)"]
		selectExisting: true

		onAccepted: {
			internal.filename = openSequenceDialog.fileUrl
			loadSequence(openSequenceDialog.fileUrl);
		}
	}

	FileDialog {
		id: saveSequenceDialog
		title: "Save As..."
		nameFilters: ["Sequence files (*.seq)"]
		selectExisting: false

		onAccepted: {
			internal.filename = saveSequenceDialog.fileUrl;
			saveSequence(saveSequenceDialog.fileUrl);
		}
	}

	onClosing: {
		if (!internal.forceClose && sequence.isModified) {
			close.accepted = false;

			// Asking the user for confirmation. The code in the
			// dialog will create a new sequence if needed
			confirmClosingDialog.open();
		}
	}
}
