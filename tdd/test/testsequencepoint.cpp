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

/**
 * \brief The class to perform unit tests
 *
 * Each private slot is a test
 */
class TestSequencePoint : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * \brief Check default point construction
	 */
	void defaultPointConstruction()
	{
		SequencePoint p;

		QCOMPARE(p.point.size(), 0);
		QCOMPARE(p.duration, 0);
		QCOMPARE(p.timeToTarget, 0);
	}

	/**
	 * \brief Check point construction
	 */
	void pointConstruction()
	{
		const QVector<double> p = QVector<double>() << 14.3 << 17.13 << 2234;
		const int d = 3424;
		const int t = 13443;
		SequencePoint point(p, d, t);

		QCOMPARE(point.point, p);
		QCOMPARE(point.duration, d);
		QCOMPARE(point.timeToTarget, t);
	}

	/**
	 * \brief Check equality works
	 */
	void pointEquality()
	{
		const QVector<double> p = QVector<double>() << 17.2 << 13.17 << 989.4;
		const int d = 745;
		const int t = 9934;
		SequencePoint point1(p, d, t);
		SequencePoint point2(p, d, t);

		QCOMPARE(point1, point2);
	}

	/**
	 * \brief Check dis-equality works
	 */
	void pointDisequality()
	{
		const QVector<double> pa = QVector<double>() << 17.2 << 13.17 << 989.4;
		const QVector<double> pb = QVector<double>() << 2.443 << 21.9 << 7;
		const int da = 745;
		const int db = 997;
		const int ta = 9934;
		const int tb = 7783;
		SequencePoint point1(pa, da, ta);
		SequencePoint point2(pa, db, tb);

		QVERIFY(point1 != point2);
	}

	/**
	 * \brief Check conversion to JSON
	 */
	void toJson()
	{
		const QVector<double> p = QVector<double>() << 17.2 << 989.4;
		const int d = 745;
		const int t = 9934;
		SequencePoint point(p, d, t);
		QJsonObject pointJsonObject = point.toJson();
		QJsonObject txtJsonObject = QJsonDocument::fromJson("{\"duration\":745, \"point\":[17.2,989.4], \"timeToTarget\":9934}").object();

		QCOMPARE(pointJsonObject, txtJsonObject);
	}

	/**
	 * \brief Check conversion from JSON is valid
	 */
	void fromJsonValid()
	{
		SequencePoint point;
		QJsonObject txtJsonObject = QJsonDocument::fromJson("{\"duration\":745, \"point\":[17.2,989.4], \"timeToTarget\":9934}").object();

		QVERIFY(point.fromJson(txtJsonObject));
	}

	/**
	 * \brief Check conversion from JSON
	 */
	void fromJson()
	{
		const QVector<double> p = QVector<double>() << 17.2 << 989.4;
		const int d = 745;
		const int t = 9934;
		SequencePoint point(p, d, t);
		SequencePoint pointFromJson;
		QJsonObject jsonObject = point.toJson();
		pointFromJson.fromJson(jsonObject);

		QCOMPARE(point, pointFromJson);
	}
};

QTEST_MAIN(TestSequencePoint)
#include "testsequencepoint.moc"
