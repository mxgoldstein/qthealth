// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QHEALTHPROVIDER_H
#define QHEALTHPROVIDER_H

#include <QObject>
#include <QString>

#include "qhealthdata.h"

class QtHealthPluginInterface;
class QHealthQuery;

class QHealthProvider : public QObject {
  Q_OBJECT
public:
  enum PermissionError {
    NoPermissionError = 0,
    PermissionUnsupported = 1,
    PermissionAPIError = 2
  };
  Q_ENUM(PermissionError)

  enum PermissionStatus {
    StatusDenied = 0,
    StatusGranted = 1,
    StatusNotRequested = 2,
    StatusUnavailable = 3,
  };
  Q_ENUM(PermissionStatus)

  enum AccessType { Read = 0, Write = 1 };
  Q_ENUM(AccessType)

  QHealthProvider();
  QHealthProvider(QString plugin);

  Q_INVOKABLE bool isAvailable() const;

  Q_INVOKABLE void requestPermissions(QHealthData::Types read,
                                      QHealthData::Types write);
  Q_INVOKABLE QHealthProvider::PermissionStatus
  permission(QHealthData::Type type, QHealthProvider::AccessType readWrite);

  Q_INVOKABLE static QString defaultPlugin();
  Q_INVOKABLE static QStringList plugins();

  Q_INVOKABLE void executeQuery(QHealthQuery *query);
signals:
  void permissionsRequested(bool success,
                            QHealthProvider::PermissionError error,
                            QString message);

private:
  void loadPlugin(QString name);

  QtHealthPluginInterface *m_plugin = nullptr;
};

#endif // QHEALTHPROVIDER_H
