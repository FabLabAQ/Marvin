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
#include "sequence.h"
#include "tutils.h"

// NOTES AND TODOS
//
// Add JSON conversion

/**
 * \brief The class to perform unit tests
 *
 * Each private slot is a test
 */
class TestSequence : public QObject
{
	Q_OBJECT

private slots:
	void emptySequence()
	{
		Sequence<3> sequence;

		QVERIFY(sequence.size() == 0);
	}

	void sequenceWithOnePoint()
	{
		Sequence<3> sequence;
		Sequence<3>::Point point;

		sequence.append(point);

		QVERIFY(sequence.size() == 1);
	}

	void initializeWithPoints()
	{
		Sequence<3> sequence{tutils::generatePoint<3>(0), tutils::generatePoint<3>(1)};

		QVERIFY(sequence.size() == 2);
	}

	void accessPoint()
	{
		using Point = Sequence<2>::Point;

		const Point p0 = tutils::generatePoint<2>(0);
		const Point p1 = tutils::generatePoint<2>(1);
		const Point p2 = tutils::generatePoint<2>(2);

		const Sequence<2> sequence{p0, p1, p2};

		QCOMPARE(sequence[0], p0);
		QCOMPARE(sequence[1], p1);
		QCOMPARE(sequence[2], p2);
	}

	void removePoint()
	{
		using Point = Sequence<3>::Point;

		const Point p0 = tutils::generatePoint<3>(0);
		const Point p1 = tutils::generatePoint<3>(1);
		const Point p2 = tutils::generatePoint<3>(2);

		Sequence<3> sequence{p0, p1, p2};

		sequence.remove(1);

		QCOMPARE(sequence[0], p0);
		QCOMPARE(sequence[1], p2);
	}
};

QTEST_MAIN(TestSequence)
#include "testsequence.moc"
