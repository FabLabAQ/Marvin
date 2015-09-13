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

#include "sequencer.h"
#include <QFile>
#include <QUrl>
#include <QDebug>

// ONLY FOR TESTING, REMOVE!!!
namespace {
	const SequencePoint minPoint(QVector<double>() << -1.0 << 40.0 << -36.0, 3, 1);
	const SequencePoint maxPoint(QVector<double>() << 4.0 << 230.0 << 75.0, 3000, 10000);
}

Sequencer::Sequencer(QObject *parent)
	: QObject(parent)
	, m_sequence(std::make_unique<Sequence>(3, minPoint, maxPoint))
{
}

void Sequencer::newSequence()
{
	m_sequence = std::make_unique<Sequence>(3, minPoint, maxPoint);

	emit sequenceChanged();
}

bool Sequencer::saveSequence(QString filename)
{
	return m_sequence->save(QUrl(filename).toLocalFile());
}

bool Sequencer::loadSequence(QString filename)
{
	m_sequence = Sequence::load(QUrl(filename).toLocalFile());

	emit sequenceChanged();

	return m_sequence->isValid();
}

bool Sequencer::fileExists(QString filename)
{
	return QFile::exists(QUrl(filename).toLocalFile());
}
