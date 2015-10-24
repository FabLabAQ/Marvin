/******************************************************************************
 * SequencerGUI                                                               *
 * Copyright (C) 2015                                                         *
 * Tomassino Ferrauto <t_ferrauto@yahoo.it>                                   *
 * Luca Anastasio <anastasio.lu@gmail.com>                                    *
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
#include "utils.h"

/**
 * \brief A single point in a sequence
 *
 * pointDimT is the dimensionality of points. Invalid points have negative
 * duration or timeToTarget
 */
template <std::size_t PointDimT>
struct SequencePoint
{
	/**
	 * \brief The dimensionality of points
	 */
	static constexpr auto pointDim = PointDimT;

	/**
	 * \brief The array type modelling a point
	 */
	using Array = std::array<double, pointDim>;

	/**
	 * \brief Constructor
	 */
	SequencePoint();

	/**
	 * \brief Constructor
	 *
	 * \param p the point
	 * \param d the duration in milliseconds
	 * \param t the time to reach this point in milliseconds
	 */
	SequencePoint(Array p, int d, int t);

	/**
	 * \brief Creates a sequence point from a JSON representation
	 *
	 * \param json the JSON object to read
	 * \return the new sequence point, which is invalid in case of errors
	 */
	static SequencePoint fromJson(const QJsonObject& json);

	/**
	 * \brief Returns the JSON representation of this object
	 *
	 * \return The JSON representation of this object
	 */
	QJsonObject toJson() const;

	/**
	 * \brief Returns true if the two points are equal
	 *
	 * \param other the other point to check
	 * \return true if the two points are equal
	 */
	bool operator==(const SequencePoint& other) const;

	/**
	 * \brief Returns true if the two points are different
	 *
	 * \param other the other point to check
	 * \return true if the two points are different
	 */
	bool operator!=(const SequencePoint& other) const;

	/**
	 * \brief Returns true if the point is valid
	 *
	 * \return true if the point is valid
	 */
	bool isValid() const
	{
		return (duration >= 0) && (timeToTarget >= 0);
	}

	/**
	 * \brief The point
	 */
	Array point;

	/**
	 * \brief For how long the step should last in milliseconds
	 *
	 * The position should not be changed for this amount of milliseconds
	 */
	int duration;

	/**
	 * \brief The time spent to reach this point from the previous position
	 *        in milliseconds
	 */
	int timeToTarget;
};

// Expoting explicitly instantiated classes
extern template class SequencePoint<2>;
extern template class SequencePoint<3>;
extern template class SequencePoint<7>;

#endif // SEQUENCEPOINT_H

