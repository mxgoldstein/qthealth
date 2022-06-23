// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthgeodata.h"

QHealthData::Type QHealthGeoData::dataType() const {
  return QHealthData::GeoData;
}

QString QHealthGeoData::uuid() const { return m_uuid; }
void QHealthGeoData::setUuid(QString uuid) { m_uuid = uuid; }

bool QHealthGeoData::isValid() const { return false; }

double QHealthGeoData::latitude() const { return m_latitude; }
void QHealthGeoData::setLatitude(double latitude) { m_latitude = latitude; }

double QHealthGeoData::longitude() const { return m_longitude; }
void QHealthGeoData::setLongitude(double longitude) { m_longitude = longitude; }

double QHealthGeoData::altitude() const { return m_altitude; }
void QHealthGeoData::setAltitude(double altitude) { m_altitude = altitude; }
