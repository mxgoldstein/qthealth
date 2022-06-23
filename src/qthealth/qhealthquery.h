// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHQUERY_H
#define QHEALTHQUERY_H

#include <qhealthdata.h>
#include <qhealthworkout.h>

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>

class QHealthQuery : public QObject {
  Q_OBJECT
public:
  enum Error {
    NoError = 0,
    APIError = 1,
    InvalidQuery = 2,
    UnsupportedDataType = 3,
    UnsupportedSortingType = 4,
    UnsupportedFilter = 5
  };
  Q_ENUM(Error)

  enum Limit { NoLimit = -1 };
  Q_ENUM(Limit);

  enum SortBy {
    Unsorted = 0,
    ByDate = 1,
    ByEnergyBurned = 2,
    ByDistance = 3,
    ByDuration = 4
  };
  Q_ENUM(SortBy);

  enum SortOrder { Ascending = 0, Descending = 1 };
  Q_ENUM(SortOrder);

  Q_PROPERTY(QHealthData::Type dataType READ dataType WRITE setDataType NOTIFY
                 dataTypeChanged)
  Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)

  Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
  Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate NOTIFY
                 startDateChanged)
  Q_PROPERTY(
      QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(QHealthWorkout::Type workoutType READ workoutType WRITE
                 setWorkoutType NOTIFY workoutTypeChanged)
  Q_PROPERTY(const QHealthWorkout *workout READ workout WRITE setWorkout NOTIFY
                 workoutChanged)
  Q_PROPERTY(QHealthQuery::SortBy sortBy READ sortBy WRITE setSortBy NOTIFY
                 sortByChanged)
  Q_PROPERTY(QHealthQuery::SortOrder sortOrder READ sortOrder WRITE setSortOrder
                 NOTIFY sortOrderChanged)

  QHealthQuery() = default;

  QHealthData::Type dataType() const;
  void setDataType(QHealthData::Type dataType);

  QString uuid() const;
  void setUuid(QString uuid);

  int limit() const;
  void setLimit(int limit);

  QDateTime startDate() const;
  void setStartDate(QDateTime startDate);

  QDateTime endDate() const;
  void setEndDate(QDateTime endDate);

  QHealthWorkout::Type workoutType() const;
  void setWorkoutType(QHealthWorkout::Type workoutType);

  const QHealthWorkout *workout() const;
  void setWorkout(const QHealthWorkout *workout);

  QHealthQuery::SortBy sortBy() const;
  void setSortBy(QHealthQuery::SortBy sortBy);

  QHealthQuery::SortOrder sortOrder() const;
  void setSortOrder(QHealthQuery::SortOrder sortOrder);

  bool isValid() const;

signals:
  void dataTypeChanged();
  void uuidChanged();
  void limitChanged();
  void startDateChanged();
  void endDateChanged();
  void workoutTypeChanged();
  void workoutChanged();
  void sortByChanged();
  void sortOrderChanged();

  void success(QList<QHealthData *> data);
  void failure(QHealthQuery::Error error, QString message);
  void aborted();

private:
  QHealthData::Type m_dataType = QHealthData::Invalid;
  QDateTime m_startDate, m_endDate;
  QString m_uuid;
  QHealthWorkout::Type m_workoutType = QHealthWorkout::Invalid;
  const QHealthWorkout *m_workout = nullptr;
  int m_limit = NoLimit;
  QHealthQuery::SortBy m_sortBy = Unsorted;
  QHealthQuery::SortOrder m_sortOrder = Ascending;
};

#endif // QHEALTHQUERY_H
