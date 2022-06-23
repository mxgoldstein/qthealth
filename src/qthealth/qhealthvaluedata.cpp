// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthvaluedata.h"

QHealthValue *QHealthValueData::value() const { return m_value; }
void QHealthValueData::setValue(QHealthValue *value) { m_value = value; }

QHealthData::Type QHealthValueData::dataType() const {
  return QHealthData::ValueData;
}

QString QHealthValueData::uuid() const { return m_uuid; }
void QHealthValueData::setUuid(QString uuid) { m_uuid = uuid; }

bool QHealthValueData::isValid() const {
  return m_value != nullptr && m_value->isValid();
}
