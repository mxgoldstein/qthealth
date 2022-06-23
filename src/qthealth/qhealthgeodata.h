// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHGEODATA_H
#define QHEALTHGEODATA_H

#include <QObject>

#include "qhealthdata.h"

class QHealthGeoData : public QHealthData {
  Q_OBJECT
  Q_PROPERTY(
      double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
  Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY
                 longitudeChanged)
  Q_PROPERTY(
      double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
public:
  QHealthGeoData() = default;

  QHealthData::Type dataType() const override;

  QString uuid() const override;
  void setUuid(QString uuid);

  bool isValid() const override;

  double latitude() const;
  void setLatitude(double latitude);

  double longitude() const;
  void setLongitude(double longitude);

  double altitude() const;
  void setAltitude(double altitude);

signals:
  void latitudeChanged();
  void longitudeChanged();
  void altitudeChanged();

private:
  QString m_uuid;

  double m_latitude = -1, m_longitude = -1;

  double m_altitude = 0;
};

#endif // QHEALTHGEODATA_H
