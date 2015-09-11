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

#ifndef SEQUENCEPOINT_H
#define SEQUENCEPOINT_H

#include <QVector>
#include <QJsonObject>

/**
 * \brief A single point in a sequence
 *
 * This structure models a single point in a sequence
 */
struct SequencePoint
{
	/**
	 * \brief Constructor
	 */
	SequencePoint() = default;

	/**
	 * \brief Constructor
	 *
	 * \param p the point
	 * \param d the duration in milliseconds
	 * \param t the time to reach this point in milliseconds
	 */
	SequencePoint(QVector<double> p, int d, int t);

	/**
	 * \brief Initializes this object from its JSON representation
	 *
	 * \param json the JSON object to read
	 * \return false in case of error
	 */
	bool fromJson(const QJsonObject& json);

	/**
	 * \brief Returns the JSON representation of this object
	 *
	 * \return The JSON representation of this object
	 */
	QJsonObject toJson() const;

	/**
	 * \brief The point
	 */
	QVector<double> point;

	/**
	 * \brief For how long the step should last in milliseconds
	 *
	 * The point should not be changed for this amount of milliseconds
	 */
	int duration;

	/**
	 * \brief The time spent to reach this point from the previous position
	 *        in milliseconds
	 */
	int timeToTarget;
};

#endif // SEQUENCEPOINT_H

