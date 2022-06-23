// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QML_H
#define QML_H

#include <qhealthdata.h>
#include <qhealthgeodata.h>
#include <qhealthprovider.h>
#include <qhealthquery.h>
#include <qhealthvalue.h>
#include <qhealthvaluedata.h>
#include <qhealthworkout.h>

#include <QObject>
#include <QQmlEngine>

struct QHealthProviderForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthProvider)
  QML_NAMED_ELEMENT(HealthProvider)
};

struct QHealthDataForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthData)
  QML_NAMED_ELEMENT(HealthData)
  QML_UNCREATABLE(
      "You cannot create health data this way. Please use one of its "
      "subclasses.")
};

struct QHealthQueryForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthQuery)
  QML_NAMED_ELEMENT(HealthQuery)
};

struct QHealthWorkoutForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthWorkout)
  QML_NAMED_ELEMENT(HealthWorkout)
};

struct QHealthValueForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthValue)
  QML_NAMED_ELEMENT(HealthValue)
};

struct QHealthValueDataForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthValueData)
  QML_NAMED_ELEMENT(HealthValueData)
};

struct QHealthGeoDataForeign : public QObject {
  Q_OBJECT
  QML_FOREIGN(QHealthGeoData)
  QML_NAMED_ELEMENT(HealthGeoData)
};

#endif
