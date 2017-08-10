/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDBusArgument>
#include "modem.h"

QString Modem::name() const {
    return m_name;
}

QDBusObjectPath Modem::objectPath() const {
    return m_objectPath;
}

QDBusArgument &operator<<(QDBusArgument &argument, const Modem& modem)
{
    QVariantMap map;
    map["Name"] = modem.m_name;

    argument.beginStructure();
    argument << modem.m_objectPath;
    argument << map;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Modem &modem)
{
    QVariantMap map;
    argument.beginStructure();
    argument >> modem.m_objectPath;
    argument >> map;
    argument.endStructure();

    modem.m_name = map["Name"].toString();
    return argument;
}
