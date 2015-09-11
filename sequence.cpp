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

#include "sequence.h"
#include <QFile>
#include <QJsonArray>

Sequence::Sequence(unsigned int pointDim, SequencePoint minVals, SequencePoint maxVals, QObject* parent)
	: QObject(parent)
	, m_pointDim(pointDim)
	, m_min(validatePoint(minVals, true))
	, m_max(validatePoint(maxVals, true))
	, m_sequence()
	, m_curPoint(-1)
{
}

void Sequence::setCurPoint(int p)
{
	if (m_sequence.isEmpty()) {
		return;
	}

	if (p < 0) {
		p = 0;
	} else if (p >= m_sequence.length()) {
		p = m_sequence.length() - 1;
	}

	if (p != m_curPoint) {
		m_curPoint = p;

		emit curPointChanged();
	}
}

std::unique_ptr<Sequence> Sequence::load(QString filename)
{
	QFile f(filename);

	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return std::make_unique<Sequence>();
	}

	return load(QJsonDocument::fromJson(f.readAll()));
}

std::unique_ptr<Sequence> Sequence::load(const QJsonDocument& json)
{
	// Here we expect an array of SequencePoints

	if (!json.isArray()) {
		return std::make_unique<Sequence>();
	}

	QList<SequencePoint> list;
	SequencePoint sp;
	// The dimension of the points, to check all points have the same
	// dimension
	int dim = -1;
	bool error = false;
	for (auto jsp: json.array()) {
		if (!jsp.isObject()) {
			error = true;
			break;
		}
		if (!sp.fromJson(jsp.toObject())) {
			error = true;
			break;
		}
		list.append(sp);
		if ((dim != -1) && (dim != sp.point.length())) {
			error = true;
			break;
		}
		dim = sp.point.length();
	}

	if (error) {
		return std::make_unique<Sequence>();
	}

	// If we get here, loading was successful, we can create the Sequence
	// object
	std::unique_ptr<Sequence> s = std::make_unique<Sequence>(dim);
	s->m_sequence = list;
	if (!list.isEmpty()) {
		s->m_curPoint = 0;
	}

	return s;
}

bool Sequence::save(QString filename) const
{
	if (!isValid()) {
		return false;
	}

	QJsonDocument doc = save();

	QFile f(filename);

	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	f.write(doc.toJson());

	if (f.error() != QFileDevice::NoError) {
		return false;
	}

	return true;
}

QJsonDocument Sequence::save() const
{
	if (!isValid()) {
		return QJsonDocument();
	}

	QJsonArray s;
	for (auto sp: m_sequence) {
		s.append(sp.toJson());
	}

	return QJsonDocument(s);
}

void Sequence::insertAfterCurrent()
{
	if (!isValid()) {
		return;
	}

	if (m_curPoint == -1) {
		m_sequence.append(SequencePoint());
	} else {
		m_sequence.insert(m_curPoint + 1, validatePoint(m_sequence[m_curPoint]));
	}

	emit numPointsChanged();

	++m_curPoint;
	emit curPointChanged();
}

void Sequence::insertBeforeCurrent()
{
	if (!isValid()) {
		return;
	}

	if (m_curPoint == -1) {
		m_sequence.append(SequencePoint());
	} else {
		m_sequence.insert(m_curPoint, validatePoint(m_sequence[m_curPoint]));
	}

	emit numPointsChanged();

	// Here we emit the curPointValuesChanged() signal even if the values
	// are the same because conceptually the index of the current point
	// didn't change but the current point did change
	emit curPointValuesChanged();
}

void Sequence::append()
{
	if (!isValid()) {
		return;
	}

	SequencePoint p = (m_curPoint == -1) ? SequencePoint() : m_sequence[m_curPoint];
	m_sequence.append(validatePoint(p));

	emit numPointsChanged();

	m_curPoint = m_sequence.length() - 1;
	emit curPointChanged();
}

void Sequence::removeCurrent()
{
	if (!isValid() || (m_curPoint == -1)) {
		return;
	}

	m_sequence.removeAt(m_curPoint);

	emit numPointsChanged();

	if (m_curPoint >= m_sequence.length()) {
		// This will set cur point to -1 if the sequence is empty
		m_curPoint = m_sequence.length() - 1;

		emit curPointChanged();
	} else {
		// Here we emit the curPointValuesChanged() signal because the
		// index of the current point didn't change but values did
		emit curPointValuesChanged();
	}
}

void Sequence::clear()
{
	if (!isValid()) {
		return;
	}

	if (!m_sequence.isEmpty()) {
		m_sequence.clear();

		emit numPointsChanged();

		m_curPoint = -1;
		emit curPointChanged();
	}
}

const SequencePoint& Sequence::min() const
{
	return m_min;
}

double Sequence::minPointCoordinate(int c) const
{
	return m_min.point[c];
}

int Sequence::minPointDuration() const
{
	return m_min.duration;
}

int Sequence::minPointTimeToTarget() const
{
	return m_min.timeToTarget;
}

const SequencePoint& Sequence::max() const
{
	return m_max;
}

double Sequence::maxPointCoordinate(int c) const
{
	return m_max.point[c];
}

int Sequence::maxPointDuration() const
{
	return m_max.duration;
}

int Sequence::maxPointTimeToTarget() const
{
	return m_max.timeToTarget;
}

const SequencePoint& Sequence::operator[](int pos) const
{
	return m_sequence[pos];
}

const SequencePoint& Sequence::point() const
{
	return m_sequence[m_curPoint];
}

double Sequence::pointCoordinate(int pos, int c) const
{
	return m_sequence[pos].point[c];
}

double Sequence::pointCoordinate(int c) const
{
	if (m_curPoint < 0) {
		return 0.0;
	}

	return pointCoordinate(m_curPoint, c);
}

int Sequence::pointDuration(int pos) const
{
	return m_sequence[pos].duration;
}

int Sequence::pointDuration() const
{
	if (m_curPoint < 0) {
		return 0;
	}

	return pointDuration(m_curPoint);
}

int Sequence::pointTimeToTarget(int pos) const
{
	return m_sequence[pos].timeToTarget;
}

int Sequence::pointTimeToTarget() const
{
	if (m_curPoint < 0) {
		return 0;
	}

	return pointTimeToTarget(m_curPoint);
}

void Sequence::setPoint(int pos, SequencePoint p)
{
	if (!isValid()) {
		return;
	}

	// Forcing point to be compliant with the set dimension and limits
	m_sequence[pos] = validatePoint(p);

	emit pointValuesChanged(pos);

	// Also checking if we have to emit the signal for changes in the
	// current point
	if (pos == m_curPoint) {
		emit curPointValuesChanged();
	}
}

void Sequence::setPoint(SequencePoint p)
{
	if (m_curPoint < 0) {
		return;
	}

	setPoint(m_curPoint, std::move(p));
}

void Sequence::setPointCoordinate(int pos, int c, double v)
{
	if (!isValid()) {
		return;
	}

	m_sequence[pos].point[c] = std::min(m_max.point[c], std::max(m_min.point[c], v));

	emit pointValuesChanged(pos);

	// Also checking if we have to emit the signal for changes in the
	// current point
	if (pos == m_curPoint) {
		emit curPointValuesChanged();
	}
}

void Sequence::setPointCoordinate(int c, double v)
{
	if (m_curPoint < 0) {
		return;
	}

	setPointCoordinate(m_curPoint, c, v);
}

void Sequence::setDuration(int pos, int d)
{
	if (!isValid()) {
		return;
	}

	m_sequence[pos].duration = std::min(m_max.duration, std::max(m_min.duration, d));

	emit pointValuesChanged(pos);

	// Also checking if we have to emit the signal for changes in the
	// current point
	if (pos == m_curPoint) {
		emit curPointValuesChanged();
	}
}

void Sequence::setDuration(int d)
{
	if (m_curPoint < 0) {
		return;
	}

	setDuration(m_curPoint, d);
}

void Sequence::setTimeToTarget(int pos, int t)
{
	if (!isValid()) {
		return;
	}

	m_sequence[pos].timeToTarget = std::min(m_max.timeToTarget, std::max(m_min.timeToTarget, t));

	emit pointValuesChanged(pos);

	// Also checking if we have to emit the signal for changes in the
	// current point
	if (pos == m_curPoint) {
		emit curPointValuesChanged();
	}
}

void Sequence::setTimeToTarget(int t)
{
	if (m_curPoint < 0) {
		return;
	}

	setTimeToTarget(m_curPoint, t);
}

SequencePoint Sequence::validatePoint(SequencePoint p, bool skipLimits) const
{
	// Resizing to the correct size
	if (p.point.length() < int(m_pointDim)) {
		// Filling the missing dimensions with 0.0
		while (p.point.length() < int(m_pointDim)) {
			p.point.append(0.0);
		};
	} else if (p.point.length() > int(m_pointDim)) {
		p.point.resize(m_pointDim);
	}

	if (skipLimits) {
		return p;
	}

	// Now checking all limits
	for (int i = 0; i < p.point.length(); ++i) {
		p.point[i] = std::min(m_max.point[i], std::max(m_min.point[i], p.point[i]));
	}
	p.duration = std::min(m_max.duration, std::max(m_min.duration, p.duration));
	p.timeToTarget = std::min(m_max.timeToTarget, std::max(m_min.timeToTarget, p.timeToTarget));

	return p;
}
