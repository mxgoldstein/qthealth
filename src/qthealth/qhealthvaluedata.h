// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHVALUEDATA_H
#define QHEALTHVALUEDATA_H

#include <QObject>

#include "qhealthdata.h"
#include "qhealthvalue.h"

class QHealthValueData : public QHealthData {
  Q_OBJECT
  Q_PROPERTY(QHealthValue *value READ value WRITE setValue NOTIFY valueChanged);

public:
  QHealthValue *value() const;
  void setValue(QHealthValue *value);

  QHealthData::Type dataType() const override;

  QString uuid() const override;
  void setUuid(QString uuid);

  bool isValid() const override;
signals:
  void valueChanged();

private:
  QString m_uuid;
  QHealthValue *m_value = nullptr;
};

#endif // QHEALTHVALUEDATA_H
