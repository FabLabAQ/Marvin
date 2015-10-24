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

#include "sequencepoint.h"
#include <QJsonArray>

template <std::size_t PointDimT>
SequencePoint<PointDimT>::SequencePoint()
	: point()
	, duration(0)
	, timeToTarget(0)
{
}

template <std::size_t PointDimT>
SequencePoint<PointDimT>::SequencePoint(Array p, int d, int t)
	: point(p)
	, duration(d)
	, timeToTarget(t)
{
}

template <std::size_t PointDimT>
SequencePoint<PointDimT> SequencePoint<PointDimT>::fromJson(const QJsonObject& json)
{
	// Marking the point as invalid
	SequencePoint seqPoint;
	seqPoint.duration = -1;
	seqPoint.timeToTarget = -1;

	// Reading the three keys: point...
	QJsonValue p = json["point"];
	if (!p.isArray()) {
		return seqPoint;
	}
	auto jsonArray = p.toArray();
	if (jsonArray.count() != pointDim) {
		return seqPoint;
	}
	for (int i = 0; i < jsonArray.count(); ++i) {
		if (!jsonArray[i].isDouble()) {
			return seqPoint;
		}
		seqPoint.point[i] = jsonArray[i].toDouble();
	}

	// ... duration...
	QJsonValue d = json["duration"];
	if (!d.isDouble()) {
		return seqPoint;
	}
	seqPoint.duration = static_cast<int>(d.toDouble());

	// ... and timeToTarget.
	QJsonValue t = json["timeToTarget"];
	if (!t.isDouble()) {
		return seqPoint;
	}
	seqPoint.timeToTarget = static_cast<int>(t.toDouble());

	return seqPoint;
}

template <std::size_t PointDimT>
QJsonObject SequencePoint<PointDimT>::toJson() const
{
	QJsonObject o;

	// Adding the three keys: point...
	QJsonArray p;
	for (auto c: point) {
		p.append(c);
	}
	o.insert("point", p);

	// ... duration...
	o.insert("duration", duration);

	// and timeToTarget
	o.insert("timeToTarget", timeToTarget);

	return o;
}

template <std::size_t PointDimT>
bool SequencePoint<PointDimT>::operator==(const SequencePoint& other) const
{
	// We check point last because it is the most expensive check
	return (other.duration == duration) &&
	       (other.timeToTarget == timeToTarget) &&
	       (other.point == point);
}

template <std::size_t PointDimT>
bool SequencePoint<PointDimT>::operator!=(const SequencePoint& other) const
{
	// We check point last because it is the most expensive check
	return !(*this == other);
}

// Explicit instantiation of some templates
template class SequencePoint<2>;
template class SequencePoint<3>;
template class SequencePoint<7>;
