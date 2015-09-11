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

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QObject>
#include <memory>
#include "sequence.h"

/**
 * \brief The class implementing a command sequencer
 *
 * This class is responsible of storing a sequence of position and of sending it
 * to the board which actually moves the robot
 * COMPLETE HERE
 * THIS SHOULD BE A QML SINGLETON
 */
class Sequencer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(Sequence* sequence READ sequence NOTIFY sequenceChanged)

public:
	explicit Sequencer(QObject *parent = 0);

	Sequence* sequence()
	{
		return m_sequence.get();
	}

signals:
	void sequenceChanged();

public slots:

private:
	std::unique_ptr<Sequence> m_sequence;
};

#endif // SEQUENCER_H
