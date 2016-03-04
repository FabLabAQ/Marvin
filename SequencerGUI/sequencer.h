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

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QObject>
#include "utils.h"
#include "sequence.h"
#include "serialcommunication.h"

/**
 * \brief The main class of the applications
 *
 * This class is meant to be instantiated only once and to be used as the QML
 * context object. It contanins the instances of the current sequence and the
 * object used for serial communication (exposed as read-only properties). It
 * also has methods to load and save sequence files.
 */
class Sequencer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(Sequence* sequence READ sequence NOTIFY sequenceChanged)
	Q_PROPERTY(SerialCommunication* serialCommunication READ serialCommunication NOTIFY serialCommunicationChanged)

public:
	/**
	 * \brief Constructor
	 *
	 * \param parent the parent object
	 */
	explicit Sequencer(QObject *parent = 0);

	/**
	 * \brief Copy constructor is deleted
	 *
	 * \param other the object to copy
	 */
	Sequencer(const Sequencer& other) = delete;

	/**
	 * \brief Move constructor is deleted
	 *
	 * \param other the object to move into this
	 */
	Sequencer(Sequencer&& other) = delete;

	/**
	 * \brief Copy operator is deleted
	 */
	Sequencer& operator=(const Sequencer& other) = delete;

	/**
	 * \brief Move operator is deleted
	 */
	Sequencer& operator=(Sequencer&& other) = delete;

	/**
	 * \brief Destructor
	 *
	 * The default one is fine
	 */
	virtual ~Sequencer() = default;

	/**
	 * \brief Returns the current sequence
	 *
	 * \return the current sequence
	 */
	Sequence* sequence()
	{
		return m_sequence.get();
	}

	/**
	 * \brief Returns the object to use for serial communication
	 *
	 * \return the object to use for serial communication
	 */
	SerialCommunication* serialCommunication()
	{
		return m_serialCommunication.get();
	}

signals:
	/**
	 * \brief The signal emitted when the sequence changes
	 */
	void sequenceChanged();

	/**
	 * \brief The signal emitted when the object for serial communication
	 *        changes
	 *
	 * This signal is never emitted, but it is needed to avoid warnings from
	 * QML (because serialCommunication is used in property bindings)
	 */
	void serialCommunicationChanged();

public slots:
	/**
	 * \brief Creates a new sequence, discarding the old one
	 */
	void newSequence();

	/**
	 * \brief Saves the sequence to file
	 *
	 * \param filename the name of the file where the sequence is to be
	 *        saved
	 * \return true if saving was successful
	 */
	bool saveSequence(QString filename);

	/**
	 * \brief Loads a sequence file
	 *
	 * \param filename the name of the file to load
	 * \return true if loading was successful
	 */
	bool loadSequence(QString filename);

private:
	/**
	 * \brief The current sequence
	 */
	std::unique_ptr<Sequence> m_sequence;

	/**
	 * \brief The object for serial communication
	 */
	std::unique_ptr<SerialCommunication> m_serialCommunication;
};

#endif // SEQUENCER_H
