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

// The item containing controls for step control (addition, removal,
// intervals...)
Item {
	id: mainItem
	implicitHeight: mainLayout.implicitHeight + (2 * mainLayout.anchors.margins)
	implicitWidth: mainLayout.implicitWidth + (2 * mainLayout.anchors.margins)

	// This is true when there is at least one step
	property bool stepsPresent: (sequence.curPoint >= 0)

	ColumnLayout {
		id: mainLayout
		anchors.fill: parent
		anchors.margins: 5

		RowLayout {
			enabled: mainItem.stepsPresent

			Text {
				text: "Current step:"
			}

			SpinBox {
				decimals: 0

				Layout.fillWidth: true
				value: sequence.curPoint
				minimumValue: 0
				maximumValue: (sequence.numPoints - 1)

				onValueChanged: sequence.setCurPoint(value)
			}
		}

		RowLayout {
			enabled: mainItem.stepsPresent

			Text {
				text: "Step duration (in ms):"
			}

			SpinBox {
				id: durationSpinBox

				decimals: 0

				Layout.fillWidth: true

				onValueChanged: sequence.setDuration(value)

				Component.onCompleted: {
					// Setting limits to sliders and spinbox and getting value from
					// sequence
					fixLimitsAndValue();

					// Connecting the signal emitted on sequence change, cur point
					// change and cur point values change to the function to fix
					// limits and and re-read value for the sequence
					onSequenceChanged.connect(fixLimitsAndValue);
					sequence.onCurPointChanged.connect(fixLimitsAndValue);
					sequence.onCurPointValuesChanged.connect(fixLimitsAndValue)
				}

				// Sets the limits and value to the ones for the current point
				function fixLimitsAndValue()
				{
					// Getting the value before changing limits, because changing
					// limits may modify the value
					var value = sequence.pointDuration();

					// Now setting limits
					durationSpinBox.minimumValue = sequence.minPointDuration()
					durationSpinBox.maximumValue = sequence.maxPointDuration()

					// Finally setting value. We only set the value of the slider,
					// this will automatically change the value of the text field
					durationSpinBox.value = value;
				}
			}
		}

		RowLayout {
			enabled: mainItem.stepsPresent

			Text {
				text: "Time to target (in ms):"
			}

			SpinBox {
				id: timeToTargetSpinBox
				decimals: 0

				Layout.fillWidth: true

				onValueChanged: sequence.setTimeToTarget(value)

				Component.onCompleted: {
					// Setting limits to sliders and spinbox and getting value from
					// sequence
					fixLimitsAndValue();

					// Connecting the signal emitted on sequence change, cur point
					// change and cur point values change to the function to fix
					// limits and and re-read value for the sequence
					onSequenceChanged.connect(fixLimitsAndValue);
					sequence.onCurPointChanged.connect(fixLimitsAndValue);
					sequence.onCurPointValuesChanged.connect(fixLimitsAndValue)
				}

				// Sets the limits and value to the ones for the current point
				function fixLimitsAndValue()
				{
					// Getting the value before changing limits, because changing
					// limits may modify the value
					var value = sequence.pointTimeToTarget();

					// Now setting limits
					timeToTargetSpinBox.minimumValue = sequence.minPointTimeToTarget()
					timeToTargetSpinBox.maximumValue = sequence.maxPointTimeToTarget()

					// Finally setting value. We only set the value of the slider,
					// this will automatically change the value of the text field
					timeToTargetSpinBox.value = value;
				}
			}
		}

		Button {
			text: "Insert step after current"

			Layout.fillWidth: true

			onClicked: sequence.insertAfterCurrent();
		}

		Button {
			text: "Insert step before current"

			Layout.fillWidth: true

			onClicked: sequence.insertBeforeCurrent()
		}

		Button {
			text: "Append step"

			Layout.fillWidth: true

			onClicked: sequence.append()
		}

		Button {
			enabled: mainItem.stepsPresent

			text: "Remove current step"

			Layout.fillWidth: true

			onClicked: sequence.removeCurrent()
		}

		Button {
			enabled: mainItem.stepsPresent

			text: "Remove all steps"

			Layout.fillWidth: true

			onClicked: sequence.clear()
		}
	}
}
