// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthquery.h"

QHealthData::Type QHealthQuery::dataType() const { return m_dataType; }
void QHealthQuery::setDataType(QHealthData::Type dataType) {
  m_dataType = dataType;
}

QString QHealthQuery::uuid() const { return m_uuid; }
void QHealthQuery::setUuid(QString uuid) {
  m_uuid = uuid;
  ;
}

int QHealthQuery::limit() const { return m_limit; }
void QHealthQuery::setLimit(int limit) { m_limit = limit; }

QDateTime QHealthQuery::startDate() const { return m_startDate; }
void QHealthQuery::setStartDate(QDateTime startDate) {
  m_startDate = startDate;
}

QDateTime QHealthQuery::endDate() const { return m_endDate; }
void QHealthQuery::setEndDate(QDateTime endDate) { m_endDate = endDate; }

QHealthWorkout::Type QHealthQuery::workoutType() const { return m_workoutType; }
void QHealthQuery::setWorkoutType(QHealthWorkout::Type workoutType) {
  m_workoutType = workoutType;
}

const QHealthWorkout *QHealthQuery::workout() const { return m_workout; }
void QHealthQuery::setWorkout(const QHealthWorkout *workout) {
  m_workout = workout;
}

QHealthQuery::SortBy QHealthQuery::sortBy() const { return m_sortBy; }
void QHealthQuery::setSortBy(QHealthQuery::SortBy sortBy) { m_sortBy = sortBy; }

QHealthQuery::SortOrder QHealthQuery::sortOrder() const { return m_sortOrder; }
void QHealthQuery::setSortOrder(QHealthQuery::SortOrder sortOrder) {
  m_sortOrder = sortOrder;
}

bool QHealthQuery::isValid() const {
  return m_dataType != QHealthData::Invalid;
}
