// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthdata.h"

QDateTime QHealthData::startDate() const { return m_startDate; }
void QHealthData::setStartDate(QDateTime startDate) { m_startDate = startDate; }

QDateTime QHealthData::endDate() const { return m_endDate; }
void QHealthData::setEndDate(QDateTime endDate) { m_endDate = endDate; }
