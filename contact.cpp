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
#include "contact.h"

QString Contact::fileName() const {
    return m_fileName;
}

QString Contact::personName() const {
    return m_personName;
}

QString Contact::phoneNumber() const {
    return "Unknown";
}

QDBusArgument &operator<<(QDBusArgument &argument, const Contact& cs)
{
    argument.beginStructure();
    argument << cs.m_fileName;
    argument << cs.m_personName;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Contact &cs)
{
    argument.beginStructure();
    argument >> cs.m_fileName;
    argument >> cs.m_personName;
    argument.endStructure();
    return argument;
}
