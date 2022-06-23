// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthworkout.h"

QHealthData::Type QHealthWorkout::dataType() const {
  return QHealthData::Workout;
}

QString QHealthWorkout::uuid() const { return m_uuid; }
void QHealthWorkout::setUuid(QString uuid) { m_uuid = uuid; }

bool QHealthWorkout::isValid() const {
  return m_type != QHealthWorkout::Invalid && startDate().isValid() &&
         endDate().isValid();
}

QHealthWorkout::Type QHealthWorkout::type() const { return m_type; }
void QHealthWorkout::setType(QHealthWorkout::Type type) { m_type = type; }

void *QHealthWorkout::nativeWorkout() const { return m_nativeWorkout; }
void QHealthWorkout::setNativeWorkout(void *nativeWorkout) {
  m_nativeWorkout = nativeWorkout;
}

QHealthValue *QHealthWorkout::duration() const { return m_duration; }
void QHealthWorkout::setDuration(QHealthValue *duration) {
  m_duration = duration;
  m_duration->setParent(this);
}

QHealthValue *QHealthWorkout::distance() const { return m_distance; }
void QHealthWorkout::setDistance(QHealthValue *distance) {
  m_distance = distance;
  m_distance->setParent(this);
}

QHealthValue *QHealthWorkout::energyBurned() const { return m_energyBurned; }
void QHealthWorkout::setEnergyBurned(QHealthValue *energyBurned) {
  m_energyBurned = energyBurned;
  m_energyBurned->setParent(this);
}
