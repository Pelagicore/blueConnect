/*
 * Copyright (C) 2017, Pelagicore AB.
 *
 * Author: Viktor Sjölind <viktor.sjolind@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BLUECONNECTPROPERTYCALLBACK_H
#define BLUECONNECTPROPERTYCALLBACK_H

#include "blueconnect.h"

class BlueConnectPropertyCallback: QObject
{
    Q_OBJECT

public:
    BlueConnectPropertyCallback(BlueConnect *blueConnect, QDBusInterface *device, QString path);
    ~BlueConnectPropertyCallback();

protected slots:
    void onPropertiesChanged(const QString &interfaceName,
                             const QVariantMap changesMap,
                             const QStringList invalidatedProperties);

private:
    QDBusInterface *device;
    BlueConnect *blueConnect;
};

#endif // BLUECONNECTPROPERTYCALLBACK_H
