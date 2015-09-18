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

// The item containing controls for servo positioning
ScrollView {
	id: mainItem

	Image {
		//implicitHeight: mainLayout.implicitHeight + (2 * mainLayout.anchors.margins)
		//implicitWidth: mainLayout.implicitWidth + (2 * mainLayout.anchors.margins)
		source: "qrc:///robot.png"
		z: 0

        SingleServoControl {
            x: 366
            y: 370
            width: 100
            servoID: 0
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 366
            y: 270
            width: 100
            servoID: 1
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 366
            y: 150
            width: 100
            servoID: 2
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 266
            y: 150
            width: 100
            servoID: 3
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 250
            y: 400
            width: 100
            servoID: 4
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 260
            y: 530
            width: 100
            servoID: 5
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 260
            y: 670
            width: 100
            servoID: 6
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 260
            y: 800
            width: 100
            servoID: 7
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 110
            y: 800
            width: 100
            servoID: 8
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 110
            y: 670
            width: 100
            servoID: 9
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 110
            y: 530
            width: 100
            servoID: 10
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 120
            y: 400
            width: 100
            servoID: 11
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 100
            y: 150
            width: 100
            servoID: 12
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 0
            y: 150
            width: 100
            servoID: 13
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 0
            y: 270
            width: 100
            servoID: 14
            orientation: Qt.Vertical
            z: 10
        }

        SingleServoControl {
            x: 0
            y: 370
            width: 100
            servoID: 15
            orientation: Qt.Vertical
            z: 10
        }
	}
}
