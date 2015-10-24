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

#include "sequence.h"

template <std::size_t PointDimT>
Sequence<PointDimT>::Sequence()
	: m_sequence()
{
}

template <std::size_t PointDimT>
Sequence<PointDimT>::Sequence(const std::initializer_list<Point>& l)
	: m_sequence(l)
{
}

template <std::size_t PointDimT>
void Sequence<PointDimT>::append(const Point& p)
{
	m_sequence.append(p);
}

template <std::size_t PointDimT>
void Sequence<PointDimT>::remove(int i)
{
	m_sequence.remove(i);
}

// Explicit instantiation of some templates
template class Sequence<2>;
template class Sequence<3>;
template class Sequence<7>;
