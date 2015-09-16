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
#include "utils.h"
#include "sequence.h"
#include "serialcommunication.h"

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
	Q_PROPERTY(SerialCommunication* serialCommunication READ serialCommunication NOTIFY serialCommunicationChanged)

public:
	explicit Sequencer(QObject *parent = 0);

	Sequence* sequence()
	{
		return m_sequence.get();
	}

	SerialCommunication* serialCommunication()
	{
		return m_serialCommunication.get();
	}

signals:
	void sequenceChanged();
	void serialCommunicationChanged();

public slots:
	void newSequence();
	bool saveSequence(QString filename);
	bool loadSequence(QString filename);
	bool fileExists(QString filename); // utility

private:
	std::unique_ptr<Sequence> m_sequence;
	std::unique_ptr<SerialCommunication> m_serialCommunication;
};

#endif // SEQUENCER_H
