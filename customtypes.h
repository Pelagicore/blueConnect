/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <QObject>
#include <QDBusObjectPath>

typedef QMap<QString,QVariantMap> InterfacesMap;
typedef QList<QString> InterfacesList;
typedef QMap<QDBusObjectPath,InterfacesMap> ObjectsMap;

Q_DECLARE_METATYPE(InterfacesMap)
Q_DECLARE_METATYPE(ObjectsMap)
Q_DECLARE_METATYPE(Contact)


#endif // CUSTOMTYPES_H
