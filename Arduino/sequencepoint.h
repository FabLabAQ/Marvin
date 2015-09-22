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

/**
 * \brief A single point of the sequence
 */
struct SequencePoint
{
	/**
	 * \brief The dimension of points
	 */
	static const unsigned char dim = 16;

	/**
	 * \brief The point coordinates
	 */
	unsigned char point[dim];

	/**
	 * \brief The duration of the point in milliseconds
	 */
	unsigned int duration;

	/**
	 * \brief The time to reach this point in milliseconds
	 */
	unsigned int timeToTarget;
};

#endif
