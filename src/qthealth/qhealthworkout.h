// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHWORKOUT_H
#define QHEALTHWORKOUT_H

#include <QDateTime>
#include <QObject>

#include "qhealthdata.h"
#include "qhealthvalue.h"

class QHealthWorkout : public QHealthData {
  Q_OBJECT
public:
  Q_PROPERTY(
      QHealthWorkout::Type type READ type WRITE setType NOTIFY typeChanged)
  Q_PROPERTY(QHealthValue *duration READ duration WRITE setDuration NOTIFY
                 durationChanged)
  Q_PROPERTY(QHealthValue *distance READ distance WRITE setDistance NOTIFY
                 distanceChanged)
  Q_PROPERTY(QHealthValue *energyBurned READ energyBurned WRITE setEnergyBurned
                 NOTIFY energyBurnedChanged)

  enum Type {
    Invalid = 0,
    Unknown = 1,
    Running = 2,
    Walking = 3,
    Swimming = 4,
    Cycling = 5,
    CoreTraining = 6
  };
  Q_ENUM(Type);

  QHealthWorkout() = default;

  QHealthData::Type dataType() const override;
  bool isValid() const override;

  QString uuid() const override;
  void setUuid(QString uuid);

  QHealthWorkout::Type type() const;
  void setType(QHealthWorkout::Type type);

  QHealthValue *duration() const;
  void setDuration(QHealthValue *duration);

  QHealthValue *distance() const;
  void setDistance(QHealthValue *distance);

  QHealthValue *energyBurned() const;
  void setEnergyBurned(QHealthValue *energyBurned);

  void *nativeWorkout() const;
  void setNativeWorkout(void *nativeWorkout);
signals:
  void typeChanged();
  void startTimeChanged();
  void endTimeChanged();
  void durationChanged();
  void distanceChanged();
  void energyBurnedChanged();

private:
  QHealthWorkout::Type m_type = Invalid;
  QString m_uuid;
  QHealthValue *m_duration = nullptr;
  QHealthValue *m_distance = nullptr;
  QHealthValue *m_energyBurned = nullptr;
  void *m_nativeWorkout = nullptr;
};

#endif // QHEALTHWORKOUT_H
