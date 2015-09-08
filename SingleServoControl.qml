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

// The item to control a single servo
Item {
	id: mainItem

	// The id of the servo controlled by this item. You MUST set this to a
	// valid value (greater or equal to 0)
	property int servoID: -1

	// The mininmum value of the servo range
	property real rangeMin: 0

	// The maxinmum value of the servo range
	property real rangeMax: 100

	// The arrangement of the slider and spinbox. This must be either
	// Qt.Horizontal or Qt.Vertical
	property int orientation: Qt.Horizontal

	GridLayout {
		// Rows and columns depend on the orientatation
		rows: (mainItem.orientation == Qt.Horizontal) ? 1 : 2
		columns:  (mainItem.orientation == Qt.Horizontal) ? 2 : 1

		Slider {
			controllare se si chiama così
		}

		SpinBox {
			per i double
		}
	}
}

