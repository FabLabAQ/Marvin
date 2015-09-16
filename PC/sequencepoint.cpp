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

#include "sequencepoint.h"
#include <QJsonArray>

SequencePoint::SequencePoint(QVector<double> p, int d, int t)
	: point(p)
	, duration(d)
	, timeToTarget(t)
{
}

bool SequencePoint::fromJson(const QJsonObject& json)
{
	point.clear();
	duration = -1;
	timeToTarget = -1;

	// Reading the three keys: point...
	QJsonValue p = json["point"];
	if (!p.isArray()) {
		return false;
	}
	for (auto v: p.toArray()) {
		if (!v.isDouble()) {
			return false;
		}
		point.append(v.toDouble());
	}

	// ... duration...
	QJsonValue d = json["duration"];
	if (!d.isDouble()) {
		return false;
	}
	duration = static_cast<int>(d.toDouble());

	// ... and timeToTarget.
	QJsonValue t = json["timeToTarget"];
	if (!t.isDouble()) {
		return false;
	}
	timeToTarget = static_cast<int>(t.toDouble());

	return true;
}

QJsonObject SequencePoint::toJson() const
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

bool SequencePoint::operator==(const SequencePoint& other) const
{
	// We check point last because it is the most expensive check
	return (other.duration == duration) &&
	       (other.timeToTarget == timeToTarget) &&
	       (other.point == point);
}
