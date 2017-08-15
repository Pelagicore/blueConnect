/*
 * Copyright (C) 2017, Pelagicore AB.
 *
 * Author: Viktor Sjölind <viktor.sjolind@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "blueconnectpropertycallback.h"
#include <QDebug>

BlueConnectPropertyCallback::BlueConnectPropertyCallback(BlueConnect *blueConnect,
                                                         QDBusInterface *device,
                                                         QString path):
    device(device), blueConnect(blueConnect), QObject(NULL)
{
    qDebug() << "Starting callback handler for: " << path;
    auto systemBus = QDBusConnection::systemBus();
    if (!systemBus.connect("org.bluez",
             path,
             "org.freedesktop.DBus.Properties",
             "PropertiesChanged",
             this,
             SLOT(onPropertiesChanged(QString,QVariantMap,QStringList))))
    {
        qWarning() << "Failed to connect PropertiesChanged signal for path: " << path;
    }
}

BlueConnectPropertyCallback::~BlueConnectPropertyCallback()
{
}

void BlueConnectPropertyCallback::onPropertiesChanged(const QString &interfaceName,
                                                      const QVariantMap /* changesMap */,
                                                      const QStringList /* invalidatedProperties */)
{
    qDebug() << Q_FUNC_INFO << interfaceName;
    if (interfaceName == "org.bluez.Device1") {
        blueConnect->updateDevice(device);
    }
}
