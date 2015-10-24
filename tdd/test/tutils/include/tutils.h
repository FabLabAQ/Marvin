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

#ifndef TUTILS_H
#define TUTILS_H

#include "sequencepoint.h"
#include "sequence.h"

/**
 * \brief A namespace with utilities for tests
 */
namespace tutils
{
	/**
	 * \brief A vector with values used to fill arrays returned by
	 *        generateArray
	 */
	extern const std::array<typename SequencePoint<1>::Array::value_type, 100> randomValues;

	/**
	 * \brief A vector with random values for duration
	 */
	extern const std::array<decltype(SequencePoint<1>::duration), 100> randomDurations;

	/**
	 * \brief A vector with random values for time to target
	 */
	extern const std::array<decltype(SequencePoint<1>::timeToTarget), 100> randomTimeToTargets;

	/**
	 * \brief Generates a random Array of values
	 *
	 * This only works for arrays with a number of elements strictly less
	 * than the elements of randomValues
	 * \param index the index of the array to generate. Points generated
	 *              with different are generally different (for values of
	 *              index not too big)
	 * \return the generated array of points
	 */
	template <std::size_t PointDimT>
	typename SequencePoint<PointDimT>::Array generateArray(int index = 0)
	{
		typename SequencePoint<PointDimT>::Array array;

		for (int i = 0; i < PointDimT; ++i) {
			const int j = (i + (PointDimT * index)) % randomValues.size();

			array[i] = randomValues[j];
		}

		return array;
	}

	/**
	 * \brief Generates a random point
	 *
	 * This only works for points with a dimension strictly less than the
	 * elements of randomValues, randomDurations and randomTimeToTargets
	 * \param index the index of the point to generate. Points generated
	 *              with different are generally different (for values of
	 *              index not too big)
	 * \return the generated point
	 */
	template <std::size_t PointDimT>
	SequencePoint<PointDimT> generatePoint(int index = 0)
	{
		SequencePoint<PointDimT> point;

		point.point = generateArray<PointDimT>(index);
		point.duration = randomDurations[index % randomDurations.size()];
		point.timeToTarget = randomTimeToTargets[index % randomTimeToTargets.size()];

		return point;
	}

	/**
	 * \brief Generates a random sequence
	 *
	 * This only works for sequences of points with a dimension strictly
	 * less than the elements of randomValues, randomDurations and
	 * randomTimeToTargets
	 * \param numElements the number of elements in the sequence
	 * \param index the index of the sequence to generate. Sequences
	 *              generated with different are generally different (for
	 *              values of index not too big)
	 * \return the generated sequence
	 */
	template <std::size_t PointDimT>
	Sequence<PointDimT> generateSequence(int numElements, int index = 0)
	{
		Sequence<PointDimT> sequence;

		for (int i = 0; i < numElements; ++i) {
			sequence.append(generatePoint<PointDimT>(index * 20));
		}

		return sequence;
	}
}

#endif
