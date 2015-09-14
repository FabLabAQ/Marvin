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

			Layout.fillWidth: true
		}

		Button {
			text: "Play sequence from current step"

			Layout.fillWidth: true
		}

		Button {
			text: "Pause"

			Layout.fillWidth: true
		}

		Button {
			text: "Stop"

			Layout.fillWidth: true
		}

		CheckBox {
			text: "Immediate mode"

			Layout.fillWidth: true
		}

		Button {
			text: "Connect/Disconnect"

			Layout.fillWidth: true
		}

		Text {
			text: "Status"

			Layout.fillWidth: true
		}
	}
}

