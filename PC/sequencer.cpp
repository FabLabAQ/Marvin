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
	const SequencePoint minPoint(QVector<double>(16, 0), 3, 1);
	const SequencePoint maxPoint(QVector<double>(16, 255), 3000, 10000);
}

Sequencer::Sequencer(QObject *parent)
	: QObject(parent)
	, m_sequence(std::make_unique<Sequence>(16, minPoint, maxPoint))
	, m_serialCommunication(std::make_unique<SerialCommunication>())
{
}

void Sequencer::newSequence()
{
	m_sequence = std::make_unique<Sequence>(16, minPoint, maxPoint);

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
