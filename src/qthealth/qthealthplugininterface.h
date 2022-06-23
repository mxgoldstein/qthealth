// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTHEALTHPLUGININTERFACE_H
#define QTHEALTHPLUGININTERFACE_H

#include <qhealthdata.h>
#include <qhealthprovider.h>
#include <qhealthquery.h>

#include <QObject>
#include <functional>

class QtHealthPluginInterface {
public:
  virtual void initialize() = 0;
  virtual bool isAvailable() = 0;
  virtual void requestPermissions(
      QHealthData::Types read, QHealthData::Types write,
      std::function<void(bool, QHealthProvider::PermissionError, QString)>
          requestDone) = 0;
  virtual QHealthProvider::PermissionStatus
  permission(QHealthData::Type permission,
             QHealthProvider::AccessType readWrite) = 0;
  virtual void executeQuery(
      QHealthQuery *query,
      std::function<void(QHealthQuery *query, bool, QList<QHealthData *>,
                         QHealthQuery::Error, QString)>
          queryDone) = 0;
};

#define QtHealthPluginInterface_IID "org.qt-project.Qt.QtHealthPluginInterface"
Q_DECLARE_INTERFACE(QtHealthPluginInterface,
                    "org.qt-project.Qt.QtHealthPluginInterface")

#endif // QTHEALTHPLUGININTERFACE_H
