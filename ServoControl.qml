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
import QtQuick.Layouts 1.1

// The item containing controls for servo positioning
ScrollView {
	id: mainItem

	Item {
		implicitHeight: mainLayout.implicitHeight + (2 * mainLayout.anchors.margins)
		implicitWidth: mainLayout.implicitWidth + (2 * mainLayout.anchors.margins)

		ColumnLayout {
			id: mainLayout
			anchors.fill: parent
			anchors.margins: 5

			SingleServoControl {
				width: 200
				height: 100
				servoID: 0
				orientation: Qt.Vertical
			}

			SingleServoControl {
				width: 200
				height: 100
				servoID: 1
				orientation: Qt.Vertical
			}

			SingleServoControl {
				width: 200
				height: 100
				servoID: 2
				orientation: Qt.Vertical
			}
		}
	}
}
