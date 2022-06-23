// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHDATA_H
#define QHEALTHDATA_H

#include <QDateTime>
#include <QObject>
#include <QString>

class QHealthData : public QObject {
  Q_OBJECT
  Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate NOTIFY
                 startDateChanged)
  Q_PROPERTY(
      QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)

public:
  enum Type {
    Invalid = 0,
    Workout = 1,
    ValueData = 2,
    HeartData = 4,
    GeoData = 8
  };

  Q_ENUM(Type)
  Q_DECLARE_FLAGS(Types, Type)

  virtual QHealthData::Type dataType() const { return Invalid; }
  Q_INVOKABLE virtual QString uuid() const { return QString(); }
  virtual bool isValid() const { return false; }

  QDateTime startDate() const;
  void setStartDate(QDateTime startDate);

  QDateTime endDate() const;
  void setEndDate(QDateTime endDate);

signals:
  void startDateChanged();
  void endDateChanged();

private:
  QDateTime m_startDate, m_endDate;
};

Q_DECLARE_METATYPE(QHealthData)
Q_DECLARE_METATYPE(QHealthData::Type)

#endif // QHEALTHDATA_H
