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

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QList>
#include <QJsonDocument>
#include <memory>
#include "sequencepoint.h"

/**
 * \brief The class modelling a sequence of points
 *
 * This class stores a list of SequencePoints. It has methods to manage the
 * list, store to file and read it back. When constructed, you must specify the
 * dimensionality of points (i.e. how many values are there in a point) and the
 * minimum and maximum limit of values. These values cannot be changed. If a
 * SequencePoint having the wrong number of elements is passed to any function,
 * the point is truncated or filled with 0 to have the expected dimensionality.
 * Also the point is clamped to stay within the limits. This class also have a
 * notion of "current point". All functions that do not explicitly take the
 * position of the point in the sequence as parameter, act on the current point.
 * \note This class makes little checks on the validity of point positions, make
 *       sure you always use valid positions. The current point, instead, always
 *       have a valid value (if the sequence is empty, its value is -1) and is
 *       automatically changed when needed if the sequence is modified (e.g. if
 *       the current point is the last point and it is removed, the current
 *       point is set to the last element after removal)
 */
class Sequence : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool isValid READ isValid)
	Q_PROPERTY(unsigned int pointDim READ pointDim)
	Q_PROPERTY(int numPoints READ numPoints NOTIFY numPointsChanged)
	Q_PROPERTY(int curPoint READ curPoint WRITE setCurPoint NOTIFY curPointChanged)

public:
	/**
	 * \brief Constructor
	 *
	 * \param pointDim the dimensionality of points (how many values are
	 *                 there in a point). If 0 the sequence is invalid
	 * \param minVals the minimum allowed values for all elements of a point
	 *                (comprising timers)
	 * \param maxVals the maximum allowed values for all elements of a point
	 *                (comprising timers)
	 * \param parent the parent QObject
	 */
	Sequence(unsigned int pointDim = 0, SequencePoint minVals = SequencePoint(), SequencePoint maxVals = SequencePoint(), QObject* parent = nullptr);

	/**
	 * \brief Copy constructor is deleted
	 *
	 * \param other the object to copy
	 */
	Sequence(const Sequence& other) = delete;

	/**
	 * \brief Move constructor is deleted
	 *
	 * \param other the object to move into this
	 */
	Sequence(Sequence&& other) = delete;

	/**
	 * \brief Copy operator is deleted
	 */
	Sequence& operator=(const Sequence& other) = delete;

	/**
	 * \brief Move operator is deleted
	 */
	Sequence& operator=(Sequence&& other) = delete;

	/**
	 * \brief Returns true if the sequence is valid
	 *
	 * \return true if the sequence is valid
	 */
	bool isValid() const
	{
		return (m_pointDim > 0);
	}

	/**
	 * \brief Returns the dimensionality of points
	 *
	 * \return the dimensionality of points
	 */
	unsigned int pointDim() const
	{
		return m_pointDim;
	}

	/**
	 * \brief Returns the number of points in the sequence
	 *
	 * \return the number of points in the sequence
	 */
	int numPoints() const
	{
		return m_sequence.length();
	}

	/**
	 * \brief Returns the current point
	 *
	 * \return the current point
	 */
	int curPoint() const
	{
		return m_curPoint;
	}

	/**
	 * \brief Sets the current point
	 *
	 * \param p the new current point. If negative, the current point is set
	 *          to 0, if greater than the index of the last point, it is set
	 *          to the last point
	 */
	Q_INVOKABLE void setCurPoint(int p);

	/**
	 * \brief Loads a sequence from file
	 *
	 * This returns a unique_ptr (we cannot retutrn by value because we have
	 * no copy nor move constructor)
	 * \param filename the name of the file to read
	 * \return the loaded sequence. The sequence is not valid in case of
	 *         errors
	 */
	static std::unique_ptr<Sequence> load(QString filename);

	/**
	 * \brief Loads a sequence from its JSON representation
	 *
	 * This returns a unique_ptr (we cannot retutrn by value because we have
	 * no copy nor move constructor)
	 * \param json the json representation of the sequence
	 * \return the loaded sequence. The sequence is not valid in case of
	 *         errors
	 */
	static std::unique_ptr<Sequence> load(const QJsonDocument& json);

	/**
	 * \brief Saves the sequence to file
	 *
	 * \param filename the name of the file to which the sequence is saved
	 * \return false in case of error, true otherwise
	 */
	bool save(QString filename) const;

	/**
	 * \brief Saves the sequence to a JSON document
	 *
	 * \return the JSON document representing the sequence
	 */
	QJsonDocument save() const;

	/**
	 * \brief Inserts a new point after the current position, equal to the
	 *        current point
	 *
	 * The inserted point becomes the current point
	 */
	Q_INVOKABLE void insertAfterCurrent();

	/**
	 * \brief Inserts a new point before the current position, equal to the
	 *        current point
	 *
	 * The inserted point becomes the current point
	 */
	Q_INVOKABLE void insertBeforeCurrent();

	/**
	 * \brief Inserts a new point at the end of the sequence, equal to the
	 *        current point
	 *
	 * The inserted point becomes the current point
	 */
	Q_INVOKABLE void append();

	/**
	 * \brief Removes the point at the curret position
	 */
	Q_INVOKABLE void removeCurrent();

	/**
	 * \brief Removes all points
	 */
	Q_INVOKABLE void clear();

	/**
	 * \brief Returns the minimum values for points
	 *
	 * \return the minimum values for points
	 */
	const SequencePoint& min() const;

	/**
	 * \brief Returns the minimum allowed coordinate for a point
	 *
	 * \param c the index of the coordinate whose minimum to return
	 * \return the minimum allowed coordinate for a point
	 */
	Q_INVOKABLE double minPointCoordinate(int c) const;

	/**
	 * \brief Returns the minimum allowd value for the duration of a point
	 *
	 * \return the minimum allowd value for the duration of a point
	 */
	Q_INVOKABLE int minPointDuration() const;

	/**
	 * \brief Returns the minimum allowed value for the time to target of a
	 *        point
	 *
	 * \return the minimum allowed value for the time to target of a point
	 */
	Q_INVOKABLE int minPointTimeToTarget() const;

	/**
	 * \brief Returns the maximum values for points
	 *
	 * \return the maximum values for points
	 */
	const SequencePoint& max() const;

	/**
	 * \brief Returns the maximum allowed coordinate for a point
	 *
	 * \param c the index of the coordinate whose maximum to return
	 * \return the maximum allowed coordinate for a point
	 */
	Q_INVOKABLE double maxPointCoordinate(int c) const;

	/**
	 * \brief Returns the maximum allowd value for the duration of a point
	 *
	 * \return the maximum allowd value for the duration of a point
	 */
	Q_INVOKABLE int maxPointDuration() const;

	/**
	 * \brief Returns the maximum allowed value for the time to target of a
	 *        point
	 *
	 * \return the maximum allowed value for the time to target of a point
	 */
	Q_INVOKABLE int maxPointTimeToTarget() const;

	/**
	 * \brief Returns the point at the given position
	 *
	 * \param pos the position in the sequence of the point
	 * \return the point at the given position
	 */
	const SequencePoint& operator[](int pos) const;

	/**
	 * \brief Returns the current point
	 *
	 * \return the current point
	 * \warning This function does not check if the current point is -1,
	 *          only use it on a non-empty sequence!
	 */
	const SequencePoint& point() const;

	/**
	 * \brief Returns a coordinate of a point
	 *
	 * \param pos the position in the sequence of the point
	 * \param c the index of the coordinate to change
	 * \return the value of the given coordinate of the given point
	 */
	Q_INVOKABLE double pointCoordinate(int pos, int c) const;

	/**
	 * \brief Returns a coordinate of the current point
	 *
	 * \param c the index of the coordinate to change
	 * \return the value of the given coordinate of the current point
	 */
	Q_INVOKABLE double pointCoordinate(int c) const;

	/**
	 * \brief Returns the duration of the point
	 *
	 * \param pos the position in the sequence of the point
	 * \return the duration of the point
	 */
	Q_INVOKABLE int pointDuration(int pos) const;

	/**
	 * \brief Returns the duration of the current point
	 *
	 * \return the duration of the current point
	 */
	Q_INVOKABLE int pointDuration() const;

	/**
	 * \brief Returns the time to target of the point
	 *
	 * \param pos the position in the sequence of the point
	 * \return the time to target of the point
	 */
	Q_INVOKABLE int pointTimeToTarget(int pos) const;

	/**
	 * \brief Returns the time to target of the current point
	 *
	 * \return the time to target of the point
	 */
	Q_INVOKABLE int pointTimeToTarget() const;

	/**
	 * \brief Sets the point at the given position
	 *
	 * \param pos the position in the sequence to change
	 * \param p the new point
	 */
	Q_INVOKABLE void setPoint(int pos, SequencePoint p);

	/**
	 * \brief Sets the values of the current point
	 *
	 * \param p the new point
	 */
	Q_INVOKABLE void setPoint(SequencePoint p);

	/**
	 * \brief Changes a single coordinate of a point
	 *
	 * \param pos the position in the sequence of the point to change
	 * \param c the index of the coordinate to change
	 * \param v the value to set
	 */
	Q_INVOKABLE void setPointCoordinate(int pos, int c, double v);

	/**
	 * \brief Changes a single coordinate of the current point
	 *
	 * \param c the index of the coordinate to change
	 * \param v the value to set
	 */
	Q_INVOKABLE void setPointCoordinate(int c, double v);

	/**
	 * \brief Sets the duration of a point
	 *
	 * \param pos the position in the sequence of the point to change
	 * \param d the new duration in milliseconds
	 */
	Q_INVOKABLE void setDuration(int pos, int d);

	/**
	 * \brief Sets the duration of the current point
	 *
	 * \param d the new duration in milliseconds
	 */
	Q_INVOKABLE void setDuration(int d);

	/**
	 * \brief Sets the time to target of a point
	 *
	 * \param pos the position in the sequence of the point to change
	 * \param d the new time to target in milliseconds
	 */
	Q_INVOKABLE void setTimeToTarget(int pos, int t);

	/**
	 * \brief Sets the time to target of the current point
	 *
	 * \param d the new time to target in milliseconds
	 */
	Q_INVOKABLE void setTimeToTarget(int t);

signals:
	/**
	 * \brief The signal emitted when the number of points in the sequence
	 *        changes
	 */
	void numPointsChanged();

	/**
	 * \brief The signal emitted when the current point changes
	 */
	void curPointChanged();

	/**
	 * \brief The signal emitted when a point changes
	 *
	 * This is emitted whenever the point position, duration or time to
	 * target changes
	 * \param pos the position in the sequence of the point that changed
	 */
	void pointValuesChanged(int pos);

	/**
	 * \brief The signal emitted when one of the values of the current point
	 *        changes
	 *
	 * This is emitted whenever the point position, duration or time to
	 * target of the current point changes. If a point different from the
	 * current one is changed, this signal is not emitted
	 */
	void curPointValuesChanged();

private:
	/**
	 * \brief Validates a point eventually changing it so that it has the
	 *        correct number of coordinates and all values are within the
	 *        limits
	 *
	 * \param p the point to validate
	 * \param skipLimits if true values are not checked to be within limits
	 * \return the validated point
	 */
	SequencePoint validatePoint(SequencePoint p, bool skipLimits = false) const;

	/**
	 * \brief The dimensionality of points
	 *
	 * If 0 the sequence is invalid
	 */
	const unsigned int m_pointDim;

	/**
	 * \brief The minimum allowed values for all elements of a point
	 *        (comprising timers)
	 */
	const SequencePoint m_min;

	/**
	 * \brief The maximum allowed values for all elements of a point
	 *        (comprising timers)
	 */
	const SequencePoint m_max;

	/**
	 * \brief The sequence
	 */
	QList<SequencePoint> m_sequence;

	/**
	 * \brief The current point in the sequence
	 *
	 * This is -1 if the sequence is empty
	 */
	int m_curPoint;
};

#endif // SEQUENCE_H
