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

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "sequencepoint.h"
#include <initializer_list>

/**
 * \brief The a list of SequencePoints
 *
 * If the point dimensionality is 0, the sequence is invalid
 */
template <std::size_t PointDimT>
class Sequence
{
public:
	/**
	 * \brief The dimension of points
	 */
	static constexpr auto pointDim = PointDimT;

	/**
	 * \brief The type of a point
	 */
	using Point = SequencePoint<pointDim>;

public:
	/**
	 * \brief Constructor. Builds an empty sequence
	 */
	Sequence();

	/**
	 * \brief Builds a list from an initializer list
	 *
	 * \param l the list from which we are initialized
	 */
	Sequence(const std::initializer_list<Point>& l);

	/**
	 * \brief Returns the number of points
	 *
	 * \return the number of points
	 */
	std::size_t size() const
	{
		return m_sequence.size();
	}

	/**
	 * \brief Appends a point to the sequence
	 *
	 * \param p the point to append
	 */
	void append(const Point& p);

	/**
	 * \brief Returns the i-th point (const version)
	 *
	 * \param i the index of the point to return
	 */
	const Point& operator[](int i) const
	{
		return m_sequence[i];
	}

	/**
	 * \brief Removes the point at position i
	 *
	 * \param i the position of the point to remove
	 */
	void remove(int i);

private:
	/**
	 * \brief The sequence of points
	 */
	QVector<Point> m_sequence;
};

// Expoting explicitly instantiated classes
extern template class SequencePoint<2>;
extern template class SequencePoint<3>;
extern template class SequencePoint<7>;

#endif // SEQUENCE_H
