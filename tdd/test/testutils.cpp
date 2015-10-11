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

#include <QtTest/QtTest>
#include "utils.h"

/**
 * \brief The class to perform unit tests
 *
 * Each private slot is a test
 */
class TestUtils : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * \brief Tests that make_unique exists and works
	 */
	void makeUnique()
	{
		class Dummy {};

		std::unique_ptr<Dummy> d = std::make_unique<Dummy>();
	}
};

QTEST_MAIN(TestUtils)
#include "testutils.moc"
