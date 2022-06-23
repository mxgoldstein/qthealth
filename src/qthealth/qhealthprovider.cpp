// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qhealthprovider.h"

#include <QPluginLoader>

#include "qthealthplugininterface.h"

#ifdef Q_OS_IOS
Q_IMPORT_PLUGIN(QtHealthPluginHealthKit)
#endif

QHealthProvider::QHealthProvider(QString plugin) {
  qRegisterMetaType<QHealthProvider::PermissionError>();
  qRegisterMetaType<QHealthQuery::Error>();
  loadPlugin(plugin);
}

QHealthProvider::QHealthProvider() : QHealthProvider(defaultPlugin()) {}
QStringList QHealthProvider::plugins() {
  QStringList plugins;
  for (QStaticPlugin &plugin : QPluginLoader::staticPlugins()) {
    if (plugin.metaData()[u"IID"_qs].toString() != QtHealthPluginInterface_IID)
      continue;
    plugins << plugin.metaData()[u"className"_qs].toString();
  }
  return plugins;
}

QString QHealthProvider::defaultPlugin() {
#ifdef Q_OS_IOS
  return "QtHealthPluginHealthKit";
#else
  return "";
#endif
}

bool QHealthProvider::isAvailable() const {
  return m_plugin != nullptr && m_plugin->isAvailable();
}

void QHealthProvider::loadPlugin(QString name) {
  for (QStaticPlugin &plugin : QPluginLoader::staticPlugins()) {
    if (plugin.metaData()[u"IID"_qs] != QtHealthPluginInterface_IID)
      continue;
    if (name.isEmpty() ||
        name == plugin.metaData()[u"className"_qs].toString()) {
      m_plugin = qobject_cast<QtHealthPluginInterface *>(plugin.instance());
      m_plugin->initialize();
    }
  }
}

void QHealthProvider::requestPermissions(QHealthData::Types read,
                                         QHealthData::Types write) {
  m_plugin->requestPermissions(
      read, write,
      [&](bool success, QHealthProvider::PermissionError error,
          QString message) {
        emit permissionsRequested(success, error, message);
      });
}

QHealthProvider::PermissionStatus
QHealthProvider::permission(QHealthData::Type type,
                            QHealthProvider::AccessType readWrite) {
  return m_plugin->permission(type, readWrite);
}

void QHealthProvider::executeQuery(QHealthQuery *query) {
  Q_ASSERT(query != nullptr);
  if (!query->isValid()) {
    emit query->failure(QHealthQuery::InvalidQuery, QString());
    return;
  }

  m_plugin->executeQuery(query, [](QHealthQuery *query, bool success,
                                   QList<QHealthData *> data,
                                   QHealthQuery::Error error, QString message) {
    if (success) {
      emit query->success(data);
    } else {
      emit query->failure(error, message);
    }
  });
}
