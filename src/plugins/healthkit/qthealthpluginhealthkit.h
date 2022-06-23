// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include <qthealthplugininterface.h>

#include <QObject>

typedef struct objc_object *id;

class QtHealthPluginHealthKit : public QObject, public QtHealthPluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QtHealthPluginInterface_IID)
  Q_INTERFACES(QtHealthPluginInterface)

public:
  QtHealthPluginHealthKit();
  ~QtHealthPluginHealthKit();

  void initialize() override;
  bool isAvailable() override;
  void requestPermissions(
      QHealthData::Types read, QHealthData::Types write,
      std::function<void(bool, QHealthProvider::PermissionError, QString)>
          requestDone) override;
  QHealthProvider::PermissionStatus
  permission(QHealthData::Type permission,
             QHealthProvider::AccessType readWrite) override;
  void
  executeQuery(QHealthQuery *query,
               std::function<void(QHealthQuery *, bool, QList<QHealthData *>,
                                  QHealthQuery::Error, QString)>
                   queryDone) override;

private:
  id m_healthStore = nullptr;
};
