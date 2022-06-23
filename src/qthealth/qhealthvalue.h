// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHVALUE_H
#define QHEALTHVALUE_H

#include <QObject>
#include <optional>

class QHealthValue : public QObject {
  Q_OBJECT
  Q_PROPERTY(QHealthValue::Type type READ type WRITE setType NOTIFY typeChanged)
public:
  enum Type { InvalidType = 0, Distance, Duration, Speed, Heartbeat, Energy };
  Q_ENUM(Type);

  enum Unit {
    InvalidUnit = 0,
    Meters,
    Kilometers,
    Feet,
    Miles,
    Seconds,
    Minutes,
    Hours,
    Days,
    Weeks,
    MetersPerSecond,
    KilometersPerHour,
    MilesPerHour,
    BeatsPerMinute,
    Joules,
    KiloCalories
  };
  Q_ENUM(Unit)

  QHealthValue() = default;
  QHealthValue(QHealthValue::Type type, double value, QHealthValue::Unit unit);

  QHealthValue::Type type() const;
  void setType(QHealthValue::Type type);

  Q_INVOKABLE double value(QHealthValue::Unit unit) const;
  Q_INVOKABLE void setValue(double value, QHealthValue::Unit unit);

  Q_INVOKABLE bool isValid() const;
  Q_INVOKABLE bool hasValue() const;
signals:
  void typeChanged();

private:
  QHealthValue::Type m_type = InvalidType;
  std::optional<double> m_value;
};

#endif // QHEALTHVALUE_H
