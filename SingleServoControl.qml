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

	// Only enabled if there is a valid point
	enabled: (sequence.curPoint >= 0)

	// The id of the servo controlled by this item. You MUST set this to a
	// valid value (greater or equal to 0)
	property int servoID: -1

	// Thid object is only needed to hide some properties from the extern
	QtObject {
		id: internal

		// The mininmum value of the servo range
		property real rangeMin: 0

		// The maxinmum value of the servo range
		property real rangeMax: 100
	}

	// The arrangement of the slider and spinbox. This must be either
	// Qt.Horizontal or Qt.Vertical
	property int orientation: Qt.Vertical

	GridLayout {
		anchors.fill: parent

		// Rows and columns depend on the orientatation
		rows: (mainItem.orientation == Qt.Horizontal) ? 1 : 2
		columns:  (mainItem.orientation == Qt.Horizontal) ? 2 : 1

		Slider {
			id: slider
			Layout.fillWidth: (mainItem.orientation == Qt.Horizontal) ? false : true

			minimumValue: internal.rangeMin
			maximumValue: internal.rangeMax
			orientation: Qt.Horizontal
			updateValueWhileDragging: true

			onValueChanged: {
				textInput.text = value

				// Here we also update the value in the sequence
				// if we have a valid servoID
				if ((mainItem.servoID >= 0) && (mainItem.servoID < sequence.pointDim)) {
					sequence.setPointCoordinate(servoID, value)
				}
			}
		}

		TextField {
			id: textInput
			Layout.fillWidth: (mainItem.orientation == Qt.Horizontal) ? false : true

			validator: DoubleValidator {
				bottom: internal.rangeMin
				top: internal.rangeMax
			}

			onTextChanged: {
				if (acceptableInput) {
					slider.value = parseFloat(text)
				}
			}
		}
	}

	Component.onCompleted: {
		// Initialize the text with the value in the slider. This is
		// needed to avoid that the text area remains blank
		textInput.text = slider.value;

		// Checking that our id is valid
		if ((mainItem.servoID < 0) || (mainItem.servoID >= sequence.pointDim)) {
			return;
		}

		// Setting limits to sliders and spinbox and getting value from
		// sequence
		reconnectAndSetData();

		// Connecting the signal emitted on sequence change
		onSequenceChanged.connect(reconnectAndSetData);
	}

	// Connects signals of the sequence and the calls fixLimitsAndValues
	function reconnectAndSetData()
	{
		// Fix limits and values explicitly
		fixLimitsAndValue()

		// Connecting the signal emitted on cur point change and cur
		// point values change to the function to fix limits and and
		// re-read value for the sequence
		sequence.onCurPointChanged.connect(fixLimitsAndValue);
		sequence.onCurPointValuesChanged.connect(fixLimitsAndValue);
	}

	// Sets the limits and value to the ones for the current point
	function fixLimitsAndValue()
	{
		// Checking that our id is valid
		if ((mainItem.servoID < 0) || (mainItem.servoID >= sequence.pointDim)) {
			return;
		}

		// Getting the value before changing limits, because changing
		// limits may modify the value
		var value = sequence.pointCoordinate(mainItem.servoID);

		// Now setting limits
		internal.rangeMin = sequence.minPointCoordinate(mainItem.servoID)
		internal.rangeMax = sequence.maxPointCoordinate(mainItem.servoID)

		// Finally setting value. We only set the value of the slider,
		// this will automatically change the value of the text field
		slider.value = value;
	}
}

