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
#include "sequencepoint.h"
#include "tutils.h"

// NOTES AND TODOS
//
//

/**
 * \brief The class to perform unit tests
 *
 * Each private slot is a test
 */
class TestSequencePoint : public QObject
{
	Q_OBJECT

private slots:
	void defaultPointConstruction()
	{
		const SequencePoint<7> p;

		QCOMPARE(p.point.size(), static_cast<decltype(p.point.size())>(7));
		QCOMPARE(p.duration, 0);
		QCOMPARE(p.timeToTarget, 0);
	}

	void pointConstruction()
	{
		const auto p = tutils::generateArray<3>();
		const int d = 3424;
		const int t = 13443;
		SequencePoint<3> point(p, d, t);

		QCOMPARE(point.point, p);
		QCOMPARE(point.duration, d);
		QCOMPARE(point.timeToTarget, t);
	}

	void pointEquality()
	{
		const auto point1 = tutils::generatePoint<3>();
		const auto point2 = point1;

		QCOMPARE(point1, point2);
	}

	void pointDisequality()
	{
		const auto point1 = tutils::generatePoint<3>(0);
		const auto point2 = tutils::generatePoint<3>(1);

		QVERIFY(point1 != point2);
	}

	void toJson()
	{
		const typename SequencePoint<2>::Array p = {17.2, 989.4};
		const int d = 745;
		const int t = 9934;
		const SequencePoint<2> point(p, d, t);
		QJsonObject pointJsonObject = point.toJson();
		QJsonObject txtJsonObject = QJsonDocument::fromJson("{\"duration\":745, \"point\":[17.2,989.4], \"timeToTarget\":9934}").object();

		QCOMPARE(pointJsonObject, txtJsonObject);
	}

	void fromJsonValid()
	{
		QJsonObject txtJsonObject = QJsonDocument::fromJson("{\"duration\":745, \"point\":[17.2,989.4], \"timeToTarget\":9934}").object();

		QVERIFY((SequencePoint<2>::fromJson(txtJsonObject)).isValid());
	}

	void fromJson()
	{
		const auto point = tutils::generatePoint<2>();
		QJsonObject jsonObject = point.toJson();
		auto pointFromJson = SequencePoint<2>::fromJson(jsonObject);

		QCOMPARE(point, pointFromJson);
	}

	void fromInvalidJson()
	{
		QJsonObject txtJsonObject1 = QJsonDocument::fromJson("{\"duration\":-745, \"point\":[17.2,989.4], \"timeToTarget\":9934}").object();
		QJsonObject txtJsonObject2 = QJsonDocument::fromJson("{\"duration\":745, \"point\":\"ciao\", \"timeToTarget\":9934}").object();
		QJsonObject txtJsonObject3 = QJsonDocument::fromJson("{\"duration\":745, \"point\":[17.2,989.4]}").object();
		QJsonObject txtJsonObject4 = QJsonDocument::fromJson("{ALL INVALID").object();

		QVERIFY(!(SequencePoint<2>::fromJson(txtJsonObject1)).isValid());
		QVERIFY(!(SequencePoint<2>::fromJson(txtJsonObject2)).isValid());
		QVERIFY(!(SequencePoint<2>::fromJson(txtJsonObject3)).isValid());
		QVERIFY(!(SequencePoint<2>::fromJson(txtJsonObject4)).isValid());
	}

	void validAndInvalidPoints()
	{
		SequencePoint<7> p1;
		p1.duration = 10;
		p1.timeToTarget = 20;
		SequencePoint<7> p2;
		p2.duration = -10;
		p2.timeToTarget = 20;
		SequencePoint<7> p3;
		p3.duration = 10;
		p3.timeToTarget = -20;
		SequencePoint<7> p4;
		p4.duration = -10;
		p4.timeToTarget = -20;

		QVERIFY(p1.isValid());
		QVERIFY(!p2.isValid());
		QVERIFY(!p3.isValid());
		QVERIFY(!p4.isValid());
	}
};

QTEST_MAIN(TestSequencePoint)
#include "testsequencepoint.moc"
