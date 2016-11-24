/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MODEM_H
#define MODEM_H

#include <QDBusArgument>

class Modem {
public:
    QString name() const;
    QDBusObjectPath objectPath() const;

private:
    friend QDBusArgument &operator<<(QDBusArgument &argument, const Modem& cs);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, Modem &cs);

    QString m_name;
    QDBusObjectPath m_objectPath;
};
QDBusArgument &operator<<(QDBusArgument &argument, const Modem& cs);
const QDBusArgument &operator>>(const QDBusArgument &argument, Modem &cs);

#endif // MODEM_H
