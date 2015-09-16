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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include "sequencer.h"
#include "sequence.h"
#include "serialcommunication.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// Registering the Sequence and SerialCommunication types to QML. It is not possible to create
	// these types directly from QML (but we don't need to)
	qmlRegisterType<Sequence>();
	qmlRegisterType<SerialCommunication>();

	Sequencer sequencer;

	QQmlApplicationEngine engine;

	// Setting the sequencer as the context object
	engine.rootContext()->setContextObject(&sequencer);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
