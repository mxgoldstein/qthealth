// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthvalue.h"

QHealthValue::QHealthValue(QHealthValue::Type type, double value,
                           QHealthValue::Unit unit) {
  setType(type);
  setValue(value, unit);
}

static double conversionFactor(QHealthValue::Type type,
                               QHealthValue::Unit unit) {
  switch (type) {
  case QHealthValue::InvalidType:
    return qQNaN();
  case QHealthValue::Distance:
    switch (unit) {
    case QHealthValue::Meters:
      return 1; // The base unit is meters
    case QHealthValue::Kilometers:
      return 1. / 1000;
    case QHealthValue::Feet:
      return 1 * 3.28084;
    case QHealthValue::Miles:
      return 1 * 0.0006213712;
    default:
      return qQNaN();
    }
    break;
  case QHealthValue::Duration:
    switch (unit) {
    case QHealthValue::Seconds:
      return 1; // The base unit is seconds
    case QHealthValue::Minutes:
      return 1. / 60;
    case QHealthValue::Hours:
      return 1. / 60 / 60;
    case QHealthValue::Days:
      return 1. / 60 / 60 / 24;
    case QHealthValue::Weeks:
      return 1. / 60 / 60 / 24 / 7;
    default:
      return qQNaN();
    }
    break;
  case QHealthValue::Speed:
    switch (unit) {
    case QHealthValue::MetersPerSecond:
      return 1; // The base unit is meters per second
    case QHealthValue::KilometersPerHour:
      return 3.6;
    case QHealthValue::MilesPerHour:
      return 2.236936;
    default:
      return qQNaN();
    }
  case QHealthValue::Heartbeat:
    switch (unit) {
    case QHealthValue::BeatsPerMinute:
      return 1; // The base unit is beats per minute
    default:
      return qQNaN();
    }
  case QHealthValue::Energy:
    switch (unit) {
    case QHealthValue::Joules:
      return 1; // The base unit is joules
    case QHealthValue::KiloCalories:
      return 0.0002390057;
    default:
      return qQNaN();
    }
  }
}

QHealthValue::Type QHealthValue::type() const { return m_type; }

void QHealthValue::setType(QHealthValue::Type type) {
  if (type != m_type)
    m_value = {};
  m_type = type;
}

double QHealthValue::value(QHealthValue::Unit unit) const {
  if (!m_value.has_value())
    return qQNaN();

  const double factor = conversionFactor(m_type, unit);

  if (factor == qQNaN())
    return qQNaN();

  return m_value.value() * factor;
}
void QHealthValue::setValue(double value, QHealthValue::Unit unit) {
  const double factor = conversionFactor(m_type, unit);

  if (factor == qQNaN())
    m_value = {};

  m_value = value / factor;
}

bool QHealthValue::isValid() const {
  return m_type != InvalidType && m_value.has_value();
}
bool QHealthValue::hasValue() const { return m_value.has_value(); }
